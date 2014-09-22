#include "HeroChoice.h"
#include "LevelChoice.h"
#include "ShopLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "../GCData/DataInitial.h"
#include "../ToolFun/JniCall.h"
#include "../SpriteNode/SystemTipNode.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "BackLayer.h"
#include "LuckySpinLayer.h"

#define  TOPMENU    4000
#define  SELECT_MENU  5000
#define  HERO_SELECT  8000
#define  SELECT_ICON  6000
#define  HERO_LOCK     7000
#define  BATTLE       9000
#define  BUY          10000
#define  PRICE        81111

#define HERO_SCALE 0.6f



HeroChoiceLayer::HeroChoiceLayer():m_coinLabel(NULL)
	,m_heroPriceLabel(NULL)
	,m_heroHeadSp(NULL)
	,m_heroSprite(NULL)
	,m_menu(NULL)
	,curSelectId(0)
	,m_target(NULL)
	,m_callFun(NULL)
	,m_rightHero(NULL)
	,m_curHeroId(0)
{
}

HeroChoiceLayer::~HeroChoiceLayer()
{

}

bool HeroChoiceLayer::init()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *spBkg = CCSprite::create("ui/shared/bkg.png");
	this->addChild(spBkg,-1);
	spBkg->setPosition(size/2);

	CCSprite *spBkgHero = CCSprite::create("ui/prepareUI/upgrade.png");
	this->addChild(spBkgHero,-1);
	spBkgHero->setPosition(ccp(size.width/2,size.height - spBkgHero->getContentSize().height/2 - 60*gScaleY));

	CCSprite * coin = CCSprite::create("ui/shared/coin.png");
	this->addChild(coin);
	coin->setPosition(ccp(280 * gScaleX, size.height - 30 * gScaleY));
	coin->setZOrder(588);

	CCMenu *menu = CCMenu::create();
	this->addChild(menu);
	menu->setTag(TOPMENU);
	menu->setTouchPriority(-130);
	menu->setPosition(CCPointZero);

	CCSprite *spNml = CCSprite::create("ui/shared/back.png");
	CCSprite *spSel = CCSprite::create("ui/shared/back.png");
	spSel->setColor(ccc3(100,100,100));
	CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml,spSel,this,
		menu_selector(HeroChoiceLayer::back));
	menu->addChild(spItem);
	spItem->setPosition(ccp(37*gScaleX,size.height-37*gScaleY));

	//show shop
	spNml = CCSprite::create("ui/shared/coinplus.png");
	spSel = CCSprite::create("ui/shared/coinplus.png");
	spSel->setColor(ccc3(100,100,100));
	spItem = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(HeroChoiceLayer::showShopLayer));
	menu->addChild(spItem);
	spItem->setPosition(ccp(430*gScaleX,size.height-30*gScaleY));
	menu->setZOrder(588);

	string coinStr = GameConfigData::initialGameData().getCoinValueStr();
	m_coinLabel = CCLabelBMFont::create(coinStr.c_str(),GOLD_FONT);
	this->addChild(m_coinLabel,588);
	m_coinLabel->setAnchorPoint(ccp(0,0.5f));
	m_coinLabel->setPosition(ccp(size.width * 5 / 6 - 100*gScaleX, size.height - 25*gScaleY));
	m_coinLabel->setScale(0.6F);

	//购物
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop,0,108);
	spBkgTop->setPosition(ccp(size.width/2,size.height - spBkgTop->getContentSize().height/2));

	CCSprite *mainbkg = CCSprite::create("ui/herochoice/hero_bkgcube.png");
	this->addChild(mainbkg,-1,109);
	mainbkg->setPosition(ccp(size.width/2,size.height - mainbkg->getContentSize().height/2-50*gScaleY ));

	CCSprite *desBkg = CCSprite::create("ui/herochoice/hero_describe.png");
	mainbkg->addChild(desBkg);
	desBkg->setPosition(ccp(desBkg->getContentSize().width/2+20,desBkg->getContentSize().height/2+9));
	SK::StringKey keys[3]={K.HERO0_NAME,K.HERO1_NAME,K.HERO2_NAME};
	heroNameLabel = CCLabelBMFont::create(gGetLanguageDesc(keys[0]).c_str() ,"font/hero_name.fnt");
	this->addChild(heroNameLabel,590);
	heroNameLabel->setAnchorPoint(ccp(1.0f,0.5f));
	heroNameLabel->setPosition(ccp(110*gScaleX,380*gScaleY));
	heroNameLabel->setScale(0.8f);

	m_heroSkillLabel = CCLabelBMFont::create(gGetLanguageDesc(K.HERO_SKILL_DES0).c_str(),"font/hero_des.fnt",180);
	m_heroSkillLabel->setLineBreakWithoutSpace(true);
	this->addChild(m_heroSkillLabel,590);
	m_heroSkillLabel->setAnchorPoint(ccp(0,0.5));
	m_heroSkillLabel->setPosition(ccp(30*gScaleX,300*gScaleY));
	m_heroSkillLabel->setScale(0.8f);


	int curHeroId = GameConfigData::initialGameData().getCurHeroId();
	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();

	CCLabelBMFont *curLevelLabel = CCLabelBMFont::create("1000","font/level_big.fnt");
	this->addChild(curLevelLabel);
	curLevelLabel->setPosition(ccp(180*gScaleX,380*gScaleY));
	string levelStr = ToolsFun::sharedTools()->intToString(heroLevel);
	curLevelLabel->setString(("Lv." + levelStr).c_str());
	curLevelLabel->setScale(0.8f);

	m_menu = CCMenu::create();
	this->addChild(m_menu,1);
	m_menu->setPosition(CCPointZero);

	//上阵
	spNml = CCSprite::create("ui/herochoice/battle.png");
	spSel = CCSprite::create("ui/herochoice/battle.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite* itemSp = CCMenuItemSprite::create(spNml,spSel,
		this,menu_selector(HeroChoiceLayer::menuTouch));
	m_menu->addChild(itemSp);
	itemSp->setTag(BATTLE);
	itemSp->setPosition(ccp(320*gScaleX,290*gScaleY));
	itemSp->setVisible(false);
	itemSp->setScale(HERO_SCALE);

	//itemSp->setEnabled(false);
	//购买
	spNml = CCSprite::create("ui/herochoice/role_buy.png");
	spSel = CCSprite::create("ui/herochoice/role_buy.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite* itembuy = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(HeroChoiceLayer::menuTouch));
	m_menu->addChild(itembuy);
	itembuy->setTag(BUY);
	itembuy->setPosition(ccp(325*gScaleX,290*gScaleY));
	itembuy->setVisible(false);
	itembuy->setScale(HERO_SCALE);

	m_heroPriceLabel=CCLabelBMFont::create("100000","font/zl_1.fnt");
	itembuy->addChild(m_heroPriceLabel);
	m_heroPriceLabel->setTag(PRICE);
	m_heroPriceLabel->setPosition(ccp(100*gScaleX,70*gScaleY));
	m_heroPriceLabel->setVisible(false);

	const char* fName[3] = 
	{
		"ui/herochoice/hero_head.png",
		"ui/herochoice/hero_head.png",
		"ui/herochoice/hero_head.png",
	};

	const CCPoint posArray[3] = {ccp(76*gScaleX,130*gScaleY),ccp(240*gScaleX,130*gScaleY),ccp(400*gScaleX,130*gScaleY)};

	for (int i=0;i!=3;++i)
	{
		spNml = CCSprite::create(fName[i]);
		spSel = CCSprite::create(fName[i]);
		spSel->setColor(ccc3(80,80,80));
		itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(HeroChoiceLayer::menuTouch));
		m_menu->addChild(itemSp);
		itemSp->setPosition(posArray[i]);
		itemSp->setTag(SELECT_MENU + i);
	}

	const char* HeroHeadName[3] =
	{
		"ui/herochoice/heroHead_0.png",
		"ui/herochoice/heroHead_1.png",
		"ui/herochoice/heroHead_2.png",

	};
	const char* HeroLock[3] = 
	{
		"ui/herochoice/hero_lock.png",
		"ui/herochoice/hero_lock.png",
		"ui/herochoice/hero_lock.png",

	};

	for (int i=0;i!=3;++i)
	{
		CCSprite *head = CCSprite::create(HeroHeadName[i]);
		CCSprite *lock = CCSprite::create(HeroLock[i]);
		this->addChild(head,588,HERO_SELECT + i);
		head->addChild(lock,590,HERO_LOCK);
		head->setPosition(posArray[i]);
		CCPoint cp = head->getContentSize()/2;
		lock->setPosition(cp);
	}

	CCSprite * select= CCSprite::create("ui/herochoice/select.png");
	this->addChild(select,588,SELECT_ICON);

	SystemTipNode *node = SystemTipNode::create();
	this->addChild(node,1113);
	node->setParentType(1);
	initHeroSp(GameConfigData::initialGameData().getCurHeroId());
	initUnLockHero();
	updateSelection();

	curSelectId =  GameConfigData::initialGameData().getCurHeroId();
	setKeypadEnabled(true);
	registerNDK();

	return true;
}

