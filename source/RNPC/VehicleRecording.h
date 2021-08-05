#include "Utils.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

// Disable fopen_s warnings
#pragma warning( disable : 4996 )

using namespace std;

struct VehicleSegment {
	VehicleSegment() : time(0), lrkeys(0), udkeys(0), keys(0), xvel(0.0F), yvel(0.0F), zvel(0.0F), weaponid(0) {}
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


class VehicleRecording {
public:
	VehicleRecording();
	void addSegment(VehicleSegment s);
	void writeToFile(char* file);
	void readFromFile(char* file);
	void reset();

	vector<VehicleSegment> vdata;

	int type;
	int updaterate;
};