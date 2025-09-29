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

#include "DataProvider.h"
#include "Run.h"
#include "Timer.h"
#include <Winbase.h>
#include <hash_map>
using namespace stdext;

bool TimersQueue::remove(Timer* timer){
	vector<Timer*>::iterator iter = find(c.begin(), c.end(), timer);
	if(iter != c.end()){
		c.erase(iter);
		return true;
	}
	return false;
}
void _timerThread (Timers* timerObject) {
	timerObject->timerThread();
}
Timers* Timers::instance;

Timer::~Timer(){
	delete run;
}

Timers::Timers () {
	id = 0;
	terminate = false;

	HANDLE thread = CreateThread(NULL, 20000,
		(LPTHREAD_START_ROUTINE)_timerThread,
		(LPVOID)this,
		NULL,
		NULL);


	timerEvent = CreateEvent( 
            NULL,     // no security attributes
            FALSE,    // auto-reset event
            FALSE,    // initial state is not signaled
            NULL);    // object not named

	InitializeCriticalSection(&critic);
}

Timers::~Timers() {
	terminate = true;
	SetEvent (timerEvent);
	DeleteCriticalSection(&critic);
}

Timer* Timers::startTimer (int msec, Run* run) {
	Timer* timer = new Timer(msec+GetTickCount(), run);
	EnterCriticalSection(&critic);
	timers.push(timer);
	LeaveCriticalSection(&critic);
	SetEvent (timerEvent);
	return timer;
}

void Timers::remove(Timer* timer){
	EnterCriticalSection(&critic);
	bool del = timers.remove(timer); 
	LeaveCriticalSection(&critic);
	SetEvent (timerEvent);
	if(del) delete timer;
}

void Timers::cancelTimer (Timer* timer) {
	remove(timer);
	SetEvent (timerEvent);
}

Timer* Timers::findMin(){
	if(timers.size() == 0)
		return NULL;
	return timers.top();
}

/*
Timers::Timer* Timers::getTimer(int id){
	for(unsigned int i=0; i<timers.size(); i++){
		if(timers[i]->id == id){
			return timers[i];
		}
	}
	return NULL;
}*/

void Timers::timerThread () {
	while (!terminate) {
		try {
			// find minimum wakeup time
			EnterCriticalSection(&critic);
			Timer* minTimer = findMin();
			if(minTimer != NULL)
				timers.pop();
			LeaveCriticalSection(&critic);
			long msec = minTimer==NULL || minTimer->t == -1 ? msec = 1000000000 : minTimer->t - GetTickCount();
			if (msec <= 0) {
				minTimer->run->run();
				delete minTimer;
				continue;
			}
			DWORD r = WaitForSingleObject(timerEvent, msec);
			if (r == WAIT_OBJECT_0){
				if(minTimer != NULL){
					EnterCriticalSection(&critic);
					timers.push(minTimer);
					LeaveCriticalSection(&critic);
				}
				continue;
			}
			if (r == WAIT_FAILED) {
				// TODO: write message
				return;
			}
			if (minTimer != NULL) {
				minTimer->run->run();
				delete minTimer;
				continue;
			}
		}
		catch (...) {
			// TODO error
		}
	}
}