void HeroChoiceLayer::updateSelection()
{
	CCSprite* select=(CCSprite*)this->getChildByTag(SELECT_ICON);
	select->setPosition(ccp(20*gScaleX + gScaleX*160*GameConfigData::initialGameData().getCurHeroId(),200*gScaleY));

	if (GameConfigData::initialGameData().getHeroStateInt(curSelectId))
	{
		schedule(schedule_selector(HeroChoiceLayer::shotBullet),0.5);
	}
}

void HeroChoiceLayer::initHeroSp(int mId)
{
	char filename[33];
	sprintf(filename,"ui/herochoice/heroPortrait_%d.png",mId);
	m_heroSprite = CCSprite::create(filename);
	this->addChild(m_heroSprite,588,04031454);
	m_heroSprite->setPosition(ccp(160*gScaleX,550*gScaleY));
	m_heroSprite->setScale(HERO_SCALE);
	SK::StringKey nameKey[3] = {K.HERO0_NAME,K.HERO1_NAME,K.HERO2_NAME};
	SK::StringKey skillKey[3] = {K.HERO_SKILL_DES0,K.HERO_SKILL_DES1,K.HERO_SKILL_DES2};

	m_heroSkillLabel->setCString(gGetLanguageDesc(skillKey[mId]).c_str());

	if (heroNameLabel)
	{
		heroNameLabel->setString(gGetLanguageDesc(nameKey[mId]).c_str());
	}
	else
	{
		heroNameLabel = CCLabelBMFont::create(gGetLanguageDesc(nameKey[mId]).c_str(),"font/golden_36.fnt");
		this->addChild(heroNameLabel,590);
	}

	SK::StringKey desKey[3] = {K.HERO0_DES,K.HERO1_DES,K.HERO2_DES};


	if (GameConfigData::initialGameData().getHeroStateInt(curSelectId))
	{
		m_menu->getChildByTag(BUY)->setVisible(false);
		m_menu->getChildByTag(BATTLE)->setVisible(true);
	}
	else
	{
		m_menu->getChildByTag(BUY)->setVisible(true);
		string priceVal="1000";
		if(m_heroPriceLabel)
		{
			if(mId==1)
			{
				m_heroPriceLabel->setString(gGetLanguageDesc(SK::PET3_PRICE).c_str());
			}
			else
			{
				m_heroPriceLabel->setString(gGetLanguageDesc(SK::PET4_PRICE).c_str());
			}

			
		}
		else
		{
			m_heroPriceLabel=CCLabelBMFont::create("100000","font/level_big.fnt");
			m_menu->getChildByTag(BUY)->addChild(m_heroPriceLabel);
			m_heroPriceLabel->setTag(PRICE);
			//m_heroPriceLabel->setPosition(ccp(70*gScaleX,50*gScaleY));
		}
		m_heroPriceLabel->setVisible(true);
		m_menu->getChildByTag(BATTLE)->setVisible(false);
	}

	if(m_rightHero!=NULL)
	{
		m_rightHero->stopAllActions();
		m_rightHero->removeFromParent();
	}

	char heroName[33];
	sprintf(heroName,"hero/hero_%d.plist",mId);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(heroName);

	memset(heroName,'\0',33);

	m_curHeroId = mId;

	sprintf(heroName,"hero%d_0.png",mId);
	m_rightHero = CCSprite::createWithSpriteFrameName(heroName);
	this->addChild(m_rightHero,3);
	m_rightHero->setPosition(ccp(320*gScaleX,400*gScaleY));

	char aniName[40];
	sprintf(aniName,"hero%d_%%d.png",mId);
	CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniBypList(aniName,4,0.15f);
	CCAnimate *ani = CCAnimate::create(animation);
	m_rightHero->runAction(CCRepeatForever::create(ani));

	initPet(0);
	initPet(1);
}

