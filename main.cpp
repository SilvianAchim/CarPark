#include <iostream>
#include <Windows.h>
#include <climits>
#include "Register.h"
#include "Globals.h"
#include "Time.h"
#include "Input.h"
#include "Zone.h"
#include "Animation.h"
#include "Notification.h"
#include "Car.h"

using namespace std;

string startTime = "00:00 AM"; // initialize start time
string text = "To register a car press enter.";

Globals global; // create objects
Register regist;
Input input;
Time time;
Animation animation;
Notification notification;

int Globals::animationSpeed = 0; // setup static variables
int Globals::timeSpeed = 0;
int Globals::hourDiff[] = {};
Zone Globals::zones[] = {};

void flush() {
	cin.clear(); // flush cin
	cin.ignore(INT_MAX, '\n'); // flush cin
	GetAsyncKeyState(VK_RETURN);
}


int main() {
	//Resize console
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 550, 425, TRUE); //width, length 

	//Read the settings
	input.init();

	//App loop
	while (true) {
		animation.updateScreen();
		animation.right("Time: " + startTime, 1); // right align the text;
		notification.showNotification(startTime); // the notification
		startTime = time.nextTime(startTime); // increment the time
		startTime = time.add(startTime); // add AM/PM
		animation.center(text, 8); // center align the text
		text = animation.animateDots(text); // animate the dots
		animation.newLine(11); // 11 blank lines
		if (GetAsyncKeyState(VK_RETURN)) { // if enter was pressed, register a car
			regist.registerCar(); // register a car
			flush(); // flush cin
		}
		Sleep(global.timeSpeed);
	}
}