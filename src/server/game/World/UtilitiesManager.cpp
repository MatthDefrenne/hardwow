#include "UtilitiesManager.h"
#include "Log.h"
#include "Chat.h"

Player* CanHelpPlayer(Player* player, Group* group) {

    Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

    bool canHelp = false;

    Group::MemberSlotList const& members = group->GetMemberSlots();

    for (Group::MemberSlotList::const_iterator itr = members.begin(); itr != members.end(); ++itr)
    {
        Group::MemberSlot const& slot = *itr;
        if (Player* member = ObjectAccessor::FindPlayer((*itr).guid)) {
            if ((leader->getLevel() - 7) >= member->getLevel()) { // We try to find a level lower than 7
                return member;
                break;
            }
        }
    }

    return nullptr;

}


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
            moreXP += 0.50;
        if (group->GetMembersCount() == 5)
            moreXP += 0.50;
    }


    return moreXP;
}

void UtilitiesManager::CompleteSpecialQuests(Player * player)
{
    Group* group = player->GetGroup();

    if (!group)
        return;

    Group::MemberSlotList const& members = group->GetMemberSlots();

    for (Group::MemberSlotList::const_iterator itr = members.begin(); itr != members.end(); ++itr)
    {
        Group::MemberSlot const& slot = *itr;
        if (Player* member = ObjectAccessor::FindPlayer((*itr).guid)) {
            if (group->IsLeader(member->GetGUID())) {
                if (Player* plr = CanHelpPlayer(member, group)) {
                    if (plr->IsAtGroupRewardDistance(member)) {
                        uint32 amount = urand(500, 1000);
                        member->ModifyMoney(member->getLevel() * amount);
                        std::string msg = "|cff00FF11Great! You have helped a lower level player than you. You get |cffF2B700" + std::to_string(amount) + "|r golds|r";
                        ChatHandler(player->GetSession()).SendSysMessage(msg.c_str());
                    }
                    else {
                        member->GetSession()->SendAreaTriggerMessage("|cffFF0000You are too far away from a low level player to get a reward.|r");
                    }
                }
            }
        }
    }
}
