//
//  TextQuery.hpp
//  primer
//
//  Created by liweijian on 2017/4/23.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef TextQuery_hpp
#define TextQuery_hpp
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "easylogging++.h"
#include "util.h"

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::ifstream;
using std::istringstream;

struct QueryResult {
    using sizetype =  vector<string>::size_type;

    sizetype totalTimes_;
    vector<pair<sizetype, string>> vtlines_;
};

class TextQuery :public handy::noncopyable{
    
    using sizetype =  vector<string>::size_type;
private:
    vector<string> filelines_;           //文件转换为每行及其对应字符串
    unordered_map<string,vector<sizetype>> strdict_;     //一个字符串，在哪些行

public:
    TextQuery(const string& filename);
    ~TextQuery() {};
    QueryResult queryText(const string& text);
    void printResult(const QueryResult& result);

};

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


void TextQuery::printResult(const QueryResult& result)
{
    std::cout<<"一共出现"<<result.totalTimes_<<"次"<<std::endl;
    size_t size = result.vtlines_.size();
    for (int i=0; i<size; i++) {
        std::cout<<"第"<<result.vtlines_[i].first<<"行"<<"\t";
        std::cout<<result.vtlines_[i].second;
        std::cout<<std::endl;
    }
}

#endif /* TextQuery_hpp */
