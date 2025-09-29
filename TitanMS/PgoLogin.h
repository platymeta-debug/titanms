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

#ifndef PGOLOGIN_H
#define PGOLOGIN_H

//#include <vector>
//#include <string>
//#include "Tools.h"
//#include <hash_map>
//using namespace std;
#include <hash_map>
#include <string>
#include "Tools.h"
using namespace std;
using namespace stdext;

	struct LogInfo{
		int UserPIds[2] ;
	int UserId;
	};

class PgoLogin {
private:
	static PgoLogin* instance;

	 hash_map <string, LogInfo> global;
public:
	PgoLogin(){
		instance = this;
	}

	static PgoLogin* getInstance(){
		if(instance == NULL)
			return new PgoLogin();
		return instance;
	}

	void setPlayerLoginId(string ip,LogInfo id)
	{
		this->global[ip] = id;
	}

	LogInfo getPlayerLoginInfo(string ip){
		if(global.find(ip) != global.end())
			return global[ip];
	}

};

#endif