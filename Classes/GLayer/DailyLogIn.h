#ifndef DAILYLOGIN_H
#define DAILYLOGIN_H
#include "cocos2d.h"
#include "DialogLayer.h"
using namespace cocos2d;

class DailyLogin : public CCLayer
{
public:
	DailyLogin();
	~DailyLogin();
	virtual bool init();
	CREATE_FUNC(DailyLogin);
	void setCurrentReward();
	void setcurrentGain();
	void gainReward(CCObject *pSender);
	void setCloseCallBack(CCObject *pSender,SEL_MenuHandler callFun);
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
	CCMenu *m_menu;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};
#endif