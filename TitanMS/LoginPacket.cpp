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

#include "PacketCreator.h"
#include "Worlds.h"
#include "World.h"
#include "Channels.h"
#include "Character.h"
#include "CharacterEquip.h"
#include "PacketWriter.h"
#include <sstream>
#include <hash_map>
using namespace stdext;

PacketWriter* PacketCreator::loginError(short errorid){
	printf("PacketCreator::loginError\n");
	//Packet packet = Packet();
	//packet.addHeader(0x55AA);
	//packet.addBytes("050031");   
	//packet.addShort(errorid);
	//packet.addBytes("000055AA");
	//pw.writeShort(LOGIN_PROCESS);
	pw.writeShort(LOGIN_HEADER_START);
	pw.writeBytes("050031");
	pw.writeShort(errorid);
	pw.writeShort(0);
	pw.writeShort(LOGIN_HEADER_END);
	return &pw;
}
PacketWriter* PacketCreator::loginProcess(char type){
	printf("PacketCreator::loginProcess\n");
	pw.writeShort(PIN);

	pw.write(type);
	return &pw;
}
PacketWriter* PacketCreator::loginConnect(char gender, string username, bool firstlogin){
	printf("PacketCreator::loginConnect\n");
	/*
		AA 55 05 00 31 00 00 E8 03 55 AA
	*/

	//pw.writeShort(LOGIN_PROCESS);
	pw.writeShort(LOGIN_HEADER_START);
	pw.writeBytes("050031");
	pw.writeShort(0);
	pw.writeShort(1000);
	pw.writeShort(LOGIN_HEADER_END);
	return &pw;
}
PacketWriter* PacketCreator::processOk(){
	printf("PacketCreator::processOk\n");
	pw.writeShort(CHECK_NAME);

	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::showWorld(World* world){
printf("PacketCreator::showWorld\n");
	pw.writeShort(LOGIN_HEADER_START);
	pw.writeBytes("480233FFFFFFFFFFFFFFFFFFFFFFFFFF0100000000000E00000001000100");
	pw.writeBytes("09003132372E302E302E31CD4200000A000000960000000C0000000000000001");
	pw.writeBytes("264300000200020009003132372E302E302E31CE42000014000000960000000C");
pw.writeBytes("0000000000000001274300000300030009003132372E302E302E31CF4200001E");
pw.writeBytes("000000960000000C0000000000000001264300000400040009003132372E302E");
pw.writeBytes("302E31D042000028000000960000000C00000000000000012743000005000500");
pw.writeBytes("09003132372E302E302E31CD42000032000000960000000C0000000000000001");
pw.writeBytes("264300000600060009003132372E302E302E31CE4200003C000000960000000C");
pw.writeBytes("0000000000000001274300000700070009003132372E302E302E31CF42000046");
pw.writeBytes("000000960000000C0000000000000001264300000800080009003132372E302E");
pw.writeBytes("302E31D042000050000000960000000C00000000000000012643000009000900");
pw.writeBytes("09003132372E302E302E31CD4200005A000000960000000C0000000000000001");
pw.writeBytes("274300000A000A0009003132372E302E302E31CE42000064000000960000000C");
pw.writeBytes("0000000000000001264300000B000B0009003132372E302E302E31CD42000000");
pw.writeBytes("000000960000000C0000000000000000274300000C000C0009003132372E302E");
pw.writeBytes("302E31CE42000000000000960000000C0000000000000000264300000D000D00");
pw.writeBytes("09003132372E302E302E31CF42000000000000960000000C0000000000000000");
pw.writeBytes("274300000E000E0009003132372E302E302E31D042000000000000960000000C");
pw.writeBytes("000000000000000026430000");
pw.writeShort(LOGIN_HEADER_END);
	/*
	pw.writeShort(SHOW_WORLDS);

	pw.write(world->getID());
	pw.writeString(Worlds::getName(world->getID()));
	pw.write(0); //Type 2-new
	pw.writeShort(0);
	pw.write(100);
	pw.write(0);
	pw.write(100);
	pw.writeShort(0);
	pw.write(world->getChannels()->getChannelsCount());
	for(int i=0; i<world->getChannels()->getChannelsCount(); i++){
		stringstream out;
		out << i+1;
		string name = Worlds::getName(world->getID()) + "-" + out.str();
		pw.writeString(name);
		pw.writeInt(0x0); // Pop - TODO
		pw.write(world->getID());
		pw.writeShort(i);
	}
	pw.writeShort(0);
	*/
	return &pw;
}
PacketWriter* PacketCreator::endWorlds(){
	printf("PacketCreator::endWorlds\n");
	pw.writeShort(SHOW_WORLDS);

	pw.write(-1);
	return &pw;
}
PacketWriter* PacketCreator::showChannels(){
	printf("PacketCreator::showChannels\n");
	pw.writeShort(WORLD_STATUS);

	pw.writeShort(0);
	return &pw;
}
PacketWriter* PacketCreator::SelectChRespone(string ip,int TCPP,int UDPP){
	printf("PacketCreator::SelectChRespone\n");
	pw.writeShort(LOGIN_HEADER_START);
	pw.writeBytes("1900");                                        //Packet Size
	pw.writeBytes("3500");                                        //Packet Id
	pw.writeString(ip);							             	  //Ch IP
	pw.writeInt(TCPP);                                          //Port
	pw.writeInt(UDPP);                                          //Udp
	pw.writeShort(LOGIN_HEADER_END);
	pw.putsize();
	return &pw;
}
PacketWriter* PacketCreator::showCharacters(vector <Character*>& chars){
	printf("PacketCreator::showCharacters\n");
	pw.writeShort(WORLD_HEADER_START);
	pw.writeBytes("0900");
	pw.writeBytes("C000");
	pw.writeBytes("CE01");
	pw.writeBytes("00000000");
	pw.writeInt(chars.size());
	for(int i=0; i<(int)chars.size(); i++){
		showCharacter(chars[i]);
	}
	return &pw;
}

void PacketCreator::showCharacter(Character* character){
	printf("PacketCreator::showCharacter\n");

	//pw.writeInt(character->getID());
	pw.writeString(character->getName(), 20);
	pw.writeString(character->getNickName(), 20);
	pw.write(character->getGender());
	pw.write(character->getLevel());
	pw.write(character->getJob());
	pw.write(character->getClass());
	pw.write(character->getClassLevel());
	pw.writeBytes("1A0A00AD08CE01");
	pw.writeInt(character->getWEP());
	pw.writeInt(character->getArmor());
	pw.writeInt(character->getCash(1));
	pw.writeInt(character->getCash(2));
	pw.writeInt(character->getCash(3));
	pw.writeInt(character->getEyes());
	pw.writeInt(character->getCash(4));
	pw.writeInt(character->getCash(5));
	pw.writeInt(character->getHair());
}
PacketWriter* PacketCreator::addCharacter(Character* character){
	printf("PacketCreator::addCharacter\n");
	pw.writeShort(ADD_CHARACTER);

	pw.write(0);
	showCharacter(character);
	pw.writeLong(0);
	return &pw;
}
PacketWriter* PacketCreator::checkName(string name, bool ok){
	printf("PacketCreator::checkName\n");
	pw.writeShort(CHECK_NAME);

	pw.writeString(name);
	pw.write(ok);
	return &pw;
}
PacketWriter* PacketCreator::removeCharacter(int characterid){
	printf("PacketCreator::removeCharacter\n");
	pw.writeShort(REMOVE_CHARACTER);

	pw.writeInt(characterid);
	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::connectChannel(int charid, short port){
	printf("PacketCreator::connectChannel\n");
	pw.writeShort(CHANNEL_INFO);

	pw.writeShort(0);

	IP* ip = Worlds::getInstance()->getIP();

 pw.write(127); // Put your IP here
 pw.write(0);
 pw.write(0);
 pw.write(1);

	pw.writeShort(port);
	pw.writeInt(charid);
	pw.writeInt(0);
	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::logBack(){
	printf("PacketCreator::logBack\n");
	pw.writeShort(LOG_BACK);

	pw.write(1);
	return &pw;
}