CMD:inv(playerid)
{
	playerInventoryPage{playerid} = 1;
	return ShowPlayerInventoryDialog(playerid);
}

CMD:additem(playerid) 
	return GiveInvItemForPlayer(playerid, itemType_HatBoater2);

CMD:removeitem(playerid) 
	return RemovePlayerInvItem(playerid, itemType_HatBoater2);