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
#include "Player.h"
#include "Players.h"
#include "MapPlayers.h"
#include "PacketCreator.h"
#include "Map.h"

void MapPlayers::add(Player* player){
	players[player->getID()] = player;
	playersv.push_back(player);
	map->send(PacketCreator().showPlayer(player));
	for(int i=0; i<(int)playersv.size(); i++){
		if(playersv[i] != player)
			player->send(PacketCreator().showPlayer(playersv[i]));
	}

}
void MapPlayers::remove(Player* player){
	if(players.find(player->getID()) != players.end()){
		players.erase(player->getID());
	}
	for(int i=0; i<(int)playersv.size(); i++){
		if(playersv[i] == player){
			playersv.erase(playersv.begin()+i);
			break;
		}
	}
	map->send(PacketCreator().removePlayer(player->getID()), player);
}