#ifndef GAME_REVIVE_LAYER_H
#define GAME_REVIVE_LAYER_H

#include "cocos2d.h"


USING_NS_CC;
class GameLayer;
class GameReviveLayer :  public CCLayer
{
public :
	GameReviveLayer();
	~GameReviveLayer();
	virtual bool init();
	CREATE_FUNC(GameReviveLayer);
	void close(CCObject *obj);
	void purchase(CCObject *obj);
	void registerNDK();
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack);
private:
	GameLayer *m_layer;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};


#endif