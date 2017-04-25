//
//  TextQuery.cpp
//  primer
//
//  Created by liweijian on 2017/4/25.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#include "TextQuery.hpp"
#include "easylogging++.h"

namespace lwj {
    TextQuery::TextQuery(const string& filename)
    {
        LOG(INFO)<<"init";
        ifstream f(filename);
        if (!f.is_open()) {
            LOG(ERROR)<<"can't not open file "+filename;
        }else {
            string line;
            while ( getline(f, line)) {
                filelines_.push_back(line);
                sizetype lineno = filelines_.size();
                istringstream ss(line);
                string word;
                while (ss>>word) {
                    strdict_[word].push_back(lineno);
                }
                lineno++;
            }
        }
    }
    
    
    QueryResult TextQuery::queryText(const string& text)
    {
        QueryResult result;
        result.totalTimes_ = strdict_[text].size();
        for (int i=0; i<result.totalTimes_; i++) {
            sizetype lineno = strdict_[text][i];
            result.vtlines_.push_back(std::make_pair(lineno, filelines_[lineno-1]));
        }
        return result;
    }
    
    
    void QueryResult::printResult()
    {
        std::cout<<"一共出现"<<totalTimes_<<"次"<<std::endl;
        size_t size = vtlines_.size();
        for (int i=0; i<size; i++) {
            std::cout<<"第"<<vtlines_[i].first<<"行"<<"\t";
            std::cout<<vtlines_[i].second;
            std::cout<<std::endl;
        }
    }
}


