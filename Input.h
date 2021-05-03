#pragma once
#include <fstream>
#include "Globals.h"

using namespace std;

class Input {
public:
	void init() { // read the settings and initialize all the variables
		ifstream fin("settings.txt");
		char input[200];
		//Input zones
		for (int i = 0; i < global.nrZones; i++) {
			fin.getline(input, 199);
			global.zones[i].name = charToString(input); // Zone name
			fin.getline(input, 199);
			global.zones[i].size = charToInt(input); // Zone capacity
			fin.getline(input, 199);
			global.zones[i].price = charToInt(input); // Zone price $/hour
			fin.getline(input, 5); // empty line
		}
		//Input hour differences 
		for (int i = 0; i < 3; i++) {
			fin.getline(input, 199);
			global.hourDiff[i] = charToInt(input);
		}
		fin.getline(input, 5); // empty line
		fin.getline(input, 199);
		global.timeSpeed = charToInt(input);
		fin.getline(input, 199);
		global.animationSpeed = charToInt(input);
		fin.close();
	}
private:
	Globals global;

	string charToString(char input[]) { // takes a char vector, returns a string
		string output;
		char* i = strchr(input, '"');
		int j = 1;
		while (i[j] != '"') {
			output.push_back(i[j]);
			j++;
		}
		return output;
	}

	int charToInt(char input[]) { // takes a char vector, returns an int
		int output = 0;
		char* i = strchr(input, '"');
		int j = 1;
		while (i[j] != '"') {
			output = output * 10 + (i[j] - '0');
			j++;
		}
		return output;
	}
};