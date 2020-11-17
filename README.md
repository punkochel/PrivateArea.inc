# PrivateArea.inc
 This library allows you to create private zones on the server in SA:MP
 
 How it works: When a player tries to enter a private area, he is thrown back.

Limitations:
To work with this library, you need to have streamer and foreach.
You can only create zones with the Sphere type (sphere).

Constants:
MAX_PRIVATE_AREA - The maximum number of privacy zones created (default 50);
PRIVATE_AREA_NONE - The private zone does not exist;
PRIVATE_AREA_FULL_LIMIT - The limit for creating private zones has been exceeded.

Functions:
public OnPlayerTriedEnterPrivateArea (playerid, areaid);
CreatePrivateArea (Float: x, Float: y, Float: z, Float: size, worldid = -1, interiorid = -1, playerid = -1, priority = 0);
DestroyPrivateArea (areaid);
DenyEntryToPrivateArea (areaid, playerid = -1);
AllowEntryToPrivateArea (areaid, playerid = -1);
IsPlayerAddInPrivateArea (playerid, areaid);


Description of functions:
OnPlayerTriedEnterPrivateArea - An autocallable function that is called when a player tries to enter a private area.

Options:
playerid - player ID;
areaid - dynamic area ID.

CreatePrivateArea - Creates a private area on the server.

Returned values:
Dynamic zone ID if successful;
PRIVATE_AREA_FULL_LIMIT - if the limit for creating private zones has been exceeded.

Options:
x, y, z - zone coordinates;
size - zone size;
worldid - virtual world (default -1);
interiorid - interior (default -1);
playerid - specify the player's Id if you want to create a zone only for him (by default -1);
priority - set the priority of the zone loading (by default 0).

DestroyPrivateArea - Destroys a private area from the server.

Returned values:
1 - if the private zone was successfully deleted;
PRIVATE_AREA_NONE - if the private zone does not exist.

Options:
areaid - dynamic zone ID;

DenyEntryToPrivateArea - Prevents the player from entering the private area.

Return values:
1 - if successful;
0 - if the player's ID is less than 0 or more than MAX_PLAYERS-1;
PRIVATE_AREA_NONE - if the private zone does not exist.

Options:
areaid - dynamic zone ID;
playerid - ID of the player who needs to deny entry (by default -1 (denies entry to all)).

AllowEntryToPrivateArea - Allows the player to enter the private area.

Returned values:
1 - if successful;
0 - if the player's ID is less than 0 or more than MAX_PLAYERS-1;
PRIVATE_AREA_NONE - if the private zone does not exist.

Options:
areaid - dynamic zone ID;
playerid - the ID of the player who needs to be allowed to enter (by default -1 (prohibits everyone from entering)).

IsPlayerAddInPrivateArea - Checks if the player has been added to the private area.

Returned values:
1 - if the player is added to the private zone;
0 - if the player's ID is less than 0 or more than MAX_PLAYERS-1, or the player is not added to the private zone;
PRIVATE_AREA_NONE - if the private zone does not exist.

Options:
playerid - player ID;
areaid - dynamic area ID.
