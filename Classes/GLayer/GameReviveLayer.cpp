#include "GameReviveLayer.h"
#include "../ToolFun/JniCall.h"
#include "../SpriteNode/HeroSprite.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "GameLayer.h"


GameReviveLayer::GameReviveLayer()
{
	m_layer = GameLayer::sharedGameLayer();
}

GameReviveLayer::~GameReviveLayer()
{
}

bool GameReviveLayer::init()
{
	if(!CCLayer::init())
		return false;

	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkgSprite = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkgSprite, 3);
	bkgSprite->setPosition(size / 2);

	CCMenu *menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	bkgSprite->addChild(menu, 1);

	CCSprite *closeNor = CCSprite::create("ui/shared/closeTipNml.png");
	CCSprite *closeSel = CCSprite::create("ui/shared/closeTipNml.png");
	closeSel->setColor(ccc3(80, 80, 80));
	CCMenuItemSprite *closeItem = CCMenuItemSprite::create(closeNor, closeSel, this, menu_selector(GameReviveLayer::close));
	closeItem->setPosition(ccp(bkgSprite->getContentSize().width * 5 / 6, bkgSprite->getContentSize().height * 6 / 7));
	menu->addChild(closeItem, 144);

	CCSprite *getNor = CCSprite::create("ui/shared/continueGame.png");
	CCSprite *getSel = CCSprite::create("ui/shared/continueGame.png");
	getSel->setColor(ccc3(80, 80, 80));
	CCMenuItemSprite *getItem = CCMenuItemSprite::create(getNor, getSel, this, menu_selector(GameReviveLayer::purchase));
	getItem->setPosition(ccp(bkgSprite->getContentSize().width / 2, 10*gScaleY));
	menu->addChild(getItem, 144);
	

	CCSprite *tip1 = CCSprite::create("DayReward/advanced.png");
	bkgSprite->addChild(tip1);
	tip1->setPosition(ccp(bkgSprite->getContentSize().width/2,bkgSprite->getContentSize().height/2-50*gScaleY));

	/*
	char buf[32];
	sprintf(buf,"%d",6);
	CCLabelTTF *label = CCLabelTTF::create(buf,"Î¢ÈíÑÅºÚ",38);
	getItem->addChild(label);
	label->setScale(0.6f);
	label->setColor(ccc3(220 ,220 ,220));
	label->setPosition(ccp(bkgSprite->getPositionX()/2,bkgSprite->getPositionY()/2 ));*/

	registerNDK();

	return true;
}

void GameReviveLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();

	if(jniNode->getParent())
	{
		jniNode->removeFromParent();
	}

	this->addChild(jniNode);
}

void GameReviveLayer::close(CCObject *obj)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);

	if(m_target)
	{
		(m_target->*m_callFun)(this);
		this->removeFromParent();
	}
}
void GameReviveLayer::purchase(CCObject *obj)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	JniCall::sharedJniCall()->setBuyFailedCallBack(this, callFunStr_selector(GameReviveLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this, callFunStr_selector(GameReviveLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(7);
}

void GameReviveLayer::purchaseSuccess(const char* str)
{
	int moneyCount = GameConfigData::initialGameData().getCoinValue();
	moneyCount+=3000;
	GameConfigData::initialGameData().setCoinValue(moneyCount);
	m_layer->reviveLevel(this);
}

void GameReviveLayer::purchaseFailed(const char* str)
{
	if(m_target)
	{
		(m_target->*m_callFun)(this);
		this->removeFromParent();
	}
}

void GameReviveLayer::setCloseCallBack(CCObject *pTarget, SEL_MenuHandler callBack)
{

	m_target = pTarget;
	m_callFun = callBack;
}