#include "LevelChoice.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/DataInitial.h"
#include "../GCData/GameConfigData.h"
#include "../GLayer/PetChoiceLayer.h"
#include "../GLayer/DailyLogIn.h"
#include "../GLayer/MainMenu.h"
#include "StartGameLoading.h"
#include "ShopLayer.h"
#include "PrepareLayer.h"
#include "HeroChoice.h"
#include "HelpLayer.h"
#include "SpriteNode/SystemTipNode.h"
#include "../GLayer/DailyPurchaseLayer.h"
#include "../ToolFun/JniCall.h"
#include "BackLayer.h"
#include "LuckySpinLayer.h"

#include <iostream>
#include <vector>
using namespace std;

#define REWARD 90000
#define LEVEL_SCALE 0.6f

LevelChoice::LevelChoice(): m_uiNode(NULL)
	, m_nodeMoved(false)
	, m_menu(NULL)
	, m_curTime(0)
	, m_lastPosY(0)
	, m_curEndSpeed(0)
	,m_pauseMove(false)
{
}

LevelChoice::~LevelChoice()
{

}

bool LevelChoice::init()
{
	ToolsFun::sharedTools()->playBkgMusic(LOGIN, true);

	string curData = ToolsFun::sharedTools()->getSystemData();
	string curMonth = ToolsFun::sharedTools()->getSystemMonth();
	int i = 1;

	setTouchEnabled(true);
	m_uiNode = CCNode::create();
	this->addChild(m_uiNode, 1);

	int maxLv = GameConfigData::initialGameData().getMaxUnLockLevel();

	CCPoint currPos = DataInitial::initialDataSource().getLeveUnLockInf(maxLv)->mPos;

	if(maxLv>=6 && maxLv<=23)
	{
		m_uiNode->setPosition(ccp(0,-currPos.y)+ccp(0,200*gScaleX));
	}

	if(maxLv>23)
	{
		int mapId = currPos.y / (800*gScaleY);
		float posY = 800*gScaleY * mapId;
		m_uiNode->setPosition(ccp(0, -posY));
	}

	//int mapId = currPos.y / (700*gScaleY);
	//float posY = 700*gScaleY * mapId;
	//m_uiNode->setPosition(ccp(0, -posY));

	for(int i = 0; i != 4; ++i)
	{
		char filename[64];
		sprintf(filename, "ui/levelChoice/levelChoice%d.jpg", i);
		CCSprite *sprite = CCSprite::create(filename);
		m_uiNode->addChild(sprite);
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 800 * i * gScaleY));
	}

	m_menu = CCMenu::create();
	m_uiNode->addChild(m_menu, 1);
	m_menu->setPosition(CCPointZero);


	/*初始化主关卡*/
	vector<LevelUnLock*> unLockConfigs = DataInitial::initialDataSource().getUnLockConfigs();

	for(int i = 0; i != unLockConfigs.size(); ++i)
	{
		LevelUnLock *unLockConfig = unLockConfigs[i];

		if(unLockConfig)
		{
			CCSprite *spriteNml = CCSprite::create(unLockConfig->mLevelNmlFile.c_str());
			CCSprite *spriteSel = CCSprite::create(unLockConfig->mLevelSelFile.c_str());
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);
			CCSprite *icon = CCSprite::create(unLockConfig->mLevelIconFile.c_str());

			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel,
				this, menu_selector(LevelChoice::choiceLevel));
			itemSprite->setScale(0.75f);
			m_menu->addChild(itemSprite, 1, 100 + i);
			itemSprite->addChild(icon, 1, 123);
			icon->setPosition(ccp(itemSprite->getContentSize().width / 2, itemSprite->getContentSize().height / 2));
			itemSprite->setPosition(unLockConfig->mPos);

			//关卡数字
			char levelStr[33];
			sprintf(levelStr, "%d", i + 1);

			CCLabelBMFont *level = CCLabelBMFont::create(levelStr, "font/level.fnt");
			itemSprite->addChild(level, 1);
			level->setPosition(ccp(itemSprite->getContentSize().width / 2, -20*gScaleY));
		}
	}


	int curHeroId = GameConfigData::initialGameData().getCurHeroId();
	char filename[64];
	sprintf(filename, "ui/levelChoice/levelRmbLock%d.png", curHeroId);
	CCSprite *tip = CCSprite::create(filename);
	int prelv = GameConfigData::initialGameData().getCurLevel();
	int unlockmax = GameConfigData::initialGameData().getMaxUnLockLevel();
	CCPoint temp = DataInitial::initialDataSource().getLeveUnLockInf(unlockmax)->mPos;

	//过关指示图标的动画
	m_uiNode->addChild(tip, 1);

	if((unlockmax - prelv) == 0 && unlockmax != 0)
	{
		CCPoint prepos = DataInitial::initialDataSource().getLeveUnLockInf(unlockmax - 1)->mPos;
		tip->setPosition(prepos);

		CCMoveBy  *tipmove = CCMoveBy::create(2, (temp - prepos));
		CCSequence *seq = CCSequence::create(tipmove, CCCallFuncN::create(this, callfuncN_selector(LevelChoice::tipMoveEnd)), NULL);
		tip->runAction(seq);
	}
	else
	{
		tip->setPosition(temp);
		tipMoveEnd(tip);
	}


	/*初始化支线关卡*/
	vector<LevelUnLock*> subUnLockConfigs = DataInitial::initialDataSource().getSubUnLockConfigs();

	for(int i = 0; i != subUnLockConfigs.size(); ++i)
	{
		LevelUnLock *unLockConfig = subUnLockConfigs[i];

		if(unLockConfig)
		{
			CCSprite *spriteNml = CCSprite::create(unLockConfig->mLevelNmlFile.c_str());
			CCSprite *spriteSel = CCSprite::create(unLockConfig->mLevelSelFile.c_str());
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);

			CCSprite *icon = CCSprite::create(unLockConfig->mLevelIconFile.c_str());
			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel,
				this, menu_selector(LevelChoice::choiceSubLevel));
			itemSprite->setScale(0.75f);
			m_menu->addChild(itemSprite, 1, 1000 + i);
			itemSprite->addChild(icon, 1, 123);
			icon->setPosition(ccp(itemSprite->getContentSize().width / 2, itemSprite->getContentSize().height / 2));
			itemSprite->setPosition(unLockConfig->mPos);

			int startCount = GameConfigData::initialGameData().getStartCout();
			char startCountStr[33];
			sprintf(startCountStr, "%d", startCount);
			CCLabelBMFont *curStartCount = CCLabelBMFont::create(startCountStr, "font/gold_big.fnt");
			icon->addChild(curStartCount, 1);
			curStartCount->setAnchorPoint(ccp(0, 0.5f));
			curStartCount->setPosition(ccp(60*gScaleX, 24*gScaleY));

			char levelStr[33];
			sprintf(levelStr, "/%d", unLockConfig->mUnLockValue0);
			CCLabelBMFont *level = CCLabelBMFont::create(levelStr, "font/gold_big.fnt");
			icon->addChild(level, 1);
			level->setAnchorPoint(ccp(0, 0.5f));
			level->setPosition(ccp(curStartCount->getPositionX() + curStartCount->getContentSize().width, curStartCount->getPositionY()));
		}
	}

	/* 初始化奖励信息*/
	vector<RewardConfig*> rewardConfigs = DataInitial::initialDataSource().getRewardConfigs();

	for(int i = 0; i != rewardConfigs.size(); ++i)
	{
		RewardConfig *rewardConfig = rewardConfigs[i];

		if(rewardConfig)
		{
			CCSprite *spriteNml = CCSprite::create(rewardConfig->mRewardNmlFile.c_str());
			CCSprite *spriteSel = CCSprite::create(rewardConfig->mRewardSelFile.c_str());
			CCSprite *sprite = CCSprite::create("ui/levelChoice/bubble.png");
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);
			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::choiceReward));
			m_menu->addChild(itemSprite, 1, 10000 + i);
			itemSprite->addChild(sprite, 2);

			itemSprite->setPosition(rewardConfig->mPos);
			sprite->setPosition(itemSprite->getContentSize() / 2);
		}
	}

	SystemTipNode *node = SystemTipNode::create();
	this->addChild(node, 1113);
	node->setParentType(2);

	registerNDK();

	if(0 || GameConfigData::initialGameData().getlastDate() == "default" || curData != GameConfigData::initialGameData().getlastDate() ||  curMonth != GameConfigData::initialGameData().getlastMonth())
	{
		gotoDailyLogIn();
	}
	else
	{
		int upCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelChoice",0);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("levelChoice",upCount+1);

		if(upCount %5==0)
		{
			showGiftBag();
			CCUserDefault::sharedUserDefault()->setIntegerForKey("levelChoice",1);
		}

		
	}

	initUnLockLevel();
	initUnLockSubLevel();
	initRewardLevel();
	initMenuItem();

	setKeypadEnabled(true);

	return true;
}

