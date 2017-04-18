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


//模版函数
template <typename T> T TAdd(T a, T b)
{
    return a + b;
}

//转换为某类型
float TGetValuedata[1024]={1,1,1};
template <typename T> T TGetValue(int i)
{
    return static_cast<T>(TGetValuedata[i]);
}


template<typename tStringType, typename tTraits = typename tStringType::traits_type>
void print_code_unit_sequence(tStringType str)
{
    using char_type = typename tTraits::char_type;
    static_assert(std::is_same<char_type, char>::value || std::is_same<char_type, char16_t>::value || std::is_same<char_type, char32_t>::value, "error");
    using unsigned_char_type = typename std::make_unsigned<char_type>::type;
    using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;
    int w = std::is_same<char, char_type>::value ? 2 : std::is_same<char16_t, char_type>::value ? 4 : 8;
    for(auto c : str) {
        auto value = static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
        std::cout << "0x" << std::hex << std::uppercase << std::setw(w) << std::setfill('0') << value << ' ';
    }
}


#endif /* templateTest_hpp */
