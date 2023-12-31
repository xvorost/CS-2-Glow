#pragma once
#include <Windows.h>
#include "ProcessManager.hpp"

namespace Offset
{
	inline DWORD EntityList;
	inline DWORD LocalPlayerController;
	inline DWORD LocalPlayerPawn;

	// https://github.com/a2x/cs2-dumper/blob/main/generated/client.dll.hpp
	struct
	{
		DWORD PlayerPawn = 0x60C; // CBasePlayerController -> m_hPawn 
		DWORD EnemySensor = 0x13E4; // m_flDetectedByEnemySensorTime
	} Entity;

	// https://github.com/a2x/cs2-dumper/blob/main/config.json
	namespace Signatures
	{
		const std::string EntityList = "48 8B 0D ?? ?? ?? ?? 48 89 7C 24 ?? 8B FA C1 EB";
		const std::string LocalPlayerController = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 4F";
		const std::string LocalPlayerPawn = "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 83 EC ?? 8B 0D";
	}

	bool UpdateOffsets();
}
