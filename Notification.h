#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <climits>
#include <set>
#include <Windows.h>
#include "Globals.h"
#include "Car.h"
#include "Time.h"
#include "Animation.h"
#include "Price.h"


using namespace std;

class Notification {
public:
	void showNotification(string time) { //show notification
		// check extend
		for (int i = 0; i < global.nrZones; i++) {
			auto x = global.zones[i].cars.lower_bound(Car("00:00", time, "0", "0", "0", true));
			if (x != global.zones[i].cars.end()) {
				while (tim.difference(x->end, time) <= 10) {
					if (x->extend) {
						animation.updateScreen();
						animation.showAnimation("Hello " + x->name + ",", 4);
						animation.showAnimation("Your booking for " + x->id + " expires in 10 minutes,", 2);
						animation.showAnimation("but you can extend it!", 2);
						animation.showAnimation("Do you want to extend?", 4);
						bool confirm = confirmExtend(x->name, x->id);
						if (confirm) {
							string endHour;
							bool selectHour = false;
							while (!selectHour) {
								animation.updateScreen();
								animation.showAnimation("Enter the new end hour: (HH:MM, eg: 08:09, 23:39)", 6);
								animation.newLine(14);
								Sleep(175);
								cin >> endHour; // enter end hour of the new desired appointment

								int timeDiff = tim.difference(endHour, x->end);
								if (timeDiff == 0 || timeDiff % 15 != 0) { // because the smallest package available if 15 min, we round the time to 15 mins
									string aux = endHour;
									endHour = tim.round15min(x->end, endHour);
									if (aux < x->end && endHour >= x->end) { // more than 24;
										animation.updateScreen();
										animation.showAnimation("The smallest available package is 15 minutes!", 3);
										animation.showAnimation("So, the end hour was updated to: " + endHour, 2);
										animation.showAnimation("Unfortunately our largest available package is 24 hours, so", 5);
										animation.showAnimation("your hours are invalid. Please try again with different hours!", 2);
										flushCin();
										Sleep(global.timeSpeed * 3);
										continue;
									}
									animation.updateScreen();
									animation.showAnimation("The smallest package available is 15 mins!", 3);
									animation.showAnimation("So, the end hour was updated to: " + endHour, 2);
									animation.newLine(2);
									Sleep(global.timeSpeed * 3);
								}
								if (freeSpaces(time, endHour, i)) {
									// available,show the price -> pay/cancel
									animation.updateScreen();
									animation.showAnimation("To extend the time:", 6);
									animation.showAnimation("From: " + tim.add(x->end), 2);
									animation.showAnimation("To: " + tim.add(endHour), 1);
									animation.showAnimation("Costs " + to_string(price.price(x->start, endHour, i)) + "$!", 2);
									bool pay = confirmPay(x->end, endHour, to_string(price.price(x->start, endHour, i)) + "$");
									if (pay) {
										//process
										animation.updateScreen();
										animation.showAnimation("PROCESSING PAYMENT:", 8);
										animation.newLine(5);
										animation.loadingBar(global.animationSpeed / 2);
										Sleep(global.timeSpeed * 2);

										//Succes screen
										animation.updateScreen();
										animation.showAnimation("PAYMENT SUCCESSFUL!", 6);
										animation.center("---------------------", 1);
										animation.showAnimation("PAID: " + to_string(price.price(x->start, endHour, i)) + " $!", 4);
										string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
										string transaction;
										for (int i = 0; i < 20; i++) {
											transaction.push_back(chars[rand() % chars.size()]);
										}
										animation.showAnimation("Transaction ID: " + transaction, 3); // + ...
										animation.newLine(6);
										Sleep(global.timeSpeed * 2);

										//Receipt screen
										animation.updateScreen();
										animation.showAnimation("PARKING RECEIPT", 3);
										animation.center("-----------------", 1);
										animation.showAnimation("The ending time has been updated!", 3);
										animation.showAnimation("Old end hour: " + tim.add(x->end), 3);
										animation.showAnimation("New end hour: " + tim.add(endHour), 1);
										animation.showAnimation("Thanks for choosing Craiova car park!", 7);

										selectHour = true;
										global.zones[i].cars.emplace(Car(x->start, endHour, x->plate, x->name, x->id, true));
										auto j = x;
										x++;
										global.zones[i].cars.erase(j);
										Sleep(global.timeSpeed * 3);
									}
									else {
										flushCin();
										selectHour = true;
										global.zones[i].cars.emplace(Car(x->start, x->end, x->plate, x->name, x->id, false));
										auto j = x;
										x++;
										global.zones[i].cars.erase(j);
										Sleep(global.timeSpeed / 3);
									}
									
								}
								else {
									animation.updateScreen();
									animation.showAnimation("Unfortunately your desired end hour is unavailable!", 5);
									animation.showAnimation("Try another one or cancel!", 2);
									bool anotherOne = confirmAgain();
									if (!anotherOne) {
										flushCin();
										selectHour = true;
										global.zones[i].cars.emplace(Car(x->start, x->end, x->plate, x->name, x->id, false));
										auto j = x;
										x++;
										global.zones[i].cars.erase(j);
										Sleep(global.timeSpeed / 3);
									}
									else flushCin();
								}
							}
						}
						else {
							global.zones[i].cars.emplace(Car(x->start, x->end, x->plate, x->name, x->id, false));
							auto j = x;
							x++;
							global.zones[i].cars.erase(j);
							Sleep(global.timeSpeed / 3);
						}
					}
					else if (x != global.zones[i].cars.end()) x++;
					if (x == global.zones[i].cars.end()) break;
					GetAsyncKeyState(VK_RETURN);
				}
			}
		}
		//check expire
		for (int i = 0; i < global.nrZones; i++) {
			auto x = global.zones[i].cars.lower_bound(Car("00:00", time, "0", "0", "0", false));
			if (x != global.zones[i].cars.end()) {
				while (tim.difference(x->end, time) == 1) {
					animation.updateScreen();
					animation.showAnimation("Hello " + x->name + ",", 5);
					animation.showAnimation("Your booking for the space " + x->id + " has expired!", 2);
					animation.newLine(13);
					auto j = x;
					x++;
					global.zones[i].cars.erase(j);
					Sleep(global.timeSpeed * 3);
					if (x == global.zones[i].cars.end()) break;
				}
			}
		}
		
	}
private:
	Globals global;
	Time tim;
	Animation animation;
	Price price;