void HeroChoiceLayer::initPet(int leftOrRight)
{
	int petId =-1;
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

	if(petId !=-1)
	{
		showHeroPet(leftOrRight,petId);
	}
}

//显示hero边上的宠物 0为左侧 1为右侧
void HeroChoiceLayer::showHeroPet(int leftOrRight,int petId)
{
	float scale[6] = {0.2f*HERO_SCALE,0.2f*HERO_SCALE,0.2f*HERO_SCALE,0.2f*HERO_SCALE,0.3f*HERO_SCALE,0.3f*HERO_SCALE};

	CCPoint leftPetPos[6] = {ccp(240*gScaleX,360*gScaleY),ccp(240*gScaleX,360*gScaleY),ccp(240*gScaleX,360*gScaleY),
		ccp(240*gScaleX,360*gScaleY),ccp(240*gScaleX,360*gScaleY),ccp(240*gScaleX,360*gScaleY)};

	CCPoint rightPetPos[6] ={ccp(400*gScaleX,360*gScaleY),ccp(400*gScaleX,360*gScaleY),ccp(390*gScaleX,360*gScaleY),
		ccp(390*gScaleX,360*gScaleY),ccp(400*gScaleX,360*gScaleY),ccp(400*gScaleX,360*gScaleY)};

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

			sp1->setScale(HERO_SCALE);
			sp2->setScale(HERO_SCALE);

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

			sp1->setScale(HERO_SCALE);
			sp2->setScale(HERO_SCALE);

			CCAnimate *ani = CCAnimate::create(animation);
			CCAnimate *ani_1 = CCAnimate::create(animation);
			sp1->runAction(CCRepeatForever::create(ani));
			sp2->runAction(CCRepeatForever::create(ani_1));
			sp1->setFlipX(true);
		}
	}

}

