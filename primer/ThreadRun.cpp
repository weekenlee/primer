//
//  ThreadRun.cpp
//  primer
//
//  Created by liweijian on 2017/1/24.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#include "ThreadRun.hpp"
#include <thread>
#include <chrono>
#include <random>
#include <iostream>

using std::cout;
using std::endl;
using std::thread;



const int ThreadRun::threadnum = 5;
int ThreadRun::globali = 0;

std::mutex runmtx;     //共享锁

void ThreadRun::func(int threadid)
{
    cout<<"started   "<< threadid<< endl;
    std::unique_lock<std::mutex> lck (runmtx); //上锁
    globali++;
    cout<<"i is : "<<globali<<" in "<<threadid<<endl;
    
    std::uniform_int_distribution<unsigned> u(10,19);
    std::default_random_engine e;
    std::this_thread::sleep_for(std::chrono::microseconds(u(e)*100000));
    
    globali--;
    cout<<"i is : "<<globali<<" in "<<threadid<<endl;
    
    cout<<"end   "<< threadid<< endl;
}

void ThreadRun::start()
{
    thread t[ThreadRun::threadnum];
    
    for (int i = 0; i < ThreadRun::threadnum; ++i)
    {
        std::uniform_int_distribution<unsigned> u(5,9);
        std::default_random_engine e;
        std::this_thread::sleep_for(std::chrono::microseconds(u(e)*100000));

        t[i] = thread(ThreadRun::func,i);
    }
    
    for (int i = 0; i < ThreadRun::threadnum;++i)
    {
        t[i].join();
    }
}
