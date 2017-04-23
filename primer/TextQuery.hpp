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
#include <istream>

#include "easylogging++.h"

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::fstream;
using std::istringstream;

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
    ~TextQuery() {};
    QueryResult queryText(const string& text);
};

TextQuery::TextQuery(const string& filename)
{
    fstream f(filename,f.out);
    if (!f.is_open()) {
        LOG(ERROR)<<"can't not open file "+filename;
    }else {
        string line;
        int lineno = 1;
        while (f>>line) {
            filelines_.push_back(std::make_pair(lineno,line));
            istringstream ss(line);
            string word;
            while (ss>>word) {
                strdict_[word].push_back(lineno);
            }
            lineno++;
        }
    }
}
#endif /* TextQuery_hpp */