void HeroChoiceLayer::shotBullet(float dt)
{
	float time = 0.5;

	if(m_rightHero)
	{
		vector<WeaponData*> mWeaponDatas;

		int curShowHeroId =m_curHeroId ;

		switch(curShowHeroId)
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

		WeaponData*  weaponLevelDatas = mWeaponDatas[GameConfigData::initialGameData().getCurHeroLevel() - 1];
		vector<WeaponBulletData> bulletDatas = weaponLevelDatas->heroWeaponLevelData;

		int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();

		int index=heroLevel/10;
		float scale=0.0f;
		int bulletId=0;
		if(heroLevel>=0&&heroLevel<HERO_MAXLEVEL)
		{
			bulletId=index*10+m_curHeroId ;//GameConfigData::initialGameData().getCurHeroId();
			scale=0.5+0.05*(heroLevel%10);
		}
		else if(heroLevel == HERO_MAXLEVEL )
		{
			bulletId= 1000+m_curHeroId; //GameConfigData::initialGameData().getCurHeroId()+ 1000;
			scale=0.5f;
		}
		
		CCSprite* sprite = CCSprite::create("bullet/bullet.png");

		vector<WeaponLevelEffectData* > levelEffectData = DataInitial::initialDataSource().getheroEffectData(m_curHeroId);

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
		}		sprite->setScale(scale);
		this->addChild(sprite);
		sprite->setPosition(m_rightHero->getPosition()+ccp(0,80*gScaleY));
		CCSize size=ToolsFun::sharedTools()->getSceneSize();
		CCSprite *spBkgTop = (CCSprite*)this->getChildByTag(108);
		CCMoveTo *moveBy = CCMoveTo::create(time,ccp(m_rightHero->getPositionX(),size.height - spBkgTop->getContentSize().height-60));

		sprite->runAction(CCSequence::create(moveBy,CCCallFunc::create(sprite,callfunc_selector(CCSprite::removeFromParent)),NULL));
	}

}

