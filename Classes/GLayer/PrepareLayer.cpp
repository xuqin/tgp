#include "PrepareLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "../GCData/DataInitial.h"
#include "GameLayer.h"
#include "LevelChoice.h"
#include "StartGameLoading.h"
#include "PetChoiceLayer.h"
#include "HeroChoice.h"
#include "ShopLayer.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "../SpriteNode/PetSprite.h"

#define PRE_SCALE 0.6f

PrepareLayer::PrepareLayer():m_moneyLabel(NULL)
	,m_nextLevelPriceLabel(NULL)
	,m_curHeroLevel(NULL)
	,m_heroHeadSp(NULL)
	,m_heroSprite(NULL)
	,m_menu(NULL)
	,m_curShowHeroId(-1)
	,m_heroNameLabel(NULL)
	,m_leftPitId(-1)
	,m_rightPetId(-1)
{
}

PrepareLayer::~PrepareLayer()
{

}

bool PrepareLayer::init()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	//背景
	CCSprite *spBkg = CCSprite::create("ui/shared/bkg.png");
	this->addChild(spBkg,-1);
	spBkg->setPosition(size/2);

	//放置人物设定背景
	CCSprite *spBkgHero = CCSprite::create("ui/prepareUI/mainbkg.png");
	this->addChild(spBkgHero,-1);
	spBkgHero->setPosition(ccp(size.width/2,size.height - spBkgHero->getContentSize().height/2 - 50*gScaleY));

	//购物
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop,0,108);
	spBkgTop->setPosition(ccp(size.width/2,size.height - spBkgTop->getContentSize().height/2));

	//人物游戏记录背景
	CCSprite *spRecoreBkg = CCSprite::create("ui/prepareUI/recordbkg.png");
	this->addChild(spRecoreBkg,-1);
	spRecoreBkg->setPosition(ccp(315*gScaleX,600*gScaleY));
	//spRecoreBkg->setScale(PRE_SCALE);

	//宠物，道具背景
	for (int i=0;i!=1;++i)
	{
		CCSprite *sp = CCSprite::create("ui/shared/placeboard.png");
		this->addChild(sp,-1);
		sp->setPosition(ccp(size.width/4 + i * size.width/2,165*gScaleY));
		sp->setScale(1.10f);
	}

	int level = GameConfigData::initialGameData().getCurLevel();
	int killTotal = 0;

	killTotal = atoi( GameConfigData::initialGameData().getKillTotal().c_str());

	char buf[64];
	sprintf(buf,"%d",killTotal);

	int levelMaxStr = GameConfigData::initialGameData().getMaxUnLockLevel();
	string strs[4] = 
	{
		GameConfigData::initialGameData().getHScores(),
		GameConfigData::initialGameData().getHLenght(),
		string(buf),
		ToolsFun::sharedTools()->intToString(levelMaxStr)
	};

	for (int i=0;i!=4;++i)
	{
		CCLabelBMFont *hScores = CCLabelBMFont::create(strs[i].c_str() ,"font/white_30.fnt");
		spRecoreBkg->addChild(hScores);
		hScores->setPosition(ccp(150*gScaleX,(165-8-38*i))*gScaleY);
		hScores->setAnchorPoint(ccp(0,0.5f));
	}

	m_menu = CCMenu::create();
	this->addChild(m_menu,1);
	m_menu->setPosition(CCPointZero);

	//所有按钮的放置
	const char* fName[9] = 
	{
		"ui/shared/back.png",//返回
		"ui/shared/coinplus.png",//添加金币
		"ui/prepareUI/head.png",//主角头像
		"ui/prepareUI/upgrade.png",//升级按钮
		"ui/shared/shop_item.png",//宠物1
		"ui/shared/shop_item.png",//宠物2
		"ui/shared/shop_item.png",//道具1
		//	"ui/shared/shop_item.png",//道具2
		"ui/shared/confirm.png"//确定开始游戏
	};

	const CCPoint posArray[9] = 
	{
		ccp(37*gScaleX,size.height-37*gScaleY),ccp(430*gScaleX,size.height-30*gScaleY),ccp(290*gScaleX,435*gScaleY),ccp(360*gScaleX,335*gScaleY),
		ccp(54*gScaleX,145*gScaleY),ccp(144*gScaleX,145*gScaleY),ccp(355*gScaleX,145*gScaleY),ccp(235*gScaleX,40*gScaleY),ccp(235*gScaleX,40*gScaleY)
	};

	for (int i=0;i!=8;++i)
	{
		CCSprite *spNml = CCSprite::create(fName[i]);
		CCSprite *spSel = CCSprite::create(fName[i]);
		spSel->setColor(ccc3(80,80,80));
		CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,
			this,menu_selector(PrepareLayer::menuTouch));
		if(i==2)
		{
			m_menu->addChild(itemSp,1,100+i);
			itemSp->setPosition(posArray[i]);
			itemSp->setScale(0.8f);
		}
		else if(i ==3)
		{
			m_menu->addChild(itemSp,1,100+i);
			itemSp->setPosition(posArray[i]);
			itemSp->setScale(PRE_SCALE);

			CCLabelBMFont *upGrade = CCLabelBMFont::create(gGetLanguageDesc(K.UPGRADE_DES).c_str(),"font/upgrade_des.fnt");
			itemSp->addChild(upGrade);
			upGrade->setPosition(ccp(80*gScaleX,-25*gScaleY));
		}
		else
		{
			m_menu->addChild(itemSp,1,100+i);
			itemSp->setPosition(posArray[i]);
		}
	}

	int curLevel  = GameConfigData::initialGameData().getCurHeroLevel();
	if (curLevel >= HERO_MAXLEVEL)
	{
		CCMenuItemSprite *upItem = (CCMenuItemSprite*)m_menu->getChildByTag(103);
		upItem->setEnabled(false);
		upItem->setVisible(false);
		if (this->getChildByTag(1430))
		{
			this->getChildByTag(1430)->setVisible(false);
		}
	}

	//金币放置最上方商城

	CCSprite *coinIcon = CCSprite::create("ui/shared/coin.png");
	this->addChild(coinIcon,2,1429);
	coinIcon->setPosition(ccp(280 * gScaleX, size.height - 30 * gScaleY));

	m_moneyLabel = CCLabelBMFont::create("10000000","font/level_big.fnt");
	this->addChild(m_moneyLabel,4);
	m_moneyLabel->setAnchorPoint(ccp(0,0.5f));
	m_moneyLabel->setPosition(ccp(300*gScaleX,size.height-25*gScaleY));
	m_moneyLabel->setScale(0.6F);

	setCoin();

	//升级所需金币数
	m_nextLevelPriceLabel = CCLabelBMFont::create("1000000000","font/level_big.fnt");
	this->addChild(m_nextLevelPriceLabel,2);
	m_nextLevelPriceLabel->setPosition(ccp(345*gScaleX,342*gScaleY));
	m_nextLevelPriceLabel->setScale(0.45);

	int curHeroId = GameConfigData::initialGameData().getCurHeroId();
	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();

	//名字label
	m_heroNameLabel = CCLabelBMFont::create("name","font/hero_name.fnt");
	this->addChild(m_heroNameLabel,10);
	m_heroNameLabel->setPosition(ccp(390*gScaleX,475*gScaleY));
	m_heroNameLabel->setScale(0.8f);

	//主角现在等级
	CCSprite* curHeroLevel=CCSprite::create("ui/prepareUI/level.png");
	this->addChild(curHeroLevel,2);
	curHeroLevel->setPosition(ccp(370*gScaleX,445*gScaleY));
	curHeroLevel->setScale(0.8f);

	//数字改变部分
	m_curHeroLevel = CCLabelBMFont::create("10000000",GOLD_FONT);
	this->addChild(m_curHeroLevel,2);
	m_curHeroLevel->setPosition(ccp(420*gScaleX,445*gScaleY));
	string levelStr = ToolsFun::sharedTools()->intToString(heroLevel);
	m_curHeroLevel->setString((levelStr).c_str());
	m_curHeroLevel->setScale(0.8f);

	//点击头像改变
	CCSprite* changeName=CCSprite::create("ui/prepareUI/changename.png");
	this->addChild(changeName,2);
	changeName->setPosition(ccp(390*gScaleX,400*gScaleY));
	changeName->setScale(0.8f);

	CCSprite *itemBkg = CCSprite::create("ui/shared/itemBkg.png");
	this->addChild(itemBkg);
	itemBkg->setPosition(ccp(360,145));
	initPet(0);
	initPet(1);
	initHeroSp(GameConfigData::initialGameData().getCurHeroId());
	initItem(0);
	//initItem(1);

	playUpEffect(heroLevel,false);

	registerNDK();

	return true;
}

void PrepareLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}


void PrepareLayer::setStartGameEnable(bool enable)
{
	if(enable==false)
	{
		m_menu->getChildByTag(107)->setVisible(false);
	}
	else
	{
		m_menu->getChildByTag(107)->setVisible(true);
	}
}

void PrepareLayer::initPet(int leftOrRight)
{
	///**初始化已经出战的左边的宠物**///
	int petId = -1;
	for (int i =0;i !=6; ++i )
	{
		if (GameConfigData::initialGameData().getPetStateById(i) == "2")
		{
			if (GameConfigData::initialGameData().getPetPosById(i) == leftOrRight)
			{
				petId = i;
			}
		}
	}

	CCMenuItemSprite *petAddLeft = (CCMenuItemSprite *)m_menu->getChildByTag(100+4+leftOrRight);
	if (petAddLeft->getChildByTag(1000))
	{
		petAddLeft->removeChildByTag(1000);
	}

	CCPoint leftPoint[6] = {petAddLeft->getContentSize()/2+ccp(20*gScaleX,0),petAddLeft->getContentSize()/2+ccp(20*gScaleX,0),petAddLeft->getContentSize()/2+ccp(30*gScaleX,0),
		petAddLeft->getContentSize()/2+ccp(30*gScaleX,0),petAddLeft->getContentSize()/2+ccp(30*gScaleX,0),petAddLeft->getContentSize()/2+ccp(30*gScaleX,0)};
	float scale[6] = {0.4*PRE_SCALE,0.4*PRE_SCALE,0.5*PRE_SCALE,0.5*PRE_SCALE,0.5*PRE_SCALE,0.5*PRE_SCALE};

	CCSprite *sp = NULL;
	if (petId != -1)
	{
		char petFile[48];
		sprintf(petFile,"ui/petUI/pet%d_0.png",petId);
		sp = CCSprite::create(petFile);
		sp->setScale(scale[petId]);
		if(leftOrRight ==0)
		{
			sp->setPosition(leftPoint[petId]);
			m_leftPitId = petId;
		}
		else
		{
			sp->setPosition(leftPoint[petId]);
			m_rightPetId  =petId;
		}

		showHeroPet(leftOrRight,petId);
	}
	else
	{
		sp = CCSprite::create("ui/shared/plus.png");	
		sp->setPosition(petAddLeft->getContentSize()/2+ccp(20*gScaleX,0));

		if(leftOrRight ==0)
		{
			if(this->getChildByTag(45456)!=NULL)
			{
				this->getChildByTag(45456)->setVisible(false);
			}
		}
		else
		{
			if(this->getChildByTag(45457)!=NULL)
			{
				this->getChildByTag(45457)->setVisible(false);
			}
		}
	}

	petAddLeft->addChild(sp,1,1000);
}

