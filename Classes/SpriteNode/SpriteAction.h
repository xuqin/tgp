#ifndef SPRITEACTION_H
#define SPRITEACTION_H
#include "cocos2d.h"
using namespace cocos2d;

class MonsterSprite;
class ItemSprite;
class SpriteAction
{
public:
	~SpriteAction();
	static SpriteAction& sharedSpriteAction();
	void spriteRunAction(MonsterSprite* sprite);
	void spriteRunAction(ItemSprite* sprite);
private:
	SpriteAction();
	SpriteAction(const SpriteAction& sp);
	SpriteAction& operator=(const SpriteAction);
};

#endif