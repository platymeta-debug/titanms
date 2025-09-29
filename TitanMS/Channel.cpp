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
#include "Channel.h"
#include "Maps.h"
#include "Players.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Transportations.h"
#include "Map.h"
#include "Worlds.h"

Channel::Channel(World* world, int id, int port){
	this->id = id;
	maps = new Maps(this);
	players = new Players();
	trans = new Transportations();
	notice = Worlds::getInstance()->getDefaultNotice();
	this->port = port;
}
void Channel::addPlayer(Player* player){
	players->add(player);
	player->getMap()->addPlayer(player);
	player->send(PacketCreator().showMassage(notice, 4, 0, true));
	
}
Map* Channel::getMap(int id){
	return maps->getMap(id);
}