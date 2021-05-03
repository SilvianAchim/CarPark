#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <climits>
#include "Globals.h"
#include "Animation.h"
#include "Time.h"
#include "Price.h"


using namespace std;

class Register {
public:
	void registerCar() { // register a car method
		//Welcome + Enter the start hour screen
		flushCin(); // flush cin
		animation.updateScreen();
		animation.showAnimation("Welcome to Craiova Car park!", 3);
		Sleep(sleepTime / 3);
		animation.showAnimation("Enter the start hour:   (HH:MM, eg: 08:09, 23:39)", 3);
		animation.newLine(13);
		Sleep(keyPressDelay);
		cin >> startHour; // enter start hour of the desired appointment

		//Enter end hour screen
		flushCin(); // flush cin
		animation.updateScreen();
		animation.center("Welcome to Craiova Car park!", 3);
		animation.showAnimation("Enter the end hour:   (HH:MM, eg: 08:09, 23:39)", 3);
		animation.newLine(14);
		Sleep(keyPressDelay);
		cin >> endHour; // enter end hour of the desired appointment

		//If the desired hours are invalid
		int timeDiff = time.difference(endHour, startHour);
		if (timeDiff == 0 || timeDiff % 15 != 0) { // because the smallest package available if 15 min, we round the time to 15 mins
			string aux = endHour;
			endHour = time.round15min(startHour, endHour);
			if (aux < startHour && endHour >= startHour) { // more than 24;
				animation.updateScreen();
				animation.showAnimation("The smallest available package is 15 minutes!", 3);
				animation.showAnimation("So, the end hour was updated to: " + endHour, 2);
				animation.showAnimation("Unfortunately our largest available package is 24 hours, so", 5);
				animation.showAnimation("your hours are invalid. Please try again with different hours!", 2);
				Sleep(global.timeSpeed * 3);
				return;
			}
			animation.updateScreen();
			animation.showAnimation("The smallest package available is 15 mins!", 3);
			animation.showAnimation("So, the end hour was updated to: " + endHour, 2);
			animation.newLine(2);
			Sleep(global.timeSpeed);
		}

		initializeIds(avalableIds); // initialize space ids

		selected = selectZone(startHour, endHour); // get the index of the selected zone
		if (!selected) return; // !selected means cancel was pressed

		//Enter licence plate screen
		flushCin(); // flush cin
		animation.updateScreen();
		animation.showAnimation("Enter your licence plate:", 5);
		animation.newLine(15);
		Sleep(keyPressDelay);
		cin >> plate;

		//Enter your name screen
		flushCin(); // flush cin
		animation.updateScreen();
		animation.showAnimation("Enter your name:", 5);
		animation.newLine(15);
		Sleep(keyPressDelay);
		cin >> name;

		bool confirm = confirmPayment(); //confirm the payment or not
		if (!confirm) return;

		//Processing payment screen
		animation.updateScreen();
		animation.showAnimation("PROCESSING PAYMENT:", 8);
		animation.newLine(5);
		animation.loadingBar(global.animationSpeed / 2);
		Sleep(global.timeSpeed);

		//Succes screen
		animation.updateScreen();
		animation.showAnimation("PAYMENT SUCCESSFUL!", 6);
		animation.center("---------------------", 1);
		animation.showAnimation("PAID: " + to_string(price.price(startHour, endHour, selected - 1)) + " $", 4);
		string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		string transaction;
		for (int i = 0; i < 20; i++) {
			transaction.push_back(chars[rand() % chars.size()]);
		}
		animation.showAnimation("Transaction ID: " + transaction, 3); // + ...
		animation.newLine(6);
		Sleep(sleepTime / 2);

		//Receipt screen
		animation.updateScreen();
		animation.showAnimation("PARKING RECEIPT", 3);
		animation.center("-----------------", 1);
		animation.showAnimation("Start hour: " + time.add(startHour), 3);
		animation.showAnimation("End hour: " + time.add(endHour), 1);
		animation.showAnimation("Space: " + *(avalableIds[selected - 1].begin()), 3);
		animation.showAnimation("Thanks for choosing Craiova car park!", 8);

		//add the car to the zone's set
		global.zones[selected - 1].cars.emplace(Car(startHour, endHour, plate, name, *(avalableIds[selected - 1].begin()), true));

		Sleep(global.timeSpeed * 3);
	}
private:
	Globals global;
	Animation animation;
	Time time;
	Price price;

