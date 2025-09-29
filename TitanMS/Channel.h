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

#ifndef CHANNEL_H
#define CHANNEL_H

class Maps;
class Map;
class Players;
class Player;
class World;
class Transportations;
#include <string>

using namespace std;

class Channel {
private:
	int id;
	Maps* maps;
	Players* players;
	World* world;
	Transportations* trans;
	string notice;
	int port;
public:
	Channel(World* world, int id, int port);
	World* getWorld(){
		return world;
	}
	Players* getPlayers(){
		return players;
	}
	Maps* getMaps(){
		return maps;
	}
	Map* getMap(int id);
	Transportations* getTransportations(){
		return trans;
	}
	int getID(){
		return id;
	}
	int getPort(){
		return port;
	}
	void setNotice(string n){
		notice = n;
	}
	void addPlayer(Player* player);
};

#endif

