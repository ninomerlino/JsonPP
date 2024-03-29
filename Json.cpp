#include"Json.hpp"
#define TAIL_PRINT 30
using namespace std
//JSONDATA
void JsonData::set_from_jsd(const JsonData& jsd){
    clear_pointer();
    _tag = jsd._tag;
    switch (jsd._tag)
    {
    case -1:
        break;
    case 0:
        _value = new string();
        *(string*)_value = (*(string*)jsd._value).c_str();
        break;
    case 1:
        _value = new long;
        *(long*)(_value) = *(long*)jsd._value;
        break;
    case 2:
        _value = new double;
        *(double*)(_value) = *(double*)jsd._value;
        break;
    case 3:
        _value = new map<string, JsonData>;
        *(map<string, JsonData>*)(_value) = *(map<string, JsonData>*)jsd._value;
        break;
    case 4:
        _value = new vector<JsonData>;
        *(vector<JsonData>*)(_value) = *(vector<JsonData>*)jsd._value;
        break;
    case 5:
        _value = new bool;
        *(bool*)(_value) = *(bool*)jsd._value;
        break;
    default:
        JsonError::InvalidJsonData((int)_tag);
    }
}
void JsonData::set_from_bool(const bool& b){
    _tag = 5;
    _value = new bool;
    *(bool*)_value = b; 
}
void JsonData::set_from_string(const char* s){
    _tag = 0;
    _value = new string();
    *(string*)_value = s;
}
void JsonData::set_from_long(const long& l){
    _tag = 1;
    _value = new long;
    *(long*)(_value) = l;
}
void JsonData::set_from_double(const double& d){
    _tag = 2;
    _value = new double;
    *(double*)(_value) = d;
}
void JsonData::set_from_map(const map<string, JsonData>& m){
    _tag = 3;
    _value = new map<string, JsonData>;
    *(map<string, JsonData>*)(_value) = m;
}
void JsonData::set_from_vector(const vector<JsonData>& v){
    _tag = 4;
    _value = new vector<JsonData>;
    *(vector<JsonData>*)(_value) = v;
}
void JsonData::clear_pointer(){
    if(_value){
        switch (_tag){
        case 0:
            delete (string*)_value;
            break;
        case 1:
            delete (long*)(_value);
            break;
        case 2:
            delete (double*)(_value);
            break;
        case 3:
            delete (map<string, JsonData>*)(_value);
            break;
        case 4:
            delete (vector<JsonData>*)(_value);
            break;
        case 5:
            delete (bool*)(_value);
            break;
        default:
            JsonError::InvalidJsonData((int)_tag);
        }
        _value = nullptr;
    }
}
JsonData::~JsonData(){clear_pointer();};
JsonData::JsonData(){}
JsonData::JsonData(const bool& b){set_from_bool(b);}
JsonData::JsonData(const JsonData& jsd){set_from_jsd(jsd);}
JsonData::JsonData(const string& s){set_from_string(s.c_str());}
JsonData::JsonData(const char* s){set_from_string(s);}
JsonData::JsonData(const long & l){set_from_long(l);}
JsonData::JsonData(const int & i){set_from_long(i);}
JsonData::JsonData(const float & f){set_from_double(f);}
JsonData::JsonData(const double& d){set_from_double(d);}
JsonData::JsonData(const map<string, JsonData>& m){set_from_map(m);}
JsonData::JsonData(const vector<JsonData>& v){set_from_vector(v);}
void JsonData::operator=(const JsonData& jsd){set_from_jsd(jsd);}

JsonData& JsonData::operator[](const string key){
    if(_tag != 3)JsonError::NotMatchingJsonDataTag(type(), "map");
    return ((map<string, JsonData>*)_value)->at(key);
}
JsonData& JsonData::operator[](const size_t key){
    if(_tag != 4)JsonError::NotMatchingJsonDataTag(type(), "list");
    return ((vector<JsonData>*)_value)->at(key);  
}
string& JsonData::str(){
    if(_tag != 0)JsonError::NotMatchingJsonDataTag(type(), "str");
    return *(string*)_value;
}
long&   JsonData::i_numb(){
    if(_tag != 1)JsonError::NotMatchingJsonDataTag(type(), "long");
    return *(long*)_value;
}
bool&   JsonData::boolean(){
    if(_tag != 5)JsonError::NotMatchingJsonDataTag(type(), "bool");
    return *(bool*)_value;
}
double& JsonData::f_numb(){
    if(_tag != 2)JsonError::NotMatchingJsonDataTag(type(), "double");
    return *(double*)_value;
}
map<string, JsonData>& JsonData::object(){
    if(_tag != 3)JsonError::NotMatchingJsonDataTag(type(), "map");
    return *(map<string, JsonData>*)_value;
}
vector<JsonData>& JsonData::list(){
    if(_tag != 4)JsonError::NotMatchingJsonDataTag(type(), "vector");
    return *(vector<JsonData>*)_value;
}
constexpr int JsonData::tag() const {return _tag;}
constexpr bool JsonData::null()const {return _tag == -1;}
string JsonData::type() const {
    switch (_tag){
        case -1:
            return "null";
        case 0:
            return "string";
        case 1:
            return "long";
        case 2:
            return "double";
        case 3:
            return "map";
        case 4:
            return "vector";
        case 5:
            return "bool";
        default:
            JsonError::InvalidJsonData((int)_tag);
    }
    return "";
}

//JSON

//pcrecpp::RE json_regex = pcrecpp::RE(JSON_PATTERN);

namespace Json{
    string read_file(const string& filename){
        ifstream stream(filename);
        if(!stream.is_open())JsonError::FileError(filename);
        stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }

