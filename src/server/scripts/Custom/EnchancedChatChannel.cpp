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

class EnchancedChatChannelScript: public PlayerScript
{
public:
    EnchancedChatChannelScript() : PlayerScript("EnchancedChatChannelScript") { }


    std::string getClassName(Player* player) {

    
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
                return "Warrior";
                break;
            case CLASS_HUNTER:
                return "Hunter";
                break;
            case CLASS_ROGUE:
                return "Rogue";
                break;
            case CLASS_DEATH_KNIGHT:
                return "DK";
                break;
            case CLASS_MAGE:
                return "Mage";
                break;
            case CLASS_WARLOCK:
                return "Warlock";
                break;
            case CLASS_PALADIN:
                return "Warrior";
                break;
            case CLASS_PRIEST:
                return "Warrior";
                break;
            case CLASS_SHAMAN:
                return "Shaman";
                break;
            case CLASS_DRUID:
                return "Druid";
                break;
            }
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild) override
    {
        msg = "[" + getClassName(player) + " level " + std::to_string(player->getLevel()) + "] " + msg;
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel) override
    {
        msg = "[" + getClassName(player) + " level " + std::to_string(player->getLevel()) + "] " + msg;
    }
};

void AddSC_EnchancedChatChannelScript()
{
    new EnchancedChatChannelScript();
}
