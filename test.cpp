#include<iostream>
#include"Json.hpp"

int main(){
    JsonData data;
    data = Json::load("file.json");
    data.list().push_back("This json was modified successfully");
    Json::dump("filedump.json", data);
    return 0;
}
//g++ test.cpp Json.cpp -o test 