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

class AnnouncerNewPlayer : public PlayerScript
{
public:
    AnnouncerNewPlayer() : PlayerScript("AnnouncerNewPlayer") { }

    void OnLogin(Player* player, bool firstLogin) {

        std::vector<std::string> WorldAnnonce = { "A new player has just arrived, Welcome ", "A new hero joins us, Welcome ", "Oh, someone new just arrived. Welcome ", "Welcome to the adventure, "};

        if(firstLogin)
            sWorld->SendServerMessage(SERVER_MSG_STRING, (WorldAnnonce[urand(0, 3)] + "|cff54b5ff" + player->GetSession()->GetPlayerName()).c_str());



    }

};

void AddSC_NewUserUtilities()
{
    new AnnouncerNewPlayer();
}