    JsonData decode(string str_json){
        size_t index = 1;//ignoring first char
        clear_json(str_json);
        try{
            if(str_json[0] == '{')
                return parse_object(str_json.c_str(), index);
            else
                return parse_list(str_json.c_str(), index);
        }catch(...){
            JsonError::JsonFormatError();
            return JsonData();
        }
    }

    JsonData load(const string& filename){
        string encoded_data = read_file(filename);
        return decode(encoded_data);
    }

    void dump(const string& filename, JsonData& jsd){
        ofstream stream(filename);
        if(!stream.is_open())JsonError::FileError(filename);
        stream << encode(jsd);
        stream.close();
    }

    string encode(JsonData& jsd)
    {
        return jsd_to_str(jsd);
    }

    void clear_json(string& str){
        size_t pos = 0;
        bool in_string = false;
        while (pos < str.size()){
            switch (str[pos])
            {
            case '"':
                in_string = (in_string)? false : true;
                pos++;
                break;
            case '\\':
                pos+=2;
                break;
            case '\t':
            case ' ':
            case '\n':
                if(!in_string)str.erase(pos, 1);
                else pos++;
                break;
            default:
                pos++;
                break;
            }
        }
    }

    //helperfunctions

    JsonData parse_number(const char* char_string, size_t& index){
        size_t start = index;
        string str_number;
        bool is_float = false;
        do{
            str_number.push_back(char_string[index]);
            if(char_string[index] == '.' || char_string[index] == 'e' || char_string[index] == 'E')
                is_float = true;
            index++;
        }while (char_string[index]!=',' && char_string[index]!='}' && char_string[index]!=']');
        if (is_float)
            return stod(str_number);
        else
            return stol(str_number);
    }

    string parse_string(const char* char_string, size_t& index){
        string retu = "";
        while (char_string[index]!='"'){
            if(char_string[index] == '\\'){//special escape sequences
                index++;
                switch (char_string[index])
                {   
                case 'b':
                    retu.push_back('\b');
                    break;
                case 't':
                    retu.push_back('\t');
                    break;
                case 'f':
                    retu.push_back('\f');
                    break;
                case 'r':
                    retu.push_back('\r');
                    break;
                case 'n':
                    retu.push_back('\n');
                    break;            
                default:// \" \\ cases
                    retu.push_back(char_string[index]);
                    break;
                }
            }else{
                retu.push_back(char_string[index]);
            }
            index++;
        };
        index++;
        return retu;
    }

    JsonData parse_value(const char* char_string, size_t& index){
            switch (char_string[index])
            {
                case '"'://string case
                    index++;
                    return JsonData(parse_string(char_string, index));
                case '{'://object case
                    index++;
                    return parse_object(char_string, index);
                case '['://object case
                    index++;
                    return parse_list(char_string, index);
                case 't':
                    index+=4;
                    return true;
                case 'f':
                    index+=5;
                    return false;
                case 'n':
                    index+=4;
                    return JsonData();
                default:
                    return parse_number(char_string, index);
            }
    }

    JsonData parse_object(const char* char_string, size_t& index){
        map<string, JsonData> object;
        string key;
        while (char_string[index]!='}'){
            index++;//key start "
            key = parse_string(char_string, index);
            index++;//ignore ":
            object[key] = parse_value(char_string, index);
            if(char_string[index]==',')index++;//skip,
        };
        index++;
        return object;
    }

    JsonData parse_list(const char* char_string, size_t& index){
        vector<JsonData> list;
        while (char_string[index]!=']'){//after null
            list.push_back(parse_value(char_string, index));
            if(char_string[index]==']')break;//this is bad i know but in case the Json is not corrected and we have somethng that is not , or ] it enters infinite loop
            index++;//skip
        };
        index++;
        return list;
    }

    string stringify_object(JsonData& jsd){
        if(jsd.object().size() == 0 )return "{}";
        string retu = "{";
        for (pair<const string, JsonData> &entry : jsd.object()){
            retu.append(stringify_string(entry.first)+":"+jsd_to_str(entry.second)+",");
        }
        retu[retu.size()-1] = '}';
        return retu;
    }

    string stringify_list(JsonData& jsd){
        if(jsd.list().size() == 0 )return "[]";
        string retu = "[";
        for (JsonData &elm : jsd.list()){
            retu.append(jsd_to_str(elm)+",");
        }
        retu[retu.size()-1] = ']';
        return retu;
    }

    string stringify_string(const string& tmp){
        string cpy = "\"";
        for (size_t i = 0; i < tmp.size(); i++){
            switch (tmp[i])
            {
            case '\b':
                cpy.append("\\b");
                break;
            case '\t':
                cpy.append("\\t");
                break;
            case '\r':
                cpy.append("\\r");
                break;
            case '\f':
                cpy.append("\\f");
                break;
            case '\n':
                cpy.append("\\n");
                break;
            case '\"':
                cpy.append("\\\"");
                break;
            case '\\':
                cpy.append("\\\\");
                break;
            default:
                cpy.push_back(tmp[i]);
                break;
            }
        }
        cpy.push_back('\"');
        return cpy;
    }

    string jsd_to_str(JsonData& jsd){
        switch (jsd.tag())
        {
        case -1:
            return "null";
        case 0:
            return stringify_string(jsd.str());
        case 1:
            return to_string(jsd.i_numb());
        case 2:
            return to_string(jsd.f_numb());
        case 3:
            return stringify_object(jsd);
        case 4:
            return stringify_list(jsd);
        case 5:
            if(jsd.boolean())
                return "true";
            return "false";
        }
        JsonError::InvalidJsonData(jsd.tag());
        return "";
    }
}