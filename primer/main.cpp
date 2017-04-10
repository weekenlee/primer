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
#include <chrono>
#include <stdexcept>
#include <map>
#include <unordered_map>
#include <cassert>
#include <bitset>
//#include <cstdlib>

#include "StrBlob.hpp"
#include "ThreadRun.hpp"
#include "func.hpp"
#include "boostpra.hpp"
#include "TaskQueue.hpp"
#include "Binary.hpp"

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
using namespace std::placeholders;
using namespace lwj;


make_global_functor(add_functor, add);

constexpr int testTemplateBinary()
{
    return 10;
}

//callback 回调
//c方式
typedef void (*EventCallback)(const char* str, bool ok);

void eventHappen(EventCallback callback)
{
    cout<<"event happen"<<endl;
    string result="result";
    callback(result.c_str(), true);
}

void callback(const char* str, bool ok)
{
    cout<<"call callback:"<<str<<endl;
}


//回调 c++ interface 形式
class Icallback
{
public:
    //可以放一组函数
    void callbackfunc() const { cout<<"c++ interface callback happy"<<endl;}
};

class event
{
public:
    event():pcb(new Icallback){}
    void eventhappen() const
    {
        cout<<"c++ interface evnet happen"<<endl;
        pcb->callbackfunc();
    }
    ~event(){delete pcb; pcb = nullptr;}
private:
    Icallback *pcb;
};


//回调， c++ 模版形式
template <class Class, typename ReturnType, typename Param>
class MyCallBack {
    
    typedef ReturnType (Class::*Method)(Param);
    
    Class *pclass;
    Method method;
    
public:
    MyCallBack(Class * _class_instance, Method _method):pclass(_class_instance), method(_method)
    {}
    
    ReturnType operator()(Param para)
    {
        return (pclass->*method)(para);
    }
    
    ReturnType execute(Param para)
    {
        return operator()(para);
    }
};


//转换为大写
char my_toupper(char c)
{
    std::locale loc;
    return std::use_facet<std::ctype<char>>(loc).toupper(c);
}

//打印智能指针
void process(std::shared_ptr<int> ptr);


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


