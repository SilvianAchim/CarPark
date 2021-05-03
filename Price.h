#pragma once
#include <string>
#include <set>
#include <map>
#include <cmath>
#include "Time.h"
#include "Globals.h"


using namespace std;

class Price {
public:
	//I have a set with different time boundaries. Then I insert the start and the end hour
	// to the set, and find the upper_bound of the start hour. Then add to the final sum
	// the time between the upper_bound and the previous value in the set, and then I increment
	// both values, until we reach the end hour.
	long long price(string start, string end, int zone) { //get the start time and the end time and the zone, returns the price
		set<string> times; // A set with the time boundaries peek/off peek hours
		map<string, int> hours; // each hour's category
		initializeTimes(times, hours, start, end);
		using ll = long long;
		ll price = 0; // the final price for the zone "i"
		auto i = times.upper_bound(start);
		if (i == times.end()) i = times.begin(); // ??
		while (true) {
			auto j = i;
			if (i == times.begin()) j = prev(times.end());
			else j--;
			// add high/off/normal hour vector
			price += ll(ceil(float(time.difference(*i, *j)) * ((global.zones[zone].price + global.hourDiff[hours[*j]]) / 60.)));
			if (*i == end) break;
			i++;
			if (i == times.end()) i = times.begin();
		}
		//add the discounts
		int diff = time.difference(end, start);
		if (diff > 60 && diff <= 180) {
			// 10% discount
			price -= ll(floor((10. * price) / 100.));
		}
		else if (diff > 180) {
			// 20% discount
			price -= ll(floor((20. * price) / 100.));
		}
		return price;
	}
private:
	Time time;
	Globals global;

	void initializeTimes(set<string>& s, map<string, int>& m, string start, string end) {
		s.emplace("07:00");
		m["07:00"] = 2;

		s.emplace("10:00");
		m["10:00"] = 0;

		s.emplace("19:00");
		m["19:00"] = 1;

		s.emplace("22:00");
		m["22:00"] = 0;

		s.emplace(start);
		s.emplace(end);

		auto i = m.upper_bound(end);
		if (i == m.end()) i = m.begin();

		m[end] = i->second;
	}

};