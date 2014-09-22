#include "ItemSprite.h"
#include "../SpriteNode/HeroSprite.h"
#include "../SpriteNode/MapNode.h"
#include "../GLayer/GameLayer.h"
#include "SpriteAniManager.h"
#include "SpriteAction.h"
#include "../GCData/GameConfigData.h"

ItemSprite::ItemSprite(int itemId): m_itemId(itemId), m_itemData(NULL), m_itemValue(-1)
{

}

ItemSprite::~ItemSprite()
{
	unschedule(schedule_selector(ItemSprite::moveToPlayer));
}

ItemSprite *  ItemSprite::createItemById(int itemId)
{
	ItemSprite *itemS = new ItemSprite(itemId);

	if(itemS && itemS->init())
	{
		itemS->autorelease();
		return itemS;
	}

	delete itemS;
	return NULL;
}

bool  ItemSprite::init()
{
	m_itemData = DataInitial::initialDataSource().getItemDataById(m_itemId);
	m_boundRadiusInitState = true;
	m_boundRadius = m_itemData->mBoundRadius;
	m_itemValue = m_itemData->mItemValue;
	char filename[64];


	if(m_itemId == 4)
	{
		sprintf(filename, "item%d_0.png", m_itemId);
		this->initWithSpriteFrameName(filename);
		CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName("item4_%d.png");
		CCAnimate *ani = CCAnimate::create(animation);
		this->runAction(CCRepeatForever::create(ani));
	}
	else
	{
		sprintf(filename, "item/item%d_0.png", m_itemId);
		this->initWithFile(filename);
	}

	return true;
}

int ItemSprite::getItemId()
{
	return m_itemId;
}

int ItemSprite::getItemValue()
{
	return m_itemValue;
}

void ItemSprite::moveToPlayer(float t)
{
	GameLayer *layer = GameLayer::sharedGameLayer();

	if(layer && layer->getHeroSprite()->getMgneticState())
	{
		CCPoint myPos = this->getPosition();
		CCPoint heroPos = GameLayer::sharedGameLayer()->getHeroPos();
		float subLength = (myPos - heroPos).getLength();
		CCPoint newPos;
		CCPoint pos = GameLayer::sharedGameLayer()->getHeroPos() - this->getPosition();


		if(pos.getLength() <300)
		{
			if(pos.getLength() < 85)
			{
				this->setPosition(heroPos);
			}
			else
			{
				float angle = pos.getAngle();
				this->setRotation(270 - angle * 180 / PI);
				int randSpeed = rand() % 300;
				randSpeed += 400;
				newPos.x = this->getPosition().x + randSpeed * cos(angle) * t;
				newPos.y = this->getPosition().y + randSpeed * sin(angle) * t;
				this->setPosition(newPos);
			}
		}
	}
}

void ItemSprite::updateItemPosWithMap(float t)
{
	CCAssert(GameLayer::sharedGameLayer() != NULL, "MonsterSprite::updatePosWithMa must not be NULL");
	bool upState = GameLayer::sharedGameLayer()->getHeroSprite()->getSpeedUpState();
	
	int curLevel = GameConfigData::initialGameData().getCurLevel();
	float speedScale = curLevel/5;

	float speed = 0;

	if(upState)
	{
		speed = DataInitial::initialDataSource().getMapNodeUpSpeed();
	}
	else
	{
		speed = DataInitial::initialDataSource().getMapNodeSpeed()+50*speedScale;
	}

	this->setPositionY(this->getPositionY() - speed  * t);
}

void ItemSprite::startMoveToPlayer()
{
	resumeSchedulerAndActions();
	schedule(schedule_selector(ItemSprite::moveToPlayer), 1 / 60.0f);
}

void ItemSprite::itemAction()
{
	//SpriteAction::sharedSpriteAction().spriteRunAction(this);
}