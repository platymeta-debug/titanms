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

#ifndef MAPDATA_H
#define MAPDATA_H

#include "TopDataStruct.h"
#include "MapMobsData.h"
#include "MapNPCsData.h"
#include "MapReactorsData.h"
#include "MapPortalsData.h"
#include "MapFootholdsData.h"

class MapData : public DataStruct {
private:
	MapMobsData* mobs;
	MapNPCsData* NPCs;
	MapReactorsData* reactors;
	MapPortalsData* portals;
	MapFootholdsData* footholds;
	int returnMap;
public:
	MapData(int id){
		this->id = id;
		mobs = new MapMobsData();
		NPCs = new MapNPCsData();
		reactors = new MapReactorsData();
		portals = new MapPortalsData();
		footholds = new MapFootholdsData();
	}

	MapMobsData* getMobsData(){
		return mobs;
	}
	MapNPCsData* getNPCsData(){
		return NPCs;
	}
	MapReactorsData* getReactorsData(){
		return reactors;
	}
	MapPortalsData* getPortalsData(){
		return portals;
	}
	MapFootholdsData* getFootholdsData(){
		return footholds;
	}
	void setReturnMap(int returnMap){
		this->returnMap = returnMap;
	}
	int getReturnMap(){
		return returnMap;
	}
};


#endif