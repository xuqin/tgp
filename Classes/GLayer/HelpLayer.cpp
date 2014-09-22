#include "HelpLayer.h"
#include "LevelChoice.h"
#include "ToolFun/ToolsFun.h"
#include "../GLayer/MainMenu.h"
#include "../GCData/DataInitial.h"
#include "../GCData/GameConfigData.h"
#include "ShopLayer.h"
#include "../ToolFun/StringKey.h"
#include "ShopLayer.h"
#include "ToolFun/JniCall.h"
#include "../ToolFun/HttpClientNode.h"
#include "BackLayer.h"
#include "LuckySpinLayer.h"

#define  COIN_TAG 6111045
#define  LEFT_TAG 6111046
#define  RIGHT_TAG 6111047
#define  PET_GAIN_START_TAG 10011
#define  UPPET_START_TAG 10021
#define  PET_BATTLE_START_TAG 10031
#define  PET_CANCEL_START_TAG 10041

#define  POS_TAG 900000


HelpLayer::HelpLayer():
	m_target(NULL)
	, m_callFun(NULL)
{

}

HelpLayer::~HelpLayer()
{

}

bool HelpLayer::init()
{
	MyScrowLayer::init();
	//initMenu();

	m_maxHeight = 1000;

	setKeypadEnabled(true);

	return true;
}

void HelpLayer::initMenu()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	setScissorRect(CCRectMake(0, 0, size.width, size.height));
	initUI();
}

void HelpLayer::initUI()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	//背景
	CCSprite * bkg = CCSprite::create("ui/shared/bkg.png");
	this->addChild(bkg, -4);
	bkg->setPosition(size / 2);

	//头部
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop, 0);
	spBkgTop->setPosition(ccp(size.width / 2, size.height - spBkgTop->getContentSize().height / 2));

	//金币放置最上方商城
	CCSprite *coinIcon = CCSprite::create("ui/shared/coin.png");
	this->addChild(coinIcon, 2, 1429);
	coinIcon->setPosition(ccp(280*gScaleX, size.height - 30*gScaleY));
	m_coinLabel = CCLabelBMFont::create("10000000", GOLD_FONT);
	this->addChild(m_coinLabel, 4);
	m_coinLabel->setAnchorPoint(ccp(0, 0.5f));
	m_coinLabel->setPosition(ccp(300*gScaleX, size.height - 25*gScaleY));
	m_coinLabel->setScale(0.6F);

	setCoin();

	CCMenu *menu = CCMenu::create();
	this->addChild(menu, 10);
	menu->setTouchPriority(-130);
	menu->setPosition(CCPointZero);
	CCSprite *spNml = CCSprite::create("ui/shared/back.png");
	CCSprite *spSel = CCSprite::create("ui/shared/back.png");
	spSel->setColor(ccc3(100, 100, 100));
	CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml, spSel, this, menu_selector(HelpLayer::back));
	menu->addChild(spItem);
	spItem->setPosition(ccp(37*gScaleX, size.height - 37*gScaleY));
	//show shop
	spNml = CCSprite::create("ui/shared/coinplus.png");
	spSel = CCSprite::create("ui/shared/coinplus.png");
	spSel->setColor(ccc3(100, 100, 100));
	spItem = CCMenuItemSprite::create(spNml, spSel, this, menu_selector(HelpLayer::showShopLayer));
	menu->addChild(spItem);
	spItem->setPosition(ccp(430*gScaleX, size.height - 30*gScaleY));

	//初始化宠物界面
	m_menu = CCMenu::create();
	m_uiNode->addChild(m_menu, 2);
	m_uiNode->setPosition(CCPointZero);
	m_menu->setPosition(CCPointZero);
	SK::StringKey nameKey[5] = {K.ITEM_NAME0, K.ITEM_NAME1, K.ITEM_NAME2, K.ITEM_NAME3, K.ITEM_NAME4};
	SK::StringKey desKey[5] = {K.ITEM_DES0, K.ITEM_DES1, K.ITEM_DES2, K.ITEM_DES3, K.ITEM_DES4};

	const char *fileName[5] = {"item/item2_0.png","item/item0_0.png","item/item1_0.png",
		"item/item3_0.png","item4_3.png"};

	for(int i = 0; i != 5; ++i)
	{
		CCSprite *petSprite = NULL;

		if(i ==4)
		{
			petSprite = CCSprite::createWithSpriteFrameName(fileName[i]);
		}
		else
		{
			petSprite  = CCSprite::create(fileName[i]);
		}
		m_uiNode->addChild(petSprite, 1);
		petSprite->setPosition(ccp(120*gScaleX, 130*gScaleY)+ccp(0,180*i));

		CCSprite* petDes = CCSprite::create("ui/petUI/pet_describe.png");
		m_uiNode->addChild(petDes, 1);
		petDes->setPosition(ccp(330*gScaleX, 140*gScaleY)+ccp(0,180*i));

		m_desFont[i] = CCLabelBMFont::create(gGetLanguageDesc(desKey[i]).c_str(), "font/item_des.fnt");
		m_uiNode->addChild(m_desFont[i], 10);
		m_desFont[i]->setPosition(ccp(240*gScaleX, 130*gScaleY)+ccp(0,180*i));
		m_desFont[i]->setAnchorPoint(CCPointZero);

		m_nameFont[i] = CCLabelBMFont::create(gGetLanguageDesc(nameKey[i]).c_str(), "font/item_des.fnt");
		m_uiNode->addChild(m_nameFont[i], 10);
		m_nameFont[i]->setPosition(ccp(240*gScaleX, 160*gScaleY)+ccp(0,180*i));
		m_nameFont[i]->setAnchorPoint(CCPointZero);
	
		CCLabelBMFont *nameLabel = m_nameFont[i];
		nameLabel->setScale(0.6f);
		nameLabel->setString(gGetLanguageDesc(nameKey[i]).c_str());

		CCLabelBMFont *desLabel = m_desFont[i];
		desLabel->setScale(0.6f);
		desLabel->setString(gGetLanguageDesc(desKey[i]).c_str()); 
	}
}

void HelpLayer::back(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);

	if(m_target)
	{
		(m_target->*m_callFun)(pSender);
		GameConfigData::initialGameData().writeData();
		this->removeFromParent();
	}
	else
	{
		LevelChoice *menuLayer = LevelChoice::create();
		ToolsFun::sharedTools()->gotoscene(menuLayer);
	}
}

void HelpLayer::showShopLayer(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	this->m_menu->setEnabled(false);
	ShopLayer *shopLayer = ShopLayer::create();
	this->addChild(shopLayer, 10);
	shopLayer->setCloseCallBack(this, menu_selector(HelpLayer::backFromShopLayer));
}
void HelpLayer::backFromShopLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(true);
}

void HelpLayer::setCloseCallBack(CCObject *pTarget, SEL_MenuHandler callBack)
{
	m_target = pTarget;
	m_callFun = callBack;

}

void HelpLayer::setCoin()
{
	int coinValue = GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);

	m_coinLabel->setString(coinStr.c_str());
}

void HelpLayer::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(HelpLayer::goGamble));
	this->addChild(layer,10000);
}
void HelpLayer::goGamble()
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}
