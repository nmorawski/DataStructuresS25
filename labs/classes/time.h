#ifndef __TIME_H
#define __TIME_H

#include <iostream>

class Time {
public:
Time();
Time(int anHour, int aMinute, int aSecond);
// ACCESSORS
int getHour() const;
int getMinute() const;
int getSecond() const;
// MODIFIERS
void setHour(int anHour);
void setMinute(int aMinute);
void setSecond(int aSecond);

void PrintAMPM() const;

private: // REPRESENTATION (member variables)
int second;
int minute;
int hour;
};

//bool IsEarlierThan(const Time& t1, const Time& t2);

#endif