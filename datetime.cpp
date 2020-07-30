//defines the function for the datetime struct

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "datetime.h"

using namespace std;

//Default Constructor for current system time/date
DateTime::DateTime(){
    //code from HW3 assignment page
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    //returns as elapsed time so need to add 1900 and 1
    hour = timeinfo->tm_hour;
    min = timeinfo->tm_min;
    sec = timeinfo->tm_sec;
    year = timeinfo->tm_year + 1900;
    month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;
}

//Alternative Constructor with parameters
//switched mm and min
//added this -> to all lines
DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day){
    this -> hour = hh;
    this -> min = mm;           
    this -> sec = ss;
    this -> year = year;
    this -> month = month;
    this -> day = day;
}

//return true if the timestep is less than others
bool DateTime::operator<(const DateTime& other) const{
    if(year < other.year){
        return true;
    }
    else if (year > other.year){
        return false;
    }
    if(month < other.month){
        return true;
    }
    else if (month > other.month){
        return false;
    }
    if(day < other.day){
        return true;
    }
    else if (day > other.day){
        return false;
    }
    if(hour < other.hour){
        return true;
    }
    else if (hour > other.hour){
        return false;
    }
    if(min < other.min){
        return true;
    }
    else if (min > other.min){
        return false;
    }
    if(sec < other.sec){
        return true;
    }
    else if (sec > other.sec){
        return false;
    }
    return false;
}

//outputs the timestamp to the given ostream in format:
//YYYY-MM-DD HH:MM:SS
//return the ostream passed in as an argument
std::ostream& operator<<(std::ostream& os, const DateTime& other){
    os << setw(4) << setfill('0') << other.year;
    os << "-";
    os << setw(2) << setfill('0') << other.month;
    os << "-";
    os << setw(2) << setfill('0') << other.day;
    os << " ";
    os << setw(2) << setfill('0') << other.hour;
    os << ":";
    os << setw(2) << setfill('0') << other.min;
    os << ":";
    os << setw(2) << setfill('0') << other.sec;
    return os;
}

//Inputs the timestamp from the given istream expecting the format:
//YYYY-MM-DD HH:MM:SS
//Returns if an error in the format with the DateTime set to the
//current system time/date
//return the istream passed in as an argument
//if the stringstream fails, returns the current datetime
std::istream& operator>>(std::istream& is, DateTime& dt){

    std::string temp;
    //get year
    getline(is,temp,'-');
    stringstream ss;
    ss.str(temp);
    ss >> dt.year;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    //get month
    getline(is,temp,'-');
    ss.str(temp);
    ss >> dt.month;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    //get day
    getline(is,temp,' ');
    ss.str(temp);
    ss >> dt.day;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    //get hour
    getline(is,temp,':');
    ss.str(temp);
    ss >> dt.hour;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    //get minute
    getline(is,temp,':');
    ss.str(temp);
    ss >> dt.min;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    //get second
    getline(is,temp,' ');
    ss.str(temp);
    ss >> dt.sec;
    if (ss.fail()){
        DateTime current;
        dt = current;
        return is;
    }
    ss.clear();

    return is;
}