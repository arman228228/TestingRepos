forward LoadAccessoriesData();
public LoadAccessoriesData()
{
    new rows;
	cache_get_row_count(rows);
	printf("[LoadAccessoriesData]: debug 1: rows %d", rows);

	if (!rows) {
        return false;
    }

    for (new i = 0; i < rows && i < MAX_ACCESSORIES; i++) {
        cache_get_value_int(i, "aIndex", AccessoriesInfo[i][aIndex]);
        cache_get_value_int(i, "aModelid", AccessoriesInfo[i][aModelID]);
        cache_get_value_int(i, "aBone", AccessoriesInfo[i][aBone]);
        cache_get_value_float(i, "aFOffsetX", AccessoriesInfo[i][aFOffsetX]);
        cache_get_value_float(i, "aFOffsetY", AccessoriesInfo[i][aFOffsetY]);
        cache_get_value_float(i, "aFOffsetZ", AccessoriesInfo[i][aFOffsetZ]);
        cache_get_value_float(i, "aFRotX", AccessoriesInfo[i][aFRotX]);
        cache_get_value_float(i, "aFRotY", AccessoriesInfo[i][aFRotY]);
        cache_get_value_float(i, "aFRotZ", AccessoriesInfo[i][aFRotZ]);
        cache_get_value_float(i, "aFScaleX", AccessoriesInfo[i][aFScaleX]);
        cache_get_value_float(i, "aFScaleY", AccessoriesInfo[i][aFScaleY]);
        cache_get_value_float(i, "aFScaleZ", AccessoriesInfo[i][aFScaleZ]);
        cache_get_value_int(i, "aMaterialcolor1", AccessoriesInfo[i][aMaterialColor1]);
        cache_get_value_int(i, "aMaterialcolor2", AccessoriesInfo[i][aMaterialColor2]);

        Iter_Add(CreatedAccessories, i);
    }
    
    printf("[LoadAccessoriesData]: Загружено %d аксессуаров.", rows);

    return true;
}

stock ShowAccessoriesMainMenu(playerid) {
    //return ShowPlayerDialog(playerid, D_ACCESSORY_MAIN_MENU, DIALOG_STYLE_LIST, "Меню", "Меню предметов\nМеню предметов конкретного игрока", ">", "x");
    return ShowPlayerDialog(playerid, D_ACCESSORY_MAIN_MENU, DIALOG_STYLE_LIST, "Меню", "\
        Меню предметов\n\
        Меню предметов конкретного игрока", ">", "x"
    );
}

stock ShowAccessoriesSecondMenu(playerid) {
    return ShowPlayerDialog(playerid, D_ACCESSORY_MENU_SERVER, DIALOG_STYLE_LIST, "Меню", "\
        1. Список созданных предметов\n\
        2. Создать предмет", ">", "x"
    );
}

stock ShowAccessoriesList(playerid) {
	static fmtDialogString[1500] = EOS;

    fmtDialogString = "{ffffff}Предмет\n";
    
    new pagesCount = playerAccessoriesMenuPage[playerid],
        isNextPage = 0,
        listitemIndex = 0, 
        fmtString[3 + 9 + 7];

    foreach(new i : CreatedAccessories) {
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
	
	return ShowPlayerDialog(playerid, D_EDIT_ACCESSORY, DIALOG_STYLE_TABLIST_HEADERS, "Список", fmtDialogString, "Далее", "Отмена");
}

stock ShowClickedAccessoryMenu(playerid, dialogid) {
    static
		fmtDialogString[] = "\
        1. Посмотреть аксессуар\n\
        2. Редактировать акссессуар\n\
        3. Удалить аксессуар"
    ;

    printf("[ShowClickedAccessoryMenu debug]: %d", isPlayerAcessoryEditor{playerid});

    if(isPlayerAcessoryEditor{playerid}) {
        strcat(fmtDialogString, "\n4. Удалить аксессуар у игрока");
    }

    return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Меню", fmtDialogString, ">", "x");
}

stock ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex) {
	static fmtDialogString[512] = EOS;

	format(fmtDialogString, sizeof(fmtDialogString), "\
        Индекс: #%d\n\
        Модель: #%d\n\
        Кость: #%s\n\
        Смещение позиции X: #%.6f\n\
        Смещение позиции Y: #%.6f\n\
        Смещение позиции Z: #%.6f\n\
        Смещение поворота X: #%.6f\n\
        Смещение поворота Y: #%.6f\n\
        Смещение поворота Z: #%.6f\n\
        Масштаб X: #%.6f\n\
        Масштаб Y: #%.6f\n\
        Масштаб Z: #%.6f\n\
        Цвет 1\n\
        Цвет 2\n\
        Редактировать\n\
        Удалить",
		ACCESSORY:accessoryIndex,

		AccessoriesInfo[ACCESSORY:accessoryIndex][aModelID], 
        AttachBones[ AccessoriesInfo[ACCESSORY:accessoryIndex][aBone] ], 
                
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetX], 
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetZ],

        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotX],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotZ],

        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleX],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleZ]
    );

	return ShowPlayerDialog(playerid, D_CLICKED_ACCESSORY_EDIT_MENU, DIALOG_STYLE_LIST, "Меню аксессуара", fmtDialogString, ">", "x");
}

