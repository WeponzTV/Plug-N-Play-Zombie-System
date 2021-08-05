#include "Utils.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

// Disable fopen_s warnings
#pragma warning( disable : 4996 )

using namespace std;

struct OnFootSegment {
	OnFootSegment() : time(0), lrkeys(0), udkeys(0), keys(0), weaponid(0), specialaction(0),
		xvel(0.0F), yvel(0.0F), zvel(0.0F), animID(0), animParams(0) {}
	unsigned int time;
	unsigned short lrkeys;
	unsigned short udkeys;
	short keys;
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

class OnFootRecording {
public:	
	OnFootRecording();
	OnFootRecording(char* file);
	void addSegment(OnFootSegment s);
	void writeToFile(char* file);
	int readFromFile(char* file);
	void reset();

	vector<OnFootSegment> ofdata;

	int type;
	int updaterate;
};