	vector<set<string>> avalableIds;
	int selected;
	string startHour, endHour, name, plate;
	const int sleepTime = 2000; // the delay after a text was printed to the screen
	const int keyPressDelay = 175; // the delay after a key was pressed

	void initializeIds(vector<set<string>>& ids) { // initialize the vector, with all the ids for each zone
		ids.clear(); // clear the current content of the vector
		for (int i = 0; i < global.nrZones; i++) { // for each zone
			set<string> aux;
			string prefix;
			for (int d = 0; d < 3; d++) { // create the prefix name
				char c = global.zones[i].name[d];
				prefix.push_back(toupper(c));
			}
			for (int j = 0; j < global.zones[i].size; j++) { // add all ids to the set
				string id = "ID-" + prefix;
				id.append(to_string(i + 1)); //in case the prefix of the zone are the same, we can have unique ids
				if (j < 9) {
					id.push_back('0');
					id.append(to_string(j + 1));
				}
				else id.append(to_string(j + 1));
				aux.emplace(id);
			}
			avalableIds.emplace_back(aux); // add all the ids to the vector
		}
	}

	void flushCin() { // flush cin
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}

	void drawArrow(int pointingArrow, vector<int>freeZones, string start, string end) { // draw the arrow and the zones to the screen
		animation.center("-Use up/down arrow keys to navigate-", 1);
		animation.center("--Press enter to select--", 1);
		animation.newLine(8);
		for (int i = 0; i < global.nrZones; i++) { // print the zones and the arrow to the screen
			if (pointingArrow - 1 == i) cout << "  -> ";
			else cout << "     ";
			cout << global.zones[i].name << "    Price: " << price.price(start, end, i) << "$    Free: " << freeZones[i] << "/" << global.zones[i].size << "\n";
		}
		animation.newLine(3);
		if (pointingArrow > global.nrZones) cout << "  -> Cancel"; // if the arrow is pointing towards the cancel button
		else cout << "     Cancel";
		animation.newLine(1);
	}

