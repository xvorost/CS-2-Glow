# CS-2-Glow Wallhack
Cheat for CS2 (Glow) - alternative for OneByteWallhack

### Discord-server:
[xvorost's project](https://discord.gg/xagvBetQ7S)

### How to complie
Use Visual Studio 2022, **be sure to add to the [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) project**. Select "Release" and compile. Compiled files goes in root x64 directory.

#### Glow method
```cpp
ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, 86400.f);
```

#### Preview
![image](https://github.com/xvorost/CS-2-Glow/assets/66530241/14fb5d7d-aa4a-420a-8253-397bcda7b58b)

![image](https://github.com/xvorost/CS-2-Glow/assets/66530241/4e4e78dd-e3fd-40de-a0ac-1772309b85b9)

#### Important Note

Please be aware that the method used for glowing entities in this cheat is detectable by VAC (Valve Anti-Cheat) and is not recommended. It's advisable to develop your own kernel driver for memory reading to avoid detection and potential consequences. Please also note that any sort of writing to memory is detectable or very hard to make undetected.
For guidance on creating a more secure approach, consider exploring alternative methods and implementing your own solutions to enhance security and reduce the risk of detection.