template <typename T>
void helper_print(const T &ve)
{
    cout<<"call "<<__func__<<endl;
    
    for (auto const &a : ve) {
        cout<<a<<" ";
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





//13.4
class Point
{
public:
    int T;
public:
    static int total;
    Point(int t=0);
    
    Point(const Point&a) {T=a.T+1;  cout<<total++<<"复制构造函数"<<T<<endl;}
    ~Point(){cout<<total++<<"析构函数"<<T<<endl;}
    
    void print() const {cout<<"Point T is: "<<T<<endl;}
    
};
int Point::total=1;
Point::Point(int t):T(t){cout<<total++<<"构造函数"<<T<<endl;}


//习题13.4
Point global;  //默认构造函数

Point foo_bar(Point arg)   //复制构造函数
{
    Point local = arg;     //复制构造函数
    Point*heap =new Point(global);   //复制构造函数
    *heap= local;
    Point pa[ 4 ] = { local, *heap };   //复制构造函数，复制构造函数，默认构造函数，默认构造函数
    return *heap;      //复制构造函数
}



void threadfunc_dieafter1min()
{
    try {
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
        throw std::runtime_error("a error");
    } catch (...) {
        
    }

}


// 消费者线程函数.
void consume(TaskQueue& queue)
{
    while (true) {
        Task t = queue.popTask();
        t.doWork();
    }
  
}


//生产者线程函数
void produce(TaskQueue& queue)
{
    while (1) {
        for (int i=0; i<10; i++) {
            Task t;
            queue.addTask(t);
        }
    }
}


//删除重复项目
void elimDumps(vector<string> &words)
{
    sort(words.begin(), words.end());
    auto end_unique = std::unique(words.begin(), words.end());
    words.erase(end_unique, words.end());
}



int main(int argc, const char * argv[]) {
  
  #ifdef TESTCIN
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
    StrBlob::f(v1);
    v1.f(v1);//类方法也可以通过对象调用，也可以用类调用

    
    //智能指针
    std::shared_ptr<int> ptr(new int(16));
    process(ptr);
    cout << "outside function: "<<ptr.use_count()<<endl;
    process(std::shared_ptr<int>(ptr));
    cout << "outside function: "<<ptr.use_count()<<endl;
    //process(std::shared_ptr<int>(ptr.get())); //闪退
    //cout << "outside function: "<<ptr.use_count()<<endl;
    
    auto pp = new int(1);
    auto sp = std::make_shared<int>(1);
    //process(pp);//报错
    process(sp);
    cout << "outside function: "<<sp.use_count()<<endl;
    //process(new int());//报错
    process(std::shared_ptr<int>(pp));

    auto ppp = sp.get();
    //delete ppp;//报错，pointer being freed was not allocated
    
    
    Point point(1);
    Point point2(point);
    foo_bar(point);
   
    
    //thread
    std::chrono::milliseconds dura(200);
    std::this_thread::sleep_for(dura);
    
    //stl算法
    int aa[]={1,2,3,4,2,1,2};
    vector<int> va(aa,aa+7);
    std::sort(va.begin(), va.end());
    std::for_each(va.begin(), va.end(), [](int a){cout<<a<<" ";});
    cout<<"find result: "<<std::binary_search(va.begin(), va.end(), 5)<<endl;
    cout<<"any bigger than 1: "<<std::any_of(va.begin(), va.end(), [](int a){ return a>1;})<<endl;
    cout<<"all bigger than 1: "<<std::all_of(va.begin(), va.end(), [](int a){ return a>1;})<<endl;
    std::reverse_copy(va.begin(), va.end(), std::ostream_iterator<int>{std::cout, " "});
    cout<<endl;
    
    
    //翻转str
    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();

    string mystr = "你_b_c_d_e";
    string::size_type pos = mystr.find_first_of("_");
    string str1 = mystr.substr(0,pos);
    string str2 = mystr.substr(pos+1, mystr.length());
    string revertstr = str2+"_"+str1;
    cout<<revertstr<<endl;
    
    std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();

    std::this_thread::sleep_for(std::chrono::microseconds(50000));
    
    cout << (tp2-t1).count()<<" tick count"<<endl;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(tp2-t1).count()<<" microseconds"<<endl;

    
    //bind
    //search 容器里查找子序列，vector也可以用
    string sss("Internationalization");
    string sssub("Nation");
    string::iterator ssspos;
    ssspos =  std::search(sss.begin(), sss.end(),
                       sssub.begin(), sssub.end(),
                       std::bind(std::equal_to<char>(),
                                 std::bind(my_toupper,_1),
                                 std::bind(my_toupper,_2))
                       );
    if (ssspos != sss.end()) {
        cout << "\"" << sssub << "\" is part of\"" << sss << "\""
        << endl;
    }
    
    //bind 成员函数
    vector<Point> points{1,2,3};
    std::for_each(points.begin(), points.end(), std::bind(&Point::print, _1));
    std::for_each(points.begin(), points.end(), [](Point &p){p.print();});

    
    //c方式回调
    EventCallback cbfunc = callback;
    eventHappen(cbfunc);
    
    //c++ interface方式回调
    event e;
    e.eventhappen();
    
    
    //c++ template 形式回调
    class A
    {
        
    public:
        
        void output()
        {
            std::cout << "I am class A :D" << std::endl;
        };
        
    };
    
    class B
    {
        
    public:
        
        bool methodB(A a)
        {
            std::cout << "I am class B :D" << std::endl;
            a.output();
            return true;
        }
        
    };
    
    A a;
    B b;
    MyCallBack<B, bool, A> *mycb;
    mycb = new MyCallBack<B,bool,A >(&b, &B::methodB);
    if((*mycb)(a))
    {
        std::cout << "CallBack Fired Successfully!" << std::endl;
    }
    else
    {
        std::cout << "CallBack Fired Unsuccessfully!" << std::endl;
    }
    
    
    //模拟线程
    ThreadRun tr;
    tr.start();
    

    cout<<BOOST_VERSION<<endl;

    
    std::function<int(int,int)> f =  std::bind(add_functor, _1, _2);
    cout<< add_functor(1,2) <<endl;
    cout<< f(2,3) <<endl;
    
    boosttimer();
    

    try {
        std::thread t(threadfunc_dieafter1min);
        t.join();

        threadfunc_dieafter1min();
        throw std::runtime_error("error");  //主线程异常捕获

    } catch (...) {
        
    }
    
    sleep(10);
    

//    单线程下
//    TaskQueue q;
//    for (int i=0; i<10; i++) {
//        Task t;
//        q.addTask(t);
//    }
//    
//    while (!q.empty()) {
//        Task t = q.popTask();
//        t.doWork();
//    }
    
    TaskQueue q;

    for (int i=0; i<10; i++) {
        std::thread consumer_thread(consume, std::ref(q));
        consumer_thread.detach();
    }
    
    for (int i=0; i<5; i++) {
        std::thread produce_thread(produce, std::ref(q));
        produce_thread.join();
    }


    auto start = std::chrono::high_resolution_clock::now();
    sleep(10);
    auto end = std::chrono::high_resolution_clock::now();
    cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << ":";
    return 0;
    
    std::vector<int> vectora{1,2,3,4,54};
    int sum = std::accumulate(vectora.cbegin(), vectora.cend(), 0);
    cout<<sum<<endl;
    
    std::vector<double> vectorb{1.1,2.2,3.0,4.8,54.5};
    double sumb = std::accumulate(vectorb.cbegin(), vectorb.cend(), 0);  //accumulate 第三个参数决定返回值类型，0返回64，否则返回正确浮点数
    cout<<sumb<<endl;
    
    std::vector<const char*> vectorc{"abc","efg"};
    std::vector<const char*> vectord{"abc","e4g"};
    cout<<std::equal(vectorc.cbegin(), vectorc.cend(), vectord.cbegin())<<endl;
    
    std::vector<int> vectore{1,2,3};
    std::fill_n(vectore.begin(), 2, 5);
    helper_print_vc(vectore);
    auto bi = std::back_inserter(vectore);
    *bi = 19;
    helper_print_vc(vectore);
    
    //10.7 a
    vector<int> vecint;
    list<int> listint;
    int input;
    while (cin>>input) {
        listint.push_back(input);
    }
    vecint.resize(listint.size());// 若无此句，无法正确
    copy(listint.cbegin(), listint.cend(), vecint.begin());
    helper_print_vc(vecint);


    //10.7 b
    vector<int> vec;
    vec.reserve(10); //分配空间，未初始化
    vec.resize(10);  //分配空间，并创建对象，改变容器大小
    std::fill_n(vec.begin(), 10, 0);
    helper_print_vc(vec);
   
    
    vector<string> vc{"a","b","e","c","c"};
    std::stable_sort(vc.begin(), vc.end());//不改变次序
    helper_print(vc);
    elimDumps(vc);
    helper_print(vc);
    


    std::map<string, int> m{{"a",2},{"c",3}};
    auto i = m.begin();
    while (i != m.end()) {
        cout<<i->first<<" "<<i->second<<endl;
        i++;
    }
    
    
    std::unordered_map<string, int> m2{{"a",3},{"b",4}};
    m2["c"] = 2;
    m2["a"] = 999;
    std::pair<std::string, int> p;
    p = {"e", 2};
    m2.insert(p);
    auto i2 = m2.begin();
    while (i2 != m2.end()) {
        cout<<i2->first<<" "<<i2->second<<endl;
        i2++;
    }

    cout<<m2.find("a")->first<<endl;
    cout<<m2.find(p.first)->first<<endl;
    m2.erase(m2.find(p.first));
    auto result =  m2.find(p.first);
    assert(result == m2.end());
    
    
    
    std::vector<std::string> exclude = { "aa", "bb", "cc", "dd", "ee", "ff" };
    for (string word; cout<<"Enter plz:\n", cin>>word&&word != "q"; ) {
        auto is_excluded = std::binary_search(exclude.begin(), exclude.end(), word);
        auto reply = is_excluded ? "excluded" : "not excluded";
        std::cout << reply << std::endl;
    }
    

    
    using Map = std::map<std::string, std::size_t>;
    using KeyType = std::map<std::string, std::size_t>::key_type;

    Map m;
    for (string word; cin>>word&&word!="q";) {
        ++m[word];
    }
    for (auto const& kv : m) {
        cout << kv.first << " : " << kv.second << "\n";
        cout<<typeid(decltype(kv)).name()<<endl;
        cout<<typeid(decltype(kv.first)).name()<<endl;
        cout<<typeid(decltype(kv.second)).name()<<endl;
        cout<<(typeid(decltype(kv.second))==typeid(std::size_t))<<endl;
        cout<<(typeid(decltype(kv.first))==typeid(string))<<endl;
        cout<<(typeid(decltype(kv.first))==typeid(char))<<endl; //不相等
    }
    cout<<typeid(KeyType).name()<<endl;

    
    StrBlob str{"a","b","c"};
    cout<<str.back()<<endl;
    str.back() = "d";
    cout<<str.back()<<endl;
    

    int i = 1<<2 | 1<<3;
    cout<<i<<endl;
    cout<<std::bitset<8>(i)<<endl;
    cout<<std::hex<<i<<endl;
    cout<<std::oct<<i<<endl;
    
    char binary_text[100];
//    itoa(i, binary_text, 2);  只有windows有这个函数
    sprintf(binary_text, "%x",i);
    printf("二进制 %s\n", binary_text);

    cout<<std::to_string(i)<<endl;
    
    string s = std::bitset<8>(i).to_string();
    cout<<s<<endl;
#endif
   
    constexpr unsigned long A = 5+5+testTemplateBinary();
    cout<<Binary<A>::value<<endl;
    cout<<binary(110)<<endl;
}





void process(std::shared_ptr<int> ptr)
{
    cout << "inside the process function : " << ptr.use_count() << endl;
}