	void flushCin() {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}

	void drawArrowPayment(int index, string time1, string time2, string price) {
		animation.updateScreen();
		animation.center("To extend the time:", 6);
		animation.center("From: " + tim.add(time1), 2);
		animation.center("To: " + tim.add(time2), 1);
		animation.center("Costs " + price, 2);
		animation.newLine(6);
		if (index == 0) { // arrow pointing to confirm
			cout << "                      -> CONFIRM PAYMENT" << endl;
			cout << "                             CANCEL" << endl;
		}
		else { // arrow pointing to cancel
			cout << "                         CONFIRM PAYMENT" << endl;
			cout << "                      ->     CANCEL" << endl;
		}
		animation.newLine(1);
	}

	bool confirmPay(string time1, string time2, string price) {
		bool start = true;
		int index = 0;
		while (true) {
			if (start) {
				start = false;
				GetAsyncKeyState(VK_UP); // flush arrow key up GetAsyncKeyState
				GetAsyncKeyState(VK_DOWN); // flush arrow key down GetAsyncKeyState
				GetAsyncKeyState(VK_RETURN); // flush enter GetAsyncKeyState
				drawArrowPayment(index, time1, time2, price);
			}
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) { // if one arrow was pressed
				if (index == 0) index++; // if index was 0, go to one
				else index--; // else go to 0
				drawArrowPayment(index, time1, time2, price);
				Sleep(175);
			}
			if (GetAsyncKeyState(VK_RETURN)) {
				GetAsyncKeyState(VK_RETURN);
				cin.clear(); // flush cin
				cin.ignore(INT_MAX, '\n'); // flush cin
				animation.updateScreen();
				return !index; // 0->confirm, 1->cancel, so I revert those
			}
		}
	}

	void drawArowAgain(int index) {
		animation.updateScreen();
		animation.center("Unfortunately your desired end hour is unavailable!", 5);
		animation.center("Try another one or cancel!", 2);
		animation.center("Try another one?", 6);
		animation.newLine(4);
		if (index == 0) { // arrow pointing to confirm
			cout << "                           ->  YES" << endl;
			cout << "                              CANCEL" << endl;
		}
		else { // arrow pointing to cancel
			cout << "                               YES" << endl;
			cout << "                           -> CANCEL" << endl;
		}
		animation.newLine(1);
	}

	bool confirmAgain() {
		bool start = true;
		int index = 0;
		while (true) {
			if (start) {
				start = false;
				GetAsyncKeyState(VK_UP); // flush arrow key up GetAsyncKeyState
				GetAsyncKeyState(VK_DOWN); // flush arrow key down GetAsyncKeyState
				GetAsyncKeyState(VK_RETURN); // flush enter GetAsyncKeyState
				drawArowAgain(index);
			}
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) { // if one arrow was pressed
				if (index == 0) index++; // if index was 0, go to one
				else index--; // else go to 0
				drawArowAgain(index);
				Sleep(175);
			}
			if (GetAsyncKeyState(VK_RETURN)) {
				GetAsyncKeyState(VK_RETURN);
				cin.clear(); // flush cin
				cin.ignore(INT_MAX, '\n'); // flush cin
				animation.updateScreen();
				return !index; // 0->confirm, 1->cancel, so I revert those
			}
		}
	}

	void drawArrows(int index, string name, string id) {
		animation.updateScreen();
		animation.center("Hello " + name + ",", 4);
		animation.center("Your booking for "+id+" expires in 10 minutes,", 2);
		animation.center("but you can extend it!", 2);
		animation.center("Do you want to extend?", 4);
		animation.newLine(4);
		if (index == 0) { // arrow pointing to confirm
			cout << "                           -> EXTEND" << endl;
			cout << "                              CANCEL" << endl;
		}
		else { // arrow pointing to cancel
			cout << "                              EXTEND" << endl;
			cout << "                           -> CANCEL" << endl;
		}
		animation.newLine(2);
	}

	int confirmExtend(string name, string id) {
		bool start = true;
		int index = 0;
		while (true) {
			if (start) {
				start = false;
				GetAsyncKeyState(VK_UP); // flush arrow key up GetAsyncKeyState
				GetAsyncKeyState(VK_DOWN); // flush arrow key down GetAsyncKeyState
				GetAsyncKeyState(VK_RETURN); // flush enter GetAsyncKeyState
				drawArrows(index, name, id);
			}
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) { // if one arrow was pressed
				if (index == 0) index++; // if index was 0, go to one
				else index--; // else go to 0
				drawArrows(index, name, id);
				Sleep(175);
			}
			if (GetAsyncKeyState(VK_RETURN)) {
				GetAsyncKeyState(VK_RETURN);
				cin.clear(); // flush cin
				cin.ignore(INT_MAX, '\n'); // flush cin
				animation.updateScreen();
				return !index; // 0->confirm, 1->cancel, so I revert those
			} 
		}
	}

	int freeSpaces(string start, string end, int zone) { // return how many spaces zone "i" has from hour start to end
		int output = global.zones[zone].size; // the number of spaces left
		multiset<array<string, 2>> times; // relevant times 
		for (auto& i : global.zones[zone].cars) { // store the relevant times and sort them after the ending time
			if ((i.start >= start && i.start <= end) || (i.end >= start && i.start < start)) {
				times.insert({ i.end, i.start}); // end time first, start time second
			}
		}
		string s; // start hour
		for (auto d = times.begin(); d != times.end(); d++) { // minimize the spaces inside the zone
			bool minimize = false;// if it was minimize       // eg: a car leaves at 13:01 and the other comes at 13:02
			s = d->at(0);                                    // this will use only 1 space, instead of 2
			for (auto i = d; i != times.end();) { // loop through the relevant times
				if (i->at(1) >= s) { // if it can be minimized, delete the minimized element from the multiset
					s = i->at(0); // curent time = end time of the minimized element
					auto j = i;
					i++; // go to the next element
					times.erase(j); // delete it
					minimize = true; // a parking space can be minimized
				}
				else i++; // go to the next element
			}
			if (minimize) { // if the spaces were minimized
				output--; // reduce the total spaces by one
				times.erase(d); // erase the first element from the multiset
				minimize = false;
				d = times.begin(); //check again if the curent spaces can be minimized
			}
		}
		output -= times.size(); // the rest of the spaces cannot be minimized, so each one of them will take an entire space
		return output; // return the number of free spaces of zone "i"
	}
};