//显示hero边上的宠物 0为左侧 1为右侧
void PrepareLayer::showHeroPet(int leftOrRight,int petId)
{
	float scale[6] = {0.2f*PRE_SCALE,0.2f*PRE_SCALE,0.2f*PRE_SCALE,0.2f*PRE_SCALE,0.3f*PRE_SCALE,0.3f*PRE_SCALE};

	CCPoint leftPetPos[6] = {ccp(70*gScaleX,360*gScaleY),ccp(70*gScaleX,360*gScaleY),ccp(70*gScaleX,360*gScaleY),
		ccp(70*gScaleX,360*gScaleY),ccp(70*gScaleX,360*gScaleY),ccp(70*gScaleX,360*gScaleY)};

	CCPoint rightPetPos[6] ={ccp(220*gScaleX,360*gScaleY),ccp(220*gScaleX,360*gScaleY),ccp(215*gScaleX,360*gScaleY),
		ccp(215*gScaleX,360*gScaleY),ccp(215*gScaleX,360*gScaleY),ccp(215*gScaleX,360*gScaleY)};

	if(leftOrRight ==0)
	{
		char fileName[64];
		sprintf(fileName, "petAni/petAni%d.png", petId);

		this->removeChildByTag(45456);

		CCSprite *sp = CCSprite::create(fileName);
		sp->setScale(scale[petId]);
		sp->setPosition(leftPetPos[petId]);
		this->addChild(sp,0,45456);


		float  posx = sp->getContentSize().width;
		float  posy = sp->getContentSize().height;

		if(petId ==0 || petId==1)
		{
			CCSprite *sp1 = CCSprite::create();
			sp->addChild(sp1);
			sp1->setPosition(ccp(posx * 2 / 3, posy / 3) + ccp(-240*gScaleX, -40*gScaleY));

			CCSprite *sp2 = CCSprite::create();
			sp->addChild(sp2);
			sp2->setPosition(ccp(posx * 2 / 3, posy / 3) - ccp(0, 40*gScaleY));
			char aniName[40];
			sprintf(aniName, "petAni/petAni%d_0_%%d.png", 0);

			CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName, 2, 0.15f);
			CCAnimate *ani = CCAnimate::create(animation);
			CCAnimate *ani_1 = CCAnimate::create(animation);

			sp1->runAction(CCRepeatForever::create(ani));
			sp2->runAction(CCRepeatForever::create(ani_1));
		}
		else if(petId == 4 || petId == 5)
		{
			CCSprite *sp1 = CCSprite::create();
			sp->addChild(sp1);
			sp1->setPosition(sp->getContentSize() / 2 - ccp(110,5));

			CCSprite *sp2 = CCSprite::create();
			sp->addChild(sp2);
			sp2->setPosition(sp->getContentSize() / 2 + ccp(110, 0));

			char aniName[40];
			sprintf(aniName,"petAni/petAni%d_0_%%d.png",petId);
			CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName,2,0.12f);

			sp1->setScale(PRE_SCALE);
			sp2->setScale(PRE_SCALE);

			CCAnimate *ani = CCAnimate::create(animation);
			CCAnimate *ani_1 = CCAnimate::create(animation);
			sp1->runAction(CCRepeatForever::create(ani));
			sp2->runAction(CCRepeatForever::create(ani_1));
			sp1->setFlipX(true);
		}
	}
	else if(leftOrRight ==1)
	{
		char fileName[64];
		sprintf(fileName, "petAni/petAni%d.png", petId);

		this->removeChildByTag(45457);

		CCSprite *sp = CCSprite::create(fileName);
		sp->setScale(scale[petId]);
		sp->setPosition(rightPetPos[petId]);
		this->addChild(sp,0,45457);


		float  posx = sp->getContentSize().width;
		float  posy = sp->getContentSize().height;

		if(petId ==0 || petId==1)
		{
			CCSprite *sp1 = CCSprite::create();
			sp->addChild(sp1);
			sp1->setPosition(ccp(posx * 2 / 3, posy / 3) - ccp(180, 40*gScaleY));

			CCSprite *sp2 = CCSprite::create();
			sp->addChild(sp2);
			sp2->setPosition(ccp(posx * 2 / 3, posy / 3) - ccp(0, 40*gScaleY));
			char aniName[40];
			sprintf(aniName, "petAni/petAni%d_0_%%d.png", 0);

			CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName, 2, 0.15f);
			CCAnimate *ani = CCAnimate::create(animation);
			CCAnimate *ani_1 = CCAnimate::create(animation);

			sp1->runAction(CCRepeatForever::create(ani));
			sp2->runAction(CCRepeatForever::create(ani_1));
		}
		else if(petId == 4 || petId == 5)
		{
			CCSprite *sp1 = CCSprite::create();
			sp->addChild(sp1);
			sp1->setPosition(sp->getContentSize() / 2 - ccp(110,5));

			CCSprite *sp2 = CCSprite::create();
			sp->addChild(sp2);
			sp2->setPosition(sp->getContentSize() / 2 + ccp(110, 0));

			char aniName[40];
			sprintf(aniName,"petAni/petAni%d_0_%%d.png",petId);
			CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName,2,0.12f);

			sp1->setScale(PRE_SCALE);
			sp2->setScale(PRE_SCALE);

			CCAnimate *ani = CCAnimate::create(animation);
			CCAnimate *ani_1 = CCAnimate::create(animation);
			sp1->runAction(CCRepeatForever::create(ani));
			sp2->runAction(CCRepeatForever::create(ani_1));
			sp1->setFlipX(true);
		}
	}
}

