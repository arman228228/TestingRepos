#if defined _inventory_included
	#endinput
#endif
#define _inventory_included

const MAX_INVENTORY_ITEM_NAME_LENGTH = 7 + 1;

enum inventoryInfoEnum
{
    itemModelID,
	itemName[MAX_INVENTORY_ITEM_NAME_LENGTH]
};

enum
{
    itemType_HatBoater2, 
    itemType_HatBowler1,

    __DUMMY_ELEMENT_,
    MAX_INVENTORY_ITEMS          = __DUMMY_ELEMENT_
};

new inventoryItemsInfo[MAX_INVENTORY_ITEMS][inventoryInfoEnum] = 
{
    { 18945, "Ўл€па 1" },
    { 18947, "Ўл€па 2" }
};

//
#define MAX_ITEMS_ON_PAGE            	15
#define D_INVENTORY_MENU                1

new
	playerInventoryItems[MAX_PLAYERS][MAX_INVENTORY_ITEMS],
    playerInventoryPage[MAX_PLAYERS char],
	playerInventoryDialogData[MAX_PLAYERS][MAX_ITEMS_ON_PAGE]
;