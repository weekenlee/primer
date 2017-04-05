//
//  StrBlob.h
//  primer
//
//  Created by liweijian on 2017/4/5.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef StrBlob_hpp
#define StrBlob_hpp

#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include <exception>

using std::vector;
using std::string;

class StrBlob {

    
public:
    using size_type = vector<string>::size_type;
    
    StrBlob():data(std::make_shared<vector<string>>()) {}
    StrBlob(std::initializer_list<string> li):data(std::make_shared<vector<string>>(li)) {}

    size_type size() {return data->size();}
    bool empty() const {return data->empty();}
    
    void push_back(const string& t) {data->push_back(t);}
    void pop_back() {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }
    
    string& front() {
        check(0, "front on empty StrBlob");
        return data->front();
    }
    
    string& back() {
        check(0, "back on empty StrBlob");
        return data->back();
    }
    
    const string& front() const{
        check(0, "front on empty StrBlob");
        return data->front();
    }
    
    const string& back() const{
        check(0, "back on empty StrBlob");
        return data->back();
    }

    
private:
    std::shared_ptr<vector<string>> data;
    
    void check(size_type i, const string &msg) const {
        if (i >= data->size()) throw std::out_of_range(msg);
    }
};


#endif /* StrBlob_h */
