//
// Created by rambo.liu on 2026/4/22.
//

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <sstream>
#include <functional>
#include <mutex>
#include <future>
#include <type_traits>

using namespace std;

//////////////////////////////////////////////////////
// 1. Serializer
//////////////////////////////////////////////////////

struct Serializer {
    template<typename T>
    static string encode(const T &val) {
        stringstream ss;
        ss << val;
        return ss.str();
    }

    static string encode(const string &val) {
        return val;
    }

    template<typename T>
    static T decode(const string &str) {
        if constexpr (is_same_v<T, string>) {
            return str;
        } else {
            T val{};
            stringstream ss(str);
            ss >> val;
            return val;
        }
    }
};

//////////////////////////////////////////////////////
// 2. function_traits
//////////////////////////////////////////////////////

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using result_type = R;
    using args_tuple = tuple<Args...>;
};

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)>
        : function_traits<R(Args...)> {
};

template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const>
        : function_traits<R(Args...)> {
};

template<typename T>
struct function_traits
        : function_traits<decltype(&T::operator())> {
};

//////////////////////////////////////////////////////
// 3. Rpc Response
//////////////////////////////////////////////////////

struct RpcResult {
    bool success = true;
    string data;
    string error;
};

//////////////////////////////////////////////////////
// 4. RpcServer
//////////////////////////////////////////////////////

class RpcServer {
public:
    template<typename Func>
    void register_service(const string &name, Func func) {
        lock_guard<mutex> lock(mtx_);

        services_[name] =
                [func](const vector<string> &args)-> RpcResult {
                    return invoke(func, args);
                };
    }

    template<typename Ret, typename... Args>
    Ret call(const string &name, Args &&... args) {
        vector<string> vec{
            Serializer::encode(args)...
        };

        RpcResult r = dispatch(name, vec);

        if (!r.success)
            throw runtime_error(r.error);

        if constexpr (is_void_v<Ret>) {
            return;
        } else {
            return Serializer::decode<Ret>(r.data);
        }
    }

    template<typename... Args>
    future<void> async_call(
        const string &name, Args &&... args) {
        return async(launch::async,
                     [this,name,args...]() {
                         call<void>(name, args...);
                     });
    }

private:
    map<string, function<RpcResult(const vector<string> &)> > services_;
    mutex mtx_;

    //////////////////////////////////////////////////////
    // invoke
    //////////////////////////////////////////////////////

    template<typename Func>
    static RpcResult invoke(
        Func func,
        const vector<string> &args) {
        try {
            using traits = function_traits<Func>;
            using tuple_t = typename traits::args_tuple;
            using ret_t = typename traits::result_type;

            auto tp = build_tuple<tuple_t>(args);

            if constexpr (is_void_v<ret_t>) {
                apply(func, tp);

                return {true, "", ""};
            } else {
                ret_t ret = apply(func, tp);

                return {
                    true,
                    Serializer::encode(ret),
                    ""
                };
            }
        } catch (exception &e) {
            return {false, "", e.what()};
        }
    }

    //////////////////////////////////////////////////////
    // tuple build
    //////////////////////////////////////////////////////

    template<typename Tuple, size_t... I>
    static auto build_tuple_impl(
        const vector<string> &args,
        index_sequence<I...>) {
        return make_tuple(
            Serializer::decode<
                decay_t<tuple_element_t<I, Tuple> >
            >(args[I])...
        );
    }

    template<typename Tuple>
    static auto build_tuple(
        const vector<string> &args) {
        constexpr size_t N =
                tuple_size_v<Tuple>;

        if (args.size() != N)
            throw runtime_error(
                "argument count mismatch");

        return build_tuple_impl<Tuple>(
            args,
            make_index_sequence<N>{});
    }

    //////////////////////////////////////////////////////
    // dispatch
    //////////////////////////////////////////////////////

    RpcResult dispatch(
        const string &name,
        const vector<string> &args) {
        lock_guard<mutex> lock(mtx_);

        auto it = services_.find(name);

        if (it == services_.end())
            return {false, "", "service not found"};

        return it->second(args);
    }
};

//////////////////////////////////////////////////////
// Demo Struct
//////////////////////////////////////////////////////

struct User {
    int id{};
    string name;
};

ostream &operator<<(ostream &os, const User &u) {
    os << u.id << " " << u.name;
    return os;
}

istream &operator>>(istream &is, User &u) {
    is >> u.id >> u.name;
    return is;
}

//////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////

int main() {
    RpcServer server;

    server.register_service(
        "add",
        [](int a, int b, int c) {
            return a + b + c;
        });

    server.register_service(
        "hello",
        [](const string &s) {
            return "hello " + s;
        });

    server.register_service(
        "print",
        [](const string &s) {
            cout << "print: " << s << endl;
        });

    cout << server.call<int>("add", 1, 2, 3) << endl;

    cout << server.call<string>(
        "hello", "rambo liu") << endl;

    server.call<void>("print", "rpc ok");

    auto f =
            server.async_call("print", "async task");

    f.get();

    return 0;
}
