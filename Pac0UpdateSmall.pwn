/*
	Plug'N'Play Zombie System by Weponz (2018)
*/

/*
	Small updrage for the WeaponZ system by Pac0 (2022)
*/
///////////////////////////////////
//Actualizado posiciones de cadaveres
//Rotaciónes y funcionamiento de cadaveres


///IMPORTANTE
/*
- RECUERDEN SACAR LAS CONDICIONALES DE EQUIPO
if(GetPlayerTeam(i) == 0 || GetPlayerTeam(i) == 3)
*/
///////////////////////////////////
#define FILTERSCRIPT
WeaponzPacAMX()
{
    new a[][] =
    {
        "Unarmed (Fist)",
        "Brass K"
    };
    #pragma unused a
}
#include <a_samp>//Credits: SA-MP
#include <rnpc>//Credits: Mauzen
#include <streamer>

#define HOLDING(%0) ((newkeys & (%0)) == (%0))
#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

//Configuration (YOU MAY EDIT THIS SECTION)
#define MAX_ZOMBIES 100//Maximum amount of zombies to spawn. (Default: 25)

//#define ZOMBIE_SKIN 162//Zombie skin id. (Default: 162)
#define ZOMBIE_COLOUR 0xFFFFFF00//Zombie colour. (Default: Red)
#define ZOMBIE_DAMAGE 8//Zombie damage. (Default: 20)
#define ZOMBIE_DETECT 40//Zombie detection range. (Default: 25)

///SKINS RANDOMS PARA LOS ZOMBIES
new SkinRand[]=
{
	78,79,132,134,135,136,137,159,160,
	162,168,196,200,209,212,213,218,230,
	239,264
};
////TUS DEFINICIONES DE TEAMS

new gTeam[MAX_PLAYERS];

#define TEAM_HUMANOS 0
#define TEAM_ZOMBIES 2
#define TEAM_LEGION 3
#define TEAM_CENTINELAS 4 

////TUS DEFINICIONES DE TEAMS
#define MAX_BODIES 500

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

forward RemoveDeadBody(npcid);

