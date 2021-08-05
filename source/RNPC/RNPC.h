//#include "OnFootSegment.h"
//#include "VehicleSegment.h"
#include "OnFootRecording.h"
#include "VehicleRecording.h"
//#include "Utils.h"
#include "./ThirdPartySource/MapAndreas.h"

#define PI								(314159265/100000000)
#define TODEGREES						(3.14159265359f / 180.0f)
#define TORADIAN						(180.0f / 3.14159265359f)

#define PLAYER_RECORDING_TYPE_NONE		0
#define PLAYER_RECORDING_TYPE_DRIVER	1
#define PLAYER_RECORDING_TYPE_ONFOOT	2

#define RNPC_SPEED_SPRINT				0.0095
#define RNPC_SPEED_RUN					0.0057
#define RNPC_SPEED_WALK					0.0015

#define MAX_HEIGHT_DIFFERENCE			1.5

//----------------------------------------------------------

class RNPC
{
public:
	RNPC();
	RNPC(CMapAndreas *ma);

	int StartBuild(int npcid, int type, int slot, float x, float y, float z);
	int StartBuild(int type, char* cname);
	int EndBuild(int autoreset);
	int AddMovement(float x1, float y1, float z1, float x2, float y2, float z2, float speed, bool zmap);
	int AddPause(int time, int comp);
	int AddAutoMovement(float x1, float y1, float z1, float x2, float y2, float z2, float speed, int maxsteps);
	int AddMovementAlt(float x1, float y1, float z1, float x2, float y2, float z2, float speed, char lock);

	int BuildVehicleRecording(int npcid, float x1, float y1, float z1, float x2, float y2, float z2, float speed);

	int updaterate;
	OnFootSegment settings;
	VehicleSegment vsettings;
	OnFootRecording ofRec;
	VehicleRecording vRec;
	CMapAndreas *MapAndreas;

	bool buildActive;
	FILE* buildFile;
	int curTime;
	int curNPCID;
	int curSlot;
	short recMode;
	float acceleration;
	float deceleration;
	float curspeed;
	char buildname[40];
};

//----------------------------------------------------------