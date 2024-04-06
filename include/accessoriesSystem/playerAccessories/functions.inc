stock ShowAccessoriesMainMenuForPl(playerid) {
    return ShowPlayerDialog(playerid, D_PLAYER_ACCESSORY_MAIN_MENU, DIALOG_STYLE_LIST, playerName[playerid], "\
        Список предметов игрока\n\
        Выдать предмет игроку", ">", "x"
    );
}

stock ShowPlayerIDInput(playerid) {
    return ShowPlayerDialog(playerid, D_INPUT_PLAYER_ID, DIALOG_STYLE_INPUT, "ИД Игрока", "Введите ID игрока", ">", "x");
}

stock ShowPlayerAccessoriesList(playerid, targetid) {
	static fmtDialogString[1500] = EOS;

    fmtDialogString = "{ffffff}Предмет\n";
    
    new pagesCount = playerAccessoriesMenuPage[playerid],
        isNextPage = 0,
        listitemIndex = 0, 
        fmtString[3 + 9 + 7];

    foreach(new i : playerAccessories[targetid]) {
        if (i >= (pagesCount * MAX_ITEMS_ON_PAGE) && i < (pagesCount * MAX_ITEMS_ON_PAGE) + MAX_ITEMS_ON_PAGE) {
			isNextPage++;
        }

		if (i >= (pagesCount - 1) * MAX_ITEMS_ON_PAGE && i < ((pagesCount - 1) * MAX_ITEMS_ON_PAGE) + MAX_ITEMS_ON_PAGE) {
			format(fmtString, sizeof(fmtString), "%d. ModelID: %d\n", 
				listitemIndex + 1, AccessoriesInfo[i][aModelID]
			);
			strcat(fmtDialogString, fmtString, sizeof(fmtDialogString));

            playerAccessoryDialogData[playerid][listitemIndex++] = i;
		}
    }
	
    if (isNextPage) {
        strcat(fmtDialogString, NEXT_PAGE_TEXT_FOR_DIALOG);
        strcat(fmtDialogString, "\n");
    }

    if (pagesCount > 1) {
        strcat(fmtDialogString, PREVIOUS_PAGE_TEXT_FOR_DIALOG);
    }
	
	return ShowPlayerDialog(playerid, D_EDIT_PLAYER_ACCESSORY, DIALOG_STYLE_TABLIST_HEADERS, "Список", fmtDialogString, "Далее", "Отмена");
}

stock DeletePlayerAccessoryData(playerid, autoID) {
    new 
        fmtQuery[104 + 7 + 7];

    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        SELECT `autoID` FROM `playersAccessoriesInformation` WHERE `playerID` = '%d' AND `accessoryIndex` = '%d'", 
        playerUserID[playerid], autoID
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery, "UpdateplayerAccessoriesData", "ddd", playerid, autoID, true);

    #if defined _accessory_debug_mode
        printf("[DeletePlayerAccessoryData debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

forward UpdateplayerAccessoriesData(playerid, autoID, bool: isDeleteCondition);
public UpdateplayerAccessoriesData(playerid, autoID, bool: isDeleteCondition) {
    new 
        rows, fmtQuery[95 + 7 + 7];

    cache_get_row_count(rows);

    if(!rows) {
        mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof fmtQuery, "\
            INSERT INTO `playersAccessoriesInformation` (playerID, accessoryIndex) VALUES ('%d', '%d')",
            playerUserID[playerid], autoID
        );
        mysql_tquery(MySQL:MySQLHandle, fmtQuery);

        SaveAccessoryModelID(AccessoriesInfo[autoID][aModelID], autoID);
    }
    else if(rows && isDeleteCondition) {
        mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof fmtQuery, "\
            DELETE FROM `playersAccessoriesInformation` WHERE `playerID` = '%d' AND `accessoryIndex` = '%d'",
            playerUserID[playerid], autoID
        );
        mysql_tquery(MySQL:MySQLHandle, fmtQuery);
    }

    #if defined _accessory_debug_mode
        printf("[UpdateplayerAccessoriesData debug]: rows %d | SQL: %s", 
            rows, fmtQuery
        );
    #endif

    return true;
}

stock DeletePlayerAccessoryItem(playerid, ACCESSORY:accessoryIndex) {
    new 
        next;

    Iter_SafeRemove(playerAccessories[playerid], ACCESSORY:accessoryIndex, next);

    DeletePlayerAccessoryData(playerid, ACCESSORY:accessoryIndex);

    #if defined _accessory_debug_mode
        printf("[DeletePlayerAccessoryItem debug]: playerid: %d, accessoryIndex: %d, playerAccessories: %d", 
            playerid, ACCESSORY:accessoryIndex, Iter_Count(playerAccessories[playerid])
        );
    #endif

    return true;
}

stock SavePlayerAccessoryData(playerid, autoID) {
    new 
        fmtQuery[78 + 7];

    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        SELECT `playerID` FROM `playersAccessoriesInformation` WHERE `playerID` = '%d'", 
        autoID
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery, "UpdateplayerAccessoriesData", "ddd", playerid, autoID, false);

    #if defined _accessory_debug_mode
        printf("[SavePlayerAccessoryData debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

forward LoadPlayerAccessoriesData(playerid);
public LoadPlayerAccessoriesData(playerid)
{
    new rows;
	cache_get_row_count(rows);
	printf("[LoadPlayerAccessoriesData]: debug 1: rows %d", rows);

	if (!rows) {
        return false;
    }

    new 
        accessoryIndex;

    for (new i = 0; i < rows && i < MAX_PLAYER_ACCESSORIES; i++) {
        cache_get_value_int(i, "accessoryIndex", accessoryIndex);

        Iter_Add(playerAccessories[playerid], accessoryIndex);
    }
    
    printf("[LoadPlayerAccessoriesData]: Загружено %d аксессуаров у %s.", rows, playerName[playerid]);

    return true;
}

#if defined OnPlayerDisconnect
    public OnPlayerDisconnect(playerid, reason)
    {
        if(Iter_Count(playerAccessories[playerid])) {
            Iter_Clear(playerAccessories[playerid]);
        }

        #if defined AC_OnPlayerDisconnect
            return AC_OnPlayerDisconnect(playerid, reason);
        #else
            return 0;
        #endif
    }

    #if defined _ALS_OnPlayerDisconnect
        #undef OnPlayerDisconnect
    #else
        #define _ALS_OnPlayerDisconnect
    #endif

    #define OnPlayerDisconnect AC_OnPlayerDisconnect

    #if defined AC_OnPlayerDisconnect
        forward AC_OnPlayerDisconnect(playerid, reason);
    #endif