int LevelChoice::getGiftBag()
{
	/*	0	对应萌妹子
		1	对应狂战士
		2	30连抽
		3	烈焰勇士礼包
		4	炽热精灵礼包
	*/

	vector<int> queue;
	queue.clear();
	
	if(GameConfigData::initialGameData().getHeroStateInt(1)!= 1)
	{
		queue.push_back(0);
	}

	if(GameConfigData::initialGameData().getHeroStateInt(2) != 1)
	{
		queue.push_back(1);
	}
	
	if(GameConfigData::initialGameData().getPetStateById(5) =="0")
	{
		queue.push_back(2);
		}
		
			if(GameConfigData::initialGameData().getPetStateById(3) =="0")
				{
						queue.push_back(3);
							}
							
								if(GameConfigData::initialGameData().getPetStateById(4) =="0")
									{
									
									queue.push_back(4);
										}


	if(queue.size() ==0)
	{
		return -1;
	}
	else
	{
		int value = rand() % queue.size();
		return queue.at(value);
	}
}

void LevelChoice::showGiftBag()
{
	int gift = getGiftBag();
	if(gift !=-1)
	{
		showGiftBag(gift);
	}
}

void LevelChoice::showGiftBag(int id)
{
	/*	0	对应萌妹子
		1	对应狂战士
		2	30连抽
		3	烈焰勇士礼包
		4	炽热精灵礼包
	*/
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,200,12313133);
	bkg->setPosition(ccp(size.width/2,size.height/2));

	char buf[64];

	if(id == 0)
	{
		sprintf(buf,"DayReward/gril.png");
	}else if(id ==1)
	{
		sprintf(buf,"DayReward/man.png");
	}else if(id ==2)
	{
		sprintf(buf,"DayReward/30lucky.png");
	}
	else if(id ==3)
	{
		sprintf(buf,"DayReward/sprite3.png");
	}
	else if(id ==4)
	{
		sprintf(buf,"DayReward/sprite4.png");
	}


	CCSprite *tip1 = CCSprite::create(buf);
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));


	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(LevelChoice::showGiftBagClose));
	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(LevelChoice::showGiftBagSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp,10,100+id);

	m_pauseMove = true;
	m_menu->setEnabled(false);

}

