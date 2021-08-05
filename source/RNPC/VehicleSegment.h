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

class VehicleSegment {
public:
	VehicleSegment();
	void toBinary(char* c);
	void reset();

	unsigned int time;
	unsigned short vehicle;
	unsigned short lrkeys;
	unsigned short udkeys;
	signed short keys;
	float quat1;
	float quat2;
	float quat3;
	float quat4;
	float xpos;
	float ypos;
	float zpos;
	float xvel;
	float yvel;
	float zvel;
	float health;
	unsigned char phealth;
	unsigned char parmour;
	unsigned char weaponid;
	unsigned char siren;
	unsigned char gear;

};