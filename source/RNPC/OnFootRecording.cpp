#include "OnFootRecording.h"

/*
	Data representation of an OnFoot NPC rec file.
*/

OnFootRecording::OnFootRecording() {
	updaterate = 100;
}
OnFootRecording::OnFootRecording(char* file) {
	updaterate = 100;
	readFromFile(file);
}

/*
	Adds a single segment to the end of the recording.
*/
void OnFootRecording::addSegment(OnFootSegment s) {
	ofdata.push_back(s);
}

/*
	Writes the whole recording to a new NPC .rec file
*/
void OnFootRecording::writeToFile(char* file) {
	FILE* buildFile = fopen(file, "wb");

	// OnFoot header, Version 3E8/1000
	char header[8] = { (char)0xE8, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
	fwrite(header, 1, 8, buildFile);

	// Convert and write segments one-by-one
	char c[72];
	for (unsigned int i = 0; i < ofdata.size(); i++) {
		itoLittleEndian(ofdata[i].time, c, 0);
		stoLittleEndian(ofdata[i].lrkeys, c, 4);
		stoLittleEndian(ofdata[i].udkeys, c, 6);
		stoLittleEndian(ofdata[i].keys, c, 8);
		ftoLittleEndian(ofdata[i].xpos, c, 10);
		ftoLittleEndian(ofdata[i].ypos, c, 14);
		ftoLittleEndian(ofdata[i].zpos, c, 18);
		ftoLittleEndian(ofdata[i].quat1, c, 22);
		ftoLittleEndian(ofdata[i].quat2, c, 26);
		ftoLittleEndian(ofdata[i].quat3, c, 30);
		ftoLittleEndian(ofdata[i].quat4, c, 34);
		c[38] = ofdata[i].health;
		c[39] = ofdata[i].armour;
		c[40] = ofdata[i].weaponid;
		c[41] = ofdata[i].specialaction;
		ftoLittleEndian(ofdata[i].xvel, c, 42);
		ftoLittleEndian(ofdata[i].yvel, c, 46);
		ftoLittleEndian(ofdata[i].zvel, c, 50);
		// Blanks are not (yet) supported parts of the rec file
		for (int blank = 54; blank < 68; blank++) c[blank] = 0;
		stoLittleEndian(ofdata[i].animID, c, 68);
		stoLittleEndian(ofdata[i].animParams, c, 70);

		fwrite(c, 1, 72, buildFile);
	}

	fclose(buildFile);
}

void OnFootRecording::reset() {
	ofdata.clear();
}

/*
	NOT FINISHED YET
*/
int OnFootRecording::readFromFile(char* file) {
	FILE* buildFile = fopen(file, "rb");
	FILE* logFile = fopen("test.log", "w");
	fprintf(logFile, "sread: %d\n", 1);
	fflush(logFile);
	char buffer[72];
	fread(buffer, 1, 8, buildFile);
	fprintf(logFile, "pread: %d %d %d", buffer[0], buffer[1], buffer[4]);
	fflush(logFile);
	// Header is little-endian now, instead of transforming, just check it in that manner
	if (buffer[0] != -24 || buffer[1] != 0x03) return -2;	// Unknown version or not a valid .rec file
	if (buffer[4] != 0x02) return -1;						// Not an OnFoot recording

	int seg = 0;
	int read = 0;
	fprintf(logFile, "pread: %d\n", read);
	fflush(logFile);
	// WHY DOES IT READ FFFFFF CHAR SOMETIMES?
	while ((read = fread(buffer, 1, 72, buildFile)) > 0) {
		fprintf(logFile, "read: %d\n", read);
		fflush(logFile);
		// Segments MUST be 72 bytes long
		if (read != 72) return -2;
		

		// Read in one segment, transforming little-endian
		ofdata.push_back(OnFootSegment());
		seg = ofdata.size() - 1;

		// OnFoot rec version 0x3E8/1000
		ofdata[seg].time = leToI(buffer, 0);
		ofdata[seg].lrkeys = leToS(buffer, 4);
		ofdata[seg].udkeys = leToS(buffer, 6);
		ofdata[seg].keys = leToS(buffer, 8);
		ofdata[seg].xpos = leToF(buffer, 10);
		ofdata[seg].ypos = leToF(buffer, 14);
		ofdata[seg].zpos = leToF(buffer, 18);
		fprintf(logFile, "pos: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		fflush(logFile);
		fprintf(logFile, "pos: %u %hi %f %f\n", ofdata[seg].time, ofdata[seg].udkeys, ofdata[seg].ypos, ofdata[seg].zpos);
		fflush(logFile);
		ofdata[seg].quat1 = leToF(buffer, 22);
		ofdata[seg].quat2 = leToF(buffer, 26);
		ofdata[seg].quat3 = leToF(buffer, 30);
		ofdata[seg].quat4 = leToF(buffer, 34);
		ofdata[seg].health = buffer[38];
		ofdata[seg].armour = buffer[39];
		ofdata[seg].weaponid = buffer[40];
		ofdata[seg].specialaction = buffer[41];
		ofdata[seg].xvel = leToF(buffer, 42);
		ofdata[seg].yvel = leToF(buffer, 46);
		ofdata[seg].zvel = leToF(buffer, 50);
		// 54-67 are unused yet
		ofdata[seg].animID = leToS(buffer, 68);
		ofdata[seg].animParams = leToS(buffer, 70);
	}

	fclose(buildFile);
	fclose(logFile);

	return seg;		// File successfully read
}

