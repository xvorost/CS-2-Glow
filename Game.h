#pragma once
#include <optional>
#include "ProcessManager.hpp"
#include "Offsets.h"

class CGame
{
private:
	struct
	{
		DWORD64 ClientDLL;
		DWORD64 EntityList;
		DWORD64 Matrix;
		DWORD64 ViewAngle;
		DWORD64 EntityListEntry;
		DWORD64 LocalController;
		DWORD64 LocalPawn;
		DWORD64 ForceJump;
	}Address;

public:

	bool InitAddress();

	DWORD64 GetClientDLLAddress();

	DWORD64 GetEntityListAddress();

	DWORD64 GetEntityListEntry();

	DWORD64 GetLocalControllerAddress();

	DWORD64 GetLocalPawnAddress();

	bool UpdateEntityListEntry();
};

inline CGame gGame;