#include "MyScrowLayer.h"
#include "../ToolFun/ToolsFun.h"
#include <iostream>
#include <vector>
using namespace std;
MyScrowLayer::MyScrowLayer(): m_uiNode(NULL)
	, m_nodeMoved(false)
	, m_menu(NULL)
	, m_curTime(0)
	, m_lastPosY(0)
	, m_curEndSpeed(0)
	, m_maxHeight(1200)
	, m_pauseMove(false)
{
}

MyScrowLayer::~MyScrowLayer()
{

}

bool MyScrowLayer::init()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_uiNode = CCNode::create();
	this->addChild(m_uiNode, -1);
	m_uiNode->setPosition(CCPointZero);

	initMenu();
	return true;
}

void MyScrowLayer::visit()
{
	MyScissorLayer::visit();
}

void MyScrowLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
}

bool MyScrowLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
if(m_pauseMove)
		return false;
	CCPoint touchePos = pTouch->getLocation();

	if(!m_rect.containsPoint(touchePos))
	{
		return false;
	}

	m_nodeMoved = false;
	CCPoint pos = pTouch->getLocation();
	m_curPos = pos;
	m_lastPos = pos;
	m_curTime = time(NULL);
	m_lastPosY = m_uiNode->getPositionY();
	unschedule(schedule_selector(MyScrowLayer::updatePosTouchEnd));
	unschedule(schedule_selector(MyScrowLayer::endSlid));
	return true;
}

void MyScrowLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	m_curPos = pTouch->getLocation();

	if((m_curPos - m_lastPos).getLength() < 7)
	{
		return ;
	}
	else
	{
		m_nodeMoved = true;
		float xMove = m_curPos.y - m_lastPos.y;
		slidDir(0, xMove);
		m_lastPos = m_curPos;
	}
}

void MyScrowLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(!m_nodeMoved)
	{
		return;
	}

	long curTime = time(NULL);
	float curPosY = m_uiNode->getPositionY();

	if(curTime == m_curTime)
	{
		if(curPosY - m_lastPosY > 0)
		{
			m_curEndSpeed = 700;
		}
		else if(curPosY - m_lastPosY < 0)
		{
			m_curEndSpeed = -700;
		}
		else
		{
			return;
		}
	}
	else
	{
		m_curEndSpeed = (curPosY - m_lastPosY) / (curTime - m_curTime);
		m_curEndSpeed *= 1.30f;
	}

	if(fabs(m_curEndSpeed) > 80)
	{
		schedule(schedule_selector(MyScrowLayer::updatePosTouchEnd));
		scheduleOnce(schedule_selector(MyScrowLayer::endSlid), 0.5f);
	}
}

void MyScrowLayer::updatePosTouchEnd(float t)
{
	m_curEndSpeed = m_curEndSpeed ;
	float xMove = m_curEndSpeed * t;
	slidDir(0, xMove);
}

void MyScrowLayer::endSlid(float t)
{
	unschedule(schedule_selector(MyScrowLayer::updatePosTouchEnd));
}

void MyScrowLayer::initMenu()
{
	/*CCMenu *m_menu = CCMenu::create();
	m_uiNode->addChild(m_menu);
	m_menu->setPosition(CCPointZero);
	for (int i=0;i!=10;++i)
	{
	    CCSprite *spNml = CCSprite::create("gomap0.png");
	    CCSprite *spSel = CCSprite::create("gomap1.png");
	    CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(MyScrowLayer::touchMenu));
	    m_menu->addChild(itemSp);
	    itemSp->setPosition(ccp(320,50+100*i));
	    m_spriteItems.push_back(itemSp);
	}*/
}


void MyScrowLayer::slidDir(int dir, float lg)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	float posY = m_uiNode->getPositionY() + lg;

	if(posY <  size.height - m_maxHeight) 
	{
		m_uiNode->setPositionY(size.height - m_maxHeight);
		return;
	}
	else if(posY > m_rect.origin.y)
	{
		m_uiNode->setPositionY(m_rect.origin.y);
		return;
	}

	m_uiNode->setPositionY(posY);
}

void MyScrowLayer::touchMenu(CCObject *pSender)
{
	if(m_nodeMoved)
	{
		return;
	}
}