void PrepareLayer::initItems()
{

}

void PrepareLayer::menuTouch(CCObject *pSender)
{ToolsFun::sharedTools()->playEffect(BUTTON);
CCNode *node = (CCNode*)pSender;
int tag = node->getTag() - 100;
switch(tag)
{
case 0:
	back(pSender);
	break;
case 1:
	showShopLayer(pSender);
	break;
case 2:
	showHeroLayer(pSender);
	break;
case 3:
	heroUp(pSender);
	break;
case 4:
case 5:
	showPetLayer(pSender);
	break;
case 6:
	{
		showShopLayer(pSender);
	}
	break;
case 7:
	//	showShopLayer(pSender);
	startGame(pSender);
	break;
case 8:
	startGame(pSender);
	break;
default:
	break;
}
}

void PrepareLayer::startGame(CCObject *pSender)
{
	CCScene *scene = CCScene::create();
	StartGameLoading *gLayer = StartGameLoading::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void PrepareLayer::back(CCObject *pSender)
{
	LevelChoice *ly = LevelChoice::create();
	CCScene *scene = CCScene::create();
	scene->addChild(ly);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void PrepareLayer::showShopLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(false);
	ShopLayer *shopLayer = ShopLayer::create();
	this->addChild(shopLayer,10);
	shopLayer->setCloseCallBack(this,menu_selector(PrepareLayer::backFromShopLayer));
}

void PrepareLayer::showPetLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(false);
	PetChoiceLayer *petLayer = PetChoiceLayer::create();
	this->addChild(petLayer,10);
	petLayer->setCloseCallBack(this,menu_selector(PrepareLayer::backFromPetLayer));
}

