    switch(dialogid) {
        case D_PLAYER_ACCESSORY_MAIN_MENU: {
            if(!response) {
                return 1;
            }

            if(!listitem) {
                playerAccessoriesMenuPage[playerid] = 1;
                ShowPlayerAccessoriesList(playerid, playerTargetID[playerid]);
            }
            else {
                SendClientMessage(playerid, -1, "Выберите аксессуар для выдачи.");

                isPlayerAcessoryEditor{playerid} = true;
                playerAccessoriesMenuPage[playerid] = 1;
                ShowAccessoriesList(playerid);
            }
            return 1;
        }
        case D_EDIT_PLAYER_ACCESSORY: {
            if(!response) {
                return ShowAccessoriesMainMenuForPl(playerid);
            }

            //#define NEXT_PAGE_TEXT_FOR_DIALOG     "Следующая страница >"
            //#define PREVIOUS_PAGE_TEXT_FOR_DIALOG "< Предыдущая страница"

            if(!strcmp(inputtext, NEXT_PAGE_TEXT_FOR_DIALOG)) {
               playerAccessoriesMenuPage[playerid]++;
            }
            else if(!strcmp(inputtext, PREVIOUS_PAGE_TEXT_FOR_DIALOG)) {
                playerAccessoriesMenuPage[playerid]--;
            }
            else
            {
                playerAccessoryIndex[playerid] = playerAccessoryDialogData[playerid][listitem];
                isPlayerAcessoryEditor{playerid} = true;

                ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
            }
            return 1;
        }
        case D_INPUT_PLAYER_ID: {
            if(!response) {
                return callcmd::menu(playerid);
            }

            playerTargetID[playerid] = strval(inputtext);

			if(!IsPlayerConnected(playerTargetID[playerid])) {
                SendClientMessage(playerid, -1, "Игрок не в сети.");
                return ShowPlayerIDInput(playerid);
            }

            ShowAccessoriesMainMenuForPl(playerid);
        }
    }