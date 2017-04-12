//
//  templateTest.hpp
//  primer
//
//  Created by liweijian on 2017/4/12.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef templateTest_hpp
#define templateTest_hpp

#include<iostream>

using std::cout;
using std::endl;

template <typename T>
class TraitsHelper {
public:
    static const bool isPointer = false;
};

template <typename T>
class TraitsHelper<T*> {
public:
    static const bool isPointer = true;
};

template <typename T>
class TestTemplate {
public:
    void func() {
        if(TraitsHelper<T>::isPointer) {
            cout<<"is pointer"<<endl;
        } else {
            cout<<"not pointer"<<endl;
        }
    }
};


template <typename T>
struct TraitsHelper2 {
    typedef T ret_type;
    typedef T par_type;
};
template <>
struct TraitsHelper2<int> {
    typedef int ret_type;
    typedef int par_type;
};
template <>
struct TraitsHelper2<float> {
    typedef float ret_type;
    typedef int par_type;
};

template <typename T>
class Test {
public:
    typename TraitsHelper2<T>::ret_type Compute(typename TraitsHelper2<T>::par_type d);
private:
    T mData;
};

#endif /* templateTest_hpp */
