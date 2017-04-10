//
//  Binary.hpp
//  primer
//
//  Created by liweijian on 2017/4/10.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef Binary_hpp
#define Binary_hpp

//编译期方法
template <unsigned long N>
class Binary {
public:
    static unsigned const value = Binary<N/10>::value*2 + N%10;
};


//编译期方法，特化，终止条件
template <>
class Binary<0> {
public:
    static unsigned const value = 0;

};


//运行时方法
unsigned binary(unsigned long N) {
    unsigned result = 0;
    for (unsigned bit = 0x1; N; N/=10, bit<<=1) {
        if (N%10) {
            result += bit;
        }
    }
    return result;
}
#endif /* Binary_hpp */
