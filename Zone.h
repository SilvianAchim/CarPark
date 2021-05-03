#pragma once
#include <set>
#include "Car.h"

using namespace std;

class Zone { // Zone class
public:
	string name;
	int size;
	long long price;
	multiset<Car> cars;

	Zone() {

	}
};