stock EditAccessoryIndex(playerid) {
	static fmtDialogString[512] = EOS;

	for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
		format(fmtDialogString, sizeof(fmtDialogString), "%sСлот %d - %s\n", 
            fmtDialogString, i, AttachBones[i]
        );
	}

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_INDEX, DIALOG_STYLE_LIST, "Индекс аксессуара", fmtDialogString, ">", "x");
}

stock EditAccessoryModel(playerid) {
	return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_MODEL, DIALOG_STYLE_INPUT, "Модель аксессуара", "\
        Введите модель от 321 до 19998", ">", "x"
    );
}

stock EditAccessoryBone(playerid) {
	static fmtDialogString[512] = EOS;

    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
		format(fmtDialogString, sizeof(fmtDialogString), "%s%s\n", 
            fmtDialogString, AttachBones[i]
        );
	}

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_BONE, DIALOG_STYLE_LIST, "Кость аксессуара", fmtDialogString, ">", "x");
}

stock EditAccessoryPos(playerid, pos) {
	playerEditCurrentAxis[playerid] = pos;

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_POS, DIALOG_STYLE_INPUT, "Позиция аксессуара", "\
        Введите значение с плавающей точкой для смещения", ">", "x"
    );
}

stock EditAccessoryColorRed(playerid, color) {
    playerEditCurrentColor[playerid] = color;

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_COLOR_RED, DIALOG_STYLE_INPUT, "Цвет", "\
        Введите количество красного цвета в формате RGBA От 0 до 255", ">", "x"
    );
}

stock EditAccessoryColorGreen(playerid, color) {
    playerEditCurrentColor[playerid] = color;

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_COLOR_GREEN, DIALOG_STYLE_INPUT, "Цвет", "\
        Введите количество зелёного цвета в формате RGBA От 0 до 255", ">", "x"
    );
}

stock EditAccessoryColorBlue(playerid, color) {
    playerEditCurrentColor[playerid] = color;

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_COLOR_BLUE, DIALOG_STYLE_INPUT, "Цвет", "\
        Введите количество синего цвета в формате RGBA От 0 до 255", ">", "x"
    );
}

stock EditAccessoryColorAlpha(playerid, color) {
    playerEditCurrentColor[playerid] = color;

    return ShowPlayerDialog(playerid, D_ACCESSORY_EDIT_COLOR_ALPHA, DIALOG_STYLE_INPUT, "Прозрачность", "\
        Введите количество прозрачности в формате RGBA От 0 до 255", ">", "x"
    );
}

stock CreateAccessoryModel(playerid) {
	return ShowPlayerDialog(playerid, D_ACCESSORY_CREATE_MODEL, DIALOG_STYLE_INPUT, "Модель аксессуара", "\
        Введите модель от 321 до 19998", ">", "x"
    );
}

