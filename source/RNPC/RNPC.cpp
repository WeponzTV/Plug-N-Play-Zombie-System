// Disable fopen_s warnings
#pragma warning( disable : 4996 )

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


#include "RNPC.h"

// --------------------- Low level ----------------------------------

RNPC::RNPC(CMapAndreas *ma)
{
	MapAndreas = ma;
	updaterate = 100;
}

RNPC::RNPC()
{
	updaterate = 100;
}

/*
	Start buildmode for a specific NPC.
*/
int RNPC::StartBuild(int npcid, int type, int slot, float x, float y, float z)
{
	if (buildActive) return 0;

	buildActive = true;
	curTime = 0;
	recMode = type;	

	curNPCID = npcid;
	curSlot = slot;
	
	sprintf(buildname, "npcmodes/recordings/rnpc%03d-%02d.rec", curNPCID, curSlot);

	if (type == PLAYER_RECORDING_TYPE_ONFOOT) {
		ofRec.reset();
		
		settings.xpos = x;
		settings.ypos = y;
		settings.zpos = z;
		settings.health = 100;
		return 1;
	} else if (type == PLAYER_RECORDING_TYPE_DRIVER) {
		vRec.reset();

		vsettings.xpos = x;
		vsettings.ypos = y;
		vsettings.zpos = z;
		vsettings.health = 1000.0;
		vsettings.phealth = 100;
		return 1;
	}

	return 0;
}

/*
	Start buildmode for a specific output file.
*/
int RNPC::StartBuild(int type, char* cname)
{
	if (buildActive) return 0;

	buildActive = true;
	curTime = 0;
	recMode = type;

	curNPCID = 0;
	curSlot = 0;

	sprintf(buildname, "npcmodes/recordings/%s.rec", cname);

	if (type == PLAYER_RECORDING_TYPE_ONFOOT) {
		ofRec.reset();
		settings.health = 100;
		return 1;
	}
	else if (type == PLAYER_RECORDING_TYPE_DRIVER) {
		vRec.reset();
		vsettings.health = 1000.0;
		vsettings.phealth = 100;
		return 1;
	}

	return 0;
}

int RNPC::EndBuild(int autoreset)
{
	if (!buildActive) return 0;

	curspeed = 0.0;	

	if (recMode == PLAYER_RECORDING_TYPE_ONFOOT) {
		if (autoreset) {
			OnFootSegment s;

			// Clear movement settings for the last segment
			s.time = settings.time+1;
			s.xpos = settings.xpos;
			s.ypos = settings.ypos;
			s.zpos = settings.zpos;
			s.quat1 = settings.quat1;
			s.quat2 = settings.quat2;
			s.quat3 = settings.quat3;
			s.quat4 = settings.quat4;
			s.xvel = 0.0;
			s.yvel = 0.0;
			s.zvel = 0.0;
			// Apply custom settings
			s.health = settings.health;
			s.armour = settings.armour;
			s.specialaction = settings.specialaction;
			s.weaponid = settings.weaponid;
			s.animID = settings.animID;
			s.animParams = settings.animParams;
			s.keys = settings.keys;
			s.lrkeys = settings.lrkeys;
			s.udkeys = 0;

			ofRec.addSegment(s);
		}

		ofRec.writeToFile(buildname);

		settings = OnFootSegment();
	} else if (recMode == PLAYER_RECORDING_TYPE_DRIVER) {
		if (autoreset) {
			VehicleSegment s;

			// Clear movement settings for the last segment
			s.time = vsettings.time+1;
			s.xpos = vsettings.xpos;
			s.ypos = vsettings.ypos;
			s.zpos = vsettings.zpos;
			s.quat1 = vsettings.quat1;
			s.quat2 = vsettings.quat2;
			s.quat3 = vsettings.quat3;
			s.quat4 = vsettings.quat4;
			s.xvel = 0.0;
			s.yvel = 0.0;
			s.zvel = 0.0;
			// Apply custom settings
			s.health = vsettings.health;
			s.phealth = vsettings.phealth;
			s.parmour = vsettings.parmour;
			s.weaponid = vsettings.weaponid;
			s.keys = vsettings.keys;
			s.lrkeys = vsettings.lrkeys;
			s.udkeys = vsettings.udkeys;
			s.siren = vsettings.siren;
			s.gear = vsettings.gear;
		
			vRec.addSegment(s);
		}

		vRec.writeToFile(buildname);

		vsettings = VehicleSegment();
	}

	acceleration = 0;		
	buildActive = false;
	return 1;
}