void PrepareLayer::heroUp(CCObject *pSender)
{

	int upCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("heropUp",0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("heropUp",upCount+1);

	if(upCount %3==0)
	{

		showFullUpgrade();
	}
	else
	{
		int curLevel  = GameConfigData::initialGameData().getCurHeroLevel();

		HeroUpgradeData upHeroData = DataInitial::initialDataSource().getHeroUpgrateData(m_curShowHeroId,curLevel);
		int nexPriece = upHeroData.mlevelPrice;
		if (nexPriece <= GameConfigData::initialGameData().getCoinValue())
		{
			int coinValue = GameConfigData::initialGameData().getCoinValue() - nexPriece;
			GameConfigData::initialGameData().setCoinValue(coinValue);
			curLevel++;
			GameConfigData::initialGameData().setCurHeroLevel(curLevel);
			playUpEffect(curLevel);
			if (curLevel >= HERO_MAXLEVEL)
			{
				CCMenuItemSprite *upItem = (CCMenuItemSprite*)pSender;
				upItem->setEnabled(false);
				upItem->setVisible(false);
				if (this->getChildByTag(1430))
				{
					this->getChildByTag(1430)->setVisible(false);
				}
			}
		}
		else
		{
			/*
			JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PrepareLayer::purchaseFailed));
			JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PrepareLayer::purchaseSuccess));
			JniCall::sharedJniCall()->purchaseByIndex(8);*/

			showFullUpgrade(1);
		}
	}
}

void PrepareLayer::showFullUpgrade(int type)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,200,12313133);
	bkg->setPosition(ccp(size.width/2,size.height/2));

	CCSprite *tip1 = CCSprite::create("DayReward/fullUpgrade.png");
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));

	CCMenuItemSprite *itemClose  = NULL;

	if(type ==1)
	{
		itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PrepareLayer::openBuyCoin));
	}
	else
	{
		itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PrepareLayer::showFullUpgradeClose));
	}
	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PrepareLayer::showFullUpgradeSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp);
	m_menu->setEnabled(false);
}

void PrepareLayer::openBuyCoin(CCObject *sender)
{
	this->removeChildByTag(12313133);
	showBuyCoin();
}

void PrepareLayer::showBuyCoin()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,200,12313154);
	bkg->setPosition(ccp(size.width/2,size.height/2));

	CCSprite *tip1 = CCSprite::create("DayReward/50000coin.png");
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));

	CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PrepareLayer::showBuyCoinClose));

	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PrepareLayer::showBuyCoinSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp);
}

void PrepareLayer::showBuyCoinClose(CCObject *sender)
{
	this->removeChildByTag(12313154);
	m_menu->setEnabled(true);
}

void PrepareLayer::showBuyCoinSure(CCObject *sender)
{
	//this->removeChildByTag(12313154);

	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PrepareLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PrepareLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(12);
	m_menu->setEnabled(true);
}

void PrepareLayer::showFullUpgradeClose(CCObject *sender)
{
	this->removeChildByTag(12313133);
	m_menu->setEnabled(true);
}

void PrepareLayer::showFullUpgradeSure(CCObject *sender)
{
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PrepareLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PrepareLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(8);
	m_menu->setEnabled(true);
}

