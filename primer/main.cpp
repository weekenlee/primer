//
//  main.cpp
//  primer
//
//  Created by liweijian on 2016/12/19.
//  Copyright © 2016年 liweijian. All rights reserved.
//

#include <iostream>
#include <memory>
#include <vector>
#include <regex>
#include <thread>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::regex;
using std::cmatch;
using std::sregex_token_iterator;
using std::thread;

//可以处理空串
vector<string> split_substr(string src, string pattern)
{
    vector<string> result;
    string::size_type pos = 0;
    src += pattern;
    string::size_type size = src.size();
    
    for (string::size_type i = 0; i < size;  i++) {
        pos = src.find(pattern, i);
        if (pos < size) {
            string s = src.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    
    return result;
}


//处理不了空串的情况
void split_findnf(const string& src, vector<string>& tokens, const string& delimeter)
{
    string::size_type lastPos = src.find_first_not_of(delimeter,0);
    string::size_type pos = src.find_first_of(delimeter, lastPos);
    
    while (string::npos != lastPos || string::npos != pos) {
        tokens.push_back(src.substr(lastPos, pos - lastPos));
        lastPos = src.find_first_not_of(delimeter, pos);
        pos = src.find_first_of(delimeter, lastPos);
    }
}


//处理不了连续空串
void split_regex(const string& src, vector<string>& tokens, const string& delimeter)
{
    regex reg(delimeter);
    sregex_token_iterator it(src.begin(), src.end(), reg, -1);
    sregex_token_iterator end;
    while (it != end) {
        tokens.push_back(*it++);
    }
}


//helper
template <typename T>
void helper_print_vc(const vector<T>& ve)
{
    for (typename vector<T>::const_iterator iter = ve.begin(); iter != ve.end(); iter++) {
        cout<<*iter<<endl;
    }
}


//函数线程任务
int count = 0;      //共享资源
std::mutex mtx;     //共享锁

void thread_task() {
    cout << "call func from thread:"<< std::this_thread::get_id() << endl;

    std::unique_lock<std::mutex> lck (mtx); //上锁

    for (int i=0; i<100; i++) {
        cout<<count++<<endl;
    }
}

class threadTask {
public:
    void operator() () const {
        cout << "call func object from thread:"<< std::this_thread::get_id() << endl;
        
        std::unique_lock<std::mutex> lck (mtx); //上锁

        for (int i=0; i<100; i++) {
            cout<<count++<<endl;
        }
    }
};


int main(int argc, const char * argv[]) {
    
    /*
     * 分隔字符串， substr，find_first_of , regex 方法实现
     *
     */
    string s = "hello, world, , hi, nihao ,,";
    
    vector<string> r = split_substr(s, ",");
    helper_print_vc(r);
    
    vector<string> r2;
    split_findnf(s, r2, ",");
    helper_print_vc(r2);

    vector<string> r3;
    split_regex(s, r3, ",");
    helper_print_vc(r3);

    
    /*
     * 多线程例， 函数指针、函数对象、lambda实现
     *
     */
    cout << "main thread:"<< std::this_thread::get_id() << endl;
    
    thread t(thread_task);
    thread t3([] {
        cout << "call lambda from thread:"<< std::this_thread::get_id() << endl;
        
        std::unique_lock<std::mutex> lck (mtx); //上锁

        for (int i=0; i<100; i++) {
            cout<<count++<<endl;
        }
    });
    thread t4{threadTask()};
    
    t.join();
    t3.join();
    t4.join();
    

    return 0;
}


