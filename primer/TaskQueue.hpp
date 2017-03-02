//
//  TaskQueue.hpp
//  primer
//
//  Created by liweijian on 2017/3/1.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef TaskQueue_hpp
#define TaskQueue_hpp

#include <queue>
#include <thread>
#include <string>
#include <iostream>

using std::queue;
using std::mutex;
using std::condition_variable;
using std::string;
using std::cout;
using std::endl;

namespace lwj {
 
class Task {
private:
    static int index_;
    string taskname_;
    
public:
    Task():taskname_{std::to_string(index_)}{index_++;}
    ~Task(){}
    void doWork()
    {
        //模拟干活
        cout<<"thread "<<taskname_<<" start working"<<endl;
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
        cout<<"thread "<<taskname_<<" end"<<endl;

    }
};

int Task::index_ = 0;

class TaskQueue {
    queue<Task> queue_;
    mutex mtx_;
    condition_variable cv_;
    
public:
    TaskQueue():queue_(),mtx_(),cv_(){}
    ~TaskQueue(){}
    
    void addTask(const Task& task);
    Task popTask();
    
    bool empty() {
        std::unique_lock<std::mutex> lck (mtx_); //上锁
        return queue_.empty();
    }
};

void TaskQueue::addTask(const Task &task)
{
    std::unique_lock<std::mutex> lock (mtx_); //上锁
    queue_.push(task);
    cv_.notify_all();
    lock.unlock();
}

Task TaskQueue::popTask()
{
    std::unique_lock<std::mutex> lock (mtx_); //上锁
    while(queue_.empty()) {
        cv_.wait(lock);
    }
    
    Task task = queue_.front();
    queue_.pop();
    lock.unlock();
    return task;
}
    
    
 

}

#endif /* TaskQueue_hpp */