int RNPC::AddMovement(float x1, float y1, float z1, float x2, float y2, float z2, float speed, bool zmap)
{
	if (!buildActive) return 0;

	if (recMode == PLAYER_RECORDING_TYPE_ONFOOT) {
		int time, steps;    

		float distance = sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) );

		float angle = atan2(y2 - y1, x2 - x1);
   
		float xvel = (speed * cos(angle));
		float yvel = (speed * sin(angle));
		float zvel = 0.0;

		angle = (-angle) / TODEGREES + 90.0F;

		if (speed < 0.00001) time = 0;
		else time = (int)(distance / speed);
   
		steps = time / updaterate;

		if (steps == 0) {
			settings.xpos = x1;
			settings.ypos = y1;
			settings.zpos = z1;
			return 0;
		}
   
		float xrate = (x2 - x1) / steps;
		float yrate = (y2 - y1) / steps;
		float zrate = (z2 - z1) / steps;
   
   
		if(steps == 0) time = 0;
		else time = time / steps;
   
		int step = 0;

		float w, x, y, z;
		EulerToQuaternion(angle, w, x, y, z);
	
		OnFootSegment s;
		
		//char c[72];	

		// Apply movement settings
		s.xvel = xvel;
		s.yvel = yvel;
		s.zvel = zvel;
		s.quat1 = w;
		s.quat2 = x;
		s.quat3 = y;
		s.quat4 = z;
		s.udkeys = 0x00FF;		
		// Apply custom settings
		s.health = settings.health;
		s.armour = settings.armour;
		s.specialaction = settings.specialaction;
		s.weaponid = settings.weaponid;
		s.animID = settings.animID;
		s.animParams = settings.animParams;
		s.keys = settings.keys;
		s.lrkeys = settings.lrkeys;

		if (fabs(speed - RNPC_SPEED_WALK) <= 0.0001) {
			s.keys = s.keys | 1024;
		} else if (fabs(speed - RNPC_SPEED_SPRINT) <= 0.0001) {
			s.keys = s.keys | 8;
		}

		while(step <= steps)
		{
			s.time = time * step + curTime;
			if (step == steps) {
				//s.udkeys = 0;
				s.xvel = 0.0;
				s.yvel = 0.0;
			}
			s.xpos = x1 + xrate * step;
			s.ypos = y1 + yrate * step;
			if (zmap) s.zpos = MapAndreas->FindZ_For2DCoord(s.xpos, s.ypos)+0.5F;
			else s.zpos = z1 + zrate * step;		
		
			ofRec.addSegment(s);

			step++;
		}
		settings.xpos = x2;
		settings.ypos = y2;
		if (zmap) settings.zpos = MapAndreas->FindZ_For2DCoord(settings.xpos, settings.ypos)+0.5F;
		else settings.zpos = z2;
		settings.xvel = xvel;
		settings.yvel = yvel;
		settings.zvel = zvel;
		settings.quat1 = w;
		settings.quat2 = x;
		settings.quat3 = y;
		settings.quat4 = z;

		curTime += (int)(distance / speed) + updaterate;

		return 1;
	} else if (recMode == PLAYER_RECORDING_TYPE_DRIVER) {
		int time, steps;    

		float distance = sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) );

		float angle = atan2(y2 - y1, x2 - x1);

		float xvel;
		float yvel;
		float zvel = 0.0;

		int accsteps;
		float accdist;
		if (acceleration > 0) {
			accsteps = (int)(speed / (acceleration) + 0.5);
			accdist = 0;
			for (int i = 0; i < accsteps; i++) {
				accdist += acceleration * i * updaterate;
			}
			time = (int)((distance - accdist) / speed) + accsteps * updaterate;
			xvel = cos(angle);
			yvel = sin(angle);
		} else {
			time = (int)(distance / speed);
			xvel = speed * cos(angle);
			yvel = speed * sin(angle);
		}

		angle = (-angle) / TODEGREES + 90.0F;
   
		steps = time / updaterate;

		if (steps == 0) {
			vsettings.xpos = x1;
			vsettings.ypos = y1;
			vsettings.zpos = z1;
			return 0;
		}
   
		float xrate = (x2 - x1) / steps;
		float yrate = (y2 - y1) / steps;
		float zrate = (z2 - z1) / steps;
   
   
		if(steps == 0) time = 0;
		else time = time / steps;
   
		int step = 0;

		float w, x, y, z;
		EulerToQuaternion(angle, w, x, y, z);
	
		VehicleSegment s;

		// Apply movement settings		
		s.xvel = xvel;
		s.yvel = yvel;
		s.zvel = zvel;
		s.quat1 = w;
		s.quat2 = x;
		s.quat3 = y;
		s.quat4 = z;
		s.udkeys = 0xFFFF;		
		// Apply custom settings
		s.health = vsettings.health;
		s.phealth = vsettings.phealth;
		s.parmour = vsettings.parmour;
		s.weaponid = vsettings.weaponid;
		s.keys = vsettings.keys;
		s.lrkeys = vsettings.lrkeys;
		s.siren = vsettings.siren;
		s.gear = vsettings.gear;


		while(step <= steps)
		{
			s.time = time * step + curTime;
			if (acceleration > 0) {
				if (step < accsteps) {
					s.xvel = xvel * acceleration * step;
					s.yvel = yvel * acceleration * step;
					s.xpos = x1 + xrate * step;
					s.ypos = y1 + yrate * step;
					if (zmap) s.zpos = MapAndreas->FindZ_For2DCoord(s.xpos, s.ypos) + 0.5F;
					else s.zpos = z1 + zrate * step;
				} else {
					s.xvel = xvel * speed;
					s.yvel = yvel * speed;

					s.xpos = x1 + xrate * step;
					s.ypos = y1 + yrate * step;
					if (zmap) s.zpos = MapAndreas->FindZ_For2DCoord(s.xpos, s.ypos) + 0.5F;
					else s.zpos = z1 + zrate * step;
				}
			} else {			
				s.xpos = x1 + xrate * step;
				s.ypos = y1 + yrate * step;
				s.zpos = z1 + zrate * step;	
			}
		
			if (step == steps) {
				s.udkeys = 0;
				s.xvel = 0.0;
				s.yvel = 0.0;
			}

			vRec.addSegment(s);

			step++;
		}
		vsettings.xpos = x2;
		vsettings.ypos = y2;
		if (zmap) vsettings.zpos = MapAndreas->FindZ_For2DCoord(vsettings.xpos, vsettings.ypos) + 0.5F;
		else vsettings.zpos = z2;
		vsettings.xvel = xvel;
		vsettings.yvel = yvel;
		vsettings.zvel = zvel;
		vsettings.quat1 = w;
		vsettings.quat2 = x;
		vsettings.quat3 = y;
		vsettings.quat4 = z;

		curTime += (int)(distance / speed) + updaterate;

		return 1;
	}
	return 0;
}

