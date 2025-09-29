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
#include <Winsock2.h>
#include "MySQLM.h"
#include "../Connection/Acceptor.h"
#include "../Connection/Connector.h"
#include "../Decoder/Decoder.h"
#include "PlayerLogin.h"
//#include "Player.h"
#include <stdlib.h>
#include "Initializing.h"
#include "PlayerHandler.h"
#include "PlayerLoginHandler.h"
#include "RemoteControlHandler.h"
#include "RemoteControl.h"
#include "Maps.h"
#include "Worlds.h"
#include "AngelScriptEngine.h"
#include "Run.h"
#include "mysql.h"
#include "Tools.h"
using namespace Tools;

int main(){ 
	int Version = 0x0C85BD89;

	printf("Initializing Angel Script Engine... ");
	if(AngelScriptEngine::Initialize())
		printf("DONE\n");
	else{
		printf("Failed to create Angel Script engine.\n");
		return 1;
	}
	Initializing::initializing(); 
	printf("Initializing Packets... ");
	PlayerLoginHandler::loadPackets();//Login + Chr Server
	PlayerHandler::loadPackets();///Game Sever
	RemoteControlHandler::loadPackets();
	printf("DONE\n");
	printf("Initializing Encryption... ");
	MasterDecoder::Initializever(Version);
	Decoder::Initialize(Version);
	MasterDecoder::Initialize(Worlds::getInstance()->getPassword());
	printf("DONE\n");
	printf("Initializing MySQL... ");
	if(MySQL::getInstance()->connectToMySQL())
		printf("DONE\n");
	else{
		printf("FAILED\n");
		return 1;
	}

	randomize();
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)  printf("Error at WSAStartup()\n");

	srand((unsigned char)time(0));


	Selector* selector = new Selector();
	Worlds::getInstance()->start(selector);
	//8484  13031
	for (int i = 0; i < 3;i++){//[0 = 13021] [1= 17002] [2= 17003] [3 = 17004]
	new Acceptor(13021 + i, selector, new PlayerLoginFactory());//Login Server
	}
	for(int i = 0; i<3;i++){
	new Acceptor(13031+i, selector, new PlayerLoginFactory(),false, true);//Charcter Server
	}
	Worlds::getInstance()->commandListener();

	return 0;
}