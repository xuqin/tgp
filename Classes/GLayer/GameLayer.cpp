#include "GameLayer.h"
#include "GameOver.h"
#include "../SpriteNode/MapNode.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "../SpriteNode/HeroSprite.h"
#include "../SpriteNode/MonsterSprite.h"
#include "../SpriteNode/MySpriteManager.h"
#include "../SpriteNode/BulletSprite.h"
#include "../SpriteNode/PetSprite.h"
#include "GamePause.h"
#include "StartGameLoading.h"
#include "LevelChoice.h"
#include "DialogLayer.h"
#include "GameReviveLayer.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "../SpriteNode/ItemSprite.h"
#include "../ToolFun/HttpClientNode.h"

#define  WARNING 1458
#define  MONSTERDEAD 1558

static GameLayer *gGameLayer = NULL;
static int gRewardType = 0;  //每日登陆奖励的类型  共有0 1 2 3 四种
GameLayer::GameLayer():m_gainScore(NULL)
	,m_gainMoney(NULL)
	,m_progressLength(NULL)
	,m_pHeadSprite(NULL)
	,m_leftPet(NULL)
	,m_rightPet(NULL)
	,m_menu(NULL)
	,m_mapNode(NULL)
	,m_storyNode(NULL)
	,m_mySpriteManager(NULL)
	,m_pasueState(false)
	,m_revive(0)
	,m_isBossLevel(false)
	,m_killMosTotal(0)
	,m_min(0)
	,m_max(0)
	,m_firstRevive(-1)
	,m_useProtect(false)
	,m_speed(10)
{
	LD("GameLayer::GameLayer()");
	m_Score =  0;
	m_Money = 0;
	m_Distance = 0;
	m_CurStar = 0;
	gGameLayer = this;

	m_curPos =CCPointZero;
	m_lastPos = CCPointZero;
	m_offectPos = CCPointZero;

	m_reviveNumber = NULL;
}

GameLayer::~GameLayer()
{
	if(m_mySpriteManager)
	{
		LD("GameLayer::~GameLayer()");
		delete m_mySpriteManager;
		m_mySpriteManager = NULL;
		gGameLayer = NULL;
	}
}


bool GameLayer::init()
{
	initUi();
	initKernelModes();

	registerNDK();

	int unlockLevel=GameConfigData::initialGameData().getCurLevel();

	StarConfig *config = DataInitial::initialDataSource().getStarConfig(unlockLevel);
	if(config!=NULL)
	{
		m_min = config->m_min;
		m_max =config->m_max;
	}

	setKeypadEnabled(true);

	return true;
}

void GameLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}

void GameLayer::initUi()
{		
	m_revive = GameConfigData::initialGameData().getReviveValue();

	ToolsFun::sharedTools()->playBkgMusic(BATTLE,true);
	this->setTouchEnabled(true);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	/*初始化得分的显示*/
	CCSprite *spriteScore = CCSprite::create("ui/gameLayerUI/text_score1.png");
	this->addChild(spriteScore,UI_ZORDER);
	spriteScore->setPosition(ccp(spriteScore->getContentSize().width/2,size.height - 40*gScaleY));
	m_gainScore = CCLabelBMFont::create("0","font/white.fnt");
	this->addChild(m_gainScore,UI_ZORDER);
	m_gainScore->setAnchorPoint(ccp(0,0.5));
	m_gainScore->setPosition(ccp(10*gScaleX,size.height - 80*gScaleY));


	/*初始化获取金钱的显示*/
	CCSprite *goldeSprite = CCSprite::create("ui/gameLayerUI/coin.png");
	this->addChild(goldeSprite,UI_ZORDER);
	goldeSprite->setPosition(ccp(size.width - goldeSprite->getContentSize().width/2 - 5*gScaleX,size.height - 80*gScaleY));
	m_gainMoney = CCLabelBMFont::create("0","font/gold_big.fnt");
	this->addChild(m_gainMoney,UI_ZORDER);
	m_gainMoney->setAnchorPoint(ccp(1,0.5));
	m_gainMoney->setPosition(ccp(goldeSprite->getPositionX() -  goldeSprite->getContentSize().width/2 - 5*gScaleX,goldeSprite->getPositionY()));

	/*初始化距离的显示*/
	CCLabelBMFont *progressLength = CCLabelBMFont::create("m","font/white.fnt");
	this->addChild(progressLength,UI_ZORDER);
	progressLength->setPosition(ccp(goldeSprite->getPositionX()+5*gScaleX,size.height - 40*gScaleY));
	m_progressLength = CCLabelBMFont::create("0","font/white.fnt");
	this->addChild(m_progressLength,UI_ZORDER);
	m_progressLength->setAnchorPoint(ccp(1,0.5f));
	m_progressLength->setPosition(ccp(progressLength->getPositionX() - progressLength->getContentSize().width/2,
	progressLength->getPositionY()));

	/*初始化进度*/
	int heroId=GameConfigData::initialGameData().getCurHeroId();
	char head[40];
	sprintf(head,"ui/gameLayerUI/head%d.png",heroId);
	CCSprite *progressBkg = CCSprite::create("ui/gameLayerUI/pg_dst.png");
	progressBkg->setScale(0.9f);
	this->addChild(progressBkg,UI_ZORDER);
	progressBkg->setPosition(ccp(size.width/2,size.height - 40*gScaleY));
	m_pHeadSprite = CCSprite::create(head);
	m_pHeadSprite->setScale(0.8f);
	progressBkg->addChild(m_pHeadSprite);
	m_pHeadSprite->setPosition(ccp(35,progressBkg->getContentSize().height/2));
	m_pHeadSprite->setTag((int)progressBkg->getContentSize().width);


	m_menu = CCMenu::create();
	this->addChild(m_menu,UI_ZORDER);
	m_menu->setPosition(CCPointZero);

	int mCount = 2;
	CCPoint posArray[2] = {ccp(50*gScaleX,60*getScaleY()),ccp(size.width/2,size.height - 100*gScaleY)};
	const char* imageFileNml[2] = {"ui/levelChoice/reward2Nml.png","ui/gameLayerUI/pause.png"};
	const char* imageFileSel[2] = {"ui/levelChoice/reward2Nml.png","ui/gameLayerUI/pause.png"};
	for (int i=0;i!=mCount; ++i)
	{
		CCSprite *startNml = CCSprite::create(imageFileNml[i]);
		CCSprite *startSel = CCSprite::create(imageFileSel[i]);
		startSel->setColor(ccc3(100,100,100));
		CCMenuItemSprite *itemSp = CCMenuItemSprite::create(startNml,startSel,this,menu_selector(GameLayer::touchMenu));
		m_menu->addChild(itemSp,1,100+i);
		itemSp->setPosition(posArray[i]);
	}

	m_reviveNumber = CCLabelBMFont::create("1000000","font/coinfont.fnt");
	this->addChild(m_reviveNumber,UI_ZORDER+1);
	m_reviveNumber->setAnchorPoint(CCPointZero);
	m_reviveNumber->setPosition(ccp(80*gScaleX,15*gScaleY));
	m_reviveNumber->setScale(0.8f);

	setReviveCount();
	
	int level=GameConfigData::initialGameData().getCurLevel();

	if(level%6==5)
	{
		m_isBossLevel = true;
	}

	if(level<=1 && DataInitial::initialDataSource().getLvTag() != SUB_LEVEL)
	{
		showGiftbag(0);
	}

	if(level==2)
	{
		bool active = CCUserDefault::sharedUserDefault()->getBoolForKey("active",false);
		if(!active)
		{
			showActiveDialog();
		}
	}
	
	char buf[64];
	sprintf(buf,"%d",level);
	
	HttpClientNode::sharedHttpClient()->eventSend(1,buf);
}

