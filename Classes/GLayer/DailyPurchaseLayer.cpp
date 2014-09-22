#include "DailyPurchaseLayer.h"
#include "../ToolFun/JniCall.h"
#include "../SpriteNode/HeroSprite.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"

bool DailyPurchaseLayer::init()
{
	if(!CCLayer::init())
		return false;

	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkgSprite = CCSprite::create("ui/timedBuyUI/baseboard.png");
	this->addChild(bkgSprite,3);
	bkgSprite->setPosition(size/2);

	CCMenu *menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	bkgSprite->addChild(menu,1);

	CCSprite *closeNor = CCSprite::create("ui/shared/closeTipNml.png");
	CCSprite *closeSel = CCSprite::create("ui/shared/closeTipNml.png");
	closeSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *closeItem = CCMenuItemSprite::create(closeNor,closeSel,this,menu_selector(DailyPurchaseLayer::close));
	closeItem->setPosition(ccp(bkgSprite->getContentSize().width *5/6,bkgSprite->getContentSize().height *6/7));
	menu->addChild(closeItem,144);

	CCSprite *getNor = CCSprite::create("ui/shared/continueGame.png");
	CCSprite *getSel = CCSprite::create("ui/shared/continueGame.png");
	getSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *getItem = CCMenuItemSprite::create(getNor,getSel,this,menu_selector(DailyPurchaseLayer::dailyPurchase));
	getItem->setPosition(ccp(bkgSprite->getContentSize().width/2,10*gScaleY));
	menu->addChild(getItem,144);
	
	/*char buf[32];
	sprintf(buf,"%s%d",ToolsFun::sharedTools()->getLanguageValue(K.RMB).c_str(),2);
	CCLabelTTF *label = CCLabelTTF::create(buf,"Î¢ÈíÑÅºÚ",38);
	getItem->addChild(label);
	label->setScale(0.6f);
	label->setColor(ccc3(220 ,220 ,220));
	label->setPosition(ccp(getItem->getContentSize().width/2,getItem->getContentSize().height + 7));
	*/

	registerNDK();
	return true;
}

void DailyPurchaseLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}


void DailyPurchaseLayer::close(CCObject *obj)
{	ToolsFun::sharedTools()->playEffect(BUTTON);
	GameConfigData::initialGameData().setDailyPurchase(0);
	GameConfigData::initialGameData().writeData();
	CCNotificationCenter::sharedNotificationCenter()->postNotification("closePurchase",NULL);
	this->removeFromParent();
}
void DailyPurchaseLayer::dailyPurchase(CCObject *obj)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(DailyPurchaseLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(DailyPurchaseLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(12);
}

void DailyPurchaseLayer::purchaseSuccess(const char* str)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("hidePurchase",NULL);
	GameConfigData::initialGameData().setDailyPurchase(0);
	GameConfigData::initialGameData().writeData();
	CCNotificationCenter::sharedNotificationCenter()->postNotification("closePurchase",NULL);
	this->removeFromParent();
}

void DailyPurchaseLayer::purchaseFailed(const char* str)
{

}