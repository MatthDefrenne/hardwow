#include "UtilitiesManager.h"


bool HasRewardQuestOrHasQuest(Player* player, uint32 quest_id) {

    if (player->GetQuestStatus(quest_id) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(quest_id) == QUEST_STATUS_INCOMPLETE)
        return true;

    return false;
}

float UtilitiesManager::CalculateMoreChanceInGroup(Player * player)
{
    Group* group = player->GetGroup();

   
    if (!group)
        return 1.0f;

    if (group->isRaidGroup()) // can't be in raid
        return 1.0f;

    if (player->GetMap()->IsDungeon()) // can't be in dungeon
        return 1.0f;

    float memberCount = 1.0f;


    if (group->GetMembersCount() >= 2)
        memberCount += 1.25;
    if (group->GetMembersCount() == 5)
        memberCount += 1.25;

    return memberCount;
}


float UtilitiesManager::CalculateMoreRateXPInGroup(Player* player, uint32 quest_id) {


    float moreXP = 1.0f;

    Group* group = player->GetGroup();

    if (!group)
        return 1.0f;


    bool ShouldGetExp = true;

    Group::MemberSlotList const& members = group->GetMemberSlots();

    for (Group::MemberSlotList::const_iterator itr = members.begin(); itr != members.end(); ++itr)
    {
        Group::MemberSlot const& slot = *itr;
        if (Player* member = ObjectAccessor::FindPlayer((*itr).guid)) {
            if (!HasRewardQuestOrHasQuest(player, quest_id))
                ShouldGetExp = false;
        }
    }

    if (ShouldGetExp) {
        if (group->GetMembersCount() >= 2)
            moreXP += 0.25;
        if (group->GetMembersCount() == 5)
            moreXP += 0.25;
    }


    return moreXP;
}
