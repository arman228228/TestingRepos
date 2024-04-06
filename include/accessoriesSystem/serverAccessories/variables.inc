#if defined _accessory_included
	#endinput
#endif
#define _accessory_included

#define _accessory_debug_mode

#if !defined ACCESSORY
	#define ACCESSORY: _:
#endif

const MAX_ACCESSORIES = 10;

enum AccessoriesInfoEnum
{
    aIndex,
    aModelID,
    aBone,

    Float: aFOffsetX,
    Float: aFOffsetY,
    Float: aFOffsetZ,
    Float: aFRotX,
    Float: aFRotY,
    Float: aFRotZ,
    Float: aFScaleX,
    Float: aFScaleY,
    Float: aFScaleZ,

    aMaterialColor1,
    aMaterialColor2
};

new AccessoriesInfo[MAX_ACCESSORIES][AccessoriesInfoEnum],
    Iterator: CreatedAccessories<MAX_ACCESSORIES>;

enum {
	POS_OFFSET_X,
	POS_OFFSET_Y,
	POS_OFFSET_Z,
	ROT_OFFSET_X,
	ROT_OFFSET_Y,
	ROT_OFFSET_Z,
	SCALE_X,
	SCALE_Y,
	SCALE_Z
};

enum {
    COLOR_0,
    COLOR_1
}


new AttachBones[18][] = {
	{"Спина"},
	{"Голова"},
	{"Плечо левой руки"},
	{"Плечо правой руки"},
	{"Левая рука"},
	{"Правая рука"},
	{"Левое бедро"},
	{"Правое бедро"},
	{"Левая нога"},
	{"Правая нога"},
	{"Правая голень"},
	{"Левая голень"},
	{"Левое предплечье"},
	{"Правое предплечье"},
	{"Левая ключица (плечо)"},
	{"Правая ключица (плечо)"},
	{"Шея"},
	{"Челюсть"}
};

new AxisPoses[9][] = {
	{"aFOffsetX"},
	{"aFOffsetY"},
	{"aFOffsetZ"},
	{"aFRotX"},
	{"aFRotY"},
	{"aFRotZ"},
	{"aFScaleX"},
	{"aFScaleY"},
	{"aFScaleZ"}
};

new
	playerAccessoriesMenuPage[MAX_PLAYERS],
	playerAccessoryDialogData[MAX_PLAYERS][MAX_ITEMS_ON_PAGE]
;

new playerEditCurrentAxis[MAX_PLAYERS], playerEditCurrentColor[MAX_PLAYERS], playerAccessoryIndex[MAX_PLAYERS], playerAccessoryColors[MAX_PLAYERS][4];

stock const NEXT_PAGE_TEXT_FOR_DIALOG[]     =        "Следующая страница >";
stock const PREVIOUS_PAGE_TEXT_FOR_DIALOG[] =        "< Предыдущая страница";