void PrepareLayer::purchaseSuccess(const char* str)
{
	if(strcmp(str,"12") ==0)
	{
		this->removeChildByTag(12313154);
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
		setCoin();
	}
	else if(strcmp(str,"8") ==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=6000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
		this->removeChildByTag(12313133);
		GameConfigData::initialGameData().setCurHeroLevel(HERO_MAXLEVEL);

		int curLevel  = GameConfigData::initialGameData().getCurHeroLevel();
		if (curLevel >= HERO_MAXLEVEL)
		{
			CCMenuItemSprite *upItem = (CCMenuItemSprite *) m_menu->getChildByTag(103);
			if(upItem)
			{
				upItem->setVisible(false);
			}

			CCMenuItemSprite *sureItem = (CCMenuItemSprite *) m_menu->getChildByTag(107);
			if(sureItem)
			{
				sureItem->setVisible(true);
			}

			if (this->getChildByTag(1430))
			{
				this->getChildByTag(1430)->setVisible(false);
			}

			playUpEffect(HERO_MAXLEVEL);
		}
	}
}
void PrepareLayer::purchaseFailed(const char* str)
{
}

void PrepareLayer::playUpEffect(int level,bool upEffect)
{
	if (level < HERO_MAXLEVEL)
	{
		HeroUpgradeData upHeroDataCur = DataInitial::initialDataSource().getHeroUpgrateData(m_curShowHeroId,level);
		string price = ToolsFun::sharedTools()->intToString(upHeroDataCur.mlevelPrice);
		m_nextLevelPriceLabel->setString(price.c_str());

		string coin = GameConfigData::initialGameData().getCoinValueStr();
		m_moneyLabel->setString(coin.c_str());

		string levelStr = ToolsFun::sharedTools()->intToString(level);
		m_curHeroLevel->setString((levelStr).c_str());
		int heroLevel=GameConfigData::initialGameData().getCurHeroLevel();
		int unlockLevel=GameConfigData::initialGameData().getCurLevel();
		LevelUnLock *unLockData = DataInitial::initialDataSource().getLeveUnLockInf(unlockLevel);
		if(unLockData->mUnLockType==HERO_LEVEL)
		{
			int askedLevel=unLockData->mUnLockValue0;
			if(heroLevel>=askedLevel)
			{
				setStartGameEnable(true);
			}
		}
	}
	else
	{
		m_nextLevelPriceLabel->setVisible(false);
		string levelStr = ToolsFun::sharedTools()->intToString(level);
		m_curHeroLevel->setString((levelStr).c_str());
	}

	if (upEffect)
	{
		///*增加升级效果**///
	}

}

void PrepareLayer::backFromPetLayer(CCObject *pSender)
{
	initPet(0);
	initPet(1);
	this->m_menu->setEnabled(true);
}

void PrepareLayer::showHeroLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(false);
	HeroChoiceLayer *petLayer = HeroChoiceLayer::create();
	this->addChild(petLayer,10);
	petLayer->setCloseCallBack(this,menu_selector(PrepareLayer::backFromHeroLayer));
}

void PrepareLayer::backFromHeroLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(true);
	initHeroSp(GameConfigData::initialGameData().getCurHeroId());
}

void PrepareLayer::backFromShopLayer(CCObject *pSender)
{
	initItem(0);
	//	initItem(1);
	setCoin();
	this->m_menu->setEnabled(true);
}

void PrepareLayer::initHeroSp(int mId)
{
	if (m_curShowHeroId != -1 && m_curShowHeroId == mId)
	{
		return;
	}

	char heroName[33];
	sprintf(heroName,"hero/hero_%d.plist",mId);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(heroName);

	m_curShowHeroId = mId;
	if (m_heroSprite)
	{
		m_heroSprite->removeFromParent();
		m_heroHeadSp->removeFromParent();
	}
	char filename[33];
	sprintf(filename,"hero%d_0.png",mId);
	m_heroSprite = CCSprite::createWithSpriteFrameName(filename);
	this->addChild(m_heroSprite,3);
	m_heroSprite->setPosition(ccp(150*gScaleX,400*gScaleY));

	char aniName[40];
	sprintf(aniName,"hero%d_%%d.png",mId);
	CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniBypList(aniName,4,0.15f);
	CCAnimate *ani = CCAnimate::create(animation);
	m_heroSprite->runAction(CCRepeatForever::create(ani));

	schedule(schedule_selector(PrepareLayer::shotBullet),0.5);

	char headFileName[48];
	sprintf(headFileName,"ui/herochoice/heroHead_%d.png",mId);
	m_heroHeadSp = CCSprite::create(headFileName);
	this->m_menu->getChildByTag(100 + 2)->addChild(m_heroHeadSp,4);
	m_heroHeadSp->setScale(0.8f);
	m_heroHeadSp->setPosition(this->m_menu->getChildByTag(100 + 2)->getContentSize()/2);

	SK::StringKey nameKey[3] = {K.HERO0_NAME,K.HERO1_NAME,K.HERO2_NAME};

	if (m_heroNameLabel)
	{
		m_heroNameLabel->setString(gGetLanguageDesc(nameKey[m_curShowHeroId]).c_str());
	}
	else
	{
		m_heroNameLabel = CCLabelBMFont::create(gGetLanguageDesc(nameKey[m_curShowHeroId]).c_str(),"font/golden_36.fnt");
		this->addChild(m_heroNameLabel,10);
		m_heroNameLabel->setPosition(ccp(390*gScaleX,475*gScaleY));
	}
}