void LevelChoice::showGiftBagSure(CCObject *sender)
{
	int itemCode[5] = {0,1,6,2,3};

	CCMenuItemSprite *item = (CCMenuItemSprite *)sender;

	if(item)
	{
		int tag = item->getTag()-100;
		JniCall::sharedJniCall()->setBuyFailedCallBack(this, callFunStr_selector(LevelChoice::purchaseFailed));
		JniCall::sharedJniCall()->setBuyCompleteCallBack(this, callFunStr_selector(LevelChoice::purchaseSuccess));
		JniCall::sharedJniCall()->purchaseByIndex(itemCode[tag]);
	}
//	this->removeChildByTag(12313133);

	m_pauseMove = false;
	m_menu->setEnabled(true);
}

void LevelChoice::showGiftBagClose(CCObject *sender)
{
	this->removeChildByTag(12313133);
	m_pauseMove = false;
	m_menu->setEnabled(true);
}

void LevelChoice::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();

	if(jniNode->getParent())
	{
		jniNode->removeFromParent();
	}

	this->addChild(jniNode);
}

void LevelChoice::initMenuItem()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	//头部
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop, 1);
	spBkgTop->setPosition(ccp(size.width / 2, size.height - spBkgTop->getContentSize().height / 2));

	//金币放置最上方商城

	CCSprite *coinIcon = CCSprite::create("ui/shared/coin.png");
	this->addChild(coinIcon, 2, 1429);
	coinIcon->setPosition(ccp(280 * gScaleX, size.height - 30 * gScaleY));

	CCSprite *starIcon = CCSprite::create("ui/levelChoice/star.png");
	this->addChild(starIcon,2);
	starIcon->setPosition(ccp(160 * gScaleX, size.height - 30 * gScaleY));

	CCMenu *menu = CCMenu::create();

	string coinStr = GameConfigData::initialGameData().getCoinValueStr();
	//总金币标签
	m_coinLabel = CCLabelBMFont::create("1000000000000", GOLD_FONT);
	m_coinLabel->setString(coinStr.c_str());
	this->addChild(m_coinLabel, 588);
	m_coinLabel->setAnchorPoint(ccp(0, 0.5f));
	m_coinLabel->setPosition(ccp(size.width * 5 / 6 - 100*gScaleX, size.height - 25*gScaleY));
	m_coinLabel->setScale(0.6F);


	CCLabelBMFont *starLabel = CCLabelBMFont::create("100",GOLD_FONT);
	this->addChild(starLabel,588);
	starLabel->setAnchorPoint(ccp(0, 0.5f));
	starLabel->setPosition(ccp(size.width - 290*gScaleX, size.height - 28*gScaleY));
	starLabel->setScale(0.6F);

	char startCount[64];
	sprintf(startCount,"%d",GameConfigData::initialGameData().getStartCout());
	starLabel->setString(startCount);

	CCSprite* choiceBase = CCSprite::create("ui/levelChoice/base.png");
	choiceBase->setPosition(ccp(size.width / 2, 35*gScaleY));
	choiceBase->setScale(0.75f);
	this->addChild(choiceBase, 1);

	m_menuNormal = CCMenu::create();
	this->addChild(m_menuNormal, 2);
	m_menuNormal->setPosition(CCPointZero);
	const char *filename[3] =
	{
		"ui/shared/back.png", "ui/shared/coinplus.png"
		, "ui/levelChoice/help_btn.png"
	};
	CCPoint posArray[3] = {ccp(37*gScaleX, size.height - 37*gScaleY), ccp(430*gScaleX, size.height - 30*gScaleY), ccp(440*gScaleX, 40*gScaleY)};

	for(int i = 0 ; i != 3; ++i)
	{
		CCSprite *spNml = CCSprite::create(filename[i]);
		CCSprite *spSel  = CCSprite::create(filename[i]);
		spSel->setColor(ccc3(80, 80, 80));
		CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml, spSel, this, menu_selector(LevelChoice::menuCloseCallback));
		m_menuNormal->addChild(itemSp, 2, 14 + i);
		itemSp->setPosition(posArray[i]);

	}

	const char *baseFile[3] =
	{
		"ui/levelChoice/shop.png"
		, "ui/levelChoice/hero.png"
		, "ui/levelChoice/pet.png"
	};


	CCPoint baseArray[3] = {ccp(140*gScaleX, 35*gScaleY), ccp(240*gScaleX, 35*gScaleY), ccp(340*gScaleX, 35*gScaleY)};

	for(int i = 0 ; i != 3; ++i)
	{
		CCSprite *spNml = CCSprite::create(baseFile[i]);
		CCSprite *spSel  = CCSprite::create(baseFile[i]);

		spSel->setColor(ccc3(80, 80, 80));
		CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml, spSel, this, menu_selector(LevelChoice::menuCloseCallback));
		m_menuNormal->addChild(itemSp, 2, 14 + i + 3);
		itemSp->setPosition(baseArray[i]);
		itemSp->setScale(0.75F);
	}
}

void LevelChoice::menuCloseCallback(CCObject* pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode* button = dynamic_cast<CCNode*>(pSender);
	int tag = button->getTag();

	switch(tag)
	{
	case 14:
		{
			back();
		}
		break;

	case 15:
	case 17:
		gotoShop();
		break;

	case 16:
		showHelp();
		break;

	case 18:
		gotoHeroInfo();
		break;

	case 19:
		gotoPetInfo();
		break;

	default:
		break;
	}
}

void LevelChoice::back()
{
	MainMenu *menuLayer = MainMenu::create();
	ToolsFun::sharedTools()->gotoscene(menuLayer);
}

void LevelChoice::gotoHelp()
{
}

