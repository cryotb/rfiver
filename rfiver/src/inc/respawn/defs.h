#pragma once

#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define ENTITY_INDEX_BY_HANDLE(X) ( X & 0xFFFF )
#define FORWARD_DECLARE_CLASS(X) class X

#define MAX_SPLITSCREEN_CLIENT_BITS 2 
#define MAX_SPLITSCREEN_CLIENTS	( 1 << MAX_SPLITSCREEN_CLIENT_BITS ) 
