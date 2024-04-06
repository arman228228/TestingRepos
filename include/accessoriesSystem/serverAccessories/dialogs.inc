    switch(dialogid) {
        case D_ACCESSORY_MAIN_MENU: {
            if(!response) {
                return 1;
            }

            if(!listitem) {
                ShowAccessoriesSecondMenu(playerid);
            }
            else {
                ShowPlayerIDInput(playerid);
            }
            return 1;
        }
        case D_ACCESSORY_MENU_SERVER: {
            if(!response) {
                return callcmd::menu(playerid);
            }

            if(!listitem) {
                playerAccessoriesMenuPage[playerid] = 1;
                ShowAccessoriesList(playerid);
            }
            else {
                CreateAccessoryModel(playerid);
            }
            return 1;
        }
        case D_EDIT_ACCESSORY: {
            if(!response) {
                playerAccessoriesMenuPage[playerid] = 0;
                
                return callcmd::menu(playerid);
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

                if(isPlayerAcessoryEditor{playerid}) {
                    new 
                        targetid = playerTargetID[playerid],
                        iterFreeID = Iter_Free(playerAccessories[targetid]);

                    if(!(0 <= iterFreeID <= MAX_PLAYER_ACCESSORIES - 1))  {
                        ShowAccessoriesMainMenuForPl(playerid);
                        return SendClientMessage(playerid, -1, "Достигнут максимальный лимит аксессуаров у игрока.");
                    }

                    foreach(new i : playerAccessories[targetid]) { 
                        if(i != playerAccessoryIndex[playerid])
                            continue;

                        ShowAccessoriesMainMenuForPl(playerid);
                        return SendClientMessage(playerid, -1, "У игрока уже есть данный аксессуар.");
                    }

                    Iter_Add(playerAccessories[targetid], playerAccessoryIndex[playerid]);

                    SavePlayerAccessoryData(targetid, playerAccessoryIndex[playerid]);

                    SendClientMessage(playerid, -1, "Аксессуар выдан");

                    ShowAccessoriesMainMenuForPl(playerid);
                }
                else {
                    ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
                }
            }
            return 1;
        }
        case D_CLICKED_ACCESSORY_MENU: {
            if(!response) {
                playerAccessoriesMenuPage[playerid] = 0;
                
                return callcmd::menu(playerid);
            }

            new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            switch(listitem) {
                case 0: {
                    SetAccessoryToPlayer(playerid, ACCESSORY:accessoryIndex);
                }
                case 1: {
                    return ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
                }
                case 2: {
                    DeleteAccessoryItem(ACCESSORY:accessoryIndex);
                }
                case 3: {
                    DeletePlayerAccessoryItem(playerid, ACCESSORY:accessoryIndex);
                }
            }
            playerAccessoriesMenuPage[playerid] = 1;
            
            if(isPlayerAcessoryEditor{playerid}) {
                ShowPlayerAccessoriesList(playerid, playerTargetID[playerid]);
            }
            else {
                ShowAccessoriesList(playerid);
            }

            return 1;
        }
        case D_ACCESSORY_CREATE_MODEL: {
            if(!response) {
                return callcmd::menu(playerid);
            }

            new inputValue = strval(inputtext);

            if(!(321 <= inputValue <= 19998)) {
                return CreateAccessoryModel(playerid);
            }

            new
                iterFreeID = Iter_Free(CreatedAccessories);

            if(!(0 <= iterFreeID <= MAX_ACCESSORIES - 1))  {
                ShowAccessoriesSecondMenu(playerid);
                return SendClientMessage(playerid, -1, "Достигнут максимальный общий лимит аксессуаров.");
            }

            SetAccessoryDefaultValues(iterFreeID);

            Iter_Add(CreatedAccessories, iterFreeID);

            AccessoriesInfo[iterFreeID][aModelID] = inputValue;
            SaveAccessoryData(iterFreeID);

            return ShowAccessoryMainEditMenu(playerid, iterFreeID);
        }
        case D_CLICKED_ACCESSORY_EDIT_MENU: {
            if(!response) {
                playerAccessoriesMenuPage[playerid] = 0;
                
                return callcmd::menu(playerid);
            }

            new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            switch(listitem) {
                case 0: EditAccessoryIndex(playerid);
                case 1: EditAccessoryModel(playerid);
                case 2: EditAccessoryBone(playerid);
                case 3: EditAccessoryPos(playerid, POS_OFFSET_X);
                case 4: EditAccessoryPos(playerid, POS_OFFSET_Y);
                case 5: EditAccessoryPos(playerid, POS_OFFSET_Z);
                case 6: EditAccessoryPos(playerid, ROT_OFFSET_X);
                case 7: EditAccessoryPos(playerid, ROT_OFFSET_Y);
                case 8: EditAccessoryPos(playerid, ROT_OFFSET_Z);
                case 9: EditAccessoryPos(playerid, SCALE_X);
                case 10: EditAccessoryPos(playerid, SCALE_Y);
                case 11: EditAccessoryPos(playerid, SCALE_Z);
                case 12: EditAccessoryColorRed(playerid, COLOR_0);
                case 13: EditAccessoryColorRed(playerid, COLOR_1);
                case 14: EditAccessoryAttachment(playerid, ACCESSORY:accessoryIndex);
                case 15: DeleteAccessoryItem(ACCESSORY:accessoryIndex);
            }
        }
        case D_ACCESSORY_EDIT_INDEX: {
            if(!response) {
                return ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
            }

            new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            AccessoriesInfo[ACCESSORY:accessoryIndex][aIndex] = listitem;
            SaveAccessoryIndex(listitem, ACCESSORY:accessoryIndex);

            return ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
        }
        case D_ACCESSORY_EDIT_BONE: {
            if(!response) {
                return ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
            }

            new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            AccessoriesInfo[ACCESSORY:accessoryIndex][aBone] = listitem;
            SaveAccessoryBone(listitem, ACCESSORY:accessoryIndex);

            return ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
        }
        case D_ACCESSORY_EDIT_MODEL: {
            if(!response) {
                return ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
            }

            new inputValue = strval(inputtext);

            if(!(321 <= inputValue <= 19998)) {
                return EditAccessoryModel(playerid);
            }

            new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            AccessoriesInfo[ACCESSORY:accessoryIndex][aModelID] = inputValue;
            SaveAccessoryModelID(inputValue, ACCESSORY:accessoryIndex);

            return ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
        }
        case D_ACCESSORY_EDIT_POS: {
            if(!response) {
                return ShowClickedAccessoryMenu(playerid, D_CLICKED_ACCESSORY_MENU);
            }

            new Float:value = floatstr(inputtext), 
                ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            switch(playerEditCurrentAxis[playerid]) {
                case POS_OFFSET_X: AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetX] = value;
                case POS_OFFSET_Y: AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetY] = value;
                case POS_OFFSET_Z: AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetZ] = value;
                case ROT_OFFSET_X: AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotX] = value;
                case ROT_OFFSET_Y: AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotY] = value;
                case ROT_OFFSET_Z: AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotZ] = value;
                case SCALE_X: AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleX] = value;
                case SCALE_Y: AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleY] = value;
                case SCALE_Z: AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleZ] = value;
            }
            
            SaveAccessorySpecificPosition(playerEditCurrentAxis[playerid], value, ACCESSORY:accessoryIndex);

            return ShowAccessoryMainEditMenu(playerid, ACCESSORY:ACCESSORY:accessoryIndex);
        }
        case D_ACCESSORY_EDIT_COLOR_RED: {
			if(!response) {
                new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];
                return ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
            }
			
            if(!(0 <= inputtext[0] <= 255)) {
                return EditAccessoryColorRed(playerid, playerEditCurrentColor[playerid]);
            }

            playerAccessoryColors[playerid][0] = inputtext[0];

            EditAccessoryColorGreen(playerid, playerEditCurrentColor[playerid]);
		}
		case D_ACCESSORY_EDIT_COLOR_GREEN: {
			if(!response) {
                return EditAccessoryColorRed(playerid, playerEditCurrentColor[playerid]);
            }
			
            if(!(0 <= inputtext[0] <= 255)) {
                return EditAccessoryColorGreen(playerid, playerEditCurrentColor[playerid]);
            }

            playerAccessoryColors[playerid][1] = inputtext[0];

            EditAccessoryColorBlue(playerid, playerEditCurrentColor[playerid]);
		}
        case D_ACCESSORY_EDIT_COLOR_BLUE: {
			if(!response) {
                return EditAccessoryColorGreen(playerid, playerEditCurrentColor[playerid]);
            }
			
            if(!(0 <= inputtext[0] <= 255)) {
                return EditAccessoryColorBlue(playerid, playerEditCurrentColor[playerid]);
            }

            playerAccessoryColors[playerid][2] = inputtext[0];

            EditAccessoryColorAlpha(playerid, playerEditCurrentColor[playerid]);
		}
		case D_ACCESSORY_EDIT_COLOR_ALPHA: {
			if(!response) {
                return EditAccessoryColorGreen(playerid, playerEditCurrentColor[playerid]);
            }

            if(!(0 <= inputtext[0] <= 255)) {
                return EditAccessoryColorBlue(playerid, playerEditCurrentColor[playerid]);
            }
            
            playerAccessoryColors[playerid][3] = inputtext[0];

            new color = RGB(playerAccessoryColors[playerid][0], playerAccessoryColors[playerid][1], playerAccessoryColors[playerid][2], playerAccessoryColors[playerid][3]),
                ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

            if(!playerEditCurrentColor[playerid]) {
                AccessoriesInfo[ACCESSORY:accessoryIndex][aMaterialColor1] = color;
            }
            else {
                AccessoriesInfo[ACCESSORY:accessoryIndex][aMaterialColor2] = color;
            }
            
            SaveAccessoryColor(playerEditCurrentColor[playerid], color, ACCESSORY:accessoryIndex);

            ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);
        }
    }