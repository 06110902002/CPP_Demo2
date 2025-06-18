//
// Created by Rambo.Liu on 2023/12/18.
//

#include <stdlib.h>
#include <type_traits>
#include <iostream>
#include <thread>
#include <future>
#include <vector>

#include <numeric>
#include "List.h"


using namespace std;


int sort_function(const void *a, const void *b) {
    printf("-----a = %d  b = %d = \n", (*(int *) a), (*(int *) b));
    return *(int *) a - *(int *) b;
}

//使用函数指针实现列表的排序
void testSortWithFun() {
    int list[5] = {54, 21, 11, 67, 22};
    qsort((void *) list, 5, sizeof(list[0]), sort_function);//起始地址，个数，元素大小，回调函数
    int x;
    for (x = 0; x < 5; x++) {
        printf("%i ", list[x]);
    }
    printf("\n");
}

//在C++里，我们通过在一个类中重载括号运算符的方法使用一个函数对象而不是一个普通函数。
template<typename T>
class display {
public:
    void operator()(const T &x) {
        cout << x << " ";
    }
};

//重载括号运算符
void testOverride() {
    int ia[] = {1, 2, 3, 4, 5};
    for_each(ia, ia + 5, display<int>());
}


void testIterator() {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.print();
    for (List<int>::iterator it = l.begin(); it != l.end(); ++it) {
        printf("---测试迭代器遍历元素 = %d \n",*it);
    }
}

#define		LIKELY(x)					((__builtin_expect(!!(x), 1)))
#define		UNLIKELY(x)					((__builtin_expect(!!(x), 0)))


/**
 * https://www.jianshu.com/p/72601d82f3df
 * std::packaged_task，它允许传入一个函数，并将函数计算的结果传递给std::future，包括函数运行时产生的异常
 * */
template<typename F,typename  T = typename std::decay<F>::type>
class Task {

public:
    T func_;
    explicit Task(F&& func):func_(std::forward<F>(func)) {

    }
    void call() {
        func_();
    }
};


int testTask(int a,int b) {
    printf("73-----a = %d, b = %d",a,b);
    return a + b;
}

/**
 * 测试future
 */
void testFuture1() {
    std::cout << "主线程 tid = " << std::this_thread::get_id() << std::endl;
    // 来自 packaged_task 的 future
    std::packaged_task<int()> task([](){
        std::cout << "\npackaged tid = " << std::this_thread::get_id() << std::endl;
        return 7;
    }); // 包装函数
    std::future<int> f1 = task.get_future();  // 获取 future
    std::thread(std::move(task)).detach(); // 在线程上运行


    // 来自 async() 的 future
    std::future<int> f2 = std::async(std::launch::async, [](){
        std::cout << "\nstd::async tid = " << std::this_thread::get_id() << std::endl;
        return 8;
    });

    // 来自 promise 的 future
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{
        std::cout << "std::promise tid = " << std::this_thread::get_id() << std::endl;
        p.set_value_at_thread_exit(9);
    }).detach();

    std::cout << "\nWaiting..." << std::flush;
    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
}

void testFuture2() {


    std::future<int> f1 = std::async(std::launch::async, [](){
        std::cout << "\nstd::async f1 tid = " << std::this_thread::get_id() << std::endl;
        int result = 0;
        for(int i = 0; i <= 25; i ++) {
            result += i;
        }
        return result;
    });

    std::future<int> f2 = std::async(std::launch::async, [](){
        std::cout << "\nstd::async f2 tid = " << std::this_thread::get_id() << std::endl;
        int result = 0;
        for(int i = 26; i <= 50; i ++) {
            result += i;
        }
        return result;
    });

    std::future<int> f3 = std::async(std::launch::async, [](){
        std::cout << "\nstd::async f3 tid = " << std::this_thread::get_id() << std::endl;
        int result = 0;
        for(int i = 51; i <= 75; i ++) {
            result += i;
        }
        return result;
    });

    std::future<int> f4 = std::async(std::launch::async, [](){
        std::cout << "\nstd::async f4 tid = " << std::this_thread::get_id() << std::endl;
        int result = 0;
        for(int i = 76; i <= 100; i ++) {
            result += i;
        }
        return result;
    });

    int f1_result = f1.get();
    int f2_result = f2.get();
    int f3_result = f3.get();
    int f4_result = f4.get();
    int all_result = f1_result + f2_result + f3_result + f4_result;
    std::cout<< " f1 result = "<<f1_result << " f2 result = "<<f2_result << " f3 result = " <<f3_result << " f4 result = " << f4_result <<
     " 总和 = "  <<all_result <<std::endl;
}



