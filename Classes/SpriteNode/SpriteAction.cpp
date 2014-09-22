#include "SpriteAction.h"
#include "MonsterSprite.h"
#include "ItemSprite.h"

SpriteAction::~SpriteAction()
{

}

SpriteAction& SpriteAction::sharedSpriteAction()
{
	static SpriteAction sp;
	return sp;
}

void SpriteAction::spriteRunAction(MonsterSprite* sprite)
{
	CCMoveBy *moveBy = CCMoveBy::create(1.0f, ccp(0, -300));
	sprite->runAction(CCRepeatForever::create(moveBy));
}

void SpriteAction::spriteRunAction(ItemSprite* sprite)
{
	CCMoveBy *moveBy = CCMoveBy::create(1.0f, ccp(0, -200));
	sprite->runAction(CCRepeatForever::create(moveBy));
}

SpriteAction::SpriteAction()
{

}
