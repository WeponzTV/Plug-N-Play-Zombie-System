/**
 *	RNPC - Recording NPC
 *	Plugin by Mauzen - 29.6.2012
 *	Based on Joe Staffs code (http://forum.sa-mp.com/showthread.php?t=344747)
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <limits>
#include <sstream>

#include "./SDK/amx/amx.h"
#include "./SDK/plugin.h"


#include "./ThirdPartySource/Invoke.cpp"
#include "RNPC.h"


#define VERSION			"0.4.1"
#define BUILD			14
#define DATE			"03.12.2014"


AMX * pAMX;

extern void * pAMXFunctions;

typedef void (*logprintf_t)(char* format, ...);

logprintf_t logprintf;

using namespace std;

CMapAndreas MapAndreas;

RNPC defnpc(&MapAndreas);


// -------------------------------------------------------
// New build mode

static cell AMX_NATIVE_CALL n_StartBuild( AMX* amx, cell* params) {
	// Get NPC position
	cell x; cell y; cell z; cell angle;
	float a; float b; float c; float d;

	g_Invoke->callNative(&PAWN::GetPlayerPos, params[1], &x, &y, &z);
	g_Invoke->callNative(&PAWN::GetPlayerFacingAngle, params[1], &angle);

	defnpc.StartBuild(params[1], params[2], params[3], amx_ctof(x), amx_ctof(y), amx_ctof(z));
	EulerToQuaternion(0.0F, 360.0F - amx_ctof(angle), 0.0F, a, b, c, d);
	defnpc.settings.quat1 = a;
	defnpc.settings.quat2 = b;
	defnpc.settings.quat3 = c;
	defnpc.settings.quat4 = d;
	defnpc.vsettings.quat1 = a;
	defnpc.vsettings.quat2 = b;
	defnpc.vsettings.quat3 = c;
	defnpc.vsettings.quat4 = d;
	return 1;
}

static cell AMX_NATIVE_CALL n_StartCustomBuild(AMX* amx, cell* params) {	
	char* cname;
	amx_StrParam(amx, params[2], cname);

	defnpc.StartBuild(params[1], cname);

	return 1;
}

static cell AMX_NATIVE_CALL n_EndBuild( AMX* amx, cell* params) {
	defnpc.EndBuild(params[1]);
	return 1;
}

static cell AMX_NATIVE_CALL n_AddMovement( AMX* amx, cell* params) {
	defnpc.AddMovement(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]),
						amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]), (params[8]==1));
	return 1;
}
static cell AMX_NATIVE_CALL n_AddMovementAlt( AMX* amx, cell* params) {
	defnpc.AddMovementAlt(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]),
						amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]), (char)params[8]);
	return 1;
}

static cell AMX_NATIVE_CALL n_ConcatMovement( AMX* amx, cell* params) {
	defnpc.AddMovement(defnpc.settings.xpos, defnpc.settings.ypos, defnpc.settings.zpos, amx_ctof(params[1]),
						amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), (params[8] == 1));
	return 1;
}

static cell AMX_NATIVE_CALL n_AddPause( AMX* amx, cell* params) {
	defnpc.AddPause(params[1], params[2]);
	return 1;
}

static cell AMX_NATIVE_CALL n_GetBuildLength( AMX* amx, cell* params) {
	return defnpc.curTime;
}

// --------------------------------------------------------
// native RNPC_SetUpdateRate(rate);

static cell AMX_NATIVE_CALL n_SetUpdateRate( AMX* amx, cell* params) {
	defnpc.updaterate = params[1];
	return 1;
}

//
// Setters
static cell AMX_NATIVE_CALL n_SetLRKeys( AMX* amx, cell* params) {
	defnpc.settings.lrkeys = (short)params[1];
	defnpc.vsettings.lrkeys = (short)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetUDKeys( AMX* amx, cell* params) {
	defnpc.settings.udkeys = (short)params[1];
	defnpc.vsettings.udkeys = (short)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetKeys( AMX* amx, cell* params) {
	defnpc.settings.keys = (short)params[1];
	defnpc.vsettings.keys = (short)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetQuaternion( AMX* amx, cell* params ) {
	defnpc.settings.quat1 = amx_ctof(params[1]);
	defnpc.settings.quat2 = amx_ctof(params[2]);
	defnpc.settings.quat3 = amx_ctof(params[3]);
	defnpc.settings.quat4 = amx_ctof(params[4]);
	defnpc.vsettings.quat1 = amx_ctof(params[1]);
	defnpc.vsettings.quat2 = amx_ctof(params[2]);
	defnpc.vsettings.quat3 = amx_ctof(params[3]);
	defnpc.vsettings.quat4 = amx_ctof(params[4]);
	return 1;
}
static cell AMX_NATIVE_CALL n_SetInternalPos( AMX* amx, cell* params) {
	defnpc.settings.xpos = amx_ctof(params[1]);
	defnpc.settings.ypos = amx_ctof(params[2]);
	defnpc.settings.zpos = amx_ctof(params[3]);
	defnpc.vsettings.xpos = amx_ctof(params[1]);
	defnpc.vsettings.ypos = amx_ctof(params[2]);
	defnpc.vsettings.zpos = amx_ctof(params[3]);
	return 1;
}
static cell AMX_NATIVE_CALL n_SetHealth( AMX* amx, cell* params) {
	defnpc.settings.health = (unsigned char)params[1];
	defnpc.vsettings.health = (unsigned char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetArmour( AMX* amx, cell* params) {
	defnpc.settings.armour = (unsigned char)params[1];
	defnpc.vsettings.parmour = (unsigned char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetSpecialAction( AMX* amx, cell* params) {
	defnpc.settings.specialaction = (unsigned char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetWeaponID( AMX* amx, cell* params) {
	defnpc.settings.weaponid = (unsigned char)params[1];
	defnpc.vsettings.weaponid = (unsigned char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetAnimID( AMX* amx, cell* params) {
	defnpc.settings.animID = (unsigned char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetAnimParams( AMX* amx, cell* params) {
	defnpc.settings.animParams = (short)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetAngleQuats( AMX* amx, cell* params) {
	float w; float x; float y; float z;

	EulerToQuaternion(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), w, x, y, z);
	defnpc.settings.quat1 = w;
	defnpc.settings.quat2 = x;
	defnpc.settings.quat3 = y;
	defnpc.settings.quat4 = z;
	defnpc.vsettings.quat1 = w;
	defnpc.vsettings.quat2 = x;
	defnpc.vsettings.quat3 = y;
	defnpc.vsettings.quat4 = z;
	return 1;
}
static cell AMX_NATIVE_CALL n_SetSirenState( AMX* amx, cell* params) {
	defnpc.vsettings.siren = (char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetGearState( AMX* amx, cell* params) {
	defnpc.vsettings.gear = (char)params[1];
	return 1;
}
static cell AMX_NATIVE_CALL n_SetDriverHealth( AMX* amx, cell* params) {
	defnpc.vsettings.phealth = (char)params[1];
	return 1;
}

static cell AMX_NATIVE_CALL n_SetAcceleration( AMX* amx, cell* params ) {
	defnpc.acceleration = amx_ctof(params[1]);
	return 1;
}
static cell AMX_NATIVE_CALL n_SetDeceleration( AMX* amx, cell* params ) {
	defnpc.deceleration = amx_ctof(params[1]);
	return 1;
}

// ---------------------------------------------------------
// ------------------------ MapAndreas
//----------------------------------------------------------
//----------------------------------------------------------
// native MapAndreas_Init(mode);

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_Init(AMX* amx, cell* params)
{
	char* cname;
	amx_StrParam(amx, params[2], cname);

	int iRetVal = MapAndreas.Init(params[1], cname, params[3]);

	if (iRetVal != MAP_ANDREAS_ERROR_SUCCESS) {
		logprintf("MapAndreas: plugin could not init!");
		logprintf("MapAndreas: check files and make sure you have enough memory!");
		return 0;
	}
	return 1;
}

//----------------------------------------------------------
// native MapAndreas_FindZ_For2DCoord(Float:X, Float:Y, &Float:Z);

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_FindZ_For2DCoord(AMX* amx, cell* params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z;

	Z = MapAndreas.FindZ_For2DCoord(X, Y);

	cell* cptr;
	amx_GetAddr(amx, params[3], &cptr);
	*cptr = amx_ftoc(Z);

	if (Z > 0.0f) return 1;

	return 0;
}

//----------------------------------------------------------
// native MapAndreas_SetZ_For2DCoord(Float:X, Float:Y, Float:Z);

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_SetZ_For2DCoord(AMX* amx, cell* params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z = amx_ctof(params[3]);

	return MapAndreas.SetZ_For2DCoord(X, Y, Z);
}

//----------------------------------------------------------
// native MapAndreas_SaveCurrentHMap(AMX* amx, cell* params);

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_SaveCurrentHMap(AMX* amx, cell* params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z = amx_ctof(params[3]);

	char* name;

	amx_StrParam(amx, params[1], name);

	return MapAndreas.SaveCurrentHMap(name);
}

//----------------------------------------------------------
// native MapAndreas_Unload();

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_Unload(AMX* amx, cell* params)
{
	return MapAndreas.Unload();
}

//----------------------------------------------------------
// native MapAndreas_FindAverageZ(Float:X, Float:Y, &Float:Z);

PLUGIN_EXTERN_C cell AMX_NATIVE_CALL n_FindAverageZ(AMX* amx, cell* params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z;

	Z = MapAndreas.GetAverageZ(X, Y);

	cell* cptr;
	amx_GetAddr(amx, params[3], &cptr);
	*cptr = amx_ftoc(Z);

	if (Z > 0.0f) return 1;

	return 0;
}

//----------------------------------------------------------

AMX_NATIVE_INFO RNPCNatives[] =
{
	{"RNPC_SetUpdateRate",		n_SetUpdateRate},
	{"RNPC_SetLRKeys",			n_SetLRKeys},
	{"RNPC_SetUDKeys",			n_SetUDKeys},
	{"RNPC_SetKeys",			n_SetKeys},
	{"RNPC_SetInternalPos",		n_SetInternalPos},
	{"RNPC_SetHealth",			n_SetHealth},
	{"RNPC_SetDriverHealth",	n_SetDriverHealth},
	{"RNPC_SetArmour",			n_SetArmour},
	{"RNPC_SetSpecialAction",	n_SetSpecialAction},
	{"RNPC_SetWeaponID",		n_SetWeaponID},
	{"RNPC_SetAnimID",			n_SetAnimID},
	{"RNPC_SetAnimParams",		n_SetAnimParams},
	{"RNPC_SetAngleQuats",		n_SetAngleQuats},
	{"RNPC_SetQuaternion",		n_SetQuaternion},
	{"RNPC_SetSirenState",		n_SetSirenState},
	{"RNPC_SetGearState",		n_SetGearState},
	{"RNPC_SetAcceleration",	n_SetAcceleration},
	{"RNPC_SetDeceleration",	n_SetDeceleration},
	{"RNPC_GetBuildLength",		n_GetBuildLength},
	{"RNPC_CreateBuild",		n_StartBuild},
	{"RNPC_CreateCustomBuild",	n_StartCustomBuild},
	{"RNPC_FinishBuild",		n_EndBuild},
	{"RNPC_AddMovement",		n_AddMovement},
	{"RNPC_AddMovementAlt",		n_AddMovementAlt},
	{"RNPC_ConcatMovement",		n_ConcatMovement},	
	{"RNPC_AddPause",			n_AddPause},
	{ "MapAndreas_Init",		n_Init },
	{ "MapAndreas_FindZ_For2DCoord",n_FindZ_For2DCoord },
	{ "MapAndreas_GetAverageZ",	n_FindAverageZ },
	{ "MapAndreas_Unload",		n_Unload },
	{ "MapAndreas_SaveCurrentHMap", n_SaveCurrentHMap },
	{ "MapAndreas_SetZ_For2DCoord", n_SetZ_For2DCoord },
	{ 0,						0 }
};

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	g_Invoke = new Invoke;

	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];	
	
	logprintf( "RNPC V%s by Mauzen (%s)", VERSION, DATE );
	// Some test stuff I was too lazy to remove
	//logprintf("Testload %d", defnpc.ofRec = OnFootRecording("npcmodes/recordings/test.rec"));
	//logprintf("fin: %d", defnpc.ofRec.ofdata.size());
	

	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{
	list<AMX*>::iterator i = g_Invoke->amx_list.begin();
	list<AMX*>::iterator l;

	for(l = g_Invoke->amx_list.end(); i != l; i++)
	{
		if(*i == pAMX)
		{
			g_Invoke->amx_list.erase(i);
			break;
		}
	}
	if(g_Invoke) delete g_Invoke;

	logprintf( "RNPC plugin unloaded." );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	pAMX = amx;
	g_Invoke->amx_list.push_back(amx);
	if (!g_Invoke->getAddresses())
	{
		//logprintf(RNPC: Resolving native addresses failed.");
	}
	return amx_Register( amx, RNPCNatives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	return AMX_ERR_NONE;
}