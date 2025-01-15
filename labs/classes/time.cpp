#include <iostream>
#include "time.h"

Time::Time() { //default constructor
    hour = 0;
    minute = 0;
    second = 0;
}
Time::Time(int anHour, int aMinute, int aSecond) { // construct from hour, minutes, & second
    hour = anHour;
    minute = aMinute;
    second = aSecond;
}
int Time::getHour() const {
    return hour;
}
int Time::getMinute() const {
    return minute;
}
int Time::getSecond() const {
    return second;
}
void Time::setHour(int h) {
    hour = h;
}
void Time::setMinute(int m) {
    minute = m;
}
void Time::setSecond(int s) {
    second = s;
}
void Time::PrintAMPM() const {
    if (hour >= 12 && hour < 24){
        int hour_time = hour-12;
        if (hour_time == 0){
            hour_time = 12;
        }
        std::cout << hour_time << ":" << ((minute < 10) ? "0" : "") << minute 
            << ":" << ((second < 10) ?"0":"") << second << " pm" << std::endl;
    } else {
        if (hour == 24 || hour == 0){
            std::cout << "12" << ":" << ((minute < 10) ? "0" : "") << minute 
            << ":" << ((second < 10) ? "0" : "") << second << " am" << std::endl;
        } else {
        std::cout << hour << ":" << ((minute < 10) ? "0" : "") << minute 
            << ":" << ((second < 10) ? "0" : "") << second << " am" << std::endl;
        }
    } 
}
/*
bool IsEarlierThan(const Time& t1, const Time& t2){
    if (t1.getHour() < t2.getHour())
        return true;
    else if (t2.getHour() < t1.getHour())
        return false;

    if (t1.getMinute() < t2.getMinute())
        return true;
    else if (t2.getMinute() < t1.getMinute())
        return false;

    if (t1.getSecond() < t2.getSecond())
        return true;
    else if (t2.getSecond() < t1.getSecond())
        return false;

    return false;
}
*/