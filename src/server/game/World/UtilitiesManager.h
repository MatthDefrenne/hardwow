#pragma once
#include "Player\Player.h"
#include "Group.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "Map.h"
#include "Player.h"
#include "DatabaseEnv.h"

class UtilitiesManager {
public:
    static float CalculateMoreChanceInGroup(Player* player);
    static float CalculateMoreRateXPInGroup(Player* player, uint32 quest_id);
};
