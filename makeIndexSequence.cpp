//
// Created by Rambo.Liu on 2024/12/4.
//  std::make_index_sequence  使用参考
// https://www.cnblogs.com/happenlee/p/14219925.html
// https://dengking.github.io/programming-language/C%2B%2B/STL/Utility-library/General-purpose/Common-vocabulary-types/std-integer_sequence/CSDN-C%2B%2B14-make_index_sequence%E5%8E%9F%E7%90%86/
#include <iostream>
#include <stdlib.h>
#include <tuple>
#include <array>
#include <string>
using namespace std;

/**-------------------------测试make_index_sequence -------------------------------*/
/**
 * 编译期 求 1，2，n 的平方值，得到的结果存在一个数组中
 * 如：1，2，3，4  平方值 为[1,4,9,16]
 * @tparam N
 * @param index
 * @return
 */
template<size_t ...N>
static constexpr auto square_nums(size_t index, std::index_sequence<N...>) {
    constexpr auto nums = std::array{N * N ...}; //得在C++ 17 以上  运行
    //return nums[index];
    return nums;
}

template<size_t N>
constexpr static auto const_nums(size_t index) {
    return square_nums(index, std::make_index_sequence<N>{});
}

void test_make_index_sequence() {
    auto res = const_nums<11>(10);
    for (int i =0; i< res.size(); i ++) {
        printf(" %d 的 平方= %lu \n",i,res.at(i));
    }

}

/**-------------------------测试make_index_sequence -------------------------------*/



/**-------------------------测试make_index_sequence 与tuple-------------------------------*/
template<size_t ... N>
decltype(auto) fun(index_sequence<N...> is)
{
    return make_tuple(N...);
}

int test_make_index_sequence_tuple()
{
    auto t = fun(make_index_sequence<3>());
    cout << std::get<0>(t) << endl;
    cout << std::get<1>(t) << endl;
    cout << std::get<2>(t) << endl;
    return 0;
}
// g++ --std=c++17 test.cpp -Wall -pedantic
/**-------------------------测试make_index_sequence 与tuple-------------------------------*/


/****-----------------std:make_index_sequence  遍历std:tuple--------------------*/
template <typename Tuple, typename Func, size_t ... N>
void func_call_tuple(const Tuple& t, Func&& func, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(func(std::get<N>(t)), 0)...});
}

template <typename ... Args, typename Func>
void travel_tuple(const std::tuple<Args...>& t, Func&& func) {
    func_call_tuple(t, std::forward<Func>(func), std::make_index_sequence<sizeof...(Args)>{});
}

int test_make_index_sequence_foreach_tuple() {
    auto t = std::make_tuple(1, 4.56, "happen lee");
    travel_tuple(t, [](auto&& item) {
        std::cout << item << ",";
    });
}

/****-----------------std:make_index_sequence  遍历std:tuple--------------------*/


/****-----------------自实现std:make_index_sequence  与 std:index_sequence--------------------*/

template<int ... N>
struct index_seq
{
};

/**
 * @brief 递归函数
 *
 * @tparam N
 * @tparam M
 */
template<int N, int ...M>
struct make_index_seq: public make_index_seq<N - 1, N - 1, M...>
{

};
/**
 * @brief 递归终止条件
 *
 * @tparam M
 */
template<int ...M>
struct make_index_seq<0, M...> : public index_seq<M...>
{

};

template<int ... N>
decltype(auto) fun(index_seq<N...> is)
{
    return make_tuple(N...);
}

int test_custom_index_sequence()
{
    auto t = fun(make_index_seq<3>());
    cout << std::get<0>(t) << endl;
    cout << std::get<1>(t) << endl;
    cout << std::get<2>(t) << endl;
    return 0;
}
// g++ --std=c++14 test.cpp -Wall -pedantic
/****-----------------自实现std:make_index_sequence  与 std:index_sequence--------------------*/



int main() {
    //test_make_index_sequence();
    test_make_index_sequence_foreach_tuple();
    //test_custom_index_sequence();
}






