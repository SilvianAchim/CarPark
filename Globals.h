#pragma once
#include <fstream>
#include <iostream>
#include "Zone.h"

using namespace std;


class Globals {
public:
	const static int nrZones=3; //If chaged, make sure to chage the settings as well
	static int hourDiff[3]; // 0-> rushHour, 1-> normalHour, 2-> offHour;
	static int timeSpeed, animationSpeed;
	static Zone zones[nrZones];
};