void PrepareLayer::initItem(int leftOrRight)
{
	if (!leftOrRight)
	{
		//*左边复活道具item**//
		int itemCount = GameConfigData::initialGameData().getReviveValue();
		CCSprite *spriteItem = NULL;//CCSprite::create("ui/levelChoice/reward2Nml.png");
		if (itemCount > 0)
		{
			spriteItem = CCSprite::create("ui/levelChoice/reward2Nml.png");
			spriteItem->setScale(0.7f);
		}
		else
		{
			spriteItem = CCSprite::create("ui/levelChoice/reward2Nml.png");
		}
		if(this->m_menu->getChildByTag(100+6)->getChildByTag(1234))
		{
			this->m_menu->getChildByTag(100+6)->removeChildByTag(1234);
		}
		this->m_menu->getChildByTag(100+6)->addChild(spriteItem,1,1234);
		spriteItem->setPosition(this->m_menu->getChildByTag(100+6)->getContentSize()/2);

		if (itemCount > 0)
		{
			string countStr = ToolsFun::sharedTools()->intToString(itemCount);
			CCLabelBMFont *labelBMFont = CCLabelBMFont::create(countStr.c_str(),"font/white_24.fnt");
			spriteItem->addChild(labelBMFont);
			labelBMFont->setPosition(ccp(spriteItem->getContentSize().width,10*gScaleY));
		}
		else
		{
			CCLabelBMFont *labelBMFont = CCLabelBMFont::create("0","font/white_24.fnt");
			spriteItem->addChild(labelBMFont);
			labelBMFont->setPosition(ccp(spriteItem->getContentSize().width-5,15*gScaleY));
		}
	}
	else
	{
		//*右边道具item**/
		/*
		int itemCount = GameConfigData::initialGameData().getBombValue();
		CCSprite *spriteItem = NULL;//CCSprite::create("ui/levelChoice/reward2Nml.png");
		if (itemCount > 0)
		{
		spriteItem = CCSprite::create("ui/levelChoice/reward1Nml.png");
		spriteItem->setScale(0.7f);
		}
		else
		{
		spriteItem = CCSprite::create("ui/shared/plus.png");
		}
		if(this->m_menu->getChildByTag(100+7)->getChildByTag(1234))
		{
		this->m_menu->getChildByTag(100+7)->removeChildByTag(1234);
		}
		this->m_menu->getChildByTag(100+7)->addChild(spriteItem,1,1234);
		spriteItem->setPosition(this->m_menu->getChildByTag(100+7)->getContentSize()/2-ccp(5*gScaleX,0));

		if (itemCount > 0)
		{
		string countStr = ToolsFun::sharedTools()->intToString(itemCount);
		CCLabelBMFont *labelBMFont = CCLabelBMFont::create(countStr.c_str(),"font/white_24.fnt");
		spriteItem->addChild(labelBMFont);
		labelBMFont->setPosition(ccp(spriteItem->getContentSize().width,10*gScaleY));
		}
		*/
	}
}

void PrepareLayer::purchaseSp(int index)
{
	purchaseSuc(index);
}

void PrepareLayer::purchaseSuc(int index)
{
	int coinValue = GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);
	m_moneyLabel->setString(coinStr.c_str());
}


void PrepareLayer::setCoin()
{
	int coinValue = GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);
	m_moneyLabel->setString(coinStr.c_str());
}