void LevelChoice::gotoHeroInfo()
{
	CCScene *scene = CCScene::create();
	HeroChoiceLayer *gLayer = HeroChoiceLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LevelChoice::gotoPetInfo()
{
	CCScene *scene = CCScene::create();
	PetChoiceLayer *gLayer = PetChoiceLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LevelChoice::gotoShop()
{
	CCScene *scene = CCScene::create();
	ShopLayer *gLayer = ShopLayer::create();
	//gLayer->init();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LevelChoice::showHelp()
{
	CCScene *scene = CCScene::create();
	HelpLayer *gLayer = HelpLayer::create();
	//gLayer->init();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LevelChoice::choiceLevel(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);

	if(m_nodeMoved)
	{
		return;
	}

	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 100;
	string levelState = GameConfigData::initialGameData().getLevelStateByLevel(tag);

	GameConfigData::initialGameData().setCurLevel(tag);

	if(levelState == "0")
	{
	}
	else if(levelState == "1")
	{
		/*第一条件值达到，显示提示信息*/
		showLevelUnLockTip(tag);
	}
	else
	{
		/*第二或者第三条件值达到*/
		startGame();
	}

	DataInitial::initialDataSource().setLvTag(LEVEL);
	GameConfigData::initialGameData().writeData();
}

void LevelChoice::choiceSubLevel(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);

	if(m_nodeMoved)
	{
		return;
	}

	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 1000;
	string levelState = GameConfigData::initialGameData().getSubLevelStateByLevel(tag);

DataInitial::initialDataSource().setLvTag(SUB_LEVEL);
	GameConfigData::initialGameData().setCurSubLevel(tag);
	if(levelState == "0")
	{
		showStartCountUI(tag);
	}
	else if(levelState == "1" || levelState == "2")
	{
		//第一条件值达到，显示提示信息
		startGame();
	}

//	DataInitial::initialDataSource().setLvTag(SUB_LEVEL);
}

int rewardselcet = 0;
void LevelChoice::choiceReward(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	int m_tag_1 = 0;
	int m_tag_2 = 1;

	if(m_nodeMoved)
	{
		return;
	}

	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 10000;
	rewardselcet = tag;
	string rewardState = GameConfigData::initialGameData().getRewardStateByReward(tag);

	if(rewardState == "0")
	{
		showRewardUI(tag, m_tag_1);
	}
	else if(rewardState == "1")
	{
		showRewardUI(tag, m_tag_2);
	}
}

void LevelChoice::setCurrentLevel(int level)
{

}

void LevelChoice::startGame()
{
	CCScene *scene = CCScene::create();
	PrepareLayer *prepareLayer = PrepareLayer::create();
	scene->addChild(prepareLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LevelChoice::showLevelUnLockTip(int level)
{
	LevelUnLock *unLockData = DataInitial::initialDataSource().getLeveUnLockInf(level);

	if(unLockData)
	{
		switch(unLockData->mUnLockType)
		{
		case RMB_UNLOCK:
			/*弹出激活的提示框*/
			{
				if(GameConfigData::initialGameData().getActiveState() == 0)
				{
					showActiveUI();
				}
				else
				{
					startGame();
				}

			}
			break;

		case HERO_LEVEL:
			{
				int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();
				int level = unLockData->mUnLockValue0;

				if(heroLevel < level)
				{
					showLevelTip(level);
				}
				else
				{
					startGame();
				}
			}
			break;

		default:
			break;
		}
	}
}

void LevelChoice::initUnLockLevel()
{
	vector<string> levelState = GameConfigData::initialGameData().getLevelState();

	for(int i = 0; i != levelState.size(); ++i)
	{
		if(levelState[i] != "0")
		{
			m_menu->getChildByTag(100 + i)->removeChildByTag(123);
		}
	}
}
void LevelChoice::initUnLockSubLevel()
{
	vector<string> sublevelState = GameConfigData::initialGameData().getSubLevelState();

	for(int i = 0; i != sublevelState.size(); ++i)
	{
		if(sublevelState[i] != "0")
		{
			m_menu->getChildByTag(1000 + i)->removeChildByTag(123);
		}
	}
}

void LevelChoice::initRewardLevel()
{
	vector<string> rewardlevelState = GameConfigData::initialGameData().getRewardState();

	for(int i = 0; i != rewardlevelState.size(); ++i)
	{
		if(rewardlevelState[i] == "2")
		{
		//	m_menu->removeChildByTag(10000 + i);
		}
	}
}

void LevelChoice::showActiveUI()
{
	setAllTouchEnabled(false);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkgSprite = CCSprite::create("ui/shared/tipBkg0.png");
	this->addChild(bkgSprite, 3);
	bkgSprite->setPosition(size / 2);

	CCLabelBMFont *tipLanguage1 = CCLabelBMFont::create(gGetLanguageDesc(K.HERO_UPGRADE_TIP3).c_str(), "font/hero_upgrade_tip.fnt");
	tipLanguage1->setAnchorPoint(CCPointZero);
	tipLanguage1->setScale(LEVEL_SCALE);
	bkgSprite->addChild(tipLanguage1, 1);
	tipLanguage1->setPosition(ccp(bkgSprite->getContentSize().width / 2-100*gScaleX, bkgSprite->getContentSize().height / 2 + 10*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkgSprite->addChild(menu, 1);
	menu->setPosition(CCPointZero);

	const char* filename[2] = {"ui/shared/continueGame.png", "ui/shared/closeTipNml.png"};
	CCPoint posArray[2] = {ccp(bkgSprite->getContentSize().width / 2, 10*gScaleY), ccp(bkgSprite->getContentSize().width * 5 / 6, bkgSprite->getContentSize().height * 6 / 7),};

	for(int i = 0; i != 2; ++i)
	{
		CCSprite *spriteNml = CCSprite::create(filename[i]);
		CCSprite *spriteSel = CCSprite::create(filename[i]);
		spriteSel->setColor(ccc3(100, 100, 100));
		spriteSel->setOpacity(100);
		CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::activeTouch));
		menu->addChild(itemSprite, 1, 100 + i);
		itemSprite->setPosition(posArray[i]);
	}
}

void LevelChoice::showLevelTip(int level)
{
	setAllTouchEnabled(false);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkgSprite = CCSprite::create("ui/shared/tipBkg0.png");
	this->addChild(bkgSprite, 3);
	bkgSprite->setPosition(size / 2);

	CCPoint offPoint = ccp(70*gScaleX,0);

	CCLabelBMFont *tipLanguage1 = CCLabelBMFont::create(gGetLanguageDesc(K.HERO_UPGRADE_TIP0).c_str(), "font/hero_upgrade_tip.fnt");
	tipLanguage1->setAnchorPoint(CCPointZero);
	tipLanguage1->setScale(0.8f);
	bkgSprite->addChild(tipLanguage1, 1);
	tipLanguage1->setPosition(ccp(bkgSprite->getContentSize().width / 2-220*gScaleX, bkgSprite->getContentSize().height / 2 + 10*gScaleY)+offPoint);

	offPoint = ccp(0,0);

	char buf[33];
	sprintf(buf, "%d", level);

	CCLabelBMFont *tipLanguage2 = CCLabelBMFont::create(buf, "font/hero_upgrade_tip.fnt");
	tipLanguage2->setAnchorPoint(CCPointZero);
	tipLanguage2->setScale(0.8f);
	bkgSprite->addChild(tipLanguage2, 1);
	if(level<10)
	{
		tipLanguage2->setPosition(ccp(tipLanguage1->getPositionX()+tipLanguage1->getContentSize().width-10,
			bkgSprite->getContentSize().height / 2 + 10*gScaleY));
	}
	else
	{
		tipLanguage2->setPosition(ccp(tipLanguage1->getPositionX()+tipLanguage1->getContentSize().width-20,
			bkgSprite->getContentSize().height / 2 + 10*gScaleY));
	}

	CCLabelBMFont *tipLanguage3 = CCLabelBMFont::create(gGetLanguageDesc(K.HERO_UPGRADE_TIP1).c_str(), "font/hero_upgrade_tip.fnt");
	tipLanguage3->setAnchorPoint(CCPointZero);
	tipLanguage3->setScale(0.8f);
	bkgSprite->addChild(tipLanguage3, 1);


	if(level<10)
	{
		tipLanguage3->setPosition(ccp(tipLanguage1->getPositionX()+tipLanguage1->getContentSize().width+20,
			bkgSprite->getContentSize().height / 2 + 10*gScaleY));
	}
	else
	{
		tipLanguage3->setPosition(ccp(tipLanguage1->getPositionX()+tipLanguage1->getContentSize().width+20,
			bkgSprite->getContentSize().height / 2 + 10*gScaleY));
	}

	CCLabelBMFont *tipLanguage4 = CCLabelBMFont::create(gGetLanguageDesc(K.HERO_UPGRADE_TIP2).c_str(), "font/hero_upgrade_tip.fnt");
	tipLanguage4->setAnchorPoint(CCPointZero);
	tipLanguage4->setScale(0.8f);
	bkgSprite->addChild(tipLanguage4, 1);
	tipLanguage4->setPosition(ccp(bkgSprite->getContentSize().width / 2-60*gScaleX, bkgSprite->getContentSize().height / 4 + 40*gScaleY));
	
	CCMenu *menu = CCMenu::create();
	bkgSprite->addChild(menu, 1);
	menu->setPosition(CCPointZero);

	const char* filename[2] = {"ui/shared/continueGame.png", "ui/shared/closeTipNml.png"};
	CCPoint posArray[2] = {ccp(bkgSprite->getContentSize().width / 2, 10*gScaleY), ccp(bkgSprite->getContentSize().width * 5 / 6, bkgSprite->getContentSize().height * 6 / 7),};

	for(int i = 0; i != 2; ++i)
	{
		CCSprite *spriteNml = CCSprite::create(filename[i]);
		CCSprite *spriteSel = CCSprite::create(filename[i]);
		spriteSel->setColor(ccc3(100, 100, 100));
		spriteSel->setOpacity(100);
		CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::levelTipTouch));
		menu->addChild(itemSprite, 1, 100 + i);
		itemSprite->setPosition(posArray[i]);
	}


}

void  LevelChoice::showStartCountUI(int level)
{
	LevelUnLock *unLockSubLevel = DataInitial::initialDataSource().getSubLevelUnLockInf(level);

	if(unLockSubLevel)
	{
		setAllTouchEnabled(false);
		CCSize size = ToolsFun::sharedTools()->getSceneSize();
		CCSprite *bkgSprite = CCSprite::create("ui/shared/tipBkg0.png");
		this->addChild(bkgSprite, 3);
		bkgSprite->setPosition(size / 2);


		CCSprite *spriteHead = CCSprite::create("ui/tipUI/heroSmile.png");
		bkgSprite->addChild(spriteHead);
		spriteHead->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height * 6 / 7));


		CCLabelBMFont *tipLanguage0 = CCLabelBMFont::create(gGetLanguageDesc(K.UN_LOCK0).c_str(), "font/golden_36.fnt");
		bkgSprite->addChild(tipLanguage0, 1);
		tipLanguage0->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height / 2 + 10*gScaleY));

		CCLabelBMFont *tipLanguage1 = CCLabelBMFont::create(gGetLanguageDesc(K.UN_LOCK1).c_str(), "font/white_30.fnt");
		bkgSprite->addChild(tipLanguage1, 1);
		tipLanguage1->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height / 5 + 10*gScaleY));

		CCSprite *start = CCSprite::create("ui/tipUI/gainStart.png");
		bkgSprite->addChild(start, 1);
		start->setPosition(ccp(bkgSprite->getContentSize().width / 2 - 80*gScaleX, bkgSprite->getContentSize().height / 3 + 20*gScaleY));
		int startCount = GameConfigData::initialGameData().getStartCout();
		char startCountStr[33];

		// modify zh star
		sprintf(startCountStr, "%d", startCount);
		CCLabelBMFont *curStartCount = CCLabelBMFont::create(startCountStr, "font/gold_big.fnt");
		bkgSprite->addChild(curStartCount, 1);
		curStartCount->setAnchorPoint(ccp(0, 0.5f));
		curStartCount->setPosition(ccp(start->getPositionX() + start->getContentSize().width / 2 + 5*gScaleX, start->getPositionY()));

		char levelStr[33];
		sprintf(levelStr, "/%d", unLockSubLevel->mUnLockValue0);
		CCLabelBMFont *startCountLabel = CCLabelBMFont::create(levelStr, "font/gold_big.fnt");
		bkgSprite->addChild(startCountLabel, 1);
		startCountLabel->setAnchorPoint(ccp(0, 0.5f));
		startCountLabel->setPosition(ccp(curStartCount->getPositionX() + curStartCount->getContentSize().width, curStartCount->getPositionY()));

		CCMenu *menu = CCMenu::create();
		bkgSprite->addChild(menu, 1);
		menu->setPosition(CCPointZero);
		const char* filename[2] = {"ui/shared/closeTipNml.png", "ui/shared/closeTipNml.png"};
		CCPoint posArray[2] = {ccp(bkgSprite->getContentSize().width * 4 / 5, bkgSprite->getContentSize().height * 6 / 7), ccp(bkgSprite->getContentSize().width - 40*gScaleX, bkgSprite->getContentSize().height - 40*gScaleY)};

		for(int i = 0; i != 1; ++i)
		{
			CCSprite *spriteNml = CCSprite::create(filename[i]);
			CCSprite *spriteSel = CCSprite::create(filename[i]);
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);
			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::subLevelTouch));
			menu->addChild(itemSprite, 1, 100 + i);
			itemSprite->setPosition(posArray[i]);
		}
	}
}

