#include<iostream>
#include"Json.hpp"


int main(){
    Json json;
    char newname[8] = "roberto";
    json.load("file.json");
    cout<<json["name"].str()<<" with age : "<<json["age"].i_numb()<<" has codes\n";
    for (size_t i = 0; i < json["codes"].list().size(); i++)
        cout<<"\t["<<i<<"] : "<<json["codes"][i].i_numb()<<"\n";
    json["childs"][0]["name"] = newname;
    json["childs"][0]["age"] = (short)10;
    json["childs"][1]["age"] = (char)1;
    json["false"] = true;
    cout<<json["childs"][0]["name"].str();
    json.dump("filedump.json");
    return 0;
}
//g++ test.cpp Json.cpp -o test 