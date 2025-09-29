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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

using namespace std;

class CharacterEquip;
class Character {
public:
	void setID(int id){
		this->id = id;
	}
	int getID(){
		return id;
	}
	void setWorld(int world){
		this->world = world;
	}
	int getWorld(){
		return world;
	}
	void setName(string name){
		this->name = name;
	}
	string getName(){
		return this->name;
	}
	void setNickName(string nickname){
		this->nickname = nickname;
	}
	string getNickName(){
		return this->nickname;
	}
	void setUserID(int userid){
		this->userid = userid;
	}
	int getUserId(){
		return this->userid;
	}
	void setLevel(unsigned char level){
		this->level = level;
	}
	unsigned char getLevel(){
		return this->level;
	}
	void setClass(char clas){
		this->clas = clas;
	}
	char getClass(){
		return this->clas;
	}	
	void setClassLevel(char clevel){
		this->clevel = clevel;
	}
	char getClassLevel(){
		return this->clevel;
	}	
	void setJob(char job){
		this->job = job;
	}
	char getJob(){
		return this->job;
	}
	void setGender(char g){
		this->gender = g;
	}
	char getGender(){
		return this->gender;
	}
	void setStr(short str){
		this->str=str;
	}
	short getStr(){
		return this->str;
	}
	void setDex(short dex){
		this->dex=dex;
	}
	short getDex(){
		return this->dex;
	}
	void setInt(short intt){
		this->intt=intt;
	}
	short getInt(){
		return this->intt;
	}
	void setLuk(short luk){
		this->luk=luk;
	}
	short getLuk(){
		return this->luk;
	}
	void setHP(int hp){
		this->hp = hp;
	}
	short getHP(){
		return this->hp;
	}
	void setMP(int mp){
		this->mp = mp;
	}
	short getMP(){
		return this->mp;
	}
	void setMaxHP(int mhp){
		this->mhp=mhp;
	}
	short getMaxHP(){
		return this->mhp;
	}
	void setMaxMP(int mmp){
		this->mmp=mmp;
	}
	short getMaxMP(){
		return this->mmp;
	}
	void setRage(short Rage){
		this->rage=Rage;
	}
	short getRage(){
		return this->rage;
	}
	void setMaxRage(short MaxRage){
		this->mrage=MaxRage;
	}
	short getMaxRage(){
		return this->mrage;
	}
	void setExp(int exp){
		this->exp = exp;
	}
	int getExp(){
		return this->exp;
	}
	void setFame(short fame){
		this->fame=fame;
	}
	short getFame(){
		return this->fame;
	}
	void setMap(int map){
		this->map=map;
	}
	int getMap(){
		return this->map;
	}
	void setPlayerX(short x){
		this->PlayerX=x;
	}
	short getPlayerX(){
		return this->PlayerX;
	}
	void setPlayerY(short y){
		this->PlayerY=y;
	}
	short getPlayerY(){
		return this->PlayerY;
	}
	void setWEP(int WepId){
		this->wep = WepId;
	}
	int getWEP(){
		return this->wep;
	}
	void setArmor(int ArmorId){
		this->armor = ArmorId;
	}
	int getArmor(){
		return this->armor;
	}
	void setHair(int HairId){
		this->hair = HairId;
	}
	int getHair(){
		return this->hair;
	}
	void setEyes(int EyesId){
		this->eyes = EyesId;
	}
	int getEyes(){
		return this->eyes;
	}
	void setCash(int CashId,int num){
		switch (num){
		case 1: this->cash1 =CashId ;break;
		case 2: this->cash2 =CashId ;break;
		case 3: this->cash3 =CashId ;break;
		case 4: this->cash4 =CashId ;break;
		case 5: this->cash5 =CashId ;break;
		}
		
	}
	int getCash(int num)
	{
		switch (num){
		case 1: return this->cash1;break;
		case 2: return this->cash2;break;
		case 3: return this->cash3;break;
		case 4: return this->cash4;break;
		case 5: return this->cash5;break;
		}
		return 0;
	}
	void setRegionId(short Regionid){
		this->RegionId=Regionid;
	}
	short getARegionId(){
		return this->RegionId;
	}
	void setMapId(short Mapid){
		this->MapId = Mapid;
	}
	short getMapId(){
		return this->MapId;
	}

	vector <CharacterEquip*>* getEquips(){
		return &equips;
	}

	~Character();
	void addEquip(CharacterEquip* equip){
		equips.push_back(equip);
	}
private:
	int id;
	int world;
	string name;
	string nickname;
	int userid;
	unsigned char level;
	char clas;
	char clevel;
	char job;
	char gender;
	short str;
	short dex;
	short intt;
	short luk;
	short hp;
	short mhp;
	short mp;
	short mmp;
	short rage;
	short mrage;
	int exp;
	short fame;
	int map;
	short PlayerX;
	short PlayerY;
	int wep;
	int armor;
	int hair;
	int eyes;
	int cash1;
	int cash2;
	int cash3;
	int cash4;
	int cash5;
	short RegionId;
	short MapId;
/*
	char skin;
	int face;
	short ap;
	short sp;
*/	
	
	
	char mappos;
	
	vector <CharacterEquip*> equips;
};

#endif