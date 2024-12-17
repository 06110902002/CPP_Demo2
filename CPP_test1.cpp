#include<iostream>
#include<cstdio>
using namespace std;


//指向函数的指针
//定义的一般形式：函数的返回值类型 (*指针变量名)(形式参数1, 形式参数2, ...);

typedef int (*fun_ptr)(int,int);

int sum(int a, int b) {
	return a + b;
}

int minus(int a,int b) {

	return a - b;
}
int divide(int a, int b) {
	return a / b;
}

//定义函数指针作为参数
int  counting( int(*p)(int,int), int a, int b) {
     int result = p(a, b);
     printf("21------函数指针作为参数计算结果为：%d\n", result);
}

// 测试函数指针作为 函数参数时 是否会修改原来的函数指针指向
// 结果是不会
// 对比  实参与形参  发现  函数指针的地址发生了变化，因为在C++ 中函数有副本机制
void testModifyFunc(int (*q)(int a, int b)) {

	q = divide;
	printf("32------形参 函数指针q 地址 =%p\n",&q );
}


//结构体作为函数参数
//将结构体变量作为函数参数进行传递时，其实传递的是全部成员的值，也就是将实参中成员的值一一赋值给对应的形参成员。因此，形参的改变不会影响到实参
struct Student
{
	int age;
	
};

void testStruct(Student stu){
	 printf("函数内修改前的形参：%d   stu 地址 = %p\n", stu.age,&stu);
     // 修改实参中的age
     stu.age = 10;
     printf("函数内修改后的形参：%d \n", stu.age);
}

//6.数组作为函数参数，不会有副本机制，即不会进行临时拷贝
//为了节约内存 数组传递的是指针
//数组作为函数参数 ，退化为指针，也就是实参可以传一个指针过来
//可以对比形参与实参的地址
void testArrayAsParams(int a[], int size) {

	printf("57-------使用指针访问元素  形参地址 %p\n",a);
	for ( int* p = a; p < a + size; p ++) {
		printf("数组作为参数，使用 指针访问元素值为 = %d\n",*(p));

	}

	printf("63-------使用数组下标访问元素 \n");
	for ( int i = 0; i < size; i ++) {
		printf("数组作为参数，使用使用数组下标访问元素为 = %d\n",a[i]);

	}
}

//指针数组作为函数形参时，是一个指针，此时如果直接sizeof(cmd) = 8(64位机器) 得到是一个指针的大小
//而不是指针数组的大小，这一点需要注意
//因为它传递的是地址 
void testPtrArray(char* cmd[], int length) {

	printf("75-----指针 sizeof(cmd) = %d\n",sizeof(cmd));


}

//
char* testGetStaicMem() {
	char str[100] = "1234567"; //向静态存储区申请内存
	char* p = str;
	return p;
}

//7 函数作用域结束之后 栈上分配的内存会被回收，如果仍想使用函数调用结束后的值，可以使用堆内存
//为了避免这种情况常常，需要将栈空间拷贝到堆内存上
char* testGetStackMem() {
	char str[3] = {'A','B','C'};
	return str;

}


//测试在堆上申请内存,堆上内存使用之后  需要手动清除
// malloc 函数向堆申请内存
char* testGetHeapMem() {
	char* str = (char*)malloc(sizeof(char) * 100);
	memset(str,99,sizeof(char) * 100);
	printf("85-------%s   %d\n",__func__,*(str + 99));
	return str;
}

/*-----------------------------小结--------------------------------*/
//对于函数是否选择传值  还是传地址，可根据参数的大小进行判断
//参数占用内存小  可用 传值方式
//参数占用内存大  建议用 传地址的方式，因为使用值传递  会造成拷贝的情况出现

//如果还是使用传值的方式，又不想避免拷贝  可以使用右值引用的方式避免本次拷贝

//结构体作为  函数参数 也是 传值的方式

/*-----------------------------小结--------------------------------*/

void* testInt(void* arg) {
	int i = *(int*) arg;
	printf("119------------i = %d\n", i);
	return nullptr;
}

/**
 * C++ 多线程thread 库的知识点：
 * 1. future
 * 2. promise
 * 3. 互斥锁
 * 4. 条件变量
 * 5. 栅栏机制
 * 6. 无锁的数据结构 （自旋锁） 需要考虑无锁时 内存的回收情况
 
 * 7. 左值引用  与右值引用的区别  移动语义
 * 左值 右值区别：
 * 看能不能对表达式取地址，如果能，则为左值，否则为右值
 * 详解文档 ：https://harttle.land/2015/10/11/cpp11-rvalue.html
 * C++ 11 中对于会产生拷贝构造函数时，如果类中实现了移动构造函数  会优先调用移动构造函数，没有
 * 再去调用拷贝构造函数
 */
class A {
public:
	int m_nAge;
	A(int age) : m_nAge(age) {
		printf("142--------A对象调用构造\n");
	} 
	A(const A& p) {
		printf("142--------A对象调用拷贝构造\n");
        m_nAge = p.m_nAge;
    }
	~A() {
		printf("142--------A对象析构了\n");
	}
};

A getRValue() {
	A a(33);
	printf("145--------临时变量 a 的址 = %p\n", &a);
	return a;
}