/**-----------------------------测试 std::async ------------------------------------*/
/**
 * 测试 函数作为async 调用对象
 * @param n
 * @param b
 * @return
 */
int async_fuc1(int n, int& b)
{
    for (int i = 0; i < 3; ++i) {
        printf("async is running\n");
        ++n;
        ++b;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    printf("n is %d, b is %d\n", n, b);
    return n + b;
}

void test_async1() {
    int n = 0;
    int b = 0;
    // n 按值传递，b 按引用传递
    auto f = std::async(std::launch::async, async_fuc1, n, std::ref(b));
    int ret = f.get();
    printf("196----result is %d\n", ret);
}

/**
 * 测试async 使用lamba 作为参数
 */
void test_async2() {
    auto f = std::async([]() -> int {
        for (int i = 0; i < 3; ++i) {
            printf("async is running\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return 100;
    });
    int ret = f.get();
    printf("result is %d\n", ret);
}

/**-----------------------------测试 std::async ------------------------------------*/


/**-----------------------------测试 std::promise ------------------------------------*/

// 异步求和
void Accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // Notify future
}

// 线程1设置值
void Do_work1(std::promise<int> &barrier)
{
    //为了突出效果，可以使线程休眠5s
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int iVal = 233;
    std::cout << "传入数据(int)：" << iVal << std::endl;
    barrier.set_value(iVal); // 设置共享状态的值, 此处和线程new_work_thread保持同步.

}

// 线程2获取值
void Do_work2(std::future<int> &fun)
{
    //阻塞函数，直到收到相关联的std::promise对象传入的数据
    auto iVal = fun.get();		//iVal = 233
    std::cout << "收到数据(int)：" << iVal << std::endl;
}

void test_promise()
{
    // 演示如何使用promise<int>在线程之间传输结果
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
    std::promise<int> accumulate_promise; // 主线程生成一个 std::promise<int> 对象.
    std::future<int> accumulate_future = accumulate_promise.get_future(); // 和 future 关联.
    std::thread work_thread(Accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise)); // 求和

    // future::get() will wait until the future has a valid result and retrieves it.
    // Calling wait() before get() is not needed
    // accumulate_future.wait();  // wait for result
    std::cout << "result=" << accumulate_future.get() << '\n'; // 异步获取最后的结果
    work_thread.join();  // wait for thread completion

    //演示如何使用promise<int>在线程之间表示状态。
    std::promise<int> barrier; // 生成一个 std::promise<int> 对象.
    std::future<int> barrier_future = barrier.get_future(); // 和 future 关联.
    std::thread new_work1_thread(Do_work1, std::ref(barrier));
    std::thread new_work2_thread(Do_work2, std::ref(barrier_future));

    new_work1_thread.join();
    new_work2_thread.join();
}



#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <unistd.h>

uint8_t* int2Bytes(int i) {
    uint8_t *buf = (uint8_t *) malloc(4 * sizeof(uint8_t));
    memset(buf,0,4);
    buf[0] = (unsigned char) i;
    buf[1] = i >> 8;
    buf[2] = i >> 16;
    buf[3] = i >> 24;
    return buf;

}

void testSocketClient() {
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //服务器端口
    servaddr.sin_port = htons(12581);
    //服务器ip，inet_addr用于IPv4的IP转换（十进制转换为二进制）
    //127.0.0.1是本地预留地址
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //设置连接超时时间
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(sock_cli, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        std::cout << "Setting socket timeout failed\n";
        close(sock_cli);
        return;
    }


    //连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("client connect");
        return;
    } else {

    }



    for (int i = 0; i < 5;i ++) {
        usleep(1 * 1000 * 1000);



        printf("连接成功  开始发送数据\n");
        //发送4 字节 的报文类型
        uint8_t *type = int2Bytes(1);
        send(sock_cli, type, 4, 0);

        //发送4 字节 的报文长度
        char* tmp = "add";
        char* msg = (char*)malloc(strlen(tmp) + 1);
        memset(msg,0,strlen(tmp) + 1);
        sprintf(msg, "%s%d", tmp, i);

        uint8_t* content = reinterpret_cast<uint8_t *>(msg);
        size_t length = strlen(tmp) + 1;
        uint8_t *frameLenght = int2Bytes(length);
        send(sock_cli, frameLenght, 4, 0);

        //发送报文数据
        send(sock_cli, content,length, 0);
    }


}

