#ifndef ITEMSPRITE_H
#define ITEMSPRITE_H

#include "MySprite.h"
#include "../GCData/DataInitial.h"

class ItemSprite : public MySprite
{
public:
	ItemSprite(int itemId);
	~ItemSprite();
	static ItemSprite * createItemById(int itemId);
	virtual bool init();
	int getItemId();
	int getItemValue();
	void moveToPlayer(float t);
	void updateItemPosWithMap(float t);
	void startMoveToPlayer();
	void itemAction();
private:
	int m_itemId;
	ItemData *m_itemData;
	int m_itemValue;

};

#endif