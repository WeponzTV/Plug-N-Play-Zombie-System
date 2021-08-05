#include "OnFootSegment.h"

OnFootSegment::OnFootSegment()
{
	reset();
}

void OnFootSegment::reset() {
	time = 0;
	lrkeys = 0;
	udkeys = 0;
	keys = 0;
	xpos = 0.0;
	ypos = 0.0;
	zpos = 0.0;
	quat1 = 0.0;
	quat2 = 0.0;
	quat3 = 0.0;
	quat4 = 0.0;
	health = 100;
	armour = 0;
	weaponid = 0;
	specialaction = 0;
	xvel = 0.0;
	yvel = 0.0;
	zvel = 0.0;
	animID = 0;
	animParams = 0;
}

void OnFootSegment::toBinary(char* c) {
	itoLittleEndian(time, c, 0);
	stoLittleEndian(lrkeys, c, 4);
	stoLittleEndian(udkeys, c, 6);
	stoLittleEndian(keys, c, 8);
	ftoLittleEndian(xpos, c, 10);
	ftoLittleEndian(ypos, c, 14);
	ftoLittleEndian(zpos, c, 18);
	ftoLittleEndian(quat1, c, 22);
	ftoLittleEndian(quat2, c, 26);
	ftoLittleEndian(quat3, c, 30);
	ftoLittleEndian(quat4, c, 34);
	c[38] = health;
	c[39] = armour;
	c[40] = weaponid;
	c[41] = specialaction;
	ftoLittleEndian(xvel, c, 42);
	ftoLittleEndian(yvel, c, 46);
	ftoLittleEndian(zvel, c, 50);
	for (int i = 54; i < 68; i++) c[i] = 0;
	stoLittleEndian(animID, c, 68);
	stoLittleEndian(animParams, c, 70);
}
