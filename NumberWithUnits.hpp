#pragma once

#include <iostream>
#include <string>
#include <map>


namespace ariel{

    class NumberWithUnits{
        private:
            double _num;
            std::string _type;

            static std::istream& getAndCheckNextCharIs(std::istream& input, char expectedChar);
            NumberWithUnits convert_type(std::string newType) const;
            
        public:
            NumberWithUnits(double num, std::string type);
            ~NumberWithUnits(){}

            static bool existing_type(std::string type);
            static void read_units(std::ifstream& units_file);
            static void recursive_convert(std::string type, std::string temp_type, double last_val);


            //-------------------------------------
            // getters:
            //-------------------------------------

            double get_num() const{
                return _num;
            };

            std::string get_type() const{
                return _type;
            }; 


            //-------------------------------------
            // binary operators:
            //-------------------------------------

            const NumberWithUnits operator+(const NumberWithUnits& other) const;
            const NumberWithUnits operator-(const NumberWithUnits& other) const;
            
            const NumberWithUnits operator+() const;
            const NumberWithUnits operator-() const;


            bool operator==(const NumberWithUnits& other) const;
            bool operator!=(const NumberWithUnits& other) const;

            bool operator>(const NumberWithUnits& other) const;
            bool operator>=(const NumberWithUnits& other) const;
            bool operator<(const NumberWithUnits& other) const;
            bool operator<=(const NumberWithUnits& other) const;


            NumberWithUnits& operator+=(const NumberWithUnits& other);
            NumberWithUnits& operator-=(const NumberWithUnits& other);


            // prefix:
            NumberWithUnits& operator++();
            NumberWithUnits& operator--();

            // postfix:
            const NumberWithUnits operator++(int dummy_flag_for_postfix_increment);
            const NumberWithUnits operator--(int dummy_flag_for_postfix_increment);


            //-------------------------------------
            // friend global binary operators
            //-------------------------------------
            
            friend const NumberWithUnits operator*(const NumberWithUnits& n, double d);
            friend const NumberWithUnits operator*(double d, const NumberWithUnits& n);


            //----------------------------------
            // friend global IO operators
            //----------------------------------

            friend std::ostream& operator<< (std::ostream& os, const NumberWithUnits& c);
            friend std::istream& operator>> (std::istream& is, NumberWithUnits& c);
        

    };
}