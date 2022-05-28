#pragma once
#include<map>
#include<string>
#include<stdexcept>
#include<fstream>
#include<sstream>
#include<vector>

class JsonData{

    private:
    char _tag = -1;
    void* _value = nullptr;
    public:
    JsonData();
    JsonData(const JsonData& jsd);
    JsonData(const char* s);
    JsonData(const std::string& s);
    JsonData(const long& l);
    JsonData(const int& i);
    JsonData(const float& f);
    JsonData(const double& d);
    JsonData(const std::map<std::string, JsonData>& m);
    JsonData(const std::vector<JsonData>& v);
    JsonData(const bool&);//explicit because bool is a dick and wants all the fuking conversion
    ~JsonData();

    //void operator=(JsonData& jsd);
    void operator=(const JsonData& jsd);

    //if JsonData is not map throws error otherwise access the map entry
    JsonData& operator[](const std::string key);

    //if JsonData is not vector throws error otherwise access the vector entry
    JsonData& operator[](const size_t key);

    //access the string of this jsondata
    std::string& str();

    //access the long of this jsondata
    long&   i_numb();

    //access the double of this jsondata
    double& f_numb();

    //access the map of this jsondata
    std::map<std::string, JsonData>& object();

    //access the vector of this jsondata
    std::vector<JsonData>& list();

    //access the bool of this jsondata
    bool& boolean();

    //returns tag of JsonData
    constexpr int tag() const;

    //returns true if JsonData is null
    constexpr bool null() const;

    //returns string version of type
    std::string type() const;

    private:
    void set_from_jsd(const JsonData& jsd);
    void set_from_string(const char* s);
    void set_from_long(const long& l);
    void set_from_double(const double& d);
    void set_from_map(const std::map<std::string, JsonData>& m);
    void set_from_vector(const std::vector<JsonData>& v);
    void set_from_bool(const bool&);
    void clear_pointer();
};

class JsonError : public std::runtime_error{
    public:
    inline JsonError(const std::string& msg): runtime_error(msg.c_str()){};
    inline static void InvalidJsonData(const int& tag){ throw JsonError("Invalid JsonData with tag "+to_string(tag));}
    inline static void NotMatchingJsonDataTag(const std::string& otype, const std::string& rtype){ throw JsonError("Invalid request of type "+rtype+" with object of type "+otype);}
    inline static void FileError(const std::string& filename){ throw JsonError("Cannot open file "+filename);}
    inline static void JsonFormatError(){ throw JsonError("Invalid Json format");}
};


namespace Json{
        //loads json from file
        JsonData load(const std::string& filename);
        //decode json from string
        JsonData decode(std::string str_json);
        //encode json to string
        std::string encode(JsonData& jsd);
        //encode json to string and then dumps it into a file
        void dump(const std::string& filename, JsonData& jsd);
        //helpers to get cleaner access to data
        std::string read_file(const std::string& filename);
        void clear_json(std::string& str);
        //parse and dump helper
        JsonData parse_value(const char* char_string, size_t& index);
        JsonData parse_object(const char* char_string, size_t& index);
        JsonData parse_list(const char* char_string, size_t& index);
        JsonData parse_number(const char* char_string, size_t& index);
        std::string parse_string(const char* char_string, size_t& index);
        std::string stringify_object(JsonData& jsd);
        std::string stringify_list(JsonData& jsd);
        std::string stringify_string(const std::string& tmp);
        std::string jsd_to_str(JsonData& jsd);
};