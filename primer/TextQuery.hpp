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

#include "util.h"

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::ifstream;
using std::istringstream;

namespace lwj {
    struct QueryResult {
        using sizetype =  vector<string>::size_type;
        
        sizetype totalTimes_;
        vector<pair<sizetype, string>> vtlines_;
        
        void printResult();

    };
    
    class TextQuery :public handy::noncopyable{
        
        using sizetype =  vector<string>::size_type;
    private:
        vector<string> filelines_;                           //文件转换为每行及其对应字符串
        unordered_map<string,vector<sizetype>> strdict_;     //一个字符串，在哪些行
        
    public:
        TextQuery(const string& filename);
        ~TextQuery() {};
        QueryResult queryText(const string& text);
        void printResult(const QueryResult& result);
    };
}




#endif /* TextQuery_hpp */