new Float:ZombieSpawns[][4] =
{
	//LS
	{793.1649, -1845.7291, 7.2300},
	{1057.3574, -1278.8195, 13.5498}, 
	{921.9029, -1104.4760, 23.7633},
	{1351.5192, -1283.8026, 12.3604}, 
	{1202.3400, -1564.2152, 12.5289}, 
	{2807.9282,-1176.8883,25.3805,173.6018}, // North in east beach - near apartments
	{2552.5417,-958.0850,82.6345,280.2542}, // Random house north of Las Colinas
	{2232.1309,-1159.5679,25.8906,103.2939}, // Jefferson motel
	{2388.1003,-1279.8933,25.1291,94.3321}, // House south of pig pen
	{2481.1885,-1536.7186,24.1467,273.4944}, // East LS - near clucking bell and car wash
	{2495.0720,-1687.5278,13.5150,359.6696}, // Outside CJ's house - lots of cars nearby
	{2306.8252,-1675.4340,13.9221,2.6271}, // House in ganton - lots of cars nearby
	{2191.8403,-1455.8251,25.5391,267.9925}, // House in south jefferson - lots of cars nearby
	{1830.1359,-1092.1849,23.8656,94.0113}, // Mulholland intersection carpark
	{2015.3630,-1717.2535,13.5547,93.3655}, // Idlewood house
	{1654.7091,-1656.8516,22.5156,177.9729}, // Right next to PD
	{1127.6726,-2037.4873,69.8059,271.5750}, // Conference Center
	{1508.6849,-1059.0846,25.0625,1.8058}, // Across the street of BANK - lots of cars in intersection carpark
	{1421.0819,-885.3383,50.6531,3.6516}, // Outside house in vinewood
	{1133.8237,-1272.1558,13.5469,192.4113}, // Near hospital
	{1235.2196,-1608.6111,13.5469,181.2655}, // Backalley west of mainstreet
	{590.4648,-1252.2269,18.2116,25.0473}, // Outside "BAnk of San Andreas"
	{842.5260,-1007.7679,28.4185,213.9953}, // North of Graveyard
	{911.9332,-1232.6490,16.9766,5.2999}, // LS Film Studio
	{477.6021,-1496.6207,20.4345,266.9252}, // Rodeo Place
	{255.4621,-1366.3256,53.1094,312.0852}, // Outside propery in richman
	{281.5446,-1261.4562,73.9319,305.0017}, // Another richman property
	{790.1918,-839.8533,60.6328,191.9514}, // Mulholland house
	{1299.1859,-801.4249,84.1406,269.5274}, // Maddoggs
	{1240.3170,-2036.6886,59.9575,276.4659}, // Verdant Bluffs
	{2215.5181,-2627.8174,13.5469,273.7786}, // Ocean docks 1
	{2509.4346,-2637.6543,13.6453,358.3565}, // Ocean Docks spawn 2*/
{1146.9098,-1266.9692,13.7205}, //
{1193.1113,-1203.9023,19.5846}, //

{1270.3960,-1285.1410,13.3533}, //
{1287.1229,-1278.0302,13.8038}, //
{1358.5311,-1320.9355,13.3906}, //

{1591.5355,-1669.3927,5.8823}, //
{1609.5145,-1679.3682,6.2188}, //
{1590.9706,-1591.6372,13.3828}, //
{1643.0918,-1539.6516,13.5712}, //
{1655.2252,-1489.3582,13.3828}, //
{1644.3336,-1454.9043,13.5469}, //
{1654.5082,-1399.7567,13.5469}, //
{1607.9657,-1380.4758,14.6297}, //
{1597.5093,-1369.1653,15.6684}, //
{1515.9596,-1385.1187,14.0409}, //
{1511.8297,-1339.0912,13.9172}, //
{1537.3629,-1262.7216,17.4090}, //
{1520.7893,-1212.5328,21.0859}, //
{1522.5737,-1166.1783,24.0781}, //
{1569.8494,-1162.8485,24.4268}, //
{1550.5402,-1163.4701,24.4411}, //
{1639.5319,-1065.1542,23.8984}, //
{1620.8530,-1028.6147,23.8984}, //
{1669.4928,-997.0919,24.0573}, //
{1741.0894,-1100.1422,24.0781}, //
{1784.2571,-1117.8373,24.0859},
{1796.1573,-1155.8900,28.8516}, //
{1717.7388,-1177.5123,23.6573}, //
{1688.2422,-1256.3334,14.8066}, //


//{2706.3762,-2065.3464,13.0582},


//{2718.7896,-2098.5056,11.1306},
//{2733.0708,-2053.0667,13.0502},
{2779.4109,-2048.0142,11.8562},
{2787.7007,-2073.0686,12.7935},
{2818.5752,-2085.6533,10.9297},
{2784.8674,-2021.0653,13.5620},

{2640.0459,-1750.7137,10.7215},
{2641.1604,-1724.4955,10.7335},
{2606.1897,-1728.5183,12.3855},
{2578.0232,-1737.4646,13.5469},
{2546.9651,-1724.2981,13.5469},
{2526.6406,-1743.4406,13.3828},
{2510.5513,-1683.8292,13.5300},
{2490.1316,-1683.4152,13.3364},
{2479.0588,-1676.5599,13.3368},
{2490.8188,-1667.8350,13.3438},
{2499.0247,-1656.0688,13.3966},
{2480.1423,-1652.5204,13.4688},

//{2378.6787,-1646.1216,13.5328},
{2387.8022,-1684.2761,15.4874},
{2394.6218,-1673.5491,14.7596},
{2365.2834,-1646.4233,13.5469},
{2358.3843,-1643.7465,14.9611},
{2347.8286,-1639.6830,14.8296},
{2340.8667,-1602.0516,17.3535},
{2346.6670,-1586.4834,21.5012},
{2341.4275,-1559.2070,23.8516},
{2349.6055,-1479.9714,24.0000},
{2330.0190,-1439.7424,24.0000},
{2347.6602,-1403.7103,23.9903} //

 //






};


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

enum body_data
{
	body_id,
	body_model,
	Float:body_x,
	Float:body_y,
	Float:body_z,
	Float:body_r,
	Float:body_dist,
	Float:body_angle,
	body_world,
	body_timer,
	bool:body_active
}

new DeadBodyData[MAX_BODIES][body_data];

stock GetZombieVictim(npcid)
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if(IsPlayerConnected(i) && !IsPlayerNPC(i) && IsPlayerSpawned(i))
	    {
			if(GetPlayerTeam(i) == 0 || GetPlayerTeam(i) == 3)
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
	}
	return INVALID_PLAYER_ID;
}

stock GetClosestVictim(npcid)
{
    new Float:dist = 1000.0, target = INVALID_PLAYER_ID, Float:pos[3], Float:pos2[3], Float:tmpdis;
    GetPlayerPos(npcid, pos[0], pos[1], pos[2]);
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
		if(GetPlayerTeam(i) == 0 || GetPlayerTeam(i) == 3)
		{
        if(!IsPlayerConnected(i) || IsPlayerNPC(i) || i == npcid || !IsPlayerSpawned(i)) continue;
        GetPlayerPos(i, pos2[0], pos2[1], pos2[2]);
        tmpdis = floatsqroot(floatpower(floatabs(floatsub(pos2[0], pos[0])), 2) + floatpower(floatabs(floatsub(pos2[1], pos[1])), 2) + floatpower(floatabs(floatsub(pos2[2], pos[2])), 2));
        if(tmpdis < dist && pos2[2] <= (pos[2] + 4.0) && GetPlayerState(i) == PLAYER_STATE_ONFOOT)
        {
            dist = tmpdis;
            target = i;
        }
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
		if(GetPlayerTeam(i) == 0 || GetPlayerTeam(i) == 3)
		{
	    if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
	    if(IsPlayerInRangeOfPoint(i, 300.0, pos[0], pos[1], pos[2]))
	    {
	    	return false;
	    }
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
	WeaponzPacAMX();
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

			new point = random(6), spawn = random(sizeof(ZombieSpawns));
		 if(point == 0) { SetPlayerPos(playerid, ZombieSpawns[spawn][0], ZombieSpawns[spawn][1], ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 1) { SetPlayerPos(playerid, ZombieSpawns[spawn][0] + 250.0, ZombieSpawns[spawn][1], ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 2) { SetPlayerPos(playerid, ZombieSpawns[spawn][0], ZombieSpawns[spawn][1] + 250.0, ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 3) { SetPlayerPos(playerid, ZombieSpawns[spawn][0] - 250.0, ZombieSpawns[spawn][1], ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 4) { SetPlayerPos(playerid, ZombieSpawns[spawn][0], ZombieSpawns[spawn][1] + 250.0, ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 5) { SetPlayerPos(playerid, ZombieSpawns[spawn][0] -250.0, ZombieSpawns[spawn][1], ZombieSpawns[spawn][2] + 500.00); }
		 else if(point == 6) { SetPlayerPos(playerid, ZombieSpawns[spawn][0], ZombieSpawns[spawn][1] + 250.0, ZombieSpawns[spawn][2] + 500.00); }
	//	 else if(pos == 7) { SetPlayerPos(zombied, ZombieSpawns[spawn][0] + 250.0, ZombieSpawns[spawn][1] + 250.0, ZombieSpawns[spawn][2] + 500.00); }
	//	 else if(pos == 8) { SetPlayerPos(zombied, ZombieSpawns[spawn][0] - 250.0, ZombieSpawns[spawn][1] - 250.0, ZombieSpawns[spawn][2] + 500.00); }
		}

		SetRNPCHealth(playerid, health);
		SetPlayerSkin(playerid, SkinRand[random(sizeof(SkinRand))]);
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
		//SendDeathMessage(killerid, ZombieData[killerid][zombie_victim], reason);
		ZombieData[killerid][zombie_victim] = INVALID_PLAYER_ID;
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(!IsPlayerNPC(playerid))
    {
		if(GetPlayerTeam(playerid) == 0 || GetPlayerTeam(playerid) == 4)
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
	}
	return 1;
}

stock GetBodySlot()
{
	for(new i = 0; i < MAX_BODIES; i++)
	{
		if(DeadBodyData[i][body_active] == false) return i;
	}
	return -1;
}

public OnRNPCDeath(npcid, killerid, reason)
{
	
	new skinzombie;
	skinzombie = GetPlayerSkin(npcid);
    if(killerid != INVALID_PLAYER_ID)
	{
		if(VictimData[killerid][victim_kill] != npcid)
		{
			VictimData[killerid][victim_kill] = npcid;
			SetTimerEx("ResetZombieKill", 4000, false, "i", killerid);
			SendDeathMessage(killerid, npcid, reason);
	    }
	}
	new slot = GetBodySlot(), Float:pos[4];
	new Float:Angle;
	GetPlayerPos(npcid, pos[0],pos[1],pos[2],pos[3]);
	GetPlayerFacingAngle(npcid,Angle);
	if(slot == -1) return 1;
	DeadBodyData[slot][body_model] = skinzombie;
	DeadBodyData[slot][body_x] = pos[0];
	DeadBodyData[slot][body_y] = pos[1];
	DeadBodyData[slot][body_z] = pos[2];
	DeadBodyData[slot][body_r] = pos[3];
	DeadBodyData[slot][body_angle] = Angle;
	DeadBodyData[slot][body_dist] = 250.0;
	DeadBodyData[slot][body_world] = GetPlayerVirtualWorld(killerid);
	DeadBodyData[slot][body_active] = true;


    
	DeadBodyData[slot][body_id] = CreateDynamicActor(DeadBodyData[slot][body_model],DeadBodyData[slot][body_x],DeadBodyData[slot][body_y],DeadBodyData[slot][body_z],DeadBodyData[slot][body_r]);

	SetDynamicActorPos(DeadBodyData[slot][body_id],DeadBodyData[slot][body_x],DeadBodyData[slot][body_y],DeadBodyData[slot][body_z],DeadBodyData[slot][body_y],DeadBodyData[slot][body_r]);
	SetDynamicActorFacingAngle(DeadBodyData[slot][body_id],DeadBodyData[slot][body_angle]);
	SetDynamicActorVirtualWorld(DeadBodyData[slot][body_id],DeadBodyData[slot][body_world]);
	ApplyDynamicActorAnimation(DeadBodyData[slot][body_id],"PED","FLOOR_hit_f",4.1,0,1,1,1,0);

	DeadBodyData[slot][body_timer] = SetTimerEx("RemoveDeadBody",70000,false,"i",slot);
	

   // ApplyAnimation(npcid, "PED", "KO_shot_front", 4.1, 0, 0, 0, 1, 0);
   // SetTimerEx("RemoveZombieBody", 10000, false, "i", npcid);
   RespawnRNPC(npcid);
	

	SetPlayerColor(npcid, 0xFFFFFF00);
	return 1;
}
/*public OnPlayerCommandText(playerid,cmdtxt[])
{
	if (strcmp("/unstuck12345",cmdtext,true, 10) == 0)
	{
		for(new i = 0; i < MAX_BODIES; i++)
		{
			if(DeadBodyData[i][body_active] = true;)
			{
				IsPlayerInRangeOfPoint(playerid,2.0,DeadBodyData[i][body_x],DeadBodyData[i][body_y],DeadBodyData[i][body_z])
				{
				DestroyActor(DeadBodyData[i][body_id]);
				DeadBodyData[i][body_id] == -1;
				DeadBodyData[i][body_active] = false;
				}
			}
		}
		return 1;
	}
	return 0;
}*/
public RemoveDeadBody(npcid)
{
	DestroyDynamicActor(DeadBodyData[npcid][body_id]);
	DeadBodyData[npcid][body_id] == -1;
	DeadBodyData[npcid][body_active] = false;
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

	format(name, sizeof(name), "Infectado[%i]", zombieid + 1);
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
						if(IsPlayerInRangeOfPoint(victimid, 4.0, pos[0], pos[1], pos[2])) //estaba en 1
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

							ApplyAnimation(i, "CHOPPA", "CHOPPA_fwd", 4.0, 0, 0, 0, 0, 0, 1);    //animacion de mordida
							
							ApplyAnimation(victimid, "ped", "DAM_armL_frmFT", 2.6, 0, 1, 1, 0, 0, 1);    //animacion de mordido

							GameTextForPlayer(victimid, "_~n~_~n~_~n~_~n~_~n~_~n~_~n~_~n~_~r~te estan mordiendo", 3000, 5);
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
						//	MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_RUN, 1);
							}
							else
							{
							MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_RUN, 1);
							//MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_SPRINT, 1);
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
						if(point == 0) { MoveRNPC(i, pos[0] + 50.0, pos[1], pos[2], RNPC_SPEED_WALK, 1); }
						else if(point == 1) { MoveRNPC(i, pos[0], pos[1] + 50.0, pos[2], RNPC_SPEED_WALK, 1); }
						else if(point == 2) { MoveRNPC(i, pos[0] - 50.0, pos[1], pos[2], RNPC_SPEED_WALK, 1); }
						else if(point == 3) { MoveRNPC(i, pos[0], pos[1] - 50.0, pos[2], RNPC_SPEED_WALK, 1); }

						else if(point == 4) { MoveRNPC(i, pos[0] + 50.0, pos[1] + 50.0, pos[2], RNPC_SPEED_WALK, 1); }
						else if(point == 5) { MoveRNPC(i, pos[0] - 50.0, pos[1] - 50.0, pos[2], RNPC_SPEED_WALK, 1); }

					//	MoveRNPC(i, pos[0], pos[1], pos[2], RNPC_SPEED_WALK, 1);
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
							if(point == 0) { MoveRNPC(i, pos[0] + 50.0, pos[1], pos[2], RNPC_SPEED_WALK, 1); }
							else if(point == 1) { MoveRNPC(i, pos[0], pos[1] + 50.0, pos[2], RNPC_SPEED_WALK, 1); }
							else if(point == 2) { MoveRNPC(i, pos[0] - 50.0, pos[1], pos[2], RNPC_SPEED_WALK, 1); }
							else if(point == 3) { MoveRNPC(i, pos[0], pos[1] - 50.0, pos[2], RNPC_SPEED_WALK, 1); }

							else if(point == 4) { MoveRNPC(i, pos[0] + 50.0, pos[1] + 50.0, pos[2], RNPC_SPEED_WALK, 1); }
							else if(point == 5) { MoveRNPC(i, pos[0] - 50.0, pos[1] - 50.0, pos[2], RNPC_SPEED_WALK, 1); }

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
