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
    Task():taskname_(std::to_string(index_))
    {
        cout<<"add job "<<index_++<<endl;
    }
    ~Task(){}
    void doWork()
    {
        //模拟干活
        cout<<"thread "<<taskname_<<" start working"<<endl;
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
        cout<<"thread "<<taskname_<<" end"<<endl;

    }
    
    string getname() {return taskname_;}
};

int Task::index_ = 0;

class TaskQueue {
    queue<Task> queue_;
    mutex mtx_;
    condition_variable cv_full_;
    condition_variable cv_empty_;
    
public:
    TaskQueue():queue_(),mtx_(),cv_full_(),cv_empty_(){}
    ~TaskQueue(){}
    
    void addTask(const Task& task);
    Task popTask();
    
    bool empty() {
        return queue_.empty();
    }
    
    bool full() {
        return queue_.size() == 5;
    }
};

void TaskQueue::addTask(const Task &task)
{
    std::unique_lock<std::mutex> lock (mtx_); //上锁
    while (full()) {
        cout<<"full..wait..."<<endl;
        cv_full_.wait(lock);
    }
    
    queue_.push(task);
    cv_empty_.notify_all();
}

Task TaskQueue::popTask()
{
    std::unique_lock<std::mutex> lock (mtx_); //上锁
    while(queue_.empty()) {
        cout<<"empty..wait..."<<endl;
        cv_empty_.wait(lock);
    }
    
    Task task = queue_.front();
    queue_.pop();
    cout<<"take job "<< task.getname()<<" to work"<<endl;
    cv_full_.notify_all();
    return task;
}


}

#endif /* TaskQueue_hpp */
