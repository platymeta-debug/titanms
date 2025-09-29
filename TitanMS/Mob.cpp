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
#include "Mob.h"
#include "DataProvider.h"
#include "Player.h"
#include "MapPlayers.h"
#include <algorithm>
#include "Map.h"
#include "PacketCreator.h"
#include "Worlds.h"
#include "Tools.h"
#include <cmath>
using namespace Tools;

class ComparePlayersDist
{
	Position pos;
public:
	bool operator()(Player* x, Player* y) const
	{
		return getDistance((Position&)pos, x->getPosition()) < getDistance((Position&)pos, y->getPosition());
	}
	ComparePlayersDist(Position& pos){
		this->pos = pos;		
	}
};

int Mob::getMaxMP(){
	return DataProvider::getInstance()->getMobMaxMP(mobid);
}
int Mob::getMaxHP(){
	return DataProvider::getInstance()->getMobMaxHP(mobid);
}
bool Mob::isBoss(){
	return DataProvider::getInstance()->isMobBoss(mobid);
}

// DON'T call it directly, call damageMob in MapMobs.
void Mob::takeDamage(Player* player, int damage){
	hp-=damage;
	int ndamage = damage;
	if(hp < 0){
		ndamage += hp;
		hp = 0;
	}
	if(playersa.find(player->getID()) == playersa.end())
		playersa[player->getID()] = ndamage;
	else
		playersa[player->getID()] += ndamage;
	// TODO party

	char per = (char)(hp * 100.0 / getMaxHP());
	if(isBoss()){
		char color = DataProvider::getInstance()->getMobTagColor(mobid);
		if(color != 0){
			map->send(PacketCreator().showBossHP(mobid, hp, getMaxHP(), per, color, DataProvider::getInstance()->getMobTagBgColor(mobid)));
		}
	}
	else if(per > 0){
		for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
			Player* p = map->getPlayers()->getPlayerByID(iter->first);
			if(p != NULL && p->getMap() == map){
				p->send(PacketCreator().showHP(id, per));
			}
		}
	}
}

void Mob::giveExp(){
	for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
		Player* p = map->getPlayers()->getPlayerByID(iter->first);
		if(p != NULL && p->getMap() == map){
			int hpk = iter->second;
			double fexp = hpk * Worlds::getInstance()-> getEXP() * 100.0 * DataProvider::getInstance()->getMobExp(mobid) / 100.0 / getMaxHP();
			int exp = static_cast<int>(std::lround(fexp));
			if(exp != 0){
				p->send(PacketCreator().gainEXP(exp, false));
				p->addExp(exp);
			}
		}
	}
}

int getDistance(Position& pos1, Position& pos2){
	return (int)(pow((double)(pos1.x+pos2.x), 2)+pow((double)(pos1.y+pos2.y), 1));
}

void Mob::findControl(bool spawn){
	vector <Player*> players = vector <Player*>(*map->getPlayers()->getPlayers());
	if(players.size() == 0){
		control = NULL;
		return;
	}
	Player* player = *min_element<vector <Player*>::iterator, ComparePlayersDist>(players.begin(), players.end(), ComparePlayersDist(pos));
	if(spawn)
		setControl(*players.begin(), spawn, DataProvider::getInstance()->getMobFirstAttack(mobid), false);
	else
		setControl(*players.begin(), spawn, false, false);

}

void Mob::setControl(Player* control, bool spawn, bool agrs, bool switchc){
	if(this->control != NULL && switchc)
		this->control->send(PacketCreator().endControlMob(id));
	this->control = control;
	if(control != NULL)
		control->send(PacketCreator().controlMob(this, spawn, agrs));
	aggressive = agrs;
}
int Mob::getKillerID(){
	int md = 0;
	int mdid = 0;
	for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
		if(iter->second > md){
			md = iter->second;
			mdid = iter->first;
		}
	}
	return mdid;
}	