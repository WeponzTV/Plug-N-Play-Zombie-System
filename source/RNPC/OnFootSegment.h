#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <limits>
#include <sstream>
#include <string>

#include "Utils.h"

using namespace std;

class OnFootSegment {
public:
	OnFootSegment();
	void toBinary(char* c);
	void reset();

	unsigned int time;
	unsigned short lrkeys;
	unsigned short udkeys;
	signed short keys;
	float xpos;
	float ypos;
	float zpos;
	float quat1;
	float quat2;
	float quat3;
	float quat4;
	unsigned char health;
	unsigned char armour;
	unsigned char weaponid;
	unsigned char specialaction;
	float xvel;
	float yvel;
	float zvel;
	unsigned short animID;
	short animParams;

};