void PrepareLayer::shotBullet(float dt)
{
	float time = 0.5;
	vector<WeaponData*> mWeaponDatas;

	switch(m_curShowHeroId)
	{
	case 0:
		mWeaponDatas = DataInitial::initialDataSource().gethero0WeaponData();
		break;

	case 1:
		mWeaponDatas = DataInitial::initialDataSource().gethero1WeaponData();
		break;

	case 2:
		mWeaponDatas = DataInitial::initialDataSource().gethero2WeaponData();
		break;
	}

	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();
	WeaponData*  weaponLevelDatas = mWeaponDatas[GameConfigData::initialGameData().getCurHeroLevel() - 1];
	vector<WeaponBulletData> bulletDatas = weaponLevelDatas->heroWeaponLevelData;
	int index=heroLevel/10;
	float scale=0.0f;
	int bulletId=0;
	if(heroLevel>=0&&heroLevel<HERO_MAXLEVEL)
	{
		bulletId=index*10+GameConfigData::initialGameData().getCurHeroId();
		scale=0.5+0.05*(heroLevel%10);
	}
	else if(heroLevel == HERO_MAXLEVEL )
	{
		bulletId=GameConfigData::initialGameData().getCurHeroId()+ 1000;
		scale=0.5f;
	}
	char bulletName[64];
	sprintf(bulletName,"bullet/bullet%d.png",bulletId);

	CCSprite* sprite = CCSprite::create("bullet/bullet.png");
	sprite->setScale(scale);
	this->addChild(sprite);
	vector<WeaponLevelEffectData* > levelEffectData = DataInitial::initialDataSource().getheroEffectData(m_curShowHeroId);

	if(levelEffectData.at(heroLevel-1))
	{
		WeaponLevelEffectData *data = levelEffectData.at(heroLevel-1);

		if(data)
		{
			int size = data->m_effectData.size();

			for(int i=0;i<size;i++)
			{
				WeaponEffectData *effectData = data->m_effectData.at(i);
				if(effectData)
				{
					CCSprite *sprite1 = CCSprite::create(effectData->m_bulletName.c_str());
					sprite1->setPosition(effectData->m_pos);
					sprite->addChild(sprite1);
				}
			}
		}
	}
	sprite->setPosition(m_heroSprite->getPosition()+ccp(0,80*gScaleY));
	CCSize size=ToolsFun::sharedTools()->getSceneSize();
	CCSprite *spBkgTop = (CCSprite*)this->getChildByTag(108);
	CCMoveTo *moveBy = CCMoveTo::create(time,ccp(m_heroSprite->getPositionX(),size.height - spBkgTop->getContentSize().height-60));
	sprite->runAction(CCSequence::create(moveBy,CCCallFunc::create(sprite,callfunc_selector(CCSprite::removeFromParent)),NULL));
}

void PrepareLayer::shotBulletPetLeft(float dt)
{
	if(m_leftPitId!=-1)
	{
		CCSprite *leftPet =(CCSprite *) this->getChildByTag(45456);
		if(leftPet)
		{
			BulletData *m_bulletData = DataInitial::initialDataSource().getBulletDataById(200+m_leftPitId);

			char buf[64];
			sprintf(buf,"bullet/bullet%d.png",m_bulletData->mBulletId);

			CCSprite *sprite = CCSprite::create(buf);
			this->addChild(sprite);
			sprite->setPosition(leftPet->getPosition()+ccp(-20*gScaleX,40*gScaleY));
			sprite->setScale(PRE_SCALE);

			CCMoveBy *moveBy = CCMoveBy::create(0.5,ccp(0,300*gScaleY));
			sprite->runAction(CCSequence::create(moveBy,CCCallFunc::create(sprite,callfunc_selector(CCSprite::removeFromParent)),NULL));
		}
	}
}

void PrepareLayer::shotBulletPetRight(float dt)
{
	if(m_rightPetId !=-1)
	{
		BulletData *m_bulletData = DataInitial::initialDataSource().getBulletDataById(200+m_rightPetId);

		CCSprite *rightPet =(CCSprite *) this->getChildByTag(45457);
		if(rightPet)
		{
			BulletData *m_bulletData = DataInitial::initialDataSource().getBulletDataById(200+m_rightPetId);

			char buf[64];
			sprintf(buf,"bullet/bullet%d.png",m_bulletData->mBulletId);

			CCSprite *sprite = CCSprite::create(buf);
			this->addChild(sprite);
			sprite->setPosition(rightPet->getPosition()+ccp(0,40*gScaleY));
			sprite->setScale(PRE_SCALE);

			CCMoveBy *moveBy = CCMoveBy::create(0.5,ccp(0,300*gScaleY));
			sprite->runAction(CCSequence::create(moveBy,CCCallFunc::create(sprite,callfunc_selector(CCSprite::removeFromParent)),NULL));
		}
	}
}