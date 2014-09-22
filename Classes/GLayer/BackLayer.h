#ifndef BACK_LAYER_H
#define BACK_LAYER_H

#include "cocos2d.h"

USING_NS_CC;

typedef void (CCObject::*SEL_CallFun)();

#define callFun_selector(_SELECTOR) (SEL_CallFun)(&_SELECTOR)

class BackLayer : public CCLayer
{
public:
	BackLayer();
	virtual bool init();
	CREATE_FUNC(BackLayer);
	void registerNDK();
	void setCallback(CCObject *obj,SEL_CallFun func);
	void backClose(CCObject *pSender);
	void backPurchase(CCObject *pSender);
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
	CCObject *m_target;
	SEL_CallFun m_func;
};

#endif