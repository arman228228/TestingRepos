ShowPlayerInventoryDialog(playerid) {
	static 
		fmtDialogString[1500] = EOS;

    new
        pagesCount = playerInventoryPage{playerid},
        isNextPage = 0,
        listitemIndex = 0,
		playerInventoryItemsCount = 0;

    fmtDialogString = "{ffffff}Предмет\t{ffffff}Количество предмета\n";
	
	for(new i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        if (!GetPlayerInvItemCount(playerid, i)) continue;

		playerInventoryDialogData[playerid][playerInventoryItemsCount++] = i;
	}

	for(new i = 0, itemIndex, fmtString[3 + MAX_INVENTORY_ITEM_NAME_LENGTH]; i < playerInventoryItemsCount; i++) // 3 + 8 | 8 - itemName symbols count
    {
        if (i >= (pagesCount * MAX_ITEMS_ON_PAGE) && i < (pagesCount * MAX_ITEMS_ON_PAGE) + MAX_ITEMS_ON_PAGE)
			isNextPage++;

		if (i >= (pagesCount - 1) * MAX_ITEMS_ON_PAGE && i < ((pagesCount - 1) * MAX_ITEMS_ON_PAGE) + MAX_ITEMS_ON_PAGE)
		{
			itemIndex = playerInventoryDialogData[playerid][listitemIndex++];

			format(fmtString, sizeof(fmtString), "%d. %s\t%d\n", 
				listitemIndex, inventoryItemsInfo[itemIndex][itemName], GetPlayerInvItemCount(playerid, itemIndex)
			);
			strcat(fmtDialogString, fmtString, sizeof(fmtDialogString));
		}
    }
	
    if (isNextPage)
        strcat(fmtDialogString, "Следующая страница >\n");

    if (pagesCount > 1)
        strcat(fmtDialogString,  "< Предыдущая страница");
	
	return ShowPlayerDialog(playerid, D_INVENTORY_MENU, DIALOG_STYLE_TABLIST_HEADERS, "Меню инвентаря", fmtDialogString, "Далее", "Отмена");
}

GiveInvItemForPlayer(playerid, inventoryItemIndex, itemCount = 1)
{
    if(!(0 <= inventoryItemIndex <= MAX_INVENTORY_ITEMS - 1))
        return false;

    printf("[GiveInvItemForPlayer]: debug: inventoryItemIndex: %d itemCount: %d playerInventoryItems[playerid][inventoryItemIndex]: %d", 
        inventoryItemIndex, itemCount, playerInventoryItems[playerid][inventoryItemIndex]
    );

    playerInventoryItems[playerid][inventoryItemIndex] += itemCount < 0 ? (itemCount * -1) : itemCount;

    return true;
}

SetInvPlayerItemCount(playerid, inventoryItemIndex, itemCount)
{
    if(!(0 <= inventoryItemIndex <= MAX_INVENTORY_ITEMS - 1))
        return false;

    printf("[SetInvPlayerItemCount]: debug 3: inventoryItemIndex: %d itemcount: %d", inventoryItemIndex, itemCount);

    playerInventoryItems[playerid][inventoryItemIndex] = itemCount < 0 ? 0 : itemCount;

    return true;
}

RemovePlayerInvItem(playerid, inventoryItemIndex, itemCount = 1)
{
    if(!(0 <= inventoryItemIndex <= MAX_INVENTORY_ITEMS - 1))
        return false;
	
    if (itemCount < 0) 
        itemCount = 0;

    if (playerInventoryItems[playerid][inventoryItemIndex] - itemCount < 0) 
        return false;

    playerInventoryItems[playerid][inventoryItemIndex] -= itemCount;

    return true;
}


GetPlayerInvItemCount(playerid, inventoryItemIndex)
{
    if(!(0 <= inventoryItemIndex <= MAX_INVENTORY_ITEMS - 1))
        return false;

    return playerInventoryItems[playerid][inventoryItemIndex];
}

forward GetPlayerInventoryData(playerid);
public GetPlayerInventoryData(playerid)
{
    new rows;
	cache_get_row_count(rows);
	printf("[GetPlayerInventoryData]: debug 4: rows %d", rows);

	if (!rows)
        return false;

    new inventoryItemIndex, itemCount;

    for (new i = 0; i < rows; i++)
    {
        cache_get_value_name_int(i, "itemCount", itemCount);

        if (itemCount == 0) 
            continue;

        cache_get_value_name_int(i, "inventoryItemIndex", inventoryItemIndex);

        SetInvPlayerItemCount(playerid, inventoryItemIndex, itemCount);
    }

    return true;
}

SavePlayerInventoryData(playerid)
{
    new
		fmtQuery[80 + 4 + 7],
        userID;

    for(new i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        if (!GetPlayerInvItemCount(playerid, i)) 
            continue;

		format(fmtQuery, sizeof(fmtQuery), "SELECT itemCount FROM inventoryItemsData WHERE playerID = '%d' AND inventoryItemIndex = '%d'", userID, i);
		printf("%s", fmtQuery);
        mysql_tquery(MySQL:MySQLHandle, fmtQuery, "UpdateInventoryItemsData", "dd", playerid, i);
    }

    return true;
}


forward UpdateInventoryItemsData(playerid, inventoryItemIndex);
public UpdateInventoryItemsData(playerid, inventoryItemIndex)
{
    new 
        rows, userID, fmtQuery[86 + 7 + 7 + 9];

    cache_get_row_count(rows);
    printf("[GetPlayerInventoryData]: debug 4: rows %d", rows);

    if (rows)
    {
        format(fmtQuery, sizeof fmtQuery, "UPDATE inventoryItemsData SET itemCount = '%d' WHERE playerID = '%d' AND inventoryItemIndex = '%d'",
            GetPlayerInvItemCount(playerid, inventoryItemIndex),
            userID,
            inventoryItemIndex);
    }
    else
    {
        format(fmtQuery, sizeof fmtQuery, "INSERT INTO inventoryItemsData (playerID, inventoryItemIndex, itemCount) VALUES ('%d', '%d', '%d')",
            userID,
            inventoryItemIndex,
            GetPlayerInvItemCount(playerid, inventoryItemIndex));
    }

    printf("%s", fmtQuery);
    mysql_tquery(MySQL:MySQLHandle, fmtQuery);

    return true;
}