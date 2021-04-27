
#include "NumberWithUnits.hpp"
#include <sstream>
#include <fstream>
#include <set>

using namespace std;


namespace ariel{
    static map<std::string, map<std::string, double>> convertion_map;


    NumberWithUnits::NumberWithUnits(double num, string type){
        if (!existing_type(type)){
            throw invalid_argument("un-existing unit");
        }
        _num = num;
        _type = type;
        // cout << "new NumberWithUnits(" << _num << ", "<< _type<< ")" << endl << endl;
    }

    bool NumberWithUnits::existing_type(string type){
        if(convertion_map.count(type) != 0){
            return true;
        }
        map<string, map<string, double>>::const_iterator it;
        for(it = convertion_map.begin();it != convertion_map.end(); ++it){
            map<string, double> map = it->second;
            if (map.count(type) != 0){
                return true;
            }
        }
        return false;

    }
    
    void NumberWithUnits::read_units(ifstream& units_file){
        string line;
        // fill direct convertions:
        while (getline(units_file, line)){
            istringstream iss(line);
            double garb_1, val;
            string conv_unit, orig_unit, garb_2;

            if (!(iss >> garb_1 >> orig_unit >> garb_2 >> val >> conv_unit)) {  // error
                break; 
            } 
            
            if (convertion_map.count(orig_unit) != 0){
                convertion_map[orig_unit][conv_unit] = val;
            }
            else{
                map<string, double> new_convert1;
                new_convert1[conv_unit] = val;
                convertion_map[orig_unit] = new_convert1;
            }
        }

        // fill un-direct convertions:
        map<string,map<string,double>>::iterator it;
        for(it=convertion_map.begin(); it!=convertion_map.end(); ++it){
            string type = it->first;
            map<string,double> map = it->second;
            recursive_convert(type, type, 1);
        }
        
        //debug:
        // cout << "convertion_map:" << endl;
        // for(map<string, map<string, double>>::const_iterator it = convertion_map.begin();
        //     it != convertion_map.end(); ++it){
        //     cout << endl << it->first << ":" << endl; 
        //     for(map<string, double>::const_iterator inner_it = it->second.begin();
        //     inner_it != it->second.end(); ++inner_it){
        //         cout << inner_it->first << ", " << inner_it->second << endl;
        //     }
        // }
         
    }

    void NumberWithUnits::recursive_convert(string orig_type, string temp_type, double path_val){
        std::map<string,double> map = convertion_map[temp_type];
        string conv_type;
        std::map<string,double>::iterator it= map.begin();
        conv_type = it->first;
        double val = it->second;
        path_val = path_val*val;
        convertion_map[orig_type][conv_type] = path_val;
        // cout<<endl<< "orig_type :"<<orig_type<<", conv_type :"<<conv_type<<", path_val: "<<path_val<<endl;

        if (convertion_map.count(conv_type) == 0){     // stop condition                cout<<"return-"<<endl;
        return;
        }
        recursive_convert(orig_type, conv_type, path_val);

    }
        

    NumberWithUnits NumberWithUnits::convert_type(std::string newType) const{
        if(!existing_type(newType)){
            throw invalid_argument("un-existing unit");
        }
        //map<string, double> convertions = convertion_map.at(this->_type);
        string error_message = "Units do not match - [" + this->_type + "] cant convert to [" + newType + "]" ;
        if (convertion_map.count(this->_type)== 0 || convertion_map.at(this->_type).count(newType) == 0){
            map<string, double>convertions = convertion_map.at(newType);
            if (convertions.count(this->_type) == 0){
                throw invalid_argument(error_message);
            }
            else{
                double val = (convertions.at(this->_type));
                val = 1/val;
                return NumberWithUnits(this->_num*val, newType);
            }
        }
        else{
            map<string, double> convertions = convertion_map.at(this->_type);
            double val = convertions.at(newType);
            return NumberWithUnits(this->_num*val, newType);
        }

        // if (convertions.count(newType) == 0){
        //     convertions = convertion_map.at(newType);
        //     if (convertions.count(this->_type) == 0){
        //         throw invalid_argument(error_message);
        //     }
        //     else{
        //         double val = (convertions.at(this->_type));
        //         val = 1/val;
        //         return NumberWithUnits(this->_num*val, newType);
        //     }
        // }
        

    }


    //-------------------------------------
    // binary operators:
    //-------------------------------------

    const NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return NumberWithUnits(_num+new_other.get_num(), _type);
    }

    const NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return NumberWithUnits(_num-new_other.get_num(), _type);
    }


    const NumberWithUnits NumberWithUnits::operator+() const{   //??
        return NumberWithUnits(+_num, _type);
    }
    
    const NumberWithUnits NumberWithUnits::operator-() const{
        return NumberWithUnits(-_num, _type);
    }


    bool NumberWithUnits::operator==(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        const double eps = 0.00001;
        return abs(_num - new_other.get_num()) < eps;
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits& other) const{
        return (!(*this == other));
    }


    bool NumberWithUnits::operator>(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num > new_other.get_num();

    }
    bool NumberWithUnits::operator>=(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num >= new_other.get_num();

    }
    bool NumberWithUnits::operator<(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num < new_other.get_num();

    }
    bool NumberWithUnits::operator<=(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num <= new_other.get_num();

    } 

    //-------------------------------------
    // placament operators:
    //-------------------------------------

    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other){
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        _num += new_other.get_num();
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other){
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        _num -= new_other.get_num();
        return *this;
    }


    // prefix:
    NumberWithUnits& NumberWithUnits::operator++() {
        _num++;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator--() {
        _num--;
        return *this;
    }

    // postfix:
    const NumberWithUnits NumberWithUnits::operator++(int dummy_flag_for_postfix_increment) {
        NumberWithUnits copy = *this;
        _num++;
        return copy;
    }
    const NumberWithUnits NumberWithUnits::operator--(int dummy_flag_for_postfix_increment) {
        NumberWithUnits copy = *this;
        _num--;
        return copy;
    }


    //-------------------------------------
    // friend binary operators
    //-------------------------------------
            
    const NumberWithUnits operator*(const NumberWithUnits& n, double d){
        return NumberWithUnits(n.get_num()*d, n.get_type());
    }
    const NumberWithUnits operator*(double d, const NumberWithUnits& n){
        return n*d;
    }

    
    //----------------------------------
    // friend global IO operators
    //----------------------------------


    
    ostream& operator<< (ostream& os, const NumberWithUnits& n){
        return (os << n._num << '[' << n._type << ']');
    }

    istream& operator>> (std::istream& is, NumberWithUnits& c){
        string type;
        double num; // = 0;
        char ch;    // = ']';
        is >> num >> ch;
        while(ch != ']') {
            if(ch != '[' && ch != ' ') {
                type.insert(type.end(), ch);
            }
            is >> ch;
        }
        if(!NumberWithUnits::existing_type(type)){
            throw invalid_argument{"invalid unit"};
        }
        c._num = num;
        c._type = type;
        return is;
    }

    istream& NumberWithUnits::getAndCheckNextCharIs(istream& input, char expectedChar) {
        char actualChar;
        input >> actualChar;
        if (!input) return input;

        if (actualChar != expectedChar) 
            // failbit is for format error
            input.setstate(ios::failbit);
        return input;
    }
}
