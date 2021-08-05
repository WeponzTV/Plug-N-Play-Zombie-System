#include "VehicleRecording.h"

/*
Data representation of an Vehicle NPC rec file.
*/

VehicleRecording::VehicleRecording() {
	updaterate = 100;
}

/*
Adds a single segment to the end of the recording.
*/
void VehicleRecording::addSegment(VehicleSegment s) {
	vdata.push_back(s);
}

/*
Writes the whole recording to a new NPC .rec file
*/
void VehicleRecording::writeToFile(char* file) {
	FILE* buildFile = fopen(file, "wb");

	// Vehicle header, Version 3E8/1000
	char header[8] = { (char)0xE8, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
	fwrite(header, 1, 8, buildFile);

	// Convert and write segments one-by-one
	char c[68];
	for (unsigned int i = 0; i < vdata.size(); i++) {
		itoLittleEndian(vdata[i].time, c, 0);
		stoLittleEndian(vdata[i].vehicle, c, 4);
		stoLittleEndian(vdata[i].lrkeys, c, 6);
		stoLittleEndian(vdata[i].udkeys, c, 8);
		stoLittleEndian(vdata[i].keys, c, 10);
		ftoLittleEndian(vdata[i].quat1, c, 12);
		ftoLittleEndian(vdata[i].quat2, c, 16);
		ftoLittleEndian(vdata[i].quat3, c, 20);
		ftoLittleEndian(vdata[i].quat4, c, 24);
		ftoLittleEndian(vdata[i].xpos, c, 28);
		ftoLittleEndian(vdata[i].ypos, c, 32);
		ftoLittleEndian(vdata[i].zpos, c, 36);
		ftoLittleEndian(vdata[i].xvel, c, 40);
		ftoLittleEndian(vdata[i].yvel, c, 44);
		ftoLittleEndian(vdata[i].zvel, c, 48);
		ftoLittleEndian(vdata[i].health, c, 52);
		c[56] = vdata[i].phealth; c[57] = vdata[i].parmour;
		c[58] = vdata[i].weaponid;
		c[59] = vdata[i].siren;
		c[60] = vdata[i].gear;
		// Blanks are not (yet) supported parts of the rec file
		for (int blank = 61; blank < 67; blank++) c[blank] = 0;

		fwrite(c, 1, 67, buildFile);
	}

	fclose(buildFile);
}

void VehicleRecording::reset() {
	vdata.clear();
}

/*
NOT FINISHED YET
*/
void VehicleRecording::readFromFile(char* file) {

}