void GameLayer::showActiveDialog()
{
	pause();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	addChild(bkg,UI_ZORDER+1);
	bkg->setPosition(ccp(size.width/2,size.height/2));
	bkg->setTag(112255);

	char buf[32];
	sprintf(buf,"DayReward/active.png");
	CCSprite *tip1 = CCSprite::create(buf);
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(GameLayer::showActiveDialogClose));
	itemClose->setPosition(ccp(bkg->getContentSize().width-40 ,bkg->getContentSize().height-60));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(GameLayer::showActiveDialogSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,25));
	menu->addChild(itemSp);
}

void GameLayer::showActiveDialogClose(CCObject *sender)
{
	this->quitLevel();
}

void GameLayer::showActiveDialogSure(CCObject *sender)
{
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(GameLayer::purchaseSuccess));
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(GameLayer::purchaseFailed));
	JniCall::sharedJniCall()->purchaseByIndex(10);

}

void GameLayer::showGiftbag(int id)
{
	pause();
	int level = GameConfigData::initialGameData().getCurLevel();

	if(level ==0 || level == 1)
	{

		char buf[32];
		sprintf(buf,"%d_%d",level,id);

		if(level == 0 &&id==0)
		{
			gRewardType = 0;
		}
		else if(level ==0 && id==1)
		{
			gRewardType = 1;
			m_reward  = true;
		}else if(level ==1 && id==0)
		{
			gRewardType = 2;
		}else if(level==1&& id==1)
		{
			gRewardType = 3;
			m_reward  = true;
		}else if(level ==2 && id ==0)
		{
			gRewardType = 4;
		}else if(level ==2&&id==1)
		{
			gRewardType = 5;
			m_reward  = true;
		}


		bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(buf,false);

		if(!flag)
		{
			CCNode *node = this->getChildByTag(4251029);
			if(node!=NULL)
			{
				node->setVisible(false);
			}

			this->removeChildByTag(112233);

			CCSize size = CCDirector::sharedDirector()->getWinSize();
			CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
			this->addChild(bkg,UI_ZORDER+1);
			bkg->setPosition(ccp(size.width/2,size.height/2));
			bkg->setTag(112233);

			char buf[32];
			if(level ==0)
			{
				sprintf(buf,"DayReward/package%d_%d.png",level,id);
			}
			else if(level ==1 || level ==2)
			{
				sprintf(buf,"DayReward/package%d_%d.png",1,id);
			}
			CCSprite *tip1 = CCSprite::create(buf);
			bkg->addChild(tip1);
			tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

			CCMenu *menu = CCMenu::create();
			bkg->addChild(menu);
			menu->setPosition(CCPointZero);

			CCSprite *spNml = CCSprite::create("DayReward/button.png");
			CCSprite *spSel = CCSprite::create("DayReward/button.png");
			spSel->setColor(ccc3(80,80,80));
			CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(GameLayer::touchRewardMenu));
			//CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,NULL);
			itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,25));
			menu->addChild(itemSp);

		}
		else
		{
			
			if(gRewardType ==1 || gRewardType ==3)
			{
				resume();
				m_levelPass = true;
				scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f);
			}
			else
			{

				resume();
			}
		}
	}
}

void GameLayer::touchRewardMenu(CCObject *sender)
{
	this->removeChildByTag(112233);

	int id=0;

	if(gRewardType ==0)
	{
		//DataInitial::initialDataSource().
		int coinValue = GameConfigData::initialGameData().getCoinValue();
		GameConfigData::initialGameData().setCoinValue(coinValue+1000);
	
	}
	else if(gRewardType==1)
	{
		int coinValue = GameConfigData::initialGameData().getCoinValue();
		GameConfigData::initialGameData().setCoinValue(coinValue+2000);
		int revive = GameConfigData::initialGameData().getReviveValue();
		GameConfigData::initialGameData().setReviveValue(revive+1);

		m_revive = GameConfigData::initialGameData().getReviveValue();
		setReviveCount();
	}
	else if(gRewardType ==2)
	{	
		int coinValue = GameConfigData::initialGameData().getCoinValue();
		GameConfigData::initialGameData().setCoinValue(coinValue+3000);
		int revive = GameConfigData::initialGameData().getReviveValue();
		GameConfigData::initialGameData().setReviveValue(revive+2);

		m_revive = GameConfigData::initialGameData().getReviveValue();
		setReviveCount();
	}
	else if(gRewardType ==3)
	{
		int coinValue = GameConfigData::initialGameData().getCoinValue();
		GameConfigData::initialGameData().setCoinValue(coinValue+4000);
		int revive = GameConfigData::initialGameData().getReviveValue();
		GameConfigData::initialGameData().setReviveValue(revive+3);

		m_revive = GameConfigData::initialGameData().getReviveValue();
		setReviveCount();
	}
	


	if(gRewardType !=1 && gRewardType!=3)
	{
		id = 0;
		resume();
	}
	else
	{
		resume();
		m_levelPass = true;
		scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f);
		id = 1;
	}

	int level = GameConfigData::initialGameData().getCurLevel();
	char buf[32];
	sprintf(buf,"%d_%d",level,id);
	CCUserDefault::sharedUserDefault()->setBoolForKey(buf,true);
}

