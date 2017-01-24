//
//  ThreadRun.hpp
//  primer
//
//  Created by liweijian on 2017/1/24.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef ThreadRun_hpp
#define ThreadRun_hpp


class ThreadRun
{
public:
    static void start();
    
private:
    static void func(int threadid);
    static int globali;
    static const  int threadnum;
};


#endif /* ThreadRun_hpp */
