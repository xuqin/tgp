#include "MapNode.h"
#include "../GCData/DataInitial.h"
#include "../GLayer/GameLayer.h"
#include "MySpriteManager.h"
#include "../GCData/GameConfigData.h"

const int gMapOrderCount[10] = {3, 3, 3, 3, 3};

MapNode::MapNode(int mapId)
{
	m_bkgId = mapId;
	m_pauseMove = false;
}

MapNode::~MapNode()
{
	for(int i = 0; i != m_bkgs.size(); ++i)
	{
		if(m_bkgs[i])
		{
			delete m_bkgs[i];
		}
	}

	m_bkgs.clear();
}

MapNode * MapNode::create(int mapId)
{
	MapNode *node = new MapNode(mapId);

	if(node && node->init())
	{
		node->autorelease();
		return node;
	}

	delete node;
	return NULL;
}

bool MapNode::init()
{
	int curLevel = GameConfigData::initialGameData().getCurLevel();
	float speedScale = curLevel/5;

	for(int i = 0; i != 1; ++i)
	{
		BkgNode *node = new BkgNode();
		node->bkgNode = CCNode::create();
		node->speed = DataInitial::initialDataSource().getMapNodeSpeed()+speedScale*50;
		this->addChild(node->bkgNode, i, 100 + i);
		m_bkgs.push_back(node);
	}

	moveInit();
	return true;
}

void MapNode::updateBkgPos(float t)
{
	if(m_pauseMove)
	{
		return;
	}

	for(int i = 0; i != 1; ++i)
	{
		CCSprite *spriteChild = (CCSprite*)(m_bkgs[i]->bkgNode->getChildByTag(1234));

		if(spriteChild == NULL)
		{
			continue;
		}

		float curPosY = m_bkgs[i]->bkgNode->getPosition().y;
		float nextPosY1 = curPosY - m_bkgs[i]->speed * t;

		if(nextPosY1 < 0 - spriteChild->getContentSize().height || ToolsFun::sharedTools()->floatEquals(nextPosY1, 0 - spriteChild->getContentSize().height))
		{
			m_bkgs[i]->bkgNode->setPositionY(0);
		}
		else
		{
			m_bkgs[i]->bkgNode->setPosition(ccp(0, nextPosY1));
		}

		GameLayer::sharedGameLayer()->getMySpriteManager()->updateMySprite(t);
	}
}



void MapNode::setBkgSpeed(float speed)
{

}

void MapNode::moveInit()
{
	CCSize sSize = ToolsFun::sharedTools()->getSceneSize();

	for(int i = 0; i != 1; ++i)
	{
		BkgNode *bk = m_bkgs[i];
		bk->initSpeed = bk->speed;
		char filename[40];
		sprintf(filename, "mapBkg/0%d.jpg", m_bkgId);
		CCSprite *bkg1Sprite = CCSprite::create(filename);
		CCSprite *bkg2Sprite =  CCSprite::create(filename);
		bkg1Sprite->setAnchorPoint(CCPointZero);
		bkg2Sprite->setAnchorPoint(CCPointZero);
		bk->bkgNode->addChild(bkg1Sprite, i, 1234);
		bk->bkgNode->addChild(bkg2Sprite, i, 5678);
		bkg1Sprite->setPositionY(0);
		bkg2Sprite->setPositionY(bkg1Sprite->getContentSize().height);
	}
}

void MapNode::startMove()
{
	schedule(schedule_selector(MapNode::updateBkgPos));
}

void MapNode::mapSpeedEnd()
{
	BkgNode *bk = m_bkgs[0];
	bk->speed = bk->initSpeed;
	CCSprite *sprite0 = (CCSprite *)bk->bkgNode->getChildByTag(1234);
	CCSprite *sprite1 = (CCSprite *)bk->bkgNode->getChildByTag(5678);
	char filename[40];
	sprintf(filename, "mapBkg/0%d.jpg", m_bkgId);
	sprite0->initWithFile(filename);
	sprite1->initWithFile(filename);
	sprite0->setAnchorPoint(CCPointZero);
	sprite1->setAnchorPoint(CCPointZero);
	sprite0->setPositionY(0);
	sprite1->setPositionY(sprite0->getContentSize().height);
}

void MapNode::pauseMove()
{
	m_pauseMove = true;
}

void MapNode::resumeMove()
{
	m_pauseMove = false;
}

void MapNode::mapSpeedUp()
{
	BkgNode *bk = m_bkgs[0];
	bk->speed = DataInitial::initialDataSource().getMapNodeUpSpeed();
	CCSprite *sprite0 = (CCSprite*)bk->bkgNode->getChildByTag(1234);
	CCSprite *sprite1 = (CCSprite *)bk->bkgNode->getChildByTag(5678);
	char flushName[33];
	sprintf(flushName, "mapBkg/map%d_blur.jpg", m_bkgId);

	sprite0->initWithFile(flushName);
	sprite1->initWithFile(flushName);
	sprite0->setAnchorPoint(CCPointZero);
	sprite1->setAnchorPoint(CCPointZero);
	sprite0->setPositionY(0);
	sprite1->setPositionY(sprite0->getContentSize().height);
}