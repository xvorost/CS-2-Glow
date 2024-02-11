# CS-2-Glow Wallhack
Cheat for CS2 (Glow) - alternative for OneByteWallhack

## Features
- Glow
- Radar Hack
- Trigger Bot

### Discord-server:
[xvorost's project](https://discord.gg/xagvBetQ7S)

### How to complie
Use Visual Studio 2022, **be sure to add to the [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) project**. Select "Release" and compile. Compiled files goes in root x64 directory.

#### Glow method
```cpp
ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, 86400.f);
```

#### Preview
![image](https://github.com/xvorost/CS-2-Glow/assets/66530241/b4c080a2-25a4-457b-9ee1-b5d5480af3a4)

![image](https://github.com/xvorost/CS-2-Glow/assets/66530241/adcf33cf-53e6-48ab-b708-3f412b795ebb)

![image](https://github.com/xvorost/CS-2-Glow/assets/66530241/4e4e78dd-e3fd-40de-a0ac-1772309b85b9)