	int free(string start, string end, int zone) { // return how many spaces zone "i" has from hour start to end
		int output = global.zones[zone].size; // the number of spaces left
		multiset<array<string, 3>> times; // relevant times 
		for (auto& i : global.zones[zone].cars) { // store the relevant times and sort them after the ending time
			if ((i.start >= start && i.start <= end) || (i.end >= start && i.start < start)) {
				times.insert({ i.end, i.start, i.id }); // end time first, start time second
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
					avalableIds[zone].erase(j->at(2));
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
		for (auto i = times.begin(); i != times.end();) {
			auto j = i;
			i++;
			avalableIds[zone].erase(j->at(2));
		}
		return output; // return the number of free spaces of zone "i"
	}

	int selectZone(string start, string end) { // select a zone method 
		int pointingArrow = 1; // pointing arrow index
		vector<int> freeZones(Globals::nrZones); // the number of spaces free in zone[i]
		for (int i = 0; i < Globals::nrZones; i++) { // initialize the freeZones
			freeZones[i] = free(start, end, i);
		}
		bool startt = true;
		while (true) {
			if (startt) { // if it's the first time
				animation.updateScreen();
				animation.showAnimation("Please select one zone!", 3);
				drawArrow(pointingArrow, freeZones, start, end);
				startt = false;
				Sleep(global.timeSpeed);
				GetAsyncKeyState(VK_RETURN); //flush enter GetAsyncKeyState
			}
			if (GetAsyncKeyState(VK_DOWN)) { //press down arrow, arrow moving down, update the screen
				Sleep(keyPressDelay);
				pointingArrow++; // move the arrow down
				if (pointingArrow > global.nrZones + 1) pointingArrow = 1; // Instead of going under the last element, go to the first element
				animation.updateScreen();
				animation.center("Please select one zone!", 3);
				drawArrow(pointingArrow, freeZones, start, end);
			}
			if (GetAsyncKeyState(VK_UP)) { //press up arrow, arrow moving up, update the screen
				Sleep(keyPressDelay);
				pointingArrow--; // move the arrow up
				if (pointingArrow == 0) pointingArrow = global.nrZones + 1; // Instead of going above the first element, go to the last element
				animation.updateScreen();
				animation.center("Please select one zone!", 3);
				drawArrow(pointingArrow, freeZones, start, end);
			}
			if (GetAsyncKeyState(VK_RETURN)) { // press enter to select
				flushCin();
				Sleep(keyPressDelay / 4);
				if (pointingArrow == global.nrZones + 1) return 0; // pressed cancel was pressed;
				else if (!freeZones[pointingArrow - 1]) { // zone full, try again, update screen
					animation.updateScreen();
					animation.showAnimation("Unfortunately the selected zone has no spaces left.", 6);
					animation.showAnimation("Comeback later or select another zone!", 2);
					Sleep(sleepTime);
					pointingArrow = 1; // the arrow is pointing towards the first element
					animation.updateScreen();
					animation.center("Please select one zone!", 3);
					drawArrow(pointingArrow, freeZones, start, end);
				}
				else return pointingArrow;
			}
		}
	}

	void drawArrowPayment(int index) { // draw arrow for confirm payment
		if (index == 0) { // arrow pointing to confirm
			cout << "                      -> CONFIRM PAYMENT" << endl;
			cout << "                             CANCEL" << endl;
		}
		else { // arrow pointing to cancel
			cout << "                         CONFIRM PAYMENT" << endl;
			cout << "                      ->     CANCEL" << endl;
		}
	}

	void drawPayment(int index) { // draw payment screen
		animation.updateScreen();
		animation.center("Confirm Payment: ", 3);
		animation.newLine(5);
		cout << "    Start hour: " << time.add(startHour) << endl;
		cout << "    End hour: " << time.add(endHour) << endl;
		cout << "    License plate: " << plate << endl;
		cout << "    Name: " << name << endl;
		cout << "    Total price: " << price.price(startHour, endHour, selected - 1) << "$" << endl;
		animation.newLine(4);
		drawArrowPayment(index);
		animation.newLine(1);
	}

	bool confirmPayment() { // confirm payment screen
		bool start = true;
		int index = 0;
		while (true) {
			if (start) {
				animation.updateScreen();
				animation.showAnimation("Confirm Payment: ", 3);
				animation.showAnimationLeft("Start hour: " + time.add(startHour), 5);
				animation.showAnimationLeft("End hour: " + time.add(endHour), 1);
				animation.showAnimationLeft("License plate: " + plate, 1);
				animation.showAnimationLeft("Name: " + name, 1);
				animation.showAnimationLeft("Total price: " + to_string(price.price(startHour, endHour, selected - 1)) + "$", 1);
				animation.newLine(5);
				drawArrowPayment(index);
				animation.newLine(1);
				GetAsyncKeyState(VK_UP); // flush arrow key up GetAsyncKeyState
				GetAsyncKeyState(VK_DOWN); // flush arrow key down GetAsyncKeyState
				GetAsyncKeyState(VK_RETURN); // flush enter GetAsyncKeyState
				flushCin(); // flush cin
				start = false;
			}
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) { // if one arrow was pressed
				if (index == 0) index++; // if index was 0, go to one
				else index--; // else go to 0
				drawPayment(index);
				Sleep(keyPressDelay);
			}
			if (GetAsyncKeyState(VK_RETURN)) return !index; // 0->confirm, 1->cancel, so I revert those
		}
	}
};
