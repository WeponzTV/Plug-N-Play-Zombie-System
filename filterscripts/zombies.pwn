/*
	Plug'N'Play Zombie System by Weponz (2018)
*/
#define FILTERSCRIPT
#include <a_samp>//Credits: SA-MP
#include <rnpc>//Credits: Mauzen

#define HOLDING(%0) ((newkeys & (%0)) == (%0))
#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

//Configuration (YOU MAY EDIT THIS SECTION)
#define MAX_ZOMBIES 25//Maximum amount of zombies to spawn. (Default: 25)

#define ZOMBIE_SKIN 162//Zombie skin id. (Default: 162)
#define ZOMBIE_COLOUR 0xFF0000FF//Zombie colour. (Default: Red)
#define ZOMBIE_DAMAGE 20//Zombie damage. (Default: 20)
#define ZOMBIE_DETECT 25//Zombie detection range. (Default: 25)

//DO NOT EDIT BELOW THIS LINE!!!
#define MAX_MOVEMENT 6
#define MAX_ROAM 10

#define RUNNER_ZOMBIE 0
#define SPRINTER_ZOMBIE 1

forward OnZombieCreate();
forward OnZombieUpdate();

forward ResetDetectRange(playerid);
forward ResetZombieKill(playerid);

forward RemoveZombieBody(npcid);

enum server_data
{
	server_zombies,
	server_zombietimer
};
new ServerData[server_data];

enum zombie_data
{
	zombie_species,
	zombie_victim,
	zombie_roam,
	zombie_movement
};
new ZombieData[MAX_ZOMBIES][zombie_data];

enum victim_data
{
	victim_detect,
	victim_timer,
	victim_status,
	victim_kill
};
new VictimData[MAX_PLAYERS][victim_data];

stock GetZombieVictim(npcid)
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if(IsPlayerConnected(i) && !IsPlayerNPC(i) && IsPlayerSpawned(i))
	    {
	        new Float:pos[3], Float:pos2[3];
			GetPlayerPos(npcid, pos[0], pos[1], pos[2]);
			GetPlayerPos(i, pos2[0], pos2[1], pos2[2]);
			if(IsPlayerInRangeOfPoint(i, VictimData[i][victim_detect], pos[0], pos[1], pos[2]) && GetPlayerState(i) == PLAYER_STATE_ONFOOT)
			{
			    return GetClosestVictim(npcid);
			}
	    }
	}
	return INVALID_PLAYER_ID;
}

stock GetClosestVictim(npcid)
{
    new Float:dist = 1000.0, target = INVALID_PLAYER_ID, Float:pos[3], Float:pos2[3], Float:tmpdis;
    GetPlayerPos(npcid, pos[0], pos[1], pos[2]);
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || IsPlayerNPC(i) || i == npcid || !IsPlayerSpawned(i)) continue;
        GetPlayerPos(i, pos2[0], pos2[1], pos2[2]);
        tmpdis = floatsqroot(floatpower(floatabs(floatsub(pos2[0], pos[0])), 2) + floatpower(floatabs(floatsub(pos2[1], pos[1])), 2) + floatpower(floatabs(floatsub(pos2[2], pos[2])), 2));
        if(tmpdis < dist && GetPlayerState(i) == PLAYER_STATE_ONFOOT)
        {
            dist = tmpdis;
            target = i;
        }
    }
    return target;
}

stock GetRandomPlayer()
{
    new players[MAX_PLAYERS], online = 0;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
	    if(!IsPlayerNPC(i) && IsPlayerConnected(i))
        {
			players[online] = i;
          	online++;
        }
    }

    if(online > 0)
    {
        return players[random(online)];
    }
    return INVALID_PLAYER_ID;
}

stock GetOnlinePlayers()
{
	new count = 0;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	  	if(IsPlayerConnected(i) && !IsPlayerNPC(i))
	  	{
			count++;
		}
	}
	return count;
}

stock bool:IsZombieStranded(npcid)
{
	new Float:pos[3];
	GetPlayerPos(npcid, pos[0], pos[1], pos[2]);

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
	    if(IsPlayerInRangeOfPoint(i, 300.0, pos[0], pos[1], pos[2]))
	    {
	    	return false;
	    }
	}
	return true;
}

