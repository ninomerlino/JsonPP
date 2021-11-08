#pragma once
#include<map>
#include<string>
#include<stdexcept>
#include<fstream>
#include<sstream>
#include<vector>
//#include<pcrecpp.h>

using namespace std;
class JsonData{

    private:
    char _tag = -1;
    void* _value = nullptr;
    public:
    JsonData();
    JsonData(const JsonData& jsd);
    JsonData(const char* s);
    JsonData(const string& s);
    JsonData(const long& l);
    JsonData(const int& i);
    JsonData(const float& f);
    JsonData(const double& d);
    JsonData(const map<string, JsonData>& m);
    JsonData(const vector<JsonData>& v);
    JsonData(const bool&);//explicit because bool is a dick and wants all the fuking conversion
    ~JsonData();

    //void operator=(JsonData& jsd);
    void operator=(const JsonData& jsd);

    //if JsonData is not map throws error otherwise access the map entry
    JsonData& operator[](const string key);

    //if JsonData is not vector throws error otherwise access the vector entry
    JsonData& operator[](const size_t key);

    //access the string of this jsondata
    string& str();

    //access the long of this jsondata
    long&   i_numb();

    //access the double of this jsondata
    double& f_numb();

    //access the map of this jsondata
    map<string, JsonData>& object();

    //access the vector of this jsondata
    vector<JsonData>& list();

    //access the bool of this jsondata
    bool& boolean();

    //returns tag of JsonData
    constexpr int tag() const;

    //returns true if JsonData is null
    constexpr bool null() const;

    //returns string version of type
    string type() const;

    private:
    void set_from_jsd(const JsonData& jsd);
    void set_from_string(const char* s);
    void set_from_long(const long& l);
    void set_from_double(const double& d);
    void set_from_map(const map<string, JsonData>& m);
    void set_from_vector(const vector<JsonData>& v);
    void set_from_bool(const bool&);
    void clear_pointer();
};

class JsonError : public std::runtime_error{
    public:
    inline JsonError(const string& msg): runtime_error(msg.c_str()){};
    inline static void InvalidJsonData(const int& tag){ throw JsonError("Invalid JsonData with tag"+to_string(tag));}
    inline static void NotMatchingJsonDataTag(const string& otype, const string& rtype){ throw JsonError("Invalid request of type "+rtype+"with object of type"+otype);}
    inline static void FileError(const string& filename){ throw JsonError("Cannot open file "+filename);}
    inline static void JsonFormatError(){ throw JsonError("Invalid Json format");}
};


class Json
    {
    private:
        //static pcrecpp::RE json_regex;
        JsonData data;
    public:
        //loads json from file
        void load(const string& filename);
        //decode json from string
        void decode(string str_json);
        //encode json to string
        string encode();
        //encode json to string and then dumps it into a file
        void dump(const string& filename);
        //helpers to get cleaner access to data
        JsonData& operator[](const string key);
        JsonData& operator[](const size_t key);
    private:
        string read_file(const string& filename);
        void clear_json(string& str);
};

//helper functions
JsonData parse_value(const char* char_string, size_t& index);
JsonData parse_object(const char* char_string, size_t& index);
JsonData parse_list(const char* char_string, size_t& index);
JsonData parse_number(const char* char_string, size_t& index);
string parse_string(const char* char_string, size_t& index);
string stringify_object(const JsonData& jsd);
string stringify_list(JsonData& jsd);
string stringify_string(const string& tmp);
string jsd_to_str(JsonData& jsd);