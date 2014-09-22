#ifndef DAILY_PURCHASE_LAYER_H
#define DAILY_PURCHASE_LAYER_H

#include "cocos2d.h"

USING_NS_CC;

class DailyPurchaseLayer :  public CCLayer
{
public :
	virtual bool init();
	CREATE_FUNC(DailyPurchaseLayer);
	void close(CCObject *obj);
	void dailyPurchase(CCObject *obj);
	void registerNDK();
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
};


#endif