stock IsHoldingFirearm(playerid)
{
	if(GetPlayerWeapon(playerid) == 30) { return 1; }
	else if(GetPlayerWeapon(playerid) == 31) { return 1; }
	else if(GetPlayerWeapon(playerid) == 32) { return 1; }
	else if(GetPlayerWeapon(playerid) == 22) { return 1; }
	else if(GetPlayerWeapon(playerid) == 23) { return 1; }
	else if(GetPlayerWeapon(playerid) == 24) { return 1; }
	else if(GetPlayerWeapon(playerid) == 33) { return 1; }
	else if(GetPlayerWeapon(playerid) == 34) { return 1; }
	else if(GetPlayerWeapon(playerid) == 25) { return 1; }
	else if(GetPlayerWeapon(playerid) == 27) { return 1; }
	else if(GetPlayerWeapon(playerid) == 26) { return 1; }
	else if(GetPlayerWeapon(playerid) == 28) { return 1; }
	else if(GetPlayerWeapon(playerid) == 29) { return 1; }
	return 0;
}

stock IsPlayerSpawned(playerid)
{
	new pState = GetPlayerState(playerid);
	return 0 <= playerid <= MAX_PLAYERS && pState != PLAYER_STATE_NONE && pState != PLAYER_STATE_WASTED && pState != PLAYER_STATE_SPECTATING;
}

