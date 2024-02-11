#include "Entity.h"

template <typename T>
bool GetDataAddressWithOffset(const DWORD64& Address, DWORD Offset, T& Data)
{
	if (Address == 0)
		return false;

	if (!ProcessMgr.ReadMemory<T>(Address + Offset, Data))
		return false;

	return true;
}

bool CEntity::UpdateController(const DWORD64& PlayerControllerAddress)
{
	if (PlayerControllerAddress == 0)
		return false;
	this->Controller.Address = PlayerControllerAddress;

	if (!this->Controller.GetHealth())
		return false;
	if (!this->Controller.GetTeamID())
		return false;

	this->Pawn.Address = this->Controller.GetPlayerPawnAddress();

	return true;
}

bool CEntity::UpdatePawn(const DWORD64& PlayerPawnAddress)
{
	if (PlayerPawnAddress == 0)
		return false;
	this->Pawn.Address = PlayerPawnAddress;

	if (!this->Pawn.GetWeaponName())
		return false;
	if (!this->Pawn.GetHealth())
		return false;
	if (!this->Pawn.GetTeamID())
		return false;

	return true;
}

bool PlayerController::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Entity.TeamID, this->TeamID);
}

bool PlayerController::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Entity.Health, this->Health);
}

bool PlayerPawn::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Pawn.iTeamNum, this->TeamID);
}

bool PlayerPawn::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Pawn.CurrentHealth, this->Health);
}

bool PlayerPawn::GetWeaponName()
{
	DWORD64 WeaponNameAddress = 0;
	char Buffer[40]{};

	WeaponNameAddress = ProcessMgr.TraceAddress(this->Address + Offset::Pawn.pClippingWeapon, { 0x10,0x20 ,0x0 });
	if (WeaponNameAddress == 0)
		return false;

	if (!ProcessMgr.ReadMemory(WeaponNameAddress, Buffer, 40))
		return false;

	WeaponName = std::string(Buffer);
	std::size_t Index = WeaponName.find("_");
	if (Index == std::string::npos || WeaponName.empty())
	{
		WeaponName = "Weapon_None";
	}
	else
	{
		WeaponName = WeaponName.substr(Index + 1, WeaponName.size() - Index - 1);
	}

	if (WeaponName == "c4")
		WeaponName = "c4 bomb";
	else if (WeaponName == "ak47")
		WeaponName = "ak-47";
	else if (WeaponName == "aug")
		WeaponName = "aug";
	else if (WeaponName == "awp")
		WeaponName = "awp";
	else if (WeaponName == "deagle")
		WeaponName = "desert eagle";
	else if (WeaponName == "glock")
		WeaponName = "glock";
	else if (WeaponName == "m4a1")
		WeaponName = "m4a1";
	else if (WeaponName == "m4a1_silencer")
		WeaponName = "m4a1-s";
	else if (WeaponName == "mac10")
		WeaponName = "mac-10";
	else if (WeaponName == "mag7")
		WeaponName = "mag-7";
	else if (WeaponName == "mp7")
		WeaponName = "mp-7";
	else if (WeaponName == "mp9")
		WeaponName = "mp-9";
	else if (WeaponName == "negev")
		WeaponName = "negev";
	else if (WeaponName == "nova")
		WeaponName = "nova";
	else if (WeaponName == "p90")
		WeaponName = "p90";
	else if (WeaponName == "p250")
		WeaponName = "p250";
	else if (WeaponName == "revolver")
		WeaponName = "revolver";
	else if (WeaponName == "sawedoff")
		WeaponName = "sawed-off";
	else if (WeaponName == "scar20")
		WeaponName = "scar-20";
	else if (WeaponName == "sg556")
		WeaponName = "sg-556";
	else if (WeaponName == "ssg08")
		WeaponName = "ssg-08";
	else if (WeaponName == "tec9")
		WeaponName = "tec-9";
	else if (WeaponName == "taser")
		WeaponName = "zeus-taser";
	else if (WeaponName == "xm1014")
		WeaponName = "xm-1014";
	else if (WeaponName == "ump45")
		WeaponName = "ump-45";
	else if (WeaponName == "mp5sd")
		WeaponName = "mp5-sd";
	else if (WeaponName == "cz75a")
		WeaponName = "pistol-cz75a";
	else if (WeaponName == "hkp2000")
		WeaponName = "usp-s";
	else if (WeaponName == "elite")
		WeaponName = "dual berettas";
	else if (WeaponName == "decoy")
		WeaponName = "decoy";
	else if (WeaponName == "smokegrenade")
		WeaponName = "smoke";
	else if (WeaponName == "flashbang")
		WeaponName = "flashbang";
	else if (WeaponName == "hegrenade")
		WeaponName = "grenade";
	else if (WeaponName == "incgrenade")
		WeaponName = "molotov";
	else if (WeaponName == "molotov")
		WeaponName = "molotov";
	else if (WeaponName == "knife")
		WeaponName = "knife";
	else if (WeaponName == "g3sg1")
		WeaponName = "g3sg1";
	else if (WeaponName == "galilar")
		WeaponName = "galil-ar";
	else if (WeaponName == "m249")
		WeaponName = "m249";
	else if (WeaponName == "famas")
		WeaponName = "famas";
	else
		WeaponName = "undefined";

	return true;
}

DWORD64 PlayerController::GetPlayerPawnAddress()
{
	DWORD64 EntityPawnListEntry = 0;
	DWORD64 EntityPawnAddress = 0;

	if (!GetDataAddressWithOffset<DWORD>(Address, Offset::Entity.PlayerPawn, this->Pawn))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x78 * (Pawn & 0x1FF), EntityPawnAddress))
		return 0;

	return EntityPawnAddress;
}
