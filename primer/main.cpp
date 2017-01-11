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
#include <fstream>
#include <algorithm>
#include <numeric>
#include <list>
#include <iterator>
#include <exception>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::regex;
using std::cmatch;
using std::sregex_token_iterator;
using std::thread;
using std::ifstream;
using std::list;

//可以处理空串
vector<string> split_substr(string src, string pattern)
{
    cout<<"call "<<__func__<<endl;
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
    cout<<"call "<<__func__<<endl;

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
    cout<<"call "<<__func__<<endl;

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
    cout<<"call "<<__func__<<endl;

    for (typename vector<T>::const_iterator iter = ve.begin(); iter != ve.end(); iter++) {
        cout<<*iter<<endl;
    }
    
    cout<<endl;
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

constexpr int bufsize()
{
    return 1024;
}




class StrBlob
{
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    
    // 操作
    void push_back(const std::string &t) {data->push_back(t);}
    void pop_back() const;
    
    // 访问
    std::string& front();
    std::string& back();
private:
    std::shared_ptr<std::vector<std::string>> data;
    // throws msg if data[i] isn't valid
    void check(size_type i, const std::string &msg) const;
};

StrBlob::StrBlob(): data(std::make_shared<vector<string>>()) { }
StrBlob::StrBlob(std::initializer_list<string> il):
data(std::make_shared<vector<string>>(il)) { }

void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}

void StrBlob::pop_back() const
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}




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
    

    //编译期不报错，但是运行期不可知
    string nulls;
    cout<<nulls[0]<<nulls[10]<<endl;
    
    //迭代器
    const string cs = "keep clarms";
    for (auto &c : cs) {
        cout<<" "<<c;
    }
    cout<<endl;
    
    
#ifdef TESTCIN
    vector<int> vi;
    int i;
    while(cin>>i) {
        vi.push_back(i);
        if (0==i) {
            break;
        }
    }
    
    vector<string> vs;
    string is;
    while (cin>>is) {
        vs.push_back(is);
        if (is.length() == 0) {
            break;
        }
    }
    
//    迭代器循环中修改容器长度，危险： 添加不行，删除危险，要看删除一个节点对另外节点有无影响
//    list，map，set时删除
//    http://mp.weixin.qq.com/s/qonNrKKZLladvIwvurQCTA
    for (auto it = vs.begin(); it != vs.end(); it++) {
        if (*it == "hi") {
            //vs.push_back("HI");  // 发生闪退
            it = vs.erase(it);
        }
    }
    
#endif
    
    //数组声明, buf和buf2 都需要 constexpr ， 否则编译不报错，但运行异常
    constexpr unsigned buf_size = 1024;
    int buf[buf_size]; buf[0]=1;
    int buf2[bufsize()]; buf2[0]=1;
    int buf3[4*7-1];    buf3[0]=1;
    //int buf4[2]="hi";// error
    
    
    //初始化vector,通过指针，可以访问到数组之外的地址
    const char ca[] = {'h','e','l','l','o'};
    const char c = 'e';
    char cb[] = {'a','b','c'};
    vector<char> vca(std::begin(ca), std::end(ca));
    const char *p = ca;
    while (*p) {
        cout<<*p<<endl;
        ++p;
    }
    
    
    //c++ 14声明新用法
    int num14 = 123'324'230;
    int onum14 = 0b00000001;
    
    
    //智能指针
    std::shared_ptr<string> pstr  = std::make_shared<string>(10,'a');
    cout << pstr.unique() <<endl;
    cout << pstr.use_count() <<endl;

    std::shared_ptr<string> pstr2(pstr);
    cout << pstr.unique() <<endl;
    cout << pstr.use_count() <<endl;

    
    //endl, ends, flush ;  ends 行为要看系统
    cout<<"hi"<<endl;
    cout<<"hi"<<std::ends;// mac 下没有空格，也没有刷新
    cout<<"hi"<<std::flush;
    cout<<"hi"<<std::ends;
    cout<<endl;
    
    
    //通过流>>读文件
    ifstream in("/Users/liweijian/Code/c++/primer/primer/primer/main.cpp");
    vector<string> words;
    if (in.is_open()) {
        string s;
        while (in >> s) {
            //cout<<s;
            words.push_back(s);
        }
    }
    
    //通过流iterator来读文件
    ifstream fin("/Users/liweijian/Code/c++/primer/primer/primer/main.cpp");
    std::istream_iterator<string> initer(fin);
    vector<string> fsv;
    std::istream_iterator<string> eof;
    while (initer != eof) {
        fsv.push_back(*initer++);
    }
    helper_print_vc(fsv);
    
    
#ifdef TESTCIN
    //使用流迭代器， sort， copy， 从标准输出流读取整数序列，将其排序后输出到标准输出
    std::istream_iterator<int> stdiniter(cin), eofstdin;
    vector<int> vnums(stdiniter,eofstdin);
    sort(vnums.begin(), vnums.end(), [](int a, int b){ return b<a;});
    copy(vnums.begin(), vnums.end(), std::ostream_iterator<int>(cout," "));
    cout<<endl;
    unique_copy(vnums.begin(), vnums.end(), std::ostream_iterator<int>(cout," ")); //只输出唯一的
    cout<<endl;
    vector<int>::iterator itervnums = vnums.end();
    //使用普通迭代器，逆序输出
    while (itervnums != vnums.begin()) {
        cout<<*(--itervnums)<<" ";
    }
    cout<<endl;
#endif
    
    //swap 函数
    vector<string> vc1(3,"vc1");
    vector<string> vc2(3,"vc2");
    vector<string>::iterator iter = vc1.begin();
    cout<<*iter<<endl;
    std::swap(vc1,vc2);
    cout<<*iter<<endl;
    cout<<*(++iter)<<endl;
    cout<<*(++iter)<<endl;
    cout<<*(++iter)<<endl;


    // reserve只是增加了可用空间，但元素还是0. fill是更新已有的元素值。
    vector<int> vint;
    helper_print_vc(vint);
    vint.reserve(10);
    fill_n(vint.begin(), 5, 5);
    helper_print_vc(vint);
    vint = {1,2,3,5};
    
    
    //copy 函数
    list<int> lint;
    copy(vint.begin(), vint.end(), lint.begin()); //如果lint没有元素，调用此方法无效,需要调用back_inserter
    copy(vint.begin(), vint.end(), back_inserter(lint));
    
#define PRINTLIST do{ list<int>::iterator iterlint = lint.begin();\
                  while (iterlint != lint.end()) {            \
                        cout<<*iterlint++<<endl;              \
                  } cout<<endl; } while (0);

    PRINTLIST
    fill(lint.begin(), lint.end(), 3);
    PRINTLIST
    
    
    //通过指针，两个对象指向同一数据
    StrBlob v1;
    {
        StrBlob v2{"a","2","3"};
        v1 = v2;
    }
    v1.pop_back();
    
    return 0;
    
}