int RNPC::AddMovementAlt(float x1, float y1, float z1, float x2, float y2, float z2, float speed, char lock)
{
	if (!buildActive) return 0;

	if (recMode == PLAYER_RECORDING_TYPE_DRIVER) {
		int steps = 0;
		
		if (acceleration < 0.0001) curspeed = speed;

		float angle = atan2(y2 - y1, x2 - x1);

		float distance = sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) );
		float travelled = 0.0;

		float xvel;
		float yvel;
		float zvel;

		if (acceleration > 0) {
			xvel = cos(angle);
			yvel = sin(angle);
			zvel = atan2(z2 - z1, sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) ));
		} else {
			xvel = speed * cos(angle);
			yvel = speed * sin(angle);
			zvel = speed * atan2(z2 - z1, sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) ));
		}

		angle = (-angle) / TODEGREES + 90.0F;
   
		   
		int step = 0;

		float w, x, y, z;
		EulerToQuaternion(0.0, angle, zvel, w, x, y, z);
	
		VehicleSegment s;
		
		//char c[67];	

		// Apply movement settings		
		s.quat1 = w;
		s.quat2 = x;
		s.quat3 = y;
		s.quat4 = z;
		//s.lrkeys = 0x0080;
		//s.udkeys = 0x0080;
		// Apply custom settings
		s.health = vsettings.health;
		s.phealth = vsettings.phealth;
		s.parmour = vsettings.parmour;
		s.weaponid = vsettings.weaponid;
		s.keys = vsettings.keys;
		s.lrkeys = vsettings.lrkeys;
		s.udkeys = vsettings.udkeys;
		s.siren = vsettings.siren;
		s.gear = vsettings.gear;
		s.time = curTime;

		s.xpos = x1;
		s.ypos = y1;
		s.zpos = z1;

		while(travelled < distance) {
			s.time += updaterate;
			if (curspeed < speed) {
				curspeed += acceleration * (updaterate / 1000.0F);
				if (curspeed > speed) curspeed = speed;
			}
			s.xvel = xvel * curspeed * (1000.0F / updaterate);
			s.yvel = yvel * curspeed * (1000.0F / updaterate);
			s.zvel = zvel * curspeed * (1000.0F / updaterate);
			s.xpos = s.xpos + xvel * curspeed * updaterate;
			s.ypos = s.ypos + yvel * curspeed * updaterate;
			s.zpos = (lock)?(MapAndreas->FindZ_For2DCoord(s.xpos, s.ypos) + 0.5F):(s.zpos + zvel * curspeed * updaterate);

			travelled += sqrt(updaterate * curspeed * updaterate * curspeed * ( (xvel) * (xvel) + (yvel) * (yvel)
				+ (zvel) * (zvel)));

			vRec.addSegment(s);

			steps++;
		}
		vsettings.xpos = x2;
		vsettings.ypos = y2;
		vsettings.zpos = z2;
		vsettings.xvel = xvel;
		vsettings.yvel = yvel;
		vsettings.quat1 = w;
		vsettings.quat2 = x;
		vsettings.quat3 = y;
		vsettings.quat4 = z;

		curTime += s.time;

		return 1;
	} else if (recMode == PLAYER_RECORDING_TYPE_ONFOOT) {
		// Temp fix
		int time, steps;    

		float distance = sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) );

		float angle = atan2(y2 - y1, x2 - x1);
   
		float xvel = (speed * cos(angle));
		float yvel = (speed * sin(angle));
		float zvel = 0.0;

		angle = (-angle) / TODEGREES + 90.0F;

		if (speed < 0.00001) time = 0;
		else time = (int)(distance / speed);
   
		steps = time / updaterate;

		if (steps == 0) {
			settings.xpos = x1;
			settings.ypos = y1;
			settings.zpos = z1;
			return 0;
		}
   
		float xrate = (x2 - x1) / steps;
		float yrate = (y2 - y1) / steps;
		float zrate = (z2 - z1) / steps;
   
   
		if(steps == 0) time = 0;
		else time = time / steps;
   
		int step = 0;

		float w, x, y, z;
		EulerToQuaternion(angle, w, x, y, z);
	
		OnFootSegment s;

		// Apply movement settings
		s.xvel = xvel;
		s.yvel = yvel;
		s.zvel = zvel;
		s.quat1 = w;
		s.quat2 = x;
		s.quat3 = y;
		s.quat4 = z;
		s.udkeys = 0x00FF;		
		// Apply custom settings
		s.health = settings.health;
		s.armour = settings.armour;
		s.specialaction = settings.specialaction;
		s.weaponid = settings.weaponid;
		s.animID = settings.animID;
		s.animParams = settings.animParams;
		s.keys = settings.keys;
		s.lrkeys = settings.lrkeys;

		if (fabs(speed - RNPC_SPEED_WALK) <= 0.0001) {
			s.keys = s.keys | 1024;
		} else if (fabs(speed - RNPC_SPEED_SPRINT) <= 0.0001) {
			s.keys = s.keys | 8;
		}

		while(step <= steps)
		{
			s.time = time * step + curTime;
			if (step == steps) {
				s.udkeys = 0;
				s.xvel = 0.0;
				s.yvel = 0.0;
			}
			s.xpos = x1 + xrate * step;
			s.ypos = y1 + yrate * step;
			s.zpos = (lock)?(MapAndreas->FindZ_For2DCoord(s.xpos, s.ypos) + 0.5F):(z1 + zrate * step);		
		
			ofRec.addSegment(s);

			step++;
		}
		settings.xpos = x2;
		settings.ypos = y2;
		settings.zpos = (lock)?(MapAndreas->FindZ_For2DCoord(x2, y2) + 0.5F):z2;
		settings.xvel = xvel;
		settings.yvel = yvel;
		settings.zvel = zvel;
		settings.quat1 = w;
		settings.quat2 = x;
		settings.quat3 = y;
		settings.quat4 = z;

		curTime += (int)(distance / speed) + updaterate;

		return 1;
	}
	return 0;
}

int RNPC::AddPause(int time, int comp)
{
	if (!buildActive) return 0;

	curspeed = 0.0;

	if (recMode == PLAYER_RECORDING_TYPE_ONFOOT) {
		settings.time = curTime+1;
		settings.udkeys = 0;
		OnFootSegment s = settings;
		ofRec.addSegment(s);

		if (comp == 1) {
			settings.time = curTime + time / 2;

			// Might be obsolete?
			ofRec.addSegment(settings);

			curTime += time / 2;
		}
		else {
			settings.time = curTime + time;
			ofRec.addSegment(settings);
			curTime += time + 1;
		}		

		return 1;
	} else if (recMode == PLAYER_RECORDING_TYPE_DRIVER) {
		vsettings.time = curTime;
		VehicleSegment s = vsettings;
		vRec.addSegment(s);

		settings.time = curTime + time;

		// Might be obsolete?
		//vRec.addSegment(vsettings);

		//curTime += time;

		curTime += time;

		return 1;
	}
	return 0;
}
