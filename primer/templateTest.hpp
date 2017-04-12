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

#endif /* templateTest_hpp */