void  LevelChoice::showRewardUI(int rewardId, int m_tag)
{
	RewardConfig *rewardConfig = DataInitial::initialDataSource().getRewardInf(rewardId);

	if(rewardConfig)
	{
		setAllTouchEnabled(false);
		CCSize size = ToolsFun::sharedTools()->getSceneSize();
		CCSprite *bkgSprite = CCSprite::create("ui/shared/tipBkg0.png");
		this->addChild(bkgSprite, 3, REWARD);
		bkgSprite->setPosition(size / 2);

		CCLabelBMFont *tipLanguage0 = CCLabelBMFont::create(gGetLanguageDesc(K.REWARD_TIP0).c_str(), "font/golden_36.fnt");
		bkgSprite->addChild(tipLanguage0, 1);
		tipLanguage0->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height * 2 / 3 + 50*gScaleY));

		int itemId = 0;
		vector<string> checkItemIdByFileName;

		string REWARD_FILE0Nml = "ui/levelChoice/reward0Nml.png";
		string REWARD_FILE1Nml = "ui/levelChoice/reward1Nml.png";
		string REWARD_FILE2Nml = "ui/levelChoice/reward2Nml.png";
		string REWARD_FILE3Nml = "ui/levelChoice/reward3Nml.png";
		checkItemIdByFileName.push_back(REWARD_FILE0Nml);
		checkItemIdByFileName.push_back(REWARD_FILE1Nml);
		checkItemIdByFileName.push_back(REWARD_FILE2Nml);
		checkItemIdByFileName.push_back(REWARD_FILE3Nml);

		for(int i = 0; i != checkItemIdByFileName.size(); ++i)
		{
			if(checkItemIdByFileName[i] == rewardConfig->mRewardNmlFile)
			{
				itemId = i;
			}
		}

		char rewardIconFile[33];

		if(itemId ==0 || itemId ==3)
		{
			sprintf(rewardIconFile, "ui/tipUI/rwardIcon%d.png", itemId);
		}
		else if(itemId ==1 || itemId==2)
		{
			sprintf(rewardIconFile, "ui/levelChoice/reward%dNml.png", itemId);
		}

		//sprintf(rewardIconFile, "ui/levelChoice/reward%dNml.png", itemId);

		CCSprite *itemSprite = CCSprite::create(rewardIconFile);
		bkgSprite->addChild(itemSprite, 1);
		itemSprite->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height / 2));

		CCLabelBMFont *Xc = CCLabelBMFont::create("x", "font/golden_36.fnt");
		bkgSprite->addChild(Xc, 12222);
		Xc->setPosition(ccp(itemSprite->getContentSize().width / 2 + itemSprite->getPositionX() + 20*gScaleX, bkgSprite->getContentSize().height / 3 + 30*gScaleY));

		char iCount[33];
		sprintf(iCount, "%d", rewardConfig->mRewardValue);
		CCLabelBMFont *itemCount = CCLabelBMFont::create(iCount, "font/price_30.fnt");
		bkgSprite->addChild(itemCount, 233);
		itemCount->setAnchorPoint(ccp(0, 0.5f));
		itemCount->setPosition(ccp(itemSprite->getContentSize().width / 2 + itemSprite->getPositionX() - 20*gScaleX, bkgSprite->getContentSize().height / 3 + 30*gScaleY) + ccp(40*gScaleX, 10*gScaleY));
		//itemCount->setPosition(ccp(300,400));


		CCMenu *menu = CCMenu::create();
		bkgSprite->addChild(menu, 1);
		menu->setPosition(CCPointZero);
		const char* filename[2] = {"ui/shared/closeTipNml.png", "ui/shared/closeTipNml.png"};
		CCPoint posArray[2] = {ccp(bkgSprite->getContentSize().width * 4 / 5, bkgSprite->getContentSize().height * 6 / 7), ccp(bkgSprite->getContentSize().width - 40*gScaleX, bkgSprite->getContentSize().height - 40*gScaleY)};

		for(int i = 0; i != 1; ++i)
		{
			CCSprite *spriteNml = CCSprite::create(filename[i]);
			CCSprite *spriteSel = CCSprite::create(filename[i]);
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);
			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::subLevelTouch));
			menu->addChild(itemSprite, 1, 100 + i);
			itemSprite->setPosition(posArray[i]);
		}

		if(m_tag)
		{
			const char* filename[1] = {"ui/shared/continueGame.png"};
			CCPoint posArray[1] = {ccp(bkgSprite->getContentSize().width / 2, 10*gScaleY)};

			CCSprite *spriteNml = CCSprite::create(filename[0]);
			CCSprite *spriteSel = CCSprite::create(filename[0]);
			spriteSel->setColor(ccc3(100, 100, 100));
			spriteSel->setOpacity(100);
			CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LevelChoice::getReward));
			menu->addChild(itemSprite, 1, 100);
			//  itemSprite->setScale(0.6f);
			itemSprite->setPosition(posArray[0]);
		}
		else
		{
			CCLabelBMFont *tipLanguage1 = CCLabelBMFont::create(gGetLanguageDesc(K.REWARD_TIP1).c_str(), "font/white_30.fnt");
			bkgSprite->addChild(tipLanguage1, 1);
			tipLanguage1->setPosition(ccp(bkgSprite->getContentSize().width / 2, bkgSprite->getContentSize().height / 4));
		}
	}
}


