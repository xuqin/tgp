#include "SystemTipNode.h"
#include "../ToolFun/StringKey.h"
#include "../ToolFun/ToolsFun.h"
#include "../SpriteNode/HeroSprite.h"
#include "GLayer/MainMenu.h"
#include "GLayer/LevelChoice.h"
#include "GLayer/ShopLayer.h"
#include "GCData/GameConfigData.h"
#include "GLayer/HeroChoice.h"
#include "../GLayer/PetChoiceLayer.h"
#include "../GLayer/LuckySpinLayer.h"


float SystemTipNode::m_timeInterval = 1;
SystemTipNode::SystemTipNode()
{

}

SystemTipNode::~SystemTipNode()
{

}

bool SystemTipNode::init()
{
	schedule(schedule_selector(SystemTipNode::updateAddtTime));
	return true;
}

void SystemTipNode::updateAddtTime(float t)
{
	static float time = 0;
	time += t;

	if(time > m_timeInterval)
	{
		time = 0;
		showTip();
	}
}

void SystemTipNode::showTip()
{
	int tipType = rand() % 3;

	if(tipType == 0)
	{
		if(GameConfigData::initialGameData().getHeroStateInt(0) == 2)
		{
			tipType++;
		}
	}

	if(tipType == 1)
	{
		if(GameConfigData::initialGameData().getHeroStateInt(1) == 2)
		{
			tipType++;
		}
	}

	if(tipType == 2)
	{
		if(GameConfigData::initialGameData().getHeroStateInt(2) == 2)
		{
			tipType++;
		}
	}

	if(tipType == 3)
	{
	}

	if(tipType == 4)
	{

		tipType++;
	}

	if(tipType >= 5)
	{
		return;
	}

	if(this->getChildByTag(5191458))
	{
		this->removeChildByTag(5191458);
	}

	CCMenu *menu = CCMenu::create();
	this->addChild(menu, 1, 5191458);
	menu->setPosition(CCPointZero);
	menu->setTouchPriority(-290);
	CCSprite *sp = CCSprite::create("ui/tipUI/tipbkg.png");
	CCSprite *spSel = CCSprite::create("ui/tipUI/tipbkg.png");
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(sp, spSel, 
		this, menu_selector(SystemTipNode::touch));
	menu->addChild(itemSp, 1, 100 + tipType);
	itemSp->setPosition(ccp(240, 750));

	float t =  10;
	SystemTipNode::setTimeInterval(t);

	if(tipType == 2)
	{
		tipType  = rand() % 2 + tipType;
	}

	itemSp->setTag(100 + tipType);

	SK::StringKey tip0Keys[5] =
	{
		K.SHOP_ITEM0,
		K.SHOP_ITEM0,
		K.SHOP_ITEM0,
		K.SHOP_ITEM0,
		K.SHOP_ITEM0
	};
	SK::StringKey tip1Keys[5] =
	{
		K.LUCKY0_DES,
		K.LUCKY1_DES,
		K.LUCKY2_DES,
		K.LUCKY3_DES,
		K.LUCKY4_DES

	};
	SK::StringKey tip2Keys[5] =
	{
		K.PET_GIT,
		K.PET_GIT,
		K.PET_GIT,
		K.PET_GIT,
		K.PET_GIT
	};

	string xing = ToolsFun::sharedTools()->getRandXing();
	string ming = ToolsFun::sharedTools()->getRandMing();

	if(tipType >= 5)
	{
		menu->removeFromParent();
		return;
	}

	string str0 = xing + ming;
	string str1 = gGetLanguageDesc(tip0Keys[tipType]);
	string str2 = gGetLanguageDesc(tip1Keys[tipType]);
	string str3 = gGetLanguageDesc(tip2Keys[tipType]);
	string strArray[4] = {str0, str1, str2, str3};
	ccColor3B colorArray3[5] = {ccc3(0, 250, 0), ccc3(250, 250, 0), ccc3(255, 0, 0), ccc3(250, 250, 0)};
	CCNode *node = CCNode::create();
	int lenthg = 2;

	for(int i = 0; i != 4; ++i)
	{
		if(strArray[i].size())
		{
			CCLabelTTF *tip = CCLabelTTF::create(strArray[i].c_str(), "微软雅黑", 22);
			node->addChild(tip);
			tip->getTexture()->setAliasTexParameters();
			tip->setPosition(ccp(tip->getContentSize().width / 2 + lenthg, 0));
			lenthg += tip->getContentSize().width;
			tip->setColor(colorArray3[i]);
		}
	}

	itemSp->addChild(node);
	node->setPosition(ccp(itemSp->getContentSize().width / 2 - lenthg / 2, itemSp->getContentSize().height / 2));
	CCSequence *seq = CCSequence::create(CCDelayTime::create(5.0f), CCFadeOut::create(0.5f),
	                                     CCCallFuncN::create(this, callfuncN_selector(SystemTipNode::removeMenu)), NULL);
	itemSp->runAction(seq);
	seq->setTag(100);
}

void SystemTipNode::setTimeInterval(float t)
{
	m_timeInterval = t;
}

void SystemTipNode::touch(CCObject *pSender)
{
	CCNode *node = (CCNode*)pSender;
	//node->stopActionByTag(100);
	int tag = ((CCNode*)pSender)->getTag() - 100;
	CCNode *parentNode = this->getParent();
	node->getParent()->removeFromParent();

	if(tag == 0  || tag ==1)
	{
		//萌妹子  狂战士
		CCScene *sc = CCScene::create();
		HeroChoiceLayer *ly = HeroChoiceLayer::create();
		sc->addChild(ly);
		ly->initHeroSp(tag+1);
		CCDirector::sharedDirector()->replaceScene(sc);
		
	}
	else if(tag ==2 || tag ==3)
	{
		//两个宠物
		CCScene *sc = CCScene::create();
		PetChoiceLayer *ly = PetChoiceLayer::create();
		sc->addChild(ly);
		ly->movePetPosition(tag+1);
		//ly->initHeroSp(tag+1);
		CCDirector::sharedDirector()->replaceScene(sc);
	}
	else if(tag == 4)
	{
		//进入抽奖界面
		CCScene *sc = CCScene::create();
		LuckySpinLayer *ly = LuckySpinLayer::create();
		sc->addChild(ly);
		CCDirector::sharedDirector()->replaceScene(sc);
	}
}

void SystemTipNode::setParentType(int type)
{
	m_parentNodeType = type;
}

void SystemTipNode::removeMenu(CCObject *pSender)
{
	if(this->getChildByTag(5191458))
	{
		this->removeChildByTag(5191458);
	}
}