class A {
public:
    A() {
        printf("361------A 构造\n");
    }
    virtual ~A() {
        printf("365------A 析构\n");
    }

    void useCall() {
        printf("------364-----\n");
    }


};


/**
 * C++ 11 特性 decltype
 * https://www.cnblogs.com/QG-whz/p/4952980.html
 *
 */
void testdecltype() {
//    我们之前使用的typeid运算符来查询一个变量的类型，这种类型查询在运行时进行。
//    RTTI机制为每一个类型产生一个type_info类型的数据，而typeid查询返回的变量相应type_info数据，
//    通过name成员函数返回类型的名称。同时在C++11中typeid还提供了hash_code这个成员函数，用于返回类型的唯一哈希值。
//    RTTI会导致运行时效率降低，且在泛型编程中，我们更需要的是编译时就要确定类型，RTTI并无法满足这样的要求。
//    编译时类型推导的出现正是为了泛型编程，在非泛型编程中，我们的类型都是确定的，根本不需要再进行推导。
//
//    而编译时类型推导，除了我们说过的auto关键字，还有本文的decltype。
//    decltype与auto关键字一样，用于进行编译时类型推导，不过它与auto还是有一些区别的。
//    decltype的类型推导并不是像auto一样是从变量声明的初始化表达式获得变量的类型，而是总是以一个普通表达式作为参数，
//    返回该表达式的类型,而且decltype并不会对表达式进行求值。
    int i = 4;
    decltype(i) a = 56.45f; //推导结果为int。a的类型为int。

}

/**
 * 测试C++ 11 中尾随返回类型
 * https://blog.csdn.net/baidu_36775904/article/details/132558695
 * @return
 */
auto testReturnType() -> int{
    return 56;
}


int main() {
    //testSortWithFun();
    //testOverride();

    //testIterator();
//    auto funType = std::bind(testTask,2,13);
//    std::packaged_task<int()> task2(funType);
//    //使用std::move 将左值转换为右值
//    Task<std::packaged_task<int()>&&> *tt = new Task<std::packaged_task<int()>&&>(std::move(task2));
//    tt->call();

//     testFuture1();
//     testFuture2();

        //test_async1();
        //test_async2();

        test_promise();




//    std::packaged_task<int(int,int)> task3(testTask);
//    std::future<int> future = task3.get_future();
//
//    // std::promise一样，std::packaged_task支持move，但不支持拷贝
//    // std::thread的第一个参数不止是函数，还可以是一个可调用对象，即支持operator()(Args...)操作
//    std::thread t(std::move(task3), 1, 2);
//    // 等待异步计算结果
//    std::cout << "1 + 2 => " << future.get() << std::endl;
//
//    t.join();

//    char* msg = "this is test msg";
//    uint8_t* content = reinterpret_cast<uint8_t *>(msg);
//
//    printf("366-------size of(msg) = %lu sizeof(content) = %lu \n",sizeof(msg), strlen(msg));

  //   testSocketClient();

//    char str[] = "The answer is ";
//    char* result = (char*)malloc(strlen(str) + 1);
//    for(int i = 0; i< 5; i ++) {
//        // 使用sprintf函数进行拼接
//        sprintf(result, "%s%d", str, i);
//        printf("%s\n", result);
//    }

    testdecltype();




    return 0;
}