void GameLayer::setReviveCount()
{
	m_revive = GameConfigData::initialGameData().getReviveValue();
	char reviveCount[32];
	sprintf(reviveCount,"%d",m_revive);
	m_reviveNumber->setString(reviveCount);
}

void GameLayer::initKernelModes()
{
	m_mySpriteManager = new MySpriteManager();
	m_mapNode = MapNode::create(GameConfigData::initialGameData().getMapBkgId());
	this->addChild(m_mapNode, -1);
	initHero();
	MapEventsNode *node = MapEventsNode::sharedMapEventsNode();
	
	int heroId = GameConfigData::initialGameData().getCurHeroId();
	if(heroId==2)
	{
		m_firstRevive=1;
	}
	if(node->getParent())
	{
		node->removeFromParent();
	}

	this->addChild(node);
	node->setPosition(CCPointZero);
}

void GameLayer::initHero()
{
	if(this->getChildByTag(2014718))
	{
		this->removeChildByTag(2014718);
	}

	int heroId = GameConfigData::initialGameData().getCurHeroId();
	assert(heroId >= 0);
	m_heroSprite = HeroSprite::createHeroById(heroId);
	this->addChild(m_heroSprite, 2588, 2014718);
	m_heroSprite->setPosition(ccp(320 * gScaleX, 100*gScaleY));
	initLeftPet();
	initRightPet();
	m_curPos = m_heroSprite->getPosition();
	
}
void GameLayer::initLeftPet()
{
	///**初始化已经出战的左边的宠物**///
	int petId = -1;

	for(int i = 0; i != 6; ++i)
	{
		if(GameConfigData::initialGameData().getPetStateById(i) == "2")
		{
			if(GameConfigData::initialGameData().getPetPosById(i) == 0)
			{
				petId = i;
			}
		}
	}

	if(this->getChildByTag(PETLEFTTAG))
	{
		this->removeChildByTag(PETLEFTTAG);
	}

	if(petId != -1)
	{
		m_leftPet = PetSprite::createPetById(petId);
		this->addChild(m_leftPet,HERO_ZORDER+500,PETLEFTTAG);
		m_leftPet->setPositionY(m_heroSprite->getPositionY());

		if(petId==5&&GameConfigData::initialGameData().getpetLevelById(petId)==PET_MAXLEVEL)
		{
			if(!m_useProtect)
			{
				m_useProtect = true;
				m_heroSprite->addProtectEffect();			
			}
		}
	}
}

void GameLayer::initRightPet()
{
	///**初始化已经出战的右边的宠物**///
	int petId = -1;

	for(int i = 0; i != 6; ++i)
	{
		if(GameConfigData::initialGameData().getPetStateById(i) == "2")
		{
			if(GameConfigData::initialGameData().getPetPosById(i) == 1)
			{
				petId = i;
			}
		}
	}

	if(this->getChildByTag(PETRIGHTTAG))
	{
		this->removeChildByTag(PETRIGHTTAG);
	}


	CCPoint rightPetPos[6] = {ccp(0,0),ccp(0,0),ccp(0,20*gScaleY),
		ccp(0,0),ccp(0,0),ccp(0,0)};

	if(petId != -1)
	{
		m_rightPet = PetSprite::createPetById(petId);
		this->addChild(m_rightPet, HERO_ZORDER+500, PETRIGHTTAG);
		m_rightPet->setPositionY(m_heroSprite->getPositionY()+rightPetPos[petId].y);

		if(petId == 5 && m_rightPet->getPetLevel() == PET_MAXLEVEL)
		{
			if(!m_useProtect)
			{
				m_useProtect = true;
				getHeroSprite()->addProtectEffect();
			}
		}
	}
}

void GameLayer::scheduleStart()
{
	LD("GameLayer::scheduleStart()");
	m_mapNode->startMove();
	MapEventsNode::sharedMapEventsNode()->startMove();
	m_mySpriteManager->spriteCollisiion();
	schedule(schedule_selector(GameLayer::updateLayer), 1.0f / 45);
	schedule(schedule_selector(GameLayer::updateMove));
}

void GameLayer::updateMove(float t)
{
	if(m_leftPet)
	{
		m_leftPet->setPosition(m_heroSprite->getPosition()-ccp(100*gScaleX,0));
	}

	if(m_rightPet)
	{
		m_rightPet->setPosition(m_heroSprite->getPosition()+ccp(100*gScaleX,0));
	}

	return ; 
	

	/*
	if(fabs(m_curPos.x - m_heroSprite->getPositionX()) < moveLenght)
	{
		if(m_curPos.x < 0)
		{
			m_curPos.x = 0;
		}
		else if(m_curPos.x > 480*gScaleX)
		{
			m_curPos.x = 480*gScaleX;
		}

		m_heroSprite->setPositionX(m_curPos.x);
		//m_heroSprite->setPosition(m_curPos);

		if(m_leftPet)
		{
			m_leftPet->setPositionX(m_curPos.x - 100*gScaleX);
		}

		if(m_rightPet)
		{
			m_rightPet->setPositionX(m_curPos.x + 100*gScaleX);
		}

		return;
	}

	if(m_curPos.x - m_heroSprite->getPositionX() > 0)
	{
		float nextPosX = m_heroSprite->getPositionX() + moveLenght;

		if(nextPosX > 480*gScaleX)
		{
			nextPosX = 480*gScaleX;
		}

		m_heroSprite->setPositionX(nextPosX);

		if(m_leftPet)
		{
			m_leftPet->setPositionX(nextPosX - 100*gScaleX);
		}

		if(m_rightPet)
		{
			m_rightPet->setPositionX(nextPosX + 100*gScaleX);
		}
	}
	else if(m_curPos.x - m_heroSprite->getPositionX() < 0)
	{
		moveLenght = -submove;
		float nextPosX = m_heroSprite->getPositionX() + moveLenght;

		if(nextPosX < 0)
		{
			nextPosX = 0;
		}

		m_heroSprite->setPositionX(nextPosX);

		if(m_leftPet)
		{
			m_leftPet->setPositionX(nextPosX - 100*gScaleX);
		}

		if(m_rightPet)
		{
			m_rightPet->setPositionX(nextPosX + 100*gScaleY);
		}
	}
	else
	{
		if(m_curPos.x < 0)
		{
			m_curPos.x = 0;
		}
		else if(m_curPos.x > 480*gScaleX)
		{
			m_curPos.x = 480*gScaleY;
		}

		m_heroSprite->setPositionX(m_curPos.x);

		if(m_leftPet)
		{
			m_leftPet->setPositionX(m_curPos.x - 100*gScaleX);
		}

		if(m_rightPet)
		{
			m_rightPet->setPositionX(m_curPos.x + 100*gScaleX);
		}
	} */
}

