#include "VehicleSegment.h"

VehicleSegment::VehicleSegment()
{
	reset();
}

void VehicleSegment::reset() {
	time = 0;
	vehicle = 1;
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
	health = 1000.0;
	phealth = 100;
	parmour = 0;
	weaponid = 0;
	siren = 0;
	gear = 0;
	xvel = 0.0;
	yvel = 0.0;
	zvel = 0.0;
}

void VehicleSegment::toBinary(char* c) {
	itoLittleEndian(time, c, 0);
	stoLittleEndian(vehicle, c, 4);
	stoLittleEndian(lrkeys, c, 6);
	stoLittleEndian(udkeys, c, 8);
	stoLittleEndian(keys, c, 10);
	ftoLittleEndian(quat1, c, 12);
	ftoLittleEndian(quat2, c, 16);
	ftoLittleEndian(quat3, c, 20);
	ftoLittleEndian(quat4, c, 24);
	ftoLittleEndian(xpos, c, 28);
	ftoLittleEndian(ypos, c, 32);
	ftoLittleEndian(zpos, c, 36);
	ftoLittleEndian(xvel, c, 40);
	ftoLittleEndian(yvel, c, 44);
	ftoLittleEndian(zvel, c, 48);
	ftoLittleEndian(health, c, 52);
	c[56] = phealth; c[57] = parmour;
	c[58] = weaponid;
	c[59] = siren;
	c[60] = gear;
	for (int i = 61; i < 67; i++) c[i] = 0;
}
