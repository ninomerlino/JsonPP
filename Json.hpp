#pragma once
#include<map>
#include<string>
#include<stdexcept>
#include<fstream>
#include<sstream>
#include<vector>
#include<pcrecpp.h>
#define CLEAN_JSON_PATTERN "(?<DEFINE>\
(?<Number>(?>[0-9]+(\.[0-9]+)*(e[0-9]+)*))\
(?<String>(?>\"(?>\\(?>[\"\\\/bfnrt]|u[a-fA-F0-9]{4})|[^\"\\\0-\x1F\x7F]+)*\"))\
(?<Value>(?>(?&String)|(?&Number)|false|true|null|(?&List)|(?&Object)))\
(?<Entry>(?>(?&String):(?&Value)))\
(?<List>(?>\[(?>(?&Value)(?>,(?&Value))*)?\]))\
(?<Object>(?>\{(?>(?&Entry)(?>,(?&Entry))*)?\}))\
(?<Json>(?>(?&Object)|(?&List)))\
)|\A(?&Json)\z"

using namespace std;

class JsonData{

    private:
    char _tag = -1;
    void* _value = nullptr;
    public:
    JsonData();
    JsonData(const JsonData& jsd);
    JsonData(const string& s);
    JsonData(const long& l);
    JsonData(const double& d);
    JsonData(const map<string, JsonData>& m);
    JsonData(const vector<JsonData>& v);
    JsonData(const bool&);
    ~JsonData();
    //set union value to string s *sets tag*
    void operator=(string s);

    //set union value to long l *sets tag*
    void operator=(long l);

    //set union value to double d *sets tag*
    void operator=(double d);

    //set union value to double d *sets tag*
    void operator=(bool b);

    //set union value to map m *sets tag*
    void operator=(map<string, JsonData> m);

    //set union value to vector v *sets tag*
    void operator=(vector<JsonData> v);

    //void operator=(JsonData& jsd);
    void operator=(JsonData jsd);

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
    int tag();

    //returns true if JsonData is null
    bool null();

    //returns string version of type
    const string& type();

    private:
    void set_from_jsd(const JsonData& jsd);
    void set_from_string(const string& s);
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
    inline static void FileError(){ throw JsonError("Invalid Json format");}
};


class Json
    {
    private:
        const pcre* valid_regex = pcre_compile2(CLEAN_JSON_PATTERN, NULL,NULL,NULL,NULL,NULL);
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
        bool validate_json(const string& str);
};

//helper functions
JsonData parse_value(const char* char_string, size_t& index);
JsonData parse_object(const char* char_string, size_t& index);
JsonData parse_list(const char* char_string, size_t& index);
JsonData parse_number(const char* char_string, size_t& index);
string parse_string(const char* char_string, size_t& index);
string stringify_object(JsonData jsd);
string stringify_list(JsonData jsd);
string jsd_to_str(JsonData jsd);