void GameLayer::add_m_score(int value)
{
	m_Score  += value;
	updateScoreLabel();
}

void GameLayer::add_m_money(int value)
{
	int count = value;

	if(m_leftPet)
	{
		int petId = m_leftPet->getPetId();
		int petLevel = m_leftPet->getPetLevel();

		if(petId ==3 && petLevel == PET_MAXLEVEL)
		{
			count*=2;
		}

	}

	if(m_rightPet)
	{
		int petId = m_rightPet->getPetId();
		int petLevel = m_rightPet->getPetLevel();

		if(petId ==3 && petLevel == PET_MAXLEVEL)
		{
			count*=2;
		}
	}

	m_Money += count;
	updateMoneyLabel();
}

void GameLayer::add_m_distance(int value)
{
	m_Distance += value;
	updateDistanceLabel();
}

void GameLayer::set_m_Star(int value)
{
	m_CurStar = value;
}

int GameLayer::getscore()
{
	return m_Score;

}

int GameLayer::getmoney()
{
	return m_Money;

}

int GameLayer::getdistance()
{
	return m_Distance;
}

int GameLayer::getCurStar()
{
	return m_CurStar;
}


void GameLayer::updateMoneyLabel()
{
	char moneyStr[34];
	sprintf(moneyStr, "%d", m_Money);
	m_gainMoney->setString(moneyStr);
}

void GameLayer::updateDistanceLabel()
{
	char disStr[34];
	sprintf(disStr, "%d", m_Distance);
	m_progressLength->setString(disStr);
}

void GameLayer::updateScoreLabel()
{
	char scoreStr[34];
	sprintf(scoreStr, "%d", m_Score);
	m_gainScore->setString(scoreStr);
}

void GameLayer::updateLayer(float t)
{
	if (m_pasueState)
	{
		return;
	}
	static float time = 0;
	time = time + t;

	if(time > 0.3f)
	{
		add_m_distance(1);
		add_m_score(2);
		time = 0;
	}

	//评星规则
	int score =  getscore();

	if(score<m_min)
	{
		set_m_Star(1);
	}
	else if(score>m_min && score<m_max)
	{
		set_m_Star(2);
	}
	else if(score>m_max)
	{
		set_m_Star(3);
	}

	int length = m_pHeadSprite->getTag() - 80*gScaleX;
	float posX = length * MapEventsNode::sharedMapEventsNode()->getMovePercent();
	m_pHeadSprite->setPositionX(posX+40*gScaleX);
	m_mySpriteManager->checkCollision(t);
	m_mySpriteManager->detMonster();
	m_mySpriteManager->deBullet();
	m_mySpriteManager->detItem();

	if(m_heroSprite->getDeadState())
	{
		m_levelPass = false;
		scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f);
	}
	else if (MapEventsNode::sharedMapEventsNode()->getParseOver() && m_mySpriteManager->getMonsterSpriteList()->size()==0)
	{
		/*if(m_isBossLevel)
		{
			m_levelPass = true;
			scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f); 
		}*/
	}
}

void GameLayer::touchMenu(CCObject *pSender)
{	
	ToolsFun::sharedTools()->playEffect(BUTTON);

	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag();
	switch(tag)
	{
		case 100:
			useInvincibleItem(pSender);
			break;
		case 101:
			pauseGame(pSender);
			break;
		default:
			break;
	}
}

void GameLayer::setallTouchEnabled(bool st)
{
	this->setTouchEnabled(st);
	m_menu->setTouchEnabled(st);
}
void GameLayer::pauseGame(CCObject *pSender)
{
	pause();
	GamePause *layer = GamePause::create();
	this->addChild(layer, UI_ZORDER + 1);
}

void GameLayer::pause(float t)
{
	m_pasueState = true;
	setallTouchEnabled(false);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(0);
}

void GameLayer::resume()
{
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
	setallTouchEnabled(true);
	m_pasueState = false;

	setReviveCount();
}

void GameLayer::gameOverDelay(float t)
{
	m_pasueState = true;
	setallTouchEnabled(false);

	if(m_levelPass)
	{
		CCAction *moveto = CCMoveTo::create(2.0, m_heroSprite->getPosition() + ccp(0, 800*gScaleY));
		m_heroSprite->runAction(moveto);

		if(m_leftPet)
		{
			CCAction *moveto1 = CCMoveTo::create(2.0,m_leftPet->getPosition()+ ccp(0,800*gScaleY));
			m_leftPet->runAction(moveto1);
		}
		if(m_rightPet)
		{
			CCAction *moveto2 = CCMoveTo::create(2.0,m_rightPet->getPosition()+ ccp(0,800*gScaleY));
			m_rightPet->runAction(moveto2);
		}

		schedule(schedule_selector(GameLayer::pause), 2.0f);
		GameOver *layer = GameOver::create();

		if(m_levelPass)
		{
			ToolsFun::sharedTools()->playEffect(GAME_COMPLETE);
			layer->initWin();
		}

		this->addChild(layer, UI_ZORDER + 1);
	}
	else 
	{		
		int heroId = GameConfigData::initialGameData().getCurHeroId();
		switch(heroId)
		{
		case 0:
			{
				ToolsFun::sharedTools()->playEffect(DEAD_4);
				break;
			}
		case 1:
			{
				ToolsFun::sharedTools()->playEffect(DEAD_2);
				break;
			}
		case 2:
			{
				ToolsFun::sharedTools()->playEffect(DEAD_3);
				break;
			}
		default:
			break;
		}

		ToolsFun::sharedTools()->playEffect(GAME_FAILED);

		if(m_firstRevive>0)
		{
			m_firstRevive--;
			pause();
			setallTouchEnabled(false);
			reviveLevel(NULL);
		}
		else
		{
			showReviveUI();
		}
	}
}
void GameLayer::showReviveUI()
{
	setallTouchEnabled(false);

	if(this->getChildByTag(2014717))
	{
		this->removeChildByTag(2014717);
	}

	pause();
	int level = GameConfigData::initialGameData().getCurLevel();

	char buf[64];
	sprintf(buf,"%d",level);

	HttpClientNode::sharedHttpClient()->eventSend(2,buf);

	GameReviveLayer *layer = GameReviveLayer::create();

	this->addChild(layer,UI_ZORDER+1,2014717);

	layer->setCloseCallBack(this,menu_selector(GameLayer::gameOver));

}

void GameLayer::gameOver(CCObject* pSender)
{
	if (!m_levelPass)
	{
		GameOver *layer = GameOver::create();
		layer->initLose();
		this->addChild(layer, UI_ZORDER + 1);
		pause();
	}
	else
	{
		pause();
	}
}

void GameLayer::useInvincibleItem(CCObject *pSender)
{
	if(m_heroSprite->getInvincible())
	{
		return;
	}

	if(m_revive ==0)
	{
		JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(GameLayer::purchaseSuccess));
		JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(GameLayer::purchaseFailed));
		JniCall::sharedJniCall()->purchaseByIndex(14);
		pauseGame(NULL);
	}
	else
	{
		m_revive --;
		GameConfigData::initialGameData().setReviveValue(m_revive);
		m_heroSprite->addInvincibleEffect();
		setReviveCount();
	}
}

void GameLayer::purchaseSuccess(const char* str)
{
	if(strcmp(str, "10") ==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=5000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
			
		int reviveCount = GameConfigData::initialGameData().getReviveValue();
		reviveCount+=3;
		GameConfigData::initialGameData().setReviveValue(reviveCount);
				
		CCUserDefault::sharedUserDefault()->setBoolForKey("active",true);
	    resume();
		this->removeChildByTag(112255);
										
		//setReviveCount();
	}
	else if(strcmp(str,"14") ==0)
	{
		m_revive+=3;;
		GameConfigData::initialGameData().setReviveValue(m_revive);
		//setReviveCount();
	}
	else if(strcmp(str,"6") ==0)
	{
		this->removeChildByTag(112275);
		
		int  count = GameConfigData::initialGameData().getLuckyCount() ;
		GameConfigData::initialGameData().setLuckyCount(count+30);
		
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
		
		GameConfigData::initialGameData().writeData(); 
	}
}

void GameLayer::purchaseFailed(const char* str)
{
}

void GameLayer::parseMapEvent(LevelEvent *event)
{
	string name = event->eventName;
	string nameC = name.substr(0, 2);

	if(nameC == "cE")
	{
		createEnemyByEvent(event);
	}
	else if(name == "addTip")
	{
		addGameTip(event);
	}
	else if(name == "pauseMove")
	{
		pauseParseEvent(event);
	}
	else if(name == "addStory")
	{
		//parseStoryEvent(event);
	}
	else if(name == "cI")
	{
		createItemByEvent(event);
	}
	else if(name == "endGame")
	{
		showEndLine(event);
	}
}

void GameLayer::showEndLine(LevelEvent *event)
{
	int firstPosX = 0;
	for(int i = 0; i != event->keyValues.size(); ++i)
	{
		int value =  atoi(event->keyValues[i].keyValue.c_str());

		 if(event->keyValues[i].keyName == "firstPosX")
		{
			firstPosX = value;
		}
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCNode *node = CCNode::create();
	for(int i=0;i<6;i++)
	{
		CCSprite *sprite = CCSprite::create("ui/tipUI/gainStart.png");
		node->addChild(sprite);
		sprite->setPositionX(sprite->getContentSize().width/2+80*i);
	}

	this->addChild(node,2580,1235896);
	node->setPosition(ccp(0,800*gScaleY));
}

void GameLayer::updateEndLinePos(float t)
{
	if(this->getChildByTag(1235896))
	{

		CCNode *node = this->getChildByTag(1235896);

		if(node->getPositionY()<m_heroSprite->getPositionY())
		{

			node->stopAllActions();
			node->removeFromParent();

			int level=GameConfigData::initialGameData().getCurLevel();

			if(level<=1)
			{
				showGiftbag(1);
			}
			else if(level ==2)
			{
				show30luckyDialog();
			}
			else
			{
				m_levelPass = true;
				scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f); 
			}

			return ;
		}

		int curLevel = GameConfigData::initialGameData().getCurLevel();
		float speedScale = curLevel/5;

		float speed = DataInitial::initialDataSource().getMapNodeSpeed()+speedScale*50;

	

		float curPosY = node->getPositionY();
		float nextPosY1 = curPosY -speed * t;

		node->setPositionY(nextPosY1);
	}
}


void GameLayer::show30luckyDialog()
{
	bool lucky = CCUserDefault::sharedUserDefault()->getBoolForKey("lucky",false);

	if(!lucky)
	{
		pause();

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
		addChild(bkg,UI_ZORDER+1);
		bkg->setPosition(ccp(size.width/2,size.height/2));
		bkg->setTag(112275);

		char buf[32];
		sprintf(buf,"DayReward/30lucky.png");
		CCSprite *tip1 = CCSprite::create(buf);
		bkg->addChild(tip1);
		tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

		CCMenu *menu = CCMenu::create();
		bkg->addChild(menu);
		menu->setPosition(CCPointZero);

		CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
		CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
		closeSel->setColor(ccc3(80,80,80));
		CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(GameLayer::show30luckyDialogClose));
		itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
		menu->addChild(itemClose);

		CCSprite *spNml = CCSprite::create("DayReward/button.png");
		CCSprite *spSel = CCSprite::create("DayReward/button.png");
		spSel->setColor(ccc3(80,80,80));
		CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(GameLayer::show30luckyDialogSure));
		itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,25));
		menu->addChild(itemSp);
	}
	else
	{
		m_levelPass = true;
		scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f); 
	}
}

void GameLayer::show30luckyDialogClose(CCObject *sender)
{
	this->removeChildByTag(112275);
	CCUserDefault::sharedUserDefault()->setBoolForKey("lucky",true);

	resume();
	m_levelPass = true;
	scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f); 
}

void GameLayer::show30luckyDialogSure(CCObject *sender)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("lucky",true);

	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(GameLayer::purchaseSuccess));
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(GameLayer::purchaseFailed));
	JniCall::sharedJniCall()->purchaseByIndex(6);

	resume();
	m_levelPass = true;
	scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f); 
}

void GameLayer::endLineActionFinish(CCNode *obj)
{
	CCSprite *sprite = (CCSprite *)obj;
	sprite->removeFromParent();

	m_levelPass = true;
	scheduleOnce(schedule_selector(GameLayer::gameOverDelay), 0.1f);
}

void GameLayer::createEnemyByEvent(LevelEvent *event)
{
	int mId = event->eventType;
	int mCount = 1;
	int firstPosX = 0;
	int addX = 0;
	int d0 = 0;
	int d1 = 0;
	int itemId = 0;

	for(int i = 0; i != event->keyValues.size(); ++i)
	{
		int value =  atoi(event->keyValues[i].keyValue.c_str());

		if(event->keyValues[i].keyName == "mCount")
		{
			mCount = value;
		}
		else if(event->keyValues[i].keyName == "firstPosX")
		{
			firstPosX = value;
		}
		else if(event->keyValues[i].keyName == "addX")
		{
			addX = value;
		}
		else if(event->keyValues[i].keyName == "speed")
		{
			d0 = value;
		}
		else if(event->keyValues[i].keyName == "default1")
		{
			d1 = value;
		}
		else if(event->keyValues[i].keyName == "itemId")
		{
			itemId = value;
		}
	}

	int curLevel = GameConfigData::initialGameData().getCurLevel();
	float speedScale = curLevel/5;

	if(d0 == 200)
	{
		d0=200+speedScale*50;
	}

	for(int i = 0; i != mCount; ++i)
	{
		int posx = firstPosX  + i * addX;
		CreateMInf mInf;
		mInf.mMonsterId = mId;
		mInf.mPosX = posx;
		mInf.mPosY = (800 + 60)*gScaleY;
		mInf.mActionSpeed = d0;
		mInf.mDeadItemId = itemId;
		mInf.mItemPercent = d1;
		MonsterSprite *sprite = m_mySpriteManager->createMonster(mInf);
		this->addChild(sprite, MONSTER_ZORDER+i);
		sprite->setPosition(ccp(mInf.mPosX, mInf.mPosY));

		if(mId == 1007)
		{
			sprite->addMonsterTip();
			sprite->setPositionY(sprite->getPositionY() + 80*gScaleY);
		}

		if(sprite->isBoss())
		{
			sprite->bossMove(0);
			MapEventsNode::sharedMapEventsNode()->pauseMapAction();
		}
	}
}

void GameLayer::createEnemy2ByBoss(CCPoint point)
{
	int curLevel = GameConfigData::initialGameData().getCurLevel();
	float speedScale = curLevel/5;

	int d0 = 400+speedScale*50;;
	
	srand(time(NULL));
	int mCount = rand()%3+2;
	
	float firstPosX = point.x;
	float addX=70;

	for(int i = 0; i != mCount; ++i)
	{
		int posx = firstPosX  + i * addX;
		CreateMInf mInf;
		mInf.mMonsterId = 2;
		mInf.mPosX = posx;
		mInf.mPosY = (600 + 60)*gScaleY;
		mInf.mActionSpeed = d0;
		mInf.mDeadItemId = 4;
		mInf.mItemPercent = 0;
		MonsterSprite *sprite = m_mySpriteManager->createMonster(mInf);
		this->addChild(sprite, MONSTER_ZORDER+i);

		sprite->setPosition(ccp(mInf.mPosX, mInf.mPosY));
	} 
}

void GameLayer::createEnemy1007ByBoss(CCPoint point)
{
	int curLevel = GameConfigData::initialGameData().getCurLevel();
	float speedScale = curLevel/5;

	int d0 = 400+speedScale*50;;

	srand(time(NULL));
	int mCount = rand()%3+1;

	float firstPosX = point.x;
	float addX=70;

	float posX[4] = {50,180,310,440};

	for(int i = 0; i != mCount; ++i)
	{
		int posx = firstPosX  + i * addX;
		CreateMInf mInf;
		mInf.mMonsterId = 1007;
		mInf.mPosX = posX[i+rand()%3];
		CCLog("%f\n",mInf.mPosX);
		mInf.mPosY = (800 + 60)*gScaleY;
		mInf.mActionSpeed = 1000;
		mInf.mDeadItemId = 4;
		mInf.mItemPercent = 0;
		MonsterSprite *sprite = m_mySpriteManager->createMonster(mInf);
		this->addChild(sprite, MONSTER_ZORDER+i);

		sprite->setPosition(ccp(mInf.mPosX, mInf.mPosY));

		if(mInf.mMonsterId == 1007)
		{
			sprite->addMonsterTip();
			sprite->setPositionY(sprite->getPositionY() + 80*gScaleY);
		}

	} 
}

void GameLayer::createItemByEvent(LevelEvent *event)
{
	int mId = event->eventType;
	int mCount = 1;
	int firstPosX = 0;
	int addX = 0;
	int d0 = 0;
	int d1 = 0;

	for(int i = 0; i != event->keyValues.size(); ++i)
	{
		int value =  atoi(event->keyValues[i].keyValue.c_str());

		if(event->keyValues[i].keyName == "mCount")
		{
			mCount = value;
		}
		else if(event->keyValues[i].keyName == "firstPosX")
		{
			firstPosX = value;
		}
		else if(event->keyValues[i].keyName == "addX")
		{
			addX = value;
		}
		else if(event->keyValues[i].keyName == "speed")
		{
			d0 = value;
		}
		else if(event->keyValues[i].keyName == "default1")
		{
			d1 = value;
		}
	}

	for(int i = 0; i != mCount; ++i)
	{
		int posx = firstPosX  + i * addX;

		int itemId=mId;

		int mPosX = posx;
		int mPosY = (800+60)*gScaleY;

		ItemSprite *sprite=m_mySpriteManager->createItemById(itemId);
		this->addChild(sprite,200);
		sprite->setPosition(ccp(mPosX,mPosY));
	}
}

void GameLayer::addGameTip(LevelEvent *event)
{
	int tipId = 0;

	for(int i = 0; i != event->keyValues.size(); ++i)
	{
		int value =  atoi(event->keyValues[i].keyValue.c_str());

		if(event->keyValues[i].keyName == "tipId")
		{
			tipId = value;
		}
	}

	showTipById(tipId);
}

void GameLayer::pauseParseEvent(LevelEvent *event)
{
	MapEventsNode::sharedMapEventsNode()->pauseMapAction();
}

void GameLayer::parseStoryEvent(LevelEvent *event)
{
	int sId = 0;

	for(int i = 0; i != event->keyValues.size(); ++i)
	{
		int value =  atoi(event->keyValues[i].keyValue.c_str());

		if(event->keyValues[i].keyName == "storyId")
		{
			sId = value;
		}
	}

	showStoryById(sId);
}

void GameLayer::showTipById(int tipId)
{
	char filename[66];
	sprintf(filename, "ui/gameTip/tip%d.png", tipId);
	CCSprite *spriteTip = CCSprite::create(filename);
	this->addChild(spriteTip, UI_ZORDER);
	spriteTip->setPosition(ccp(640*gScaleX, 700*gScaleY));
	CCSequence *seq = CCSequence::create(CCMoveTo::create(1.0f, ccp(240*gScaleX, 700*gScaleY))
	                                     , CCDelayTime::create(0.4f)
	                                     , CCFadeOut::create(0.4f)
	                                     , CCCallFuncN::create(this, callfuncN_selector(GameLayer::removeChild))
	                                     , NULL);
	spriteTip->runAction(seq);
}

void GameLayer::showStoryById(int sId)
{
	if(m_storyNode)
	{
		return;
	}

	pause();
	DialogLayer *dialogLayer = DialogLayer::create();
	dialogLayer->init();
	dialogLayer->initDialog2(sId);
	dialogLayer->setCallBackFun(this,callfuncInt_selector(GameLayer::storyEnd));
	this->addChild(dialogLayer,UI_ZORDER);

	MapEventsNode::sharedMapEventsNode()->pauseMapAction();
	this->setTouchEnabled(false);
	m_menu->setTouchEnabled(false);
}

void GameLayer::storyEnd(int ok)
{
	this->setTouchEnabled(true);
	m_menu->setTouchEnabled(true);
	resume();
}

CCPoint GameLayer::getHeroPos()
{
	return m_heroSprite->getPosition();
}

HeroSprite *GameLayer::getHeroSprite()
{
	return m_heroSprite;
}

void GameLayer::mapSpeedUp()
{
	if(m_mapNode)
	{
		m_mapNode->mapSpeedUp();
	}
}

void GameLayer::mapSpeedUpEnd()
{
	if(m_mapNode)
	{
		m_mapNode->mapSpeedEnd();
	}
}

void GameLayer::addMTip(int mId , CCPoint tippos)
{	
	ToolsFun::sharedTools()->playEffect(GAME_WARNING);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite * Mtip = CCSprite::create("monster/warning.png");
	this->addChild(Mtip, 158, WARNING);
	Mtip->setPosition(tippos - ccp(0, 200*gScaleX));
	CCAction *blink = CCBlink::create(1.5, 3);
	Mtip->runAction(blink);
	Mtip->runAction(CCSequence::create(CCDelayTime::create(1.5),CCCallFunc::create(Mtip,callfunc_selector(CCSprite::removeFromParent)),NULL));
}

void GameLayer::removeMTip()
{
	this->removeChildByTag(WARNING);
}
//add zh

void GameLayer::showDeadthEffect(CCPoint dead_pos,int mId)
{   
	switch(mId){
	case 0:
	case 5:
		{
		//break;
	       }
	case 1:
		{
		//break;
	       }
	case 2:
		{
		//break;
	       }
	case 3:
		//break;
	case 4:
		{
			ToolsFun::sharedTools()->playEffect(DEAD_1);
			char aniKeyName[40];
			sprintf(aniKeyName,"smoke_%%d.png");
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			CCSprite *sp = CCSprite::createWithSpriteFrameName("smoke_0.png");
			this->addChild(sp,58,MONSTERDEAD);
			sp->setPosition(dead_pos);
			CCSequence *seq = CCSequence::create(ani,CCFadeOut::create(0.05f),CCCallFuncN::create(this,callfuncN_selector(GameLayer::removeDeadEffect)),NULL);

			sp->runAction(seq);
			break;
	       }
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		{
			ToolsFun::sharedTools()->playEffect(DEAD_2);

			char aniKeyName0[40];
			sprintf(aniKeyName0,"attack_%%d.png");
			CCAnimation *animation0 = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName0);
			CCAnimate *ani0 = CCAnimate::create(animation0);
			CCSprite * sp0 = CCSprite::createWithSpriteFrameName("attack_0.png");
			this->addChild(sp0,58,MONSTERDEAD);
			sp0->setPosition(dead_pos);
			CCSequence *seq0 = CCSequence::create(ani0,CCFadeOut::create(0.05F),CCCallFuncN::create(this,callfuncN_selector(GameLayer::removeDeadEffect)),NULL);

			sp0->runAction(seq0);

			char aniKeyName[40];
			sprintf(aniKeyName,"blast_%%d.png");
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			CCSprite * sp = CCSprite::createWithSpriteFrameName("blast_0.png");
			this->addChild(sp,58,MONSTERDEAD);
			sp->setPosition(dead_pos);
			CCSequence *seq = CCSequence::create(ani,CCFadeOut::create(0.05F),CCCallFuncN::create(this,callfuncN_selector(GameLayer::removeDeadEffect)),NULL);
			sp->runAction(seq);

			list<MonsterSprite *> *monsterList =  m_mySpriteManager->getMonsterSpriteList();

			bool resumeParse = false;

			for(list<MonsterSprite*>::iterator iter = monsterList->begin();iter != monsterList->end();iter++)
			{
				if(*iter)
				{
					if((*iter)->isBoss() && !(*iter)->getDeadState())
					{
						resumeParse = true;
						break;
					}
				}
			}
			
			if(!resumeParse)
			{
				MapEventsNode::sharedMapEventsNode()->resumeMapAction();
			}

			break;
		}
	case 1000:
	case 1001:
	case 1002:
	case 1003:
	case 1004:
	case 1005:
	case 1006:
	case 1007:
		{

		}
		break;
	default:
		{
			/*
			ToolsFun::sharedTools()->playEffect(DEAD_3);
			char aniKeyName[40];
			sprintf(aniKeyName,"blood%d_%%d.png",0);
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			CCSprite * sp = CCSprite::createWithSpriteFrameName("blood0_0.png");
			this->addChild(sp,58,MONSTERDEAD);
			sp->setPosition(dead_pos);
			sp->setScale(0.3);
			CCSequence *seq = CCSequence::create(ani,CCFadeOut::create(0.05F),CCCallFuncN::create(this,callfuncN_selector(GameLayer::removeDeadEffect)),NULL);

			sp->runAction(seq);*/

			ToolsFun::sharedTools()->playEffect(DEAD_1);
			char aniKeyName[40];
			sprintf(aniKeyName,"smoke_%%d.png");
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			CCSprite *sp = CCSprite::createWithSpriteFrameName("smoke_0.png");
			this->addChild(sp,58,MONSTERDEAD);
			sp->setPosition(dead_pos);
			CCSequence *seq = CCSequence::create(ani,CCFadeOut::create(0.05f),CCCallFuncN::create(this,callfuncN_selector(GameLayer::removeDeadEffect)),NULL);

			sp->runAction(seq);
			break;
		}
	}

	m_killMosTotal+=1;

}

