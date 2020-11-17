/*
	Author: punkochel
	Version: 1.2	
*/

#if defined _privatearea_included
	#endinput
#endif

#define _privatearea_included

#if !defined _streamer_included
	#error privatearea.inc must be included after streamer.inc
#endif

#if !defined _FOREACH_INC_TEST
	#error privatearea.inc must be included after foreach.inc
#endif

#if !defined MAX_PRIVATE_AREA
	#define MAX_PRIVATE_AREA 50
#endif

#define PRIVATE_AREA_NONE 			-1
#define PRIVATE_AREA_FULL_LIMIT		-1

/*
	Collback's registration
*/
forward OnPlayerTriedEnterPrivateArea(playerid, areaid);


/*
	Variables
*/
static enum privateAreaInformation {

	AreaId,
	OutlineAreaId,
	bool:BannedPlayerId[MAX_PLAYERS]
};
static privateAreaInfo[MAX_PRIVATE_AREA][privateAreaInformation];
static Float:PlayerCoord_X[MAX_PLAYERS];
static Float:PlayerCoord_Y[MAX_PLAYERS];
static Float:PlayerCoord_Z[MAX_PLAYERS];


/*
	Function's
*/
stock CreatePrivateArea(Float:x, Float:y, Float:z, Float:size, worldid = -1, interiorid = -1, playerid = -1, priority = 0) {

	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][AreaId] != 0)
			continue;

		privateAreaInfo[i][AreaId] = CreateDynamicSphere(x, y, z, size, worldid, interiorid, playerid, priority);
		privateAreaInfo[i][OutlineAreaId] = CreateDynamicSphere(x, y, z, size+3.5, worldid, interiorid, playerid, priority);

		return privateAreaInfo[i][AreaId];
	}
	return PRIVATE_AREA_FULL_LIMIT;
}

stock DestroyPrivateArea(areaid) {

	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][AreaId] == areaid) {

			DestroyDynamicArea(areaid);
			DestroyDynamicArea(privateAreaInfo[i][OutlineAreaId]);

			// Clear data players
			for(i = 0; i < MAX_PLAYERS; i++) {

				privateAreaInfo[i][BannedPlayerId][i] = false;
				PlayerCoord_X[i] = 0.0; 
				PlayerCoord_Y[i] = 0.0; 
				PlayerCoord_Z[i] = 0.0;
			}
			return 1;
		}
	}
	return PRIVATE_AREA_NONE;
}

stock DenyEntryToPrivateArea(areaid, playerid = -1) {

	if(playerid == -1) {

		for(new i; i < MAX_PRIVATE_AREA; i++) {

			if(privateAreaInfo[i][AreaId] == areaid) {

				foreach(new j:Player) 
					privateAreaInfo[i][BannedPlayerId][j] = true;

				return 1;
			}
		}
		return PRIVATE_AREA_NONE;
	}

	// if(playerid < 0 || playerid > MAX_PLAYERS-1) return 0;
	if (!(0 <= playerid <= MAX_PLAYERS-1)) 
		return 0;

	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][AreaId] == areaid) {

			privateAreaInfo[i][BannedPlayerId][playerid] = true;
			return 1;
		}
	}
	return PRIVATE_AREA_NONE;
}

stock AllowEntryToPrivateArea(areaid, playerid = -1) {

	if(playerid == -1) {

		for(new i; i < MAX_PRIVATE_AREA; i++) {

			if(privateAreaInfo[i][AreaId] == areaid) {

				foreach(new j:Player) 
					privateAreaInfo[i][BannedPlayerId][j] = false;

				return 1;
			}
		}
		return PRIVATE_AREA_NONE;
	}

	if (!(0 <= playerid <= MAX_PLAYERS-1)) 
		return 0;

	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][AreaId] == areaid) {

			privateAreaInfo[i][BannedPlayerId][playerid] = false;
			return 1;
		}
	}
	return PRIVATE_AREA_NONE;
}

stock IsPlayerAddInPrivateArea(playerid, areaid) {

	if (!(0 <= playerid <= MAX_PLAYERS-1)) 
		return 0;
	
	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][AreaId] == areaid) {

			if(privateAreaInfo[i][BannedPlayerId][playerid] == true)
				return 1;
			else 
				return 0;
		}
	}
	return PRIVATE_AREA_NONE;
}

/*
	Interception's
*/
public OnPlayerEnterDynamicArea(playerid, areaid)
{
	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][OutlineAreaId] == areaid) {

			if(privateAreaInfo[i][BannedPlayerId][playerid] != true)
				break;

			if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) 
				GetVehiclePos(GetPlayerVehicleID(playerid), PlayerCoord_X[playerid], PlayerCoord_Y[playerid], PlayerCoord_Z[playerid]);
			else
				GetPlayerPos(playerid, PlayerCoord_X[playerid], PlayerCoord_Y[playerid], PlayerCoord_Z[playerid]);

			break;
		}
		else if(privateAreaInfo[i][AreaId] == areaid) {

			if(privateAreaInfo[i][BannedPlayerId][playerid] != true || PlayerCoord_X[playerid] == 0.0)
				break;

			if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) 
				SetVehiclePos(GetPlayerVehicleID(playerid), PlayerCoord_X[playerid], PlayerCoord_Y[playerid], PlayerCoord_Z[playerid]);
			else {

				SetPlayerPos(playerid, PlayerCoord_X[playerid], PlayerCoord_Y[playerid], PlayerCoord_Z[playerid]);
				SetCameraBehindPlayer(playerid);
			}

			CallLocalFunction(!"OnPlayerTriedEnterPrivateArea", !"dd", playerid, privateAreaInfo[i][AreaId]);
			break;
		}
	}

#if defined ap_OnPlayerEnterDynamicArea
    return ap_OnPlayerEnterDynamicArea(playerid, areaid);
#endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
    #undef    OnPlayerEnterDynamicArea
#else
    #define    _ALS_OnPlayerEnterDynamicArea
#endif
#define    OnPlayerEnterDynamicArea    ap_OnPlayerEnterDynamicArea
#if defined ap_OnPlayerEnterDynamicArea
forward ap_OnPlayerEnterDynamicArea(playerid);
#endif  


public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	for(new i; i < MAX_PRIVATE_AREA; i++) {

		if(privateAreaInfo[i][OutlineAreaId] == areaid) {

			if(privateAreaInfo[i][BannedPlayerId][playerid] != true || PlayerCoord_X[playerid] == 0.0)
				break;

			PlayerCoord_X[playerid] = 0.0; 
			PlayerCoord_Y[playerid] = 0.0; 
			PlayerCoord_Z[playerid] = 0.0;
			break;
		}
	}

#if defined ap_OnPlayerLeaveDynamicArea
    return ap_OnPlayerLeaveDynamicArea(playerid, areaid);
#endif
}
#if defined _ALS_OnPlayerLeaveDynamicArea
    #undef    OnPlayerLeaveDynamicArea
#else
    #define    _ALS_OnPlayerLeaveDynamicArea
#endif
#define    OnPlayerLeaveDynamicArea    ap_OnPlayerLeaveDynamicArea
#if defined ap_OnPlayerLeaveDynamicArea
forward ap_OnPlayerLeaveDynamicArea(playerid);
#endif
