#include<future>
#include<iostream>
#include<thread>
#include<chrono>

using std::cout;
using std::endl;
using std::flush;

int do_some_job()
{
     cout<<endl<<std::this_thread::get_id()<<"job start"<<endl<<flush;
     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
     cout<<endl<<std::this_thread::get_id()<<"job done"<<endl<<flush;
     return 10;
}

void sub_wait_get(std::future<int> &fut)
{
    cout<<endl<<flush;
    cout<<std::this_thread::get_id()<<" is waiting for main thread"<<endl<<flush;
    cout<<std::this_thread::get_id()<<" get "<<fut.get()<<endl<<flush;
    return;
}

int asyncmain()
{
    std::promise<int> pro;
    std::future<int> fut = pro.get_future();
    std::thread t(sub_wait_get, std::ref(fut));

    std::future<int> result = std::async(std::launch::async,do_some_job);
    std::chrono::milliseconds span(10);
    while (result.wait_for(span) == std::future_status::timeout)
         std::cout << '.' <<flush;
    cout<<endl<<flush;
    pro.set_value(result.get());
    t.join();

    return 0;
}
