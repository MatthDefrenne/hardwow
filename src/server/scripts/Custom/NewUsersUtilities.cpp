/*
* Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "Channel.h"
#include "Group.h"
#include "Guild.h"
#include "Log.h"
#include "Player.h"
#include "WorldSession.h"
#include "World.h"
#include "Language.h"
#include "ScriptPCH.h"
#include "Language.h"
#include "World.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "Log.h"
#include "SpellMgr.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "DBCStores.h"
#include "DatabaseEnv.h"
#include "Group.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "Log.h"

class AnnouncerNewPlayer : public PlayerScript
{
public:
    AnnouncerNewPlayer() : PlayerScript("AnnouncerNewPlayer") { }

    void OnLogin(Player* player, bool firstLogin) {

        std::vector<std::string> WorldAnnonce = { "A new player has just arrived, Welcome ", "A new hero joins us, Welcome ", "Oh, someone new just arrived. Welcome ", "Welcome to the adventure, "};

        if (firstLogin) {
            sWorld->SendServerMessage(SERVER_MSG_STRING, (WorldAnnonce[urand(0, 3)] + "|cff54b5ff" + player->GetSession()->GetPlayerName()).c_str());
        }
    }
};

class CreatureKill : public PlayerScript
{
public:
    CreatureKill() : PlayerScript("CreatureKill") { }

    void OnCreatureKill(Player* killer, Creature* killed) {


    }
};



class TeleporterForNewPlayerScript : public CreatureScript
{
public:
    TeleporterForNewPlayerScript() : CreatureScript("TeleporterForNewPlayerScript") { }

    static bool OnGossipHello(Player * player, Creature * creature)
    {
        if (player->GetTeam() == ALLIANCE) {
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Human start zone", GOSSIP_SENDER_MAIN, 1);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Drawf/Gnome start zone", GOSSIP_SENDER_MAIN, 3);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Night Elf start zone", GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Draenei start zone", GOSSIP_SENDER_MAIN, 11);
        }
        else {
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Orc/Troll start zone", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Tauren start zone", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Blood Elf start zone", GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, GOSSIP_ACTION_TAXI, "Teleport to Undead start zone", GOSSIP_SENDER_MAIN, 5);
        }
       
        SendGossipMenuFor(player, 50050, creature->GetGUID());
        return true;
    }

    static bool OnGossipSelect(Player* player, Creature* me, uint32 /*uiSender*/, uint32 uiActio)
    {

        QueryResult result = WorldDatabase.PQuery("SELECT map, position_x, position_y, position_z, orientation FROM playercreateinfo WHERE race = %u LIMIT 1", uiActio);

        Field* field = result->Fetch();
        uint32 map = field[0].GetUInt32();
        float x = field[1].GetFloat();
        float y = field[2].GetFloat();
        float z = field[3].GetFloat();
        float o = field[4].GetFloat();

        player->TeleportTo(map, x, y, z, o);

        CloseGossipMenuFor(player);
        return true;
    }

    struct MyAI : public ScriptedAI
    {
        MyAI(Creature* creature) : ScriptedAI(creature) { }
        bool GossipHello(Player* player) override
        {
            return OnGossipHello(player, me);
        }
        bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
        {
            uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            return OnGossipSelect(player, me, sender, action);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new MyAI(creature);
    }
};


void AddSC_NewUserUtilities()
{
    new AnnouncerNewPlayer();
    new TeleporterForNewPlayerScript();
}
