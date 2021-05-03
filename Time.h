#pragma once
#include <string>

using namespace std;

class Time {
public:
	string nextTime(string s) { // take time s, return s+1;
		//s -> hh:mm
		int hour = (s[0] - '0') * 10 + (s[1] - '0');
		int minute = (s[3] - '0') * 10 + (s[4] - '0');
		string h, m;
		minute++;
		if (minute >= 60) {
			minute %= 60;
			hour++;
			if (hour >= 24) hour %= 24;
		}
		if (minute <= 9) m.push_back('0');
		if (hour <= 9) h.push_back('0');
		m.append(to_string(minute));
		h.append(to_string(hour));
		string output;
		output.append(h + ':' + m);
		return output;
	}

	string add(string s) { // add PM AM to time 's'
		int hour = (s[0] - '0') * 10 + (s[1] - '0');
		if (hour < 12) s.append(" AM");
		else s.append(" PM");
		return s;
	}

	int difference(string time1, string time2) {// return time1-time2 in minutes
		int hour1 = (time1[0] - '0') * 10 + (time1[1] - '0'); // tranform hour1 to int
		int minute1 = (time1[3] - '0') * 10 + (time1[4] - '0'); // tranform minutes1 to int

		int hour2 = (time2[0] - '0') * 10 + (time2[1] - '0'); // tranform hour2 to int
		int minute2 = (time2[3] - '0') * 10 + (time2[4] - '0'); // tranform minutes2 to int

		int output = 0; // the minutes needed to to reach time1, from time 2

		//first make both hour's minutes equal
		if (minute1 >= minute2) output += minute1 - minute2; // increment the output with the needed minutes
		else {
			output += (60 - minute2) + minute1; // increment the output with the needed minutes
			hour2++; 
			hour2 %= 24;
		}
		//make the hours equal
		if (hour1 >= hour2) output += (hour1 - hour2) * 60;
		else output += ((24 - hour2) + hour1) * 60; // increment the output with the needed minutes

		return output;
	}

	string round15min(string start, string end) { // return the end time when the difference of the minutes between the time will be divisible with 15
		int diff = difference(end, start); // the difference in minutes between the 2 times;
		while (diff % 15 != 0 || diff == 0) {
			end = nextTime(end); // increment the end by 1 minute
			diff = difference(end, start);// calculate the difference again
		}
		return end;
	}

private:

};
