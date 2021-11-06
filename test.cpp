#include<iostream>
#include"Json.hpp"


int main(){
    Json json;
    json.load("file.json");
    cout<<json["name"].str()<<"with age : "<<json["age"].i_numb()<<"has codes\n";
    for (size_t i = 0; i < json["codes"].list().size(); i++)
        cout<<"\t["<<i<<"] : "<<json["codes"][i].i_numb()<<"\n";
    json["childs"][0]["name"] = "roberto";
    json.dump("file.json");
    return 0;
}
//g++ test.cpp Json.cpp -o test 