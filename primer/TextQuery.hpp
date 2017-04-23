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
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;

struct QueryResult {
    int totalTimes_;
    vector<pair<int, string>> vtlines_;
};

class TextQuery {
private:
    vector<pair<int, string>> filelines_;           //文件转换为每行及其对应字符串
    unordered_map<string,vector<int>> strdict_;     //一个字符串，在哪些行
public:
    TextQuery(const string& filename);
    ~TextQuery();
    QueryResult queryText(const string& text);
};

TextQuery::TextQuery(const string& filename)
{
    
}
#endif /* TextQuery_hpp */