void HeroChoiceLayer::initUnLockHero()
{
	vector<string> herostate = GameConfigData::initialGameData().getHeroStates();
	for (int i=0;i!=herostate.size();++i)
	{
		if (herostate[i] != "0")
		{
			this->getChildByTag(HERO_SELECT + i)->removeChildByTag(HERO_LOCK);
		}
	}
}

void HeroChoiceLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}

void HeroChoiceLayer::menuTouch(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCMenuItemSprite *msp =dynamic_cast<CCMenuItemSprite*>(pSender);
	int tag = msp->getTag();
	int menuid = tag - SELECT_MENU;
	switch (menuid)
	{
	case 0 : case 1 : case 2 : 

		{	curSelectId=menuid;
		this->removeChildByTag(04031454);

		initHeroSp(tag-SELECT_MENU);
		}
		break;
	case 4000:
		{
			GameConfigData::initialGameData().setCurHeroId(curSelectId);
			updateSelection();
		}
		break;
	case 5000:
		{
			int itemCode[2] = {0,1};
			JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(HeroChoiceLayer::purchaseFailed));
			JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(HeroChoiceLayer::purchaseSuccess));
			JniCall::sharedJniCall()->purchaseByIndex(itemCode[curSelectId-1]);
		}
		break;
	default:
		break;
	}
}


void HeroChoiceLayer::back(CCObject* obj)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	if (m_target)
	{
		(m_target->*m_callFun)(obj);
		this->removeFromParent();
	}
	else
	{
		LevelChoice *menuLayer = LevelChoice::create();
		ToolsFun::sharedTools()->gotoscene(menuLayer);
	}
}

void HeroChoiceLayer::showShopLayer(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCMenu * add = (CCMenu *)this->getChildByTag(TOPMENU);
	add->setEnabled(false);
	ShopLayer *shopLayer = ShopLayer::create();
	this->addChild(shopLayer,599);
	m_menu->setEnabled(false);
	/*shopLayer->setTouchPriority(200);*/
	shopLayer->setCloseCallBack(this,menu_selector(HeroChoiceLayer::backFromShopLayer));
}
void HeroChoiceLayer::backFromShopLayer(CCObject *pSender)
{
	CCMenu * add = (CCMenu *)this->getChildByTag(TOPMENU);
	add->setEnabled(true);
	m_menu->setEnabled(true);
}

void HeroChoiceLayer::setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack)
{
	m_target = pTarget;
	m_callFun = callBack;
}


void HeroChoiceLayer::purchaseFailed(const char* str)
{
}

void HeroChoiceLayer::purchaseSuccess(const char* str)
{
	m_menu->getChildByTag(BUY)->setVisible(false);
	m_menu->getChildByTag(BATTLE)->setVisible(true);

	if(strcmp(str, "0") == 0)
	{
		GameConfigData::initialGameData().setHeroStateInt(curSelectId , "1");
		GameConfigData::initialGameData().writeData();
	}
	else if(strcmp(str, "1") == 0)
	{
		GameConfigData::initialGameData().setHeroStateInt(curSelectId , "1");
		GameConfigData::initialGameData().writeData();
	}

	initUnLockHero();
}

void HeroChoiceLayer::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(HeroChoiceLayer::goGamble));
	this->addChild(layer,10000);
}
void HeroChoiceLayer::goGamble()
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}