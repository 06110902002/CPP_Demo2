cmake_minimum_required(VERSION 3.23)
project(CPP_Test1)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fno-elide-constructors) #关闭编译器优化

include_directories(.)

add_executable(CPP_Test1
        #CPP_test1.cpp
        #CPP_test2.cpp
        #CPP_test3.cpp
        List.h
        log.h
        #CPP_test4.cpp
        #CPP_test5.cpp
        #CPP_test6.cpp
        log.h
        #FunctionTraits.cpp
        #makeIndexSequence.cpp #需在 C++ 17 运行
        #TypeName.cpp
#        enable_if_test.cpp
#        msgpack.hpp
        #special.cpp
#        timer.cpp
#        TimeNode.cpp
#        TimeNode.h
        LeetCode.cpp
        #sharePtr.cpp
        #DataSruct.cpp
        datastruct/LinkList.cpp
        datastruct/LinkList.h
        datastruct/TreeNode.h
        #datastruct/WeakPtr.h
        )
