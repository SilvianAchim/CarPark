#pragma once
#include <Windows.h>
#include <climits>
#include "Globals.h"


using namespace std;

class Animation {
public:

	void newLine(int lines) { // print new Lines
		for (int i = 0; i < lines; i++) {
			cout << endl;
		}
	}

	void showAnimation(string s, int newLines) { //center text animation
		newLine(newLines);
		for (unsigned int i = 0; i < 32 - s.size() / 2; i++) { // center the text
			cout << " ";
		}
		for (unsigned int i = 0; i < s.size(); i++) { // print the text to the screen
			cout << s[i];
			Sleep(global.animationSpeed); // animation speed
		}
	}

	void showAnimationLeft(string s, int newLines) { // align the text on the left
		newLine(newLines);
		cout << "    ";
		for (unsigned int i = 0; i < s.size(); i++) {
			cout << s[i];
			Sleep(global.animationSpeed); // animation speed
		}
	}

	void center(string s, int newLines) { //center align the string
		newLine(newLines);
		for (unsigned int i = 0; i < 32 - s.size() / 2; i++) { // center the text
			cout << " ";
		}
		for (unsigned int i = 0; i < s.size(); i++) { // print the text to the screen
			cout << s[i];
		}
	}

	void right(string s, int newLines) { // right align the string to the screen
		newLine(newLines);
		for (unsigned int i = 0; i < 64 - s.size(); i++) {
			cout << " ";
		}
		for (unsigned int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
	}

	void showTemplate() { // show the template to the screen
		cout << "################################################################" << endl;
		cout << "#######              Craiova parking system              #######" << endl;
		cout << "################################################################" << endl;
	}

	void updateScreen() { //update the screen with the default template
		system("cls");
		showTemplate();
	}

	void loadingBar(int animationTime) { // loading bar method
		for (int i = 0; i <= 100; i++) {
			updateScreen();
			center("PROCESSING PAYMENT:", 8);
			newLine(5);
			string s;
			if (i < 9) s.push_back(' ');
			s.append(to_string(i)+"% -|");
			for (int j = 1; j <= i/10; j++) {
				s.push_back('|');
			}
			for (int j = (i / 10) + 1; j <= 10; j++) {
				s.push_back(' ');
			}
			s.append("- 100%");
			center(s, 1);
			newLine(6);
			Sleep(animationTime);
		}
		
	}

	string animateDots(string s) { // get string s, animate the ending dots
		int nr = 0, i = s.size() - 1;
		char c = s[i];
		while (c == '.') {
			nr++;
			i--;
			c = s[i];
		}
		if (nr < 3) s.push_back('.');
		else s = s.substr(0, s.size() - 2);
		return s;
	}
	
private:
	Globals global;
};