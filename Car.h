#pragma once
#include <string>

using namespace std;

class Car { // Car class
public:
    string end;
    string start;
    string plate;
    string name;
    string id;
    bool extend;

    Car(string st, string ed, string pl, string na, string i, bool ex) { //Build class
        start = st;
        end = ed;
        plate = pl;
        name = na;
        id = i;
        extend = ex;
    }

    bool operator < (const Car& c) const { //Overwrite operator '<' so we can sort the multiset
        if (this->end == c.end) { // if the end times are equal sort by the start time
            return this->start < c.start;
        }
        else return this->end < c.end; // sort by the end time ascending 
    }
};
