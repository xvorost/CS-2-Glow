void Glow() {
    while (true) {
        // Update EntityList Entry
        gGame.UpdateEntityListEntry();

        DWORD64 LocalControllerAddress = 0;
        DWORD64 LocalPawnAddress = 0;

        while (LocalControllerAddress == 0) {
            ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress);
            if (LocalControllerAddress == 0) Sleep(100);
        }
        ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress);

        // LocalEntity
        CEntity LocalEntity;
        static int LocalPlayerControllerIndex = 1;
        LocalEntity.UpdateController(LocalControllerAddress);
        LocalEntity.UpdatePawn(LocalPawnAddress);

        for (int i = 0; i < 64; i++)
        {
            CEntity Entity;
            DWORD64 EntityAddress = 0;
            ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (i + 1) * 0x78, EntityAddress);
            if (EntityAddress == LocalEntity.Controller.Address)
            {
                LocalPlayerControllerIndex = i;
            }
            Entity.UpdateController(EntityAddress);
            Entity.UpdatePawn(Entity.Pawn.Address);

            if (!EnableGlow)
            {
                ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, off);
            }
            else {
                ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, on);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