public OnFilterScriptInit()
{
    for(new i = 0; i < MAX_ZOMBIES; i++)
	{
	    ZombieData[i][zombie_species] = RUNNER_ZOMBIE;
	    ZombieData[i][zombie_victim] = INVALID_PLAYER_ID;
	    ZombieData[i][zombie_movement] = 0;
	}

	ServerData[server_zombies] = 0;
	ServerData[server_zombietimer] = SetTimer("OnZombieCreate", 80, true);//Just incase, as some servers require you to do this.

	SetTimer("OnZombieUpdate", 1000, true);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if(IsPlayerNPC(playerid))
	{
	    new players = GetOnlinePlayers(), targetid = GetRandomPlayer(), position = random(10), Float:pos[3], health = random(25) + 15;
		if(players > 0 && targetid != INVALID_PLAYER_ID)
		{
			GetPlayerPos(targetid, pos[0], pos[1], pos[2]);

			ClearAnimations(playerid);

			SetPlayerPos(playerid, pos[0], pos[1], pos[2] + 500.0);
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);

			if(position == 0) { SetPlayerPos(playerid, pos[0] + 100.0, pos[1], pos[2] + 500.00); }
			else if(position == 1) { SetPlayerPos(playerid, pos[0], pos[1] + 100.0, pos[2] + 500.00); }
			else if(position == 2) { SetPlayerPos(playerid, pos[0] - 100.0, pos[1], pos[2] + 500.00); }
			else if(position == 3) { SetPlayerPos(playerid, pos[0], pos[1] - 100.0, pos[2] + 500.00); }
			else if(position == 4) { SetPlayerPos(playerid, pos[0] + 150.0, pos[1], pos[2] + 500.00); }
			else if(position == 5) { SetPlayerPos(playerid, pos[0], pos[1] + 150.0, pos[2] + 500.00); }
			else if(position == 6) { SetPlayerPos(playerid, pos[0] - 150.0, pos[1], pos[2] + 500.00); }
			else { SetPlayerPos(playerid, pos[0], pos[1] - 150.0, pos[2] + 500.00); }
		}
		else
		{
			ClearAnimations(playerid);

			SetPlayerPos(playerid, 0.0, 0.0, 0.0 + 500.00);
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);

			if(position == 0) { SetPlayerPos(playerid, 0.0 + 100.0, 0.0, 0.0 + 500.00); }
			else if(position == 1) { SetPlayerPos(playerid, 0.0, 0.0 + 100.0, 0.0 + 500.00); }
			else if(position == 2) { SetPlayerPos(playerid, 0.0 - 100.0, 0.0, 0.0 + 500.00); }
			else if(position == 3) { SetPlayerPos(playerid, 0.0, 0.0 - 100.0, 0.0 + 500.00); }
			else if(position == 4) { SetPlayerPos(playerid, 0.0 + 150.0, 0.0, 0.0 + 500.00); }
			else if(position == 5) { SetPlayerPos(playerid, 0.0, 0.0 + 150.0, 0.0 + 500.00); }
			else if(position == 6) { SetPlayerPos(playerid, 0.0 - 150.0, 0.0, 0.0 + 500.00); }
		}

		SetRNPCHealth(playerid, health);
		SetPlayerSkin(playerid, ZOMBIE_SKIN);
		SetPlayerColor(playerid, ZOMBIE_COLOUR);

		RNPC_SetShootable(playerid, 1);
		RNPC_ToggleVehicleCollisionCheck(playerid, 1);

		ZombieData[playerid][zombie_victim] = INVALID_PLAYER_ID;

	    ZombieData[playerid][zombie_roam] = 0;
	    ZombieData[playerid][zombie_movement] = 0;
	}
	else if(!IsPlayerNPC(playerid))
	{
    	VictimData[playerid][victim_status] = 0;
	    VictimData[playerid][victim_detect] = ZOMBIE_DETECT;
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    if(IsPlayerNPC(killerid))
	{
		new health = random(25) + 15;
		SetRNPCHealth(killerid, health);
		ApplyAnimation(killerid, "BOMBER", "BOM_Plant", 4.1, 0, 1, 1, 1, 0, 1);
		SendDeathMessage(killerid, ZombieData[killerid][zombie_victim], reason);
		ZombieData[killerid][zombie_victim] = INVALID_PLAYER_ID;
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(!IsPlayerNPC(playerid))
    {
		if(PRESSED(KEY_JUMP) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)//Shift
		{
		    if(VictimData[playerid][victim_status] == 0)
		    {
			    VictimData[playerid][victim_detect] = (ZOMBIE_DETECT * 2);
				VictimData[playerid][victim_status] = 1;
				VictimData[playerid][victim_timer] = SetTimerEx("ResetDetectRange", 25000, false, "i", playerid);
			}
			return 1;
		}

        if(HOLDING(KEY_SPRINT) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)//Space
		{
		    if(VictimData[playerid][victim_status] == 0)
		    {
			    VictimData[playerid][victim_detect] = (ZOMBIE_DETECT * 2);
				VictimData[playerid][victim_status] = 1;
				VictimData[playerid][victim_timer] = SetTimerEx("ResetDetectRange", 25000, false, "i", playerid);
			}
			return 1;
		}

		if(HOLDING(KEY_FIRE) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)//LMB
		{
			if(IsHoldingFirearm(playerid) == 1)
			{
			    if(GetPlayerWeapon(playerid) != 23)
			    {
					VictimData[playerid][victim_detect] = (ZOMBIE_DETECT * 4);
					if(VictimData[playerid][victim_status] == 1) { KillTimer(VictimData[playerid][victim_timer]); }
					VictimData[playerid][victim_status] = 1;
					VictimData[playerid][victim_timer] = SetTimerEx("ResetDetectRange", 25000, false, "i", playerid);
				}
			}
			return 1;
		}
	}
	return 1;
}

public OnRNPCDeath(npcid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID)
	{
		if(VictimData[killerid][victim_kill] != npcid)
		{
			VictimData[killerid][victim_kill] = npcid;
			SetTimerEx("ResetZombieKill", 4000, false, "i", killerid);
			SendDeathMessage(killerid, npcid, reason);
	    }
	}

	SetPlayerColor(npcid, 0xFFFFFF00);

    ApplyAnimation(npcid, "PED", "KO_shot_front", 4.1, 0, 0, 0, 1, 0);
    SetTimerEx("RemoveZombieBody", 10000, false, "i", npcid);
	return 1;
}

public ResetDetectRange(playerid)
{
    VictimData[playerid][victim_status] = 0;
    VictimData[playerid][victim_detect] = ZOMBIE_DETECT;
	return 1;
}

public ResetZombieKill(playerid)
{
    VictimData[playerid][victim_kill] = INVALID_PLAYER_ID;
	return 1;
}

public RemoveZombieBody(npcid)
{
	RespawnRNPC(npcid);
	return 1;
}

public OnZombieCreate()
{
	RNPC_SetUpdateRate(80);

    MapAndreas_Init(MAP_ANDREAS_MODE_NOBUFFER);

	new type = random(4), name[MAX_PLAYER_NAME], zombieid = ServerData[server_zombies];
	if(type == SPRINTER_ZOMBIE)
	{
		ZombieData[zombieid][zombie_species] = SPRINTER_ZOMBIE;
	}
	else
	{
		ZombieData[zombieid][zombie_species] = RUNNER_ZOMBIE;
	}

	ZombieData[zombieid][zombie_victim]= INVALID_PLAYER_ID;

	format(name, sizeof(name), "Zombie_%i", zombieid + 1);
	ConnectRNPC(name);

	ServerData[server_zombies]++;

	if(ServerData[server_zombies] >= MAX_ZOMBIES)
	{
		KillTimer(ServerData[server_zombietimer]);
		printf("Number of zombies created: %i", ServerData[server_zombies]);
	}
	return 1;
}

