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
#include "Map.h"
#include "MapPlayers.h"
#include "MapData.h"
#include "MapMobs.h"
#include "MapReactors.h"
#include "MapNPCs.h"
#include "MapDrops.h"
#include "MapsData.h"
#include "MapData.h"
#include "MapFootholdsData.h"
#include "MapFootholdData.h"
#include "MapPortalsData.h"
#include "PacketWriter.h"
#include "DataProvider.h"
#include "PvP.h"
#include "Mob.h"
#include "Reactor.h"
#include "MapPortalData.h"
using namespace Tools;

Map::Map(int id){
	this->id = id;
	players = new MapPlayers(this);
	mobs = new MapMobs(this);
	npcs = new MapNPCs(this);
	drops = new MapDrops(this);
	reactors = new MapReactors(this);
	this->spawn = spawn; 
	pvp = NULL;

	vector <MapPortalData*>* pl = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getData();
	for(int i=0; i<(int)pl->size(); i++)
		portals[(*pl)[i]->getFromPortal()] = (*pl)[i];
}

Drop* Map::getDrop(int id){
	return drops->getDropByID(id);
}
Mob* Map::getMob(int id){
	return mobs->getMobByID(id);
}
NPC* Map::getNPC(int id){
	return npcs->getNPCByID(id);
}
Player* Map::getPlayer(int id){
	return players->getPlayerByID(id);
}
Reactor* Map::getReactor(int id){
	return reactors->getReactorByID(id);
}

void Map::addPlayer(Player* player){
	players->add(player);
	mobs->show(player);
	mobs->checkSpawn();
	npcs->show(player);
	reactors->show(player);
	reactors->checkSpawn();
	drops->show(player);
	drops->checkDrops();
	if(pvp != NULL)
		pvp->showPvP(player);
}
void Map::removePlayer(Player* player){
	players->remove(player);
	mobs->checkSpawn();
}

Position Map::getPortalPosition(int pid){
	Position p;
	p.x = 0;
	p.y = 0;
	MapPortalData* pd = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getDataByID(pid);
	if(pd != NULL){
		p.x = pd->getX();
		p.y = pd->getY();
	}
	return p;
}
MapPortalData* Map::getPortal(int pid){
	return MapsData::getInstance()->getDataByID(id)->getPortalsData()->getDataByID(pid);
}
void Map::send(PacketWriter* pw){
	players->send(pw);
}
void Map::send(PacketWriter* pw, Player* player){
	players->send(pw, player);
}
void Map::send(PacketWriter* pw, Player* player, Position pos){
	players->send(pw, player, pos);
}
int Map::getReturnMap(){
	return DataProvider::getInstance()->getReturnMap(id);
}

Mob* Map::spawnMob(int id, int x, int y){
	Position pos;
	pos.x = x;
	pos.y = y;
	return mobs->spawnMob(id, pos.x, pos.y);
}
Reactor* Map::spawnReactor(int id, int x, int y){
	Position pos;
	pos.x = x;
	pos.y = y;
	pos = findFoothold(pos);
	return reactors->spawnReactor(id, pos.x, pos.y);
}
void Map::killMob(Mob* mob){
	if(mob->getMap() == this)
		mobs->kill(mob, NULL);
}
void Map::destroyReactor(Reactor* reactor){
	if(reactor->getMap() == this)
		reactors->destroy(reactor);
}
int Map::getMobsCount(){
	return mobs->getMobsCount();
}
Reactor* Map::getReactorByReactorID(int id){
	return reactors->getReactorByReactorID(id);
}

void Map::setSpawn(bool spawn){
	this->spawn = spawn;
	if(spawn)
		mobs->checkSpawn();
}
void Map::clearDrops(){
	drops->clear();
}
void Map::clearReactors(){
	reactors->clear();
}
void Map::clearMobs(){
	mobs->clear();
}
void Map::clear(){
	clearMobs();
	clearDrops();
	clearReactors();
}
int Map::getClosestSpawnPos(Position pos){
	vector <MapPortalData*>* pl = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getData();
	if(pl->size() <= 0) return 0;
	Position ppos;
	ppos.x = (*pl)[0]->getX();
	ppos.y = (*pl)[0]->getY();
	int close = 0; 
	int closed = getDistance(ppos, pos);
	for(int i=1; i<(int)pl->size(); i++){
		if((*pl)[i]->getType() == 0){
			ppos.x = (*pl)[i]->getX();
			ppos.y = (*pl)[i]->getY();
			int dis = getDistance(ppos, pos);
			if(dis < closed){
				close = i;
				closed = dis;
			}
		}
	}
	return close;
}

int Map::getFoothold(Position pos){
	short x = pos.x;
	short y = pos.y;
	bool first=1;
	short maxy;
	int fid = 0;
	MapData* md = MapsData::getInstance()->getDataByID(id);
	if(md == NULL) return 0;
	vector <MapFootholdData*>* tdata = md->getFootholdsData()->getData();
	short x1, x2, y1, y2;
	for(int i=0; i<(int)tdata->size(); i++){
		MapFootholdData* data = (*tdata)[i];
		x1 = data->getX1();
		x2 = data->getX2();
		y1 = data->getY1();
		y2 = data->getY2();
		if((x>=x1 && x<=x2) || (x>=x2 && x<=x1)){
			if(first){
				if(x1 == x2){
					continue;
				}
				maxy = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				fid = data->getID();
				if(maxy >= y)
					first=0;
			}
			else{
				if(x1 == x2){
					continue;
				}
				short cmax = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(cmax < maxy && cmax >= y){
					fid = data->getID();
					maxy = cmax;
				}
			}
		}
	}
	return fid;
}
int Map::findFootholdMiddleX(int fh){
	MapData* md = MapsData::getInstance()->getDataByID(id);
	if(md == NULL) return 0;
	MapFootholdData* fhd = md->getFootholdsData()->getDataByID(fh);
	if(fh == NULL) return 0;
	return (int)((fhd->getX1() + fhd->getX2())/2);
}
Position Map::findFoothold(Position pos){
	short x = pos.x;
	short y = pos.y;
	bool first=1;
	short maxy;
	MapData* md = MapsData::getInstance()->getDataByID(id);
	if(md == NULL) return pos;
	vector <MapFootholdData*>* tdata = md->getFootholdsData()->getData();
	short x1, x2, y1, y2;
	for(int i=0; i<(int)tdata->size(); i++){
		MapFootholdData* data = (*tdata)[i];
		x1 = data->getX1();
		x2 = data->getX2();
		y1 = data->getY1();
		y2 = data->getY2();
		if((x>=x1 && x<=x2) || (x>=x2 && x<=x1)){
			if(first){
				if(x1 == x2){
					continue;
				}
				maxy = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(maxy >= y)
					first=0;
			}
			else{
				if(x1 == x2){
					continue;
				}
				short cmax = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(cmax < maxy && cmax >= y)
					maxy = cmax;
			}
		}
	}
	if(!first){
		Position newpos;
		newpos.x = x;
		newpos.y = maxy;
		return newpos;
	}
	return pos;
}
void Map::startPvP(){
	if(pvp == NULL){
		pvp = new PvP(this);
		pvp->startPvP();
	}
}