void LevelChoice::getReward(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	int tag = rewardselcet;
	GameConfigData::initialGameData().setRewardState(tag, "2");
	GameConfigData::initialGameData().writeData();
	initRewardLevel();
	RewardConfig *rewardConfig = DataInitial::initialDataSource().getRewardInf(tag);
	string REWARD_FILE0Nml = "ui/levelChoice/reward0Nml.png";
	string REWARD_FILE1Nml = "ui/levelChoice/reward1Nml.png";
	string REWARD_FILE2Nml = "ui/levelChoice/reward2Nml.png";
	string REWARD_FILE3Nml = "ui/levelChoice/reward3Nml.png";

	//根据初始化图片得到对应的物品
	if(rewardConfig->mRewardNmlFile == REWARD_FILE0Nml)
	{
		GameConfigData::initialGameData().setPetState(0, "1");
	}
	else if(rewardConfig->mRewardNmlFile == REWARD_FILE1Nml)
	{
		int curbomb = GameConfigData::initialGameData().getBombValue() ;
		curbomb += rewardConfig->mRewardValue;
		GameConfigData::initialGameData().setBombValue(curbomb);
	}
	else if(rewardConfig->mRewardNmlFile == REWARD_FILE2Nml)
	{
		int currevive = GameConfigData::initialGameData().getReviveValue() ;
		currevive += rewardConfig->mRewardValue;
		GameConfigData::initialGameData().setReviveValue(currevive);
	}
	else if(rewardConfig->mRewardNmlFile == REWARD_FILE3Nml)
	{
		int curcoin = GameConfigData::initialGameData().getCoinValue() ;
		curcoin += rewardConfig->mRewardValue;
		GameConfigData::initialGameData().setCoinValue(curcoin);	
		setCoin();
	}
	if(this->getChildByTag(REWARD))
	{
		this->removeChildByTag(REWARD);
		setAllTouchEnabled(true);
	}
	GameConfigData::initialGameData().writeData();
}