public OnZombieUpdate()
{
	for(new i = 0; i < MAX_ZOMBIES; i++)
	{
		if(IsPlayerConnected(i) || IsPlayerNPC(i))
		{
			if(GetRNPCHealth(i) > 0)
			{
				new victimid, Float:pos[3];
				victimid = GetZombieVictim(i);
				GetPlayerPos(i, pos[0], pos[1], pos[2]);

				if(victimid != INVALID_PLAYER_ID)//Attack
			  	{
					if(IsPlayerInRangeOfPoint(victimid, VictimData[victimid][victim_detect], pos[0], pos[1], pos[2]))
					{
						if(IsPlayerInRangeOfPoint(victimid, 1.0, pos[0], pos[1], pos[2]))
						{
							ZombieData[i][zombie_victim] = victimid;

							ZombieData[i][zombie_movement] = 0;
							ZombieData[i][zombie_roam] = 0;

							RNPC_CreateBuild(i, PLAYER_RECORDING_TYPE_ONFOOT);
							RNPC_AddPause(100);
							RNPC_SetKeys(KEY_FIRE);
							RNPC_AddPause(100);
							RNPC_SetKeys(0);
							RNPC_FinishBuild();
							RNPC_StartBuildPlayback(i);

							new Float:health;
							GetPlayerHealth(victimid, health);
							SetPlayerHealth(victimid, health - ZOMBIE_DAMAGE);

							GameTextForPlayer(victimid, "~r~You are being bitten!", 3000, 5);
							continue;
						}
						else
						{
							ZombieData[i][zombie_movement] = 0;
							ZombieData[i][zombie_roam] = 0;

							GetPlayerPos(victimid, pos[0], pos[1], pos[2]);

							if(ZombieData[i][zombie_species] == SPRINTER_ZOMBIE)
							{
								MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_SPRINT, 1);
							}
							else
							{
								MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_RUN, 1);
							}
							continue;
						}
					}
				}
				else if(victimid == INVALID_PLAYER_ID)//Roam
				{
				    if(ZombieData[i][zombie_roam] == 0)
				    {
						ZombieData[i][zombie_roam]++;
				        ZombieData[i][zombie_victim] = INVALID_PLAYER_ID;

						RNPC_SetKeys(0);

						new point = random(6);
						if(point == 0) { pos[0] = pos[0] + 100.0; }
						else if(point == 1) { pos[0] = pos[0] - 100.0; }
						else if(point == 2) { pos[1] = pos[1] + 100.0; }
						else if(point == 3) { pos[1] = pos[1] - 100.0; }

						else if(point == 4) { pos[0] = pos[0] + 100.0; pos[1] = pos[1] + 100.0; }
						else if(point == 5) { pos[0] = pos[0] - 100.0; pos[1] = pos[1] - 100.0; }

						MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_WALK, 1);
						continue;
				    }
				    else
				    {
						ZombieData[i][zombie_roam]++;

						if(ZombieData[i][zombie_roam] >= MAX_ROAM)
						{
							ZombieData[i][zombie_roam] = 0;
							ZombieData[i][zombie_victim] = INVALID_PLAYER_ID;

							RNPC_SetKeys(0);

							new point = random(6);
							if(point == 0) { pos[0] = pos[0] + 100.0; }
							else if(point == 1) { pos[0] = pos[0] - 100.0; }
							else if(point == 2) { pos[1] = pos[1] + 100.0; }
							else if(point == 3) { pos[1] = pos[1] - 100.0; }

							else if(point == 4) { pos[0] = pos[0] + 100.0; pos[1] = pos[1] + 100.0; }
							else if(point == 5) { pos[0] = pos[0] - 100.0; pos[1] = pos[1] - 100.0; }

							MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_WALK, 1);

			            	ZombieData[i][zombie_movement]++;

							if(ZombieData[i][zombie_movement] >= MAX_MOVEMENT && IsZombieStranded(i) == true)
							{
								ZombieData[i][zombie_movement] = 0;
								SetRNPCHealth(i, 0.0);
							}
							continue;
						}
					}
				}
			}
		}
	}
	return 1;
}

