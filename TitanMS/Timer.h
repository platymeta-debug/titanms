/*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef TIMER_H
#define TIMER_H

#include <queue>
#include <hash_map>
#include "windows.h"
using namespace std;

class Run;

class Timer {
public:
	long t;
	Run* run;
	Timer (long t, Run* run):t(t),run(run){}
	~Timer();
};
struct CompareTimers /* : public std::binary_function<Timer*, Timer*, bool> */ {
	bool operator()(const Timer* x, const Timer* y) const {
		return x->t > y->t;
	}
};
class TimersQueue : public priority_queue<Timer*, vector <Timer*>, CompareTimers> {
public:
	bool remove(Timer* timer);
};
class Timers {
public:

	Timers ();
	~Timers();

	Timer* startTimer 
		(int msec, Run* run);

	void cancelTimer (Timer* timer);

	void timerThread();

	static Timers* getInstance(){
		if(instance == NULL)
			instance = new Timers();
		return instance;
	}

private:
	static Timers* instance;
	bool terminate;
	CRITICAL_SECTION critic;

	TimersQueue timers;
	//vector<Timer*> timers;
	HANDLE timerEvent;
	Timer* findMin();
	//Timer* getTimer(int id);
	void remove(Timer* timer);
	int id;
};

#endif