stock SetAccessoryToPlayer(playerid, ACCESSORY:accessoryIndex) {
    SetPlayerAttachedObject(
        playerid, AccessoriesInfo[ACCESSORY:accessoryIndex][aIndex], AccessoriesInfo[ACCESSORY:accessoryIndex][aModelID], AccessoriesInfo[ACCESSORY:accessoryIndex][aBone], 
        
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetX], 
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetZ],

        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotX],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotZ],

        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleX],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleY],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleZ],

        AccessoriesInfo[ACCESSORY:accessoryIndex][aMaterialColor1],
        AccessoriesInfo[ACCESSORY:accessoryIndex][aMaterialColor2]
    );

    #if defined _accessory_debug_mode
        printf("[SetAccessoryToPlayer debug]: aModelID: %d", AccessoriesInfo[ACCESSORY:accessoryIndex][aModelID]);
    #endif

    return true;
}

stock EditAccessoryAttachment(playerid, ACCESSORY:accessoryIndex) {
	if(SetAccessoryToPlayer(playerid, ACCESSORY:accessoryIndex)) {
        EditAttachedObject(playerid, AccessoriesInfo[ACCESSORY:accessoryIndex][aIndex]);
    }

    return true;
}

stock SetAccessoryDefaultValues(ACCESSORY:accessoryIndex) {
    for(new AccessoriesInfoEnum: i; i < AccessoriesInfoEnum; i ++) {
        if(!(aFScaleX <= i <= aFScaleZ) || i == aBone)
            continue;

        AccessoriesInfo[ACCESSORY:accessoryIndex][i] = 0;
    }

    AccessoriesInfo[ACCESSORY:accessoryIndex][aBone] = 1;

    AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleX] = AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleY] = AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleZ] = 0.121;
}