void LevelChoice::levelTipTouch(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 100;

	if(tag)
	{
		node->getParent()->getParent()->removeFromParentAndCleanup(true);
		setAllTouchEnabled(true);
	}
	else
	{
		CCScene *scene = CCScene::create();
		PrepareLayer *prepareLayer = PrepareLayer::create();
		prepareLayer->setStartGameEnable(false);
		scene->addChild(prepareLayer);
		CCDirector::sharedDirector()->replaceScene(scene);
	}
}

void LevelChoice::activeTouch(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 100;

	if(tag)
	{
		node->getParent()->getParent()->removeFromParentAndCleanup(true);
		setAllTouchEnabled(true);
	}
	else
	{
		node->getParent()->getParent()->removeFromParentAndCleanup(true);
		setAllTouchEnabled(true);
		JniCall::sharedJniCall()->setBuyFailedCallBack(this, callFunStr_selector(LevelChoice::purchaseFailed));
		JniCall::sharedJniCall()->setBuyCompleteCallBack(this, callFunStr_selector(LevelChoice::purchaseSuccess));
		JniCall::sharedJniCall()->purchaseByIndex(10);

	}
}

void LevelChoice::purchaseSuccess(const char* str)
{
if(strcmp(str,"10") ==0)
	{
		GameConfigData::initialGameData().setActiveState(1);
		startGame();
	}
	else if(strcmp(str,"0") ==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=3000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);

		GameConfigData::initialGameData().setHeroStateInt(1 , "1");
	}
	else if(strcmp(str,"1") ==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
		GameConfigData::initialGameData().setHeroStateInt(2 , "1");
	}
	else if(strcmp(str,"6") ==0)
	{
	
		int count = GameConfigData::initialGameData().getLuckyCount();
		GameConfigData::initialGameData().setLuckyCount(count+30);

		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
	}
	else if(strcmp(str,"2") ==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=3000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);

		GameConfigData::initialGameData().setPetState(3,"1");
		GameConfigData::initialGameData().setPetPos(3,-1);
	}
	else if(strcmp(str,"3") ==0)
	{

		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);

		GameConfigData::initialGameData().setPetState(4,"1");
		GameConfigData::initialGameData().setPetPos(4,-1);
	}

	this->removeChildByTag(12313133);

	GameConfigData::initialGameData().writeData();
}