int GameLayer::getKillMonTotal()
{
	return m_killMosTotal;
}

void GameLayer::removeDeadEffect(CCNode *node)
{
	node->removeFromParent();
}

void GameLayer::setInvincibleItem(bool enable)
{
	CCMenuItemSprite *itemSp = (CCMenuItemSprite*)(m_menu->getChildByTag(100));
	itemSp->setEnabled(enable);
}

MySpriteManager *GameLayer::getMySpriteManager()
{
	return m_mySpriteManager;
}

void GameLayer::addBullet(BulletSprite *bullet, int zOrder)
{
	this->addChild(bullet, zOrder);
}

void GameLayer::addBullet(HeroBulletSprite *bullet,int zOrder )
{
	this->addChild(bullet, zOrder);
}
void GameLayer::resumeLevel(CCLayer *layer)
{
	layer->removeFromParentAndCleanup(true);
	list<MonsterSprite *> *monsterList =  m_mySpriteManager->getMonsterSpriteList();

	bool resumeParse = false;

	for(list<MonsterSprite*>::iterator iter = monsterList->begin();iter != monsterList->end();iter++)
	{
		if(*iter)
		{
			if((*iter)->isBoss() && !(*iter)->getDeadState())
			{
				resumeParse = true;
				break;
			}
		}
	}

	if(!resumeParse)
	{
		MapEventsNode::sharedMapEventsNode()->resumeMapAction();
	}

	//MapEventsNode::sharedMapEventsNode()->resumeMapAction();
	this->setTouchEnabled(true);
	m_menu->setTouchEnabled(true);
	resume();
	setReviveCount();
}

void  GameLayer::reviveLevel(CCLayer *layer)
{
	if(layer)
	{
		layer->removeFromParentAndCleanup(true);
	}

	initHero();

	m_heroSprite->addInvincibleEffect();
	m_mySpriteManager->detAllMonster();
	setallTouchEnabled(true);

	resume();
		
	setallTouchEnabled(true);	
	registerNDK();
}

void GameLayer::quitLevel()
{
	LD("quitlevel");
	resume();
	releaseGameLayerData();
	LevelChoice *levelLayer = LevelChoice::create();
	CCScene *scene = CCScene::create();
	scene->addChild(levelLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void GameLayer::releaseGameLayerData()
{
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
	MapEventsNode::sharedMapEventsNode()->removeMapEventNode();
	StartGameLoading::releaseImageInGame();
	m_heroSprite->stopAllActions();
	unscheduleAllSelectors();
	this->stopAllActions();
	m_mySpriteManager->setThreadState(THREAD_END);
	m_mapNode->removeAllChildren();
	this->removeFromParentAndCleanup(true);
}

void GameLayer::retryLevel()
{
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
	m_heroSprite->stopAllActions();
	this->unscheduleAllSelectors();
	this->stopAllActions();
	m_mySpriteManager->setThreadState(THREAD_END);
	m_mapNode->stopAllActions();
	m_mapNode->unscheduleAllSelectors();
	m_mapNode->removeAllChildren();
	MapEventsNode::sharedMapEventsNode()->retryMapEvent();
	this->removeAllChildrenWithCleanup(true);
	delete m_mySpriteManager;
	m_mySpriteManager = NULL;
	CCDirector::sharedDirector()->getRunningScene()->removeAllChildrenWithCleanup(true);
	gGameLayer = NULL;
	CCScene *scene = CCScene::create();
	GameLayer *gLayer = GameLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
	gLayer->scheduleStart();
}

void GameLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 100, false);
}


bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//获取触摸初始坐标
	m_nodeMoved = false;
	CCPoint pos = pTouch->getLocation();
	m_curPos = pos;
	//m_lastPos = pos; 
	return true;
}


void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	//获取触摸坐标  
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	CCPoint pos = pTouch->getDelta();
	CCPoint currentPos = m_heroSprite->getPosition();
	currentPos = ccpAdd(currentPos, pos);
	currentPos = ccpClamp(currentPos, CCPointZero, ccp(size.width, size.height));
	m_heroSprite->setPosition(currentPos);
}


void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	m_lastPos = m_curPos;
	m_curPos = pTouch->getLocation();

	m_offectPos = m_curPos-m_lastPos;
}

GameLayer *GameLayer::sharedGameLayer()
{
	return gGameLayer;
}

void GameLayer::keyBackClicked()
{
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniCall::sharedJniCall()->exit();;
#endif*/

	pauseGame(NULL);
}