stock SaveAccessoryIndex(index, ACCESSORY:accessoryIndex) {
    new fmtQuery[74 + 2 + 7];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `aIndex` = '%d' WHERE `autoID` = '%d'",
        index, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryIndex]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessoryBone(bone, ACCESSORY:accessoryIndex) {
    new fmtQuery[71 + 2 + 7];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `aBone` = '%d' WHERE `autoID` = '%d'",
        bone, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryBone]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessoryModelID(modelID, ACCESSORY:accessoryIndex) {
    new fmtQuery[74 + 6 + 7];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `aModelid` = '%d' WHERE `autoID` = '%d'",
        modelID, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryModelID]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessoryPositions(Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ, ACCESSORY:accessoryIndex) {
    new fmtQuery[400];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `aFOffsetX` = '%f', `aFOffsetY` = '%f', `aFOffsetZ` = '%f', `aFRotX` = '%f', `aFRotY` = '%f', \
        `aFRotZ` = '%f', `aFScaleX` = '%f', `aFScaleY` = '%f', `aFScaleZ` = '%f' WHERE `autoID` = '%d'",
        fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryPositions debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessorySpecificPosition(axisID, Float: axisValue, ACCESSORY:accessoryIndex) {
    new fmtQuery[69 + 9 + 6 + 7];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `%s` = '%f' WHERE `autoID` = '%d'",
        AxisPoses[axisID], axisValue, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessorySpecificPosition debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessoryColor(colorID, color, ACCESSORY:accessoryIndex) {
    new fmtQuery[69 + 15 + 6 + 7];
    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        UPDATE `accessoriesInformation` SET `%s` = '%d' WHERE `autoID` = '%d'",
        !colorID ? "aMaterialcolor1" : "aMaterialcolor2", color, ACCESSORY:accessoryIndex
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryColor]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock SaveAccessoryData(autoID) {
    new 
        fmtQuery[67 + 7];

    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        SELECT `autoID` FROM `accessoriesInformation` WHERE `autoID` = '%d'", 
        autoID
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery, "UpdateAccessoryData", "dd", autoID, false);

    #if defined _accessory_debug_mode
        printf("[SaveAccessoryData debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

forward UpdateAccessoryData(autoID, bool: isDeleteCondition);
public UpdateAccessoryData(autoID, bool: isDeleteCondition) {
    new 
        rows, fmtQuery[59 + 7];

    cache_get_row_count(rows);

    if(!rows) {
        mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof fmtQuery, "\
            INSERT INTO `accessoriesInformation` (autoID) VALUES ('%d')",
            autoID
        );
        mysql_tquery(MySQL:MySQLHandle, fmtQuery);

        SaveAccessoryModelID(AccessoriesInfo[autoID][aModelID], autoID);
    }
    else if(rows && isDeleteCondition) {
        mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof fmtQuery, "\
            DELETE FROM `accessoriesInformation` WHERE `autoID` = '%d'",
            autoID
        );
        mysql_tquery(MySQL:MySQLHandle, fmtQuery);
    }

    #if defined _accessory_debug_mode
        printf("[UpdateAccessoryData debug]: rows %d | SQL: %s", 
            rows, fmtQuery
        );
    #endif

    return true;
}

stock DeleteAccessoryData(autoID) {
    new 
        fmtQuery[67 + 7];

    mysql_format(MySQL:MySQLHandle, fmtQuery, sizeof(fmtQuery), "\
        SELECT `autoID` FROM `accessoriesInformation` WHERE `autoID` = '%d'", 
        autoID
    );
    mysql_tquery(MySQL:MySQLHandle, fmtQuery, "UpdateAccessoryData", "dd", autoID, true);

    #if defined _accessory_debug_mode
        printf("[DeleteAccessoryData debug]: %s", 
            fmtQuery
        );
    #endif

    return true;
}

stock DeleteAccessoryItem(ACCESSORY:accessoryIndex) {
    new 
        next;

    Iter_SafeRemove(CreatedAccessories, ACCESSORY:accessoryIndex, next);

    DeleteAccessoryData(ACCESSORY:accessoryIndex);

    #if defined _accessory_debug_mode
        printf("[DeleteAccessoryItem debug]: accessoryIndex: %d, CreatedAccessories: %d", 
            ACCESSORY:accessoryIndex, Iter_Count(CreatedAccessories)
        );
    #endif

    return true;
}

#if defined OnPlayerEditAttachedObject
	public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ) {
		new ACCESSORY:accessoryIndex = playerAccessoryDialogData[playerid][ playerAccessoryIndex[playerid] ];

        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetX] = fOffsetX;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetY] = fOffsetY;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFOffsetZ] = fOffsetZ;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotX] = fRotX;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotY] = fRotY;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFRotZ] = fRotZ;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleX] = fScaleX;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleY] = fScaleY;
        AccessoriesInfo[ACCESSORY:accessoryIndex][aFScaleZ] = fScaleZ;

        SaveAccessoryPositions(fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, ACCESSORY:accessoryIndex);

        ShowAccessoryMainEditMenu(playerid, ACCESSORY:accessoryIndex);

        RemovePlayerAttachedObject(playerid, AccessoriesInfo[ACCESSORY:accessoryIndex][aIndex]);

		#if defined ac_OnPlayerEditAttachedObject
			return ac_OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
		#else
			return 0;
		#endif
	}

	#if defined _ALS_OnPlayerEditAttachedObject
		#undef OnPlayerEditAttachedObject
	#else
		#define _ALS_OnPlayerEditAttachedObject
	#endif

	#define OnPlayerEditAttachedObject AC_OnPlayerEditAttachedObject

	#if defined AC_OnPlayerEditAttachedObject
		forward AC_OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ);
	#endif
#endif

#if defined OnDialogResponse
    public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
    {
        #if defined _accessory_included
            #include "../include/accessoriesSystem/serverAccessories/dialogs.inc"
            #include "../include/accessoriesSystem/playerAccessories/dialogs.inc"
        #endif

        #if defined AC_OnDialogResponse
            return AC_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
        #else
            return 0;
        #endif
    }

    #if defined _ALS_OnDialogResponse
        #undef OnDialogResponse
    #else
        #define _ALS_OnDialogResponse
    #endif

    #define OnDialogResponse AC_OnDialogResponse

    #if defined AC_OnDialogResponse
        forward AC_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
    #endif

#if defined OnGameModeInit
    public OnGameModeInit() {
        Iter_Init(playerAccessories);

        #if defined AC_OnGameModeInit
            return AC_OnGameModeInit();
        #else
            return 0;
        #endif
    }

    #if defined _ALS_OnGameModeInit
        #undef OnGameModeInit
    #else
        #define _ALS_OnGameModeInit
    #endif

    #define OnGameModeInit AC_OnGameModeInit

    #if defined AC_OnGameModeInit
        forward AC_OnGameModeInit();
    #endif

stock RGB(red, green, blue, alpha)
{
	return (red * 16777216) + (green * 65536) + (blue * 256) + alpha;
}