void LevelChoice::purchaseFailed(const char* str)
{
}

void LevelChoice::subLevelTouch(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag() - 100;

	node->getParent()->getParent()->removeFromParentAndCleanup(true);
	setAllTouchEnabled(true);
}

void LevelChoice::setAllTouchEnabled(bool state)
{
	this->setTouchEnabled(state);
	m_menu->setTouchEnabled(state);
}


void LevelChoice::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
}

bool LevelChoice::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_pauseMove)
		return false;

	m_nodeMoved = false;
	CCPoint pos = pTouch->getLocation();
	m_curPos = pos;
	m_lastPos = pos;
	m_curTime = time(NULL);
	m_lastPosY = m_uiNode->getPositionY();
	unschedule(schedule_selector(LevelChoice::updatePosTouchEnd));
	unschedule(schedule_selector(LevelChoice::endSlid));
	return true;
}

void LevelChoice::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	m_curPos = pTouch->getLocation();

	if((m_curPos - m_lastPos).getLength() < 12)
	{
		return ;
	}
	else
	{
		m_nodeMoved = true;
		float posY = m_uiNode->getPositionY() + m_curPos.y - m_lastPos.y;

		if(posY < -3 * size.height)  //-160 )
		{
			m_uiNode->setPositionY(-3 * size.height); // -160 );
			return;
		}
		else if(posY > 0)
		{
			m_uiNode->setPositionY(0);
			return;
		}

		m_uiNode->setPositionY(posY);
		m_lastPos = m_curPos;
	}
}

void LevelChoice::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(!m_nodeMoved)
	{
		return;
	}

	long curTime = time(NULL);
	float curPosY = m_uiNode->getPositionY();

	if(curTime == m_curTime)
	{
		if(curPosY - m_lastPosY > 0)
		{
			m_curEndSpeed = 700;
		}
		else if(curPosY - m_lastPosY < 0)
		{
			m_curEndSpeed = -700;
		}
		else
		{
			return;
		}
	}
	else
	{
		m_curEndSpeed = (curPosY - m_lastPosY) / (curTime - m_curTime);
		m_curEndSpeed *= 1.30f;
	}

	if(fabs(m_curEndSpeed) > 80)
	{

		schedule(schedule_selector(LevelChoice::updatePosTouchEnd));
		scheduleOnce(schedule_selector(LevelChoice::endSlid), 0.5f);
	}
}

void LevelChoice::updatePosTouchEnd(float t)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	m_curEndSpeed = m_curEndSpeed ;
	double xMove = m_curEndSpeed * t;
	float posY = m_uiNode->getPositionY() + xMove;

	if(posY < -3 * size.height)  //-160 )
	{
		m_uiNode->setPositionY(-3 * size.height); // -160 );
		return;
	}
	else if(posY > 0)
	{
		m_uiNode->setPositionY(0);
		return;
	}

	m_uiNode->setPositionY(posY);
}

void LevelChoice::endSlid(float t)
{
	unschedule(schedule_selector(LevelChoice::updatePosTouchEnd));
}

void LevelChoice::tipMoveEnd(CCNode *node)
{
	node->setPositionY(node->getPositionY() + 50 * gScaleY);
	CCAction * move = CCMoveTo::create(0.5, ccp(node->getPositionX(), node->getPositionY() + 80 * gScaleY));
	CCAction * move1 = CCMoveTo::create(0.5, ccp(node->getPositionX(), node->getPositionY() + 30 * gScaleY));
	CCArray * arr = CCArray::create();
	arr->addObject(move);
	arr->addObject(move1);
	CCSequence * sep = CCSequence::create(arr);
	node->runAction(CCRepeatForever::create(sep));
}


void LevelChoice::dailyPurchase()
{
	/*
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(LevelChoice::hideDailyPurchase), "hidePurchase", NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(LevelChoice::closeDailyPurchase), "closePurchase", NULL);

	DailyPurchaseLayer *layer = DailyPurchaseLayer::create();
	addChild(layer, 1000);
	m_menu->setEnabled(false);*/
}

void LevelChoice::hideDailyPurchase(CCObject *obj)
{
	m_menu->setEnabled(true);
	//m_menu->getChildByTag(203)->setVisible(false);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "closePurchase");
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "hidePurchase");

}

void LevelChoice::closeDailyPurchase(CCObject *obj)
{
	m_menu->setEnabled(true);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "closePurchase");
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "hidePurchase");
}

void LevelChoice::gotoDailyLogIn()
{
	m_menu->setEnabled(false);
	DailyLogin *layer = DailyLogin::create();
	this->addChild(layer, 6166);
	layer->setCloseCallBack(this, menu_selector(LevelChoice::backFromDailyLogIn));
}

void LevelChoice::backFromDailyLogIn(CCObject *pSender)
{
	if(GameConfigData::initialGameData().getDailyPurchase() == 1)
	{
		dailyPurchase();
	}
	else
	{
		m_menu->setEnabled(true);
	}
	setCoin();
}

void LevelChoice::setCoin()
{
	string coinStr = GameConfigData::initialGameData().getCoinValueStr();
	m_coinLabel->setString(coinStr.c_str());
}

void LevelChoice::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(LevelChoice::goGamble));
	this->addChild(layer,10000);
}

void LevelChoice::goGamble()
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}