class Person {

    char* name;

public:

    Person(const char* p){
        size_t n = strlen(p) + 1;
        name = new char[n];
        memcpy(name, p, n);
        printf("164------Person 构造\n");
    }
    const Person& operator=(const Person& p){
        printf("170-----------operator=\n");
        return *this;
    }

    Person(const Person& p) {
        size_t n = strlen(p.name) + 1;
        name = new char[n];
        memcpy(name, p.name, n);
        printf("169------Person 拷贝构造\n");
    }

    ~Person(){ 
    	delete[] name; 
    	printf("172------Person 析构\n");
    }
};

Person getAlice(){
    Person p("alice");      // 对象创建。调用构造函数，一次 new 操作
    printf("190---------地址 = %p\n",  &p);
    return p;               // 返回值创建。调用拷贝构造函数，一次 new 操作
                            // p 析构。一次 delete 操作
    					    // 实际上  编译会对返回值做优化，所以并不会打印 一次普通可构造与一次拷贝构造
    						// 这种优化叫 NRVO 
}








int main() {


	//1字符串处理  假设右边的字符串中有好几个\0，strcpy函数只会拷贝第1个\0之前的内容，后面的内容不拷贝
	// char s[10];
	// char c[] = {'m', 'j', '\0', 'l', 'm', 'j', '\0'};
	// char* sp = s;
	// strcpy(s, c);
	
	// printf("s = %s   sizeof(s) = %lud sizeof(sp) = %d\n", s,sizeof(s),sizeof(sp));

	// //2 指针
	// char a = 10;
	// printf("修改前，a的值：%d\n", a);
	// //指针指向的为地址，所以将a 地址赋值给指针p
	// char *p = &a;
	// //修改指针p 指向内存地址的值，所以会改变a 的值
	// *p = 9;
	// printf("修改后，a的值：%d sizeof(p) = %lu \n", a,sizeof(p));

	// //指针变量指向的都是地址，所以大小 在同环境都一样的
	// int e = 23;
	// int* d = &e;
	// double f = 24;
	// double* g = &f;
	// printf("sizeof(d) = %d   sizeof(g) = %d\n",sizeof(d),sizeof(g));

	//3 需要注意
	//char a[] = "lmj";定义的是一个字符串变量！
	//char *p2 = "lmj";定义的是一个字符串常量！  实际的 是 const char* p2 = "lmj";



	//定义一个指针变量p，指向sum函数 函数指针  为指针 它指向一个函数
	// int (*func)(int a,int b) = sum;
 //    // 或者 int (*p)(int, int) = sum;
 //    // 或者 int (*p)() = sum;

 //    // 利用指针变量p调用函数
 //     int result = (*func)(1, 3);
 //     // 或者 int result = p(1, 3);
 //     printf("55----函数指针测试 %d\n", result);



 //     //函数指针作为参数
//      counting(sum,3,5);


 //     //5.结构体的使用
//      Student stu = {30};
//      printf("修改结构体前的实参：%d  stu 地址 = %p \n", stu.age,&stu);
//      testStruct(stu);
//	  printf("修改结构体后的实参：%d \n", stu.age);


	 //函数指针 作为形参时需要注意
	 // int(* funcp2)(int a,int b) = sum;
	 // printf("97------函数指针 sum %d\n", funcp2(3,4));
	 // testModifyFunc(funcp2);
	 // printf("97------func2地址 %p\n", &funcp2);
	 // printf("97------函数指针作为形参修改之后 sum %d\n", funcp2(3,4));



	 // int array[5] = {1,2,3,4,5};
	 // int* arrayPtr = array;
	 // printf("138------数组的地址：%p\n",array);
	 // testArrayAsParams(array,5);
	 // testArrayAsParams(arrayPtr,5);

	 //测试堆空间与栈空间
	 // char* stackPtr = testGetStackMem();
	 // if (stackPtr) {
	 // 	printf("138------栈空间已经回收所以看不到 testGetStackMem 返回的内容：%s\n",stackPtr);
	 // }

	 //  char* heapPtr = testGetHeapMem();
	 // if (heapPtr) {
	 // 	//strcpy(heapPtr,"hello world");
	 // 	printf("169------堆上空间拿到数据  所以这条日志能看到 拷贝内容：%d\n",*heapPtr);
	 // 	free(heapPtr);
	 // }

	 // char* stacicPtr = testGetStaicMem();
	 // if (stacicPtr) {
	 // 	printf("138------静态常量区空间没有回收，所以能看到 testGetStaicMem 返回的内容：%s\n",stacicPtr);
	 // }
	 

	 // char* arrayPtr2[] = {"A","B","C"};
	 // testPtrArray(arrayPtr2,sizeof(arrayPtr8));

	//测试右值引用
	// A a1 = getRValue();
	// printf("247-----------注意看析构函数a = %d   a 地址 = %p\n", a1.m_nAge, &a1);
	// A && a =  getRValue();
	// printf("243-----------测试使用右值引用可以减少一次临时对象的生成 ，注意看析构函数a = %d   a 地址 = %p\n", a.m_nAge, &a);



//	Person p = getAlice();
//	printf("299-----------= %p\n", &p);




	return 0;
}