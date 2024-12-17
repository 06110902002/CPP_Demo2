#include<iostream>
#include<cstdio>
#include<stdio.h>
using namespace std;

//1. 使用结构体作为函数返回值
// 
typedef struct Student {

public:

    Student() {
        printf("13----student 构造 \n");
    }

	~Student() {
		printf("16------析构对象地址 = %p\n", this);
	}

	char name[50];
	int age;
	
} Student;


typedef struct Preson{

	float weight;
	int age;
	
} Preson;

Student getStudent(char* p, int age) {
	Student stu;      //临时变量 存放在栈区 函数作用域结束后  会被回收
	printf("16------打印参数 p = %s,  age = %d, 临时栈区对象stu 的地址 = %p\n",p,age,&stu );
	stu.age = age;
	strcpy(stu.name,p);

	return stu;       //return 有副本机制 会拷贝一份stu 返回给函数调用者
}

//返回结构体指针
Student* getStrcuPtr(char* p, int age) {
	Student stu;      //临时变量 存放在栈区 函数作用域结束后  会被回收
	printf("34------打印临时栈区对象stu 的地址 = %p\n",&stu );
	stu.age = age;
	strcpy(stu.name,p);
	Student* stuPtr = &stu;

	return stuPtr;       //return 有副本机制 此时拷贝的是 stu 栈上的内存地址
}

//返回结构体指针
Preson* getPersonPtr(float weight, int age) {
	Preson person;      //临时变量 存放在栈区 函数作用域结束后  会被回收
	person.age = age;
	person.weight = weight;

	Preson* pptr = &person;
	printf("50------打印临时栈区对象参数 weight = %f, age = %d,  person 的地址 = %p ,  pptr = %p \n",weight,age,&person,pptr);
	return pptr;       //return 有副本机制 此时拷贝的是 stu 栈上的内存地址
}

void testDuoPtr() {

	char arr[2][2] = {
		{'A','C'},
		{'B','D'}
	};

	printf("60------使用数组访问数组元素的情况 arr[1][1] = %c\n",arr[1][1]);

	char (*ptr)[2] = arr;

	printf("60------使用二级指针的访问数组元素情况 arr[1][1] = %c\n",*(*(ptr + 1) + 1));

	// char arr[2] = {'A','C'};
	// char* ptr= arr;
	// printf("60------使用二级指针的访问数组元素情况 arr[0][0] = %c\n",*(ptr + 1));

	//二级指针遍历数组
	char* er[] = {
		"A","C","B"
	};
	char** p = NULL;
	int arrSize = sizeof(er) - 1;
	for (p = er; p < er + arrSize; p ++) {
		printf("76------使用二级指针的访问数组元素情况 地址 = %p  值 = %s\n",p, *p);

	}

}

//测试二级指针 修改 函数指针的指向
int jia(int a,int b) {
	return a + b;
}

int jian(int a, int b) {
	return a - b;
}

/**
 * 将函数指针作为参数传递时 函数有副本机制，在函数内部修改，并不会影响外部实参的修改，
 */
void testFunPtrParams(int(*p)(int,int)) {
	printf("96------函数内部  函数指针p 的地址 = %p\n", &p);
	p = jian;
}

/**
 * 将函数指针p 的地址作为参数 ，然后修改其地址指向的内容  可以达到修改其指向的目的
 */
void testErJiPtr(int (**p)(int, int)) {
	*p = jian;
}
/**
 * 测试二级指针  实现拷贝字符串
 * @params str :源字符串
 * @params ptr : 目标字符串指针地址
 */
int testErJiPtrCopyStr(char **ptr,const char *str) {
    if (!str) str = "";
    strcpy(*ptr, str);
    printf("116------------testErJiPtrCopyStr  *ptr = %s\n",*ptr);
    *ptr += strlen(str) + 1;
    return 0;
}

int testErJiPtrCopyStr2(char *dst,const char *str) {
    if (!str) str = "";
    strcpy(dst, str);
    return 0;
}



class AVMessage {

public:
	int id;
	int age;
	int weight;

	AVMessage() {
		printf("92-----AVMessage()\n");
	}

	void test() {
		printf("79------age = %d  id = %d  weight = %d\n", id,age,weight);
	}
	~AVMessage() {
		printf("123------AVMessage 析构了\n");
	}
};

void testAvMsg(AVMessage* msg) {

	msg->age = 33;
	printf("130----testAvMsg msg address = %p,msg id = %d  age = %d\n",msg,msg->id,msg->age);
	//delete msg;

}

// 小结 
// 函数 只是将代码存放在代码区，即函数内对应的机器码存放代码区
// 如下面这个函数 只是将向 栈空间 申请avMsg 申请内存 空间的机器码放在代码区
// AVMessage avMsg;  创建一个对象会对应一些机器码，具体可以雪颜查看汇编代码
// 真正的avMsg 还是存放在栈空间的
// void test(int a, int b,int c) {
// 	AVMessage avMsg;
// 	av.id = a;
// }

//看到67节处
class VideoMsg : AVMessage{
public:
	void t() {
		printf("116---------%d\n",id);
	}
};

//class 默认为私有继承
class AudioMsg : VideoMsg {
public:

	int sampleRate;

	// AudioMsg():AudioMsg(44100) {

	// 	//不能使用如下 方式调用
	// 	//AudioMsg(44100);

	// }

	AudioMsg(int sampleRate) {

		this->sampleRate = sampleRate;
	}

	void t2() {
		//此处 因为是私有私有继承  所以编译时 因为id 因为权限访问不了
		//printf("123---------%d\n",id);
	}

	void testConstruct() {
		printf("141---------sampleRate = %d\n",sampleRate);
	}

};


//测试多态的实现
class Animal {

public:

	virtual void speak() {
		printf("152------Animal %s\n",__func__);
	}

	virtual void run() {
		printf("156------Animal %s\n",__func__);
	}
};

class Cat : public Animal {

	int m_nAge;
	int m_nLift;

	void speak() {
		printf("163------Cat %s\n",__func__);
	}

	void run() {
		printf("167------Cat %s\n",__func__);
	}
};

class Dog : public Animal {

	void speak() {
		printf("174------Dog %s\n",__func__);
	}

	void run() {
		printf("174------Dog %s\n",__func__);
	}
};


void testDuoTai(Animal *animal) {

	if (animal) {
		animal->speak();
		animal->run();
	}
}



/**
 * C++ 类中静态成员变量 初始化 时  需要在类的外面进行
 * 同时静态成员变量不属于对象
 */
class TestStatic {
	public:
		static int age;

	private:
		char* name;
};
int TestStatic::age = 21;

//拷贝构造：复用已经存在对象构造一个新对象  会调用拷贝构造
//拷贝生成的新对象  与被拷贝的对象 成员变量完全一样
//通俗讲 就是将一个对象拷贝一份
//注：使用拷贝构造时 需要注意 浅拷贝的情况出现
class Car {

	int m_price;
	int m_length;
	char* m_name;

public:

	Car(int price,int length):m_price(price),m_length(length) {
		printf("226----car Construct \n");
	}

	//拷贝构造函数 格式固定
	//如果声明拷贝构造函数  必须将  需要被拷贝对象的成员 进行显示拷贝操作
	//否则拷贝会失败
	//2.如果未声明 也会拷贝成功  即默认也会拷贝成功
	// Car(const Car& car) {
	// 	printf("232------this is copy construct\n");
	// 	m_price = car.price;
	// 	m_length = car.length;
	// }


	 // Car(const Car& car) : m_price(car.m_price),m_length(car.m_length) {
	 // 	if (car.m_name) {
	 // 		//申请新的内存空间  实现深拷贝，因为m_name 有可能指向一个栈空间对象
	 // 		m_name = new char(strlen(car.m_name) + 1);
	 // 		strcpy(m_name,car.m_name);
	 // 	}

	 // }

};

//拷贝构造 继承 中的使用

class Company{
	
public:
	int m_age;
	Company(int age = 0) : m_age(age) {

	}
	//声明拷贝构造
	Company(const Company& complay) : m_age(complay.m_age) {

		printf("316------company 拷贝构造 调用\n");

	}
};

class Nio : public Company {

public:
	int m_type;
	Nio(int age = 0,int type = 0) : m_type(type),Company(age) {

	}

	//声明拷贝构造
	// Nio(const Nio& nio) : m_type:(nio.m_type) {

	// }

	//如果通过拷贝构造函数 构造对象时  想使用父类的m_age 成员  拷贝函数如下 
	Nio(const Nio& nio) : m_type(nio.m_type),Company(nio) {

		printf("272----调用拷贝构造函数\n");
	}

};

//测试指针作为 函数参数的 副本机制

class Book {

public:
	char* m_name;
	Book(char* name) {
		m_name = name;
	}

};

void testPtrAsFuncParams(Book* book) {
	printf("304------testPtrAsFuncParams book 地址 = %p\n",&book);
	book = new Book("English");
	printf("306------testPtrAsFuncParams book 地址 = %p  name = %s\n",book,book->m_name);
}
//测试指针作为 函数参数的 副本机制

//编译器 在以下情况下会为类生成构造函数
// 1. 成员变量初始化了
// 2. 有虚函数
// 3. 有虚继承
// 4. 这个类的成员变量 包含另一个类 且这个类 有显示声明构造函数
// 看到 102 节处 adb shell am start -n com.android.relay.service/com.android.relay.test.MainActivity
//adb shell am start -n com.alps.iqiyi/com.alps.video.play.VideoPlayActivity

// 模版  声明与实现不能分离的原因为：
// 模版实现的函数在没有调用时并不会生成函数,也即不会有函数地址
// 所有 在调用时 通过函数地址 链接不到 函数的实现
// 但是直接在写在头文件中 是可以的，因为其他地方调用进行include 时  是直接将这个头文件
// 拷贝过来的，即将模版函数实现也拷贝过来了，调用的时候就会生成相应的函数，连接阶段就能
// 对应的函数的实现
// 106 节 有提到 不能分离的原因

uint8_t* int2Bytes(int i) {
    uint8_t* buf = (uint8_t *) malloc(4 * sizeof(uint8_t));
    //unsigned char buf[4] = {0};
    buf[0] = (unsigned char) i;
    buf[1] = i >> 8;
    buf[2] = i >> 16;
    buf[3] = i >> 24;
    return buf;

}

int byte2Int(uint8_t *bytes) {
    int value;
    value = (int) ((bytes[0] & 0xFF)
                   | ((bytes[1] & 0xFF) << 8)
                   | ((bytes[2] & 0xFF) << 16)
                   | ((bytes[3] & 0xFF) << 24));
    return value;
}

//智能指针 没必要指向 栈对象 ，指向栈对象的情况可能导致多次析构
// 多次析构的原因：栈对象 离开作用域自析构一次   智能指针  离开作用域也会导致 指向的对象析构一次
// 注意：
// 智能指针  其自身就是在栈上  创建了一个对象，然后用这个对象 来指向堆空间,当智能指针 这个栈对象离开作用域时
// 销毁自身时  连同去销毁其指向的堆空间对象，如果 引入了 引用计数  那么这时 对引用计数进行 减1 操作
// 等于0 时  将指向的堆空间 销毁 即可
int byteutils_get_int(unsigned char* b, int offset) {
    return ((b[offset + 3] & 0xff) << 24) | ((b[offset + 2] & 0xff) << 16) | ((b[offset + 1] & 0xff) << 8) | (b[offset] & 0xff);
}

short byteutils_get_short(unsigned char* b, int offset) {
    return (short) ((b[offset + 1] << 8) | (b[offset] & 0xff));
}

float byteutils_get_float(unsigned char* b, int offset) {
    //unsigned char tmp[4] = {b[offset + 3], b[offset + 2], b[offset + 1], b[offset]};
    return *((float *)(b + offset));
}

void getMem(char* p,int num) {
	//printf("115  p   addr = %p\n", &p);
	p = (char*) malloc(sizeof(char) * num);

}




int main() {


//	char* name = "liu";
//	Student stu  = getStudent(name,23);
//	printf("241------测试结构体作为函数参数  name = %s  age = %d  stu 地址 = %p \n",stu.name,stu.age,&stu);

	//测试结构体指针  作为 函数返回值
	// char* name2 = "liuxiaobing";
	// Student* stuPtr  = getStrcuPtr(name2,25);
	// printf("44------测试结构体指针作为函数参数  stuPtr->name = %s  stuPtr->age = %d  stuPtr 地址 = %p \n",stuPtr->name,stuPtr->age,stuPtr);

	
//	 Preson* pptr  = getPersonPtr(120.0f,45);
//	 printf("57------测试结构体指针作为函数参数  pptr->weight = %f  pptr->age = %d  pptr 地址 = %p \n",pptr->weight,pptr->age,pptr);


	//测试多级指针
	//testDuoPtr();
	// int (*p)(int,int) = jia;
	// printf("388------ 函数指针初始值 结果  = %d  地址 = %p\n",p(10,20),&p);
	// testFunPtrParams(p);
	// printf("397------ 修改函数指针后的结果  = %d\n",p(10,20));
	// //将函数指针p 的址传进去，	然后修改其地址指向的内容，可以达到  修改其指向的目的
	// testErJiPtr(&p);
	// printf("401------ 将函数指针p 的地址作为参数 ，然后修改其地址指向的内容  的结果= %d\n",p(10,20));


	//
	// AVMessage avMsg;
	// avMsg.age = 21;
	// avMsg.id = 2022;
	// avMsg.weight = 120;

	// // 指针访问成员变量的本质 这里需要注意
	// // avMsgPtr 指向 avMsg.age 的地址
	// // AVMessage* avMsgPtr = avMsg； 这行代表avMsgPtr 指向avMsg 起始地址
	// // 在对象中  使用指针访问时  这里需要注意，看看指针指向的址是哪个
	// AVMessage* avMsgPtr = (AVMessage*)&avMsg.age;
	// // 此时 2021 赋值给 avMsgPtr 起始地址 + 4 因 id 占用4字节
	// // 所以它向偏移4字节
	// avMsgPtr->id = 2021;
	// // avMsgPtr 再偏移4 字节
	// avMsgPtr->age = 150;
	// avMsgPtr->test();

	//测试C++ 当中  class 继承默认为私有
	// struct 为默认为  public 继承
	// AudioMsg audio;
	// audio.t2();

	// AudioMsg* audio2 = new AudioMsg();
	// audio2->testConstruct();

	// Animal* animal = new Cat();
	// testDuoTai(animal);
	//对于有虚函数而言此时cat最前面4/8个字节（4 = 32bit 系统  8 = 64bit） 存入虚表地址
	//所以较它的大小 = sizeof(虚表地址) + sizeof(成员变量)
	//printf("241-----sizeof(Cat)= %lu\n",sizeof(Cat));

	// AVMessage* avMsg = new AVMessage();
	// avMsg->age = 21;
	// avMsg->id = 2022;
	// avMsg->weight = 120;
	// printf("448----------avMsg address = %p\n", avMsg);

	// testAvMsg(avMsg);

	//printf("451------after avMsg age = %d\n", avMsg->age);

	//测试 类 静态成员就是
	// TestStatic::age = 23;
	// printf("281-------test class static member age = %d\n", TestStatic::age);


	//测试拷贝构造函数
	// Car car3(23,500);
	// Car car4(car3);
	// printf("314-----copy construct price = %d\n", car4.m_price);

	//测试拷贝构造 函数的在继承中的使用
	// Nio nio(7,2000);
	// //调用拷贝构造 方式一
	// Nio nio2(nio);
	// //调用拷贝构造 方式二
	// Nio nio3 = nio;

	// printf("测试拷贝构造函数在继承中的使用 nio2.m_age = %d  nio2.m_type = %d\n", nio2.m_age,nio2.m_type);

	// Book* book = new Book("Chiense");
	// printf("392--------book address = %p  name = %s\n",&book,book->m_name);
	// testPtrAsFuncParams(book);
	// printf("394--------afters testPtrAsFuncParams book address = %p  name = %s\n",book,book->m_name);

	// const char* s1 = "123";
	// const char* s2 = "abc";
	// char* s3;
	// sprintf(s3, "%s%s", s1, s2);

	// char str1[10];
	// char str2[10] = "999";
	// strcpy(str1, "11");            // 先拷贝
	// strcat(str1, str2);            // 再追加
	// printf("414----------%s\n", s3);
	

	//指针类型  与大小  注意 a *a,&a 区别
	// int a[2][3] = {
	// 	1,2,3,
	// 	4,5,6
	// };
	// int (*b)[3] = a;
	// //printf("445 ------ a = %p  *a = %p &a = %p\n",a,*a,&a); 
	// printf("445 ------ *a = %d  *&a = %d **a = %d \n",sizeof(*a),sizeof (*&a),sizeof(**a)); 
	// printf("446 ------ *a = %d  *&a = %d **a = %d \n",*a,*&a,**a); 
	// printf("453----使用指针访问二维数组\n");
	// for (int i = 0; i < 2; i ++) {
	// 	for(int j = 0; j < 3; j++) {
	// 		//printf("%3d", *(a[i] + j));
	// 		printf("%3d", *(*(a + i) + j));
	// 	}
	// 	printf("\n");
	// }



	// const char* str1 = "123";
	// char ctrl_path[64] = "";
	
	// char* ss = ctrl_path;
	
	// char** result = &ss;
	// printf("531-------ctrl_path add = %p   ss value = %d   &*result = %d\n", ctrl_path,ss,*result);
	// testErJiPtrCopyStr(result,str1);
	// printf("537-------ctrl_path add = %p   ss value = %d   &*result = %d\n", ctrl_path,ss,*result);

	// printf("528------拷贝后   ss = %s   ctrl_path add = %p  \n", ss,ctrl_path);




	// char ss[20] = {0};
	// char* ptr = ss;
	// char** ptr2 = &ptr;

	// //strcpy(ss,"abc");
	// testErJiPtrCopyStr2(ptr,"liu");

	// printf("530----------ptr = %s   ss = %s  ptr2 = %s\n",ptr,ss,*ptr2);

	// unsigned char bytes[] = {0x01,0x01};


	// short value = (short) ((bytes[0] & 0xFF)
 //                   | ((bytes[1] & 0xFF) << 8));

	//  short value2 = (short) (((bytes[0] & 255) << 8) + (bytes[1] & 255));

 //     printf("555--------:%d   value2 = %d \n", value,value2);  

	// int a = 34;
	// int* aptr = &a;
	
	// printf("562------a = %d   aptr =  %d\n",*aptr / 8,(*aptr) / 8 );

	// clock_t parse_start_time = clock();
	// unsigned char packet[] = {2,0,0,0,2,1,3,0};
	// int payloadsize = byteutils_get_int(packet, 0);
 //                // FIXME: 这里计算方式需要再确认
 //    short payloadtype = (short) (byteutils_get_short(packet, 4) & 0xff); // 取2 byte  得到类型
 //    short payloadoption = byteutils_get_short(packet, 6);  // 取2 byte, 如果类型= 2，则为0x1e，否则为6
	
	// clock_t parse_finish_time = clock();
 //    printf("583-----payloadsize = %d,  type = %d, parse time = %f(ms)",payloadsize,payloadsize,(double)(parse_finish_time - parse_start_time));

 
   

	// int* ptr = new int(10);
 //    int* ref = ptr;
 //    delete ptr;

 //    printf("598------------ref = %d\n", *ref);






return 0;
}




















