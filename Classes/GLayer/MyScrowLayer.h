#ifndef MYSCROWLAYER_H
#define MYSCROWLAYER_H

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "MyScissorLayer.h"
#include "MyScrowLayer.h"
using namespace std;
using namespace cocos2d;
class GameLayer;
class MyScrowLayer : public MyScissorLayer
{
public:
	MyScrowLayer();
	virtual ~MyScrowLayer();
	virtual bool init();
	virtual void visit();
	CREATE_FUNC(MyScrowLayer);
public:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void endSlid(float t);
	virtual void updatePosTouchEnd(float t);
	virtual void initMenu();
	virtual void slidDir(int dir,float lg);
	virtual void touchMenu(CCObject *pSender);

protected:
	CCNode *m_uiNode;
	bool m_nodeMoved;
	CCPoint m_curPos;
	CCPoint m_lastPos;
	CCMenu *m_menu;
	long m_curTime;
	float m_lastPosY;
	float m_curEndSpeed;
	vector<CCMenuItemSprite*> m_spriteItems;
	int m_maxHeight;
	bool m_pauseMove;
};
#endif