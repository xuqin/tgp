#include "BackLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "../ToolFun/JniCall.h"
#include "../NDKHelper/NDKHelper.h"
#include "../GCData/GameConfigData.h"


BackLayer::BackLayer():m_target(NULL),m_func(NULL)
{

}

bool BackLayer::init()
{
	if(!CCLayer::init())
		return false;

	setTouchEnabled(true);

	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,1);
	bkg->setPosition(ccp(size.width/2,size.height/2));
	bkg->setTag(112255);

	char buf[32];
	sprintf(buf,"DayReward/30lucky.png");
	CCSprite *tip1 = CCSprite::create(buf);
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);
	menu->setTouchPriority(-204);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(BackLayer::backClose));
	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(BackLayer::backPurchase));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp);

	registerNDK();

	return true;
}

void BackLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}

void BackLayer::backClose(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	SendMessageWithParams("quitGame", NULL);
#endif 

	CCLayer *layer = (CCLayer *)this->getParent();
	if(layer!=NULL)
	{
		layer->setKeypadEnabled(true);
	}

	this->removeFromParent();
}

void BackLayer::setCallback(CCObject *obj,SEL_CallFun func)
{
	m_target = obj;
	m_func = func;
}

void BackLayer::backPurchase(CCObject *pSender)
{
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(BackLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(BackLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(6);
}

void BackLayer::purchaseSuccess(const char* str)
{
	//HeroConfigData::sharedHeroConfigData()->writeData();
		
	int count = GameConfigData::initialGameData().getLuckyCount();
	GameConfigData::initialGameData().setLuckyCount(count+30);
	
	int moneyCount = GameConfigData::initialGameData().getCoinValue();
	moneyCount+=4000;
	GameConfigData::initialGameData().setCoinValue(moneyCount);
	
	GameConfigData::initialGameData().writeData();

	if(m_target)
	{
		(m_target->*m_func)();
	}

	CCLayer *layer = (CCLayer *)this->getParent();
	if(layer!=NULL)
	{
		layer->setKeypadEnabled(true);
	}

	this->removeFromParent();
}

void BackLayer::purchaseFailed(const char* str)
{
}
void BackLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -203, true);
}

bool BackLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}