#pragma once
#include "Game.h"

struct C_UTL_VECTOR
{
	DWORD64 Count = 0;
	DWORD64 Data = 0;
};

class PlayerController
{
public:
	DWORD64 Address = 0;
	DWORD Pawn = 0;
public:
	DWORD64 GetPlayerPawnAddress();
};

class PlayerPawn
{
public:

	DWORD64 Address = 0;
};

class CEntity
{
public:
	PlayerController Controller;
	PlayerPawn Pawn;
public:
	bool UpdateController(const DWORD64& PlayerControllerAddress);
	bool UpdatePawn(const DWORD64& PlayerPawnAddress);
};