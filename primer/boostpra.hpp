//
//  boostpra.hpp
//  primer
//
//  Created by liweijian on 2017/2/13.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef boostpra_hpp
#define boostpra_hpp

#include <boost/version.hpp>
#include <boost/timer.hpp>
#include <boost/progress.hpp>

using std::cout;
using std::endl;

void boosttimer()
{
    boost::timer bt;
    boost::progress_timer t2; //析构时自动输出

    cout<<bt.elapsed_max()<<endl;
    cout<<bt.elapsed_min()<<endl;
    cout<<bt.elapsed()<<endl;
}


#endif /* boostpra_hpp */
