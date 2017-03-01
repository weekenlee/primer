//
//  TaskQueue.hpp
//  primer
//
//  Created by liweijian on 2017/3/1.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef TaskQueue_hpp
#define TaskQueue_hpp

#include <vector>

using std::vector;

class Task {
private:
    void doWork();
public:
    Task();
    ~Task();
    void work();
};

class TaskQueue {
    vector<Task> queue_;
    
public:
    TaskQueue();
    ~TaskQueue();
    
    void addTask(const Task& task);
    void delTask(const Task& task);
};


#endif /* TaskQueue_hpp */
