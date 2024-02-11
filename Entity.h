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
	int TeamID = 0;
	int Health = 0;
	DWORD Pawn = 0;
public:
	bool GetTeamID();
	bool GetHealth();
	DWORD64 GetPlayerPawnAddress();
};

class PlayerPawn
{
public:
	DWORD64 Address = 0;
	std::string WeaponName;
	int Health;
	int TeamID;

public:
	bool GetWeaponName();
	bool GetHealth();
	bool GetTeamID();
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
