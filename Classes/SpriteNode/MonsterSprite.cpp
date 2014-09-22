#include "MonsterSprite.h"
#include "../GCData/DataInitial.h"
#include "SpriteAniManager.h"
#include "SpriteAction.h"
#include "MySpriteManager.h"
#include "../GLayer/GameLayer.h"
#include "BulletSprite.h"
#include "HeroSprite.h"
#include "../GCData/DataInitial.h"
#include "../GCData/GameConfigData.h"

MonsterWeapon gMonsterWeapon1[5] =
{
	&MonsterSprite::shoot0BulletOne,
	&MonsterSprite::shoot1BulletOne,
	&MonsterSprite::shoot2BulletOne,
	&MonsterSprite::shoot3BulletOne,
	&MonsterSprite::shoot4BulletOne
};

MonsterWeapon gMonsterWeapon2[5] =
{
	&MonsterSprite::shoot0BulletTwo,
	&MonsterSprite::shoot1BulletTwo,
	&MonsterSprite::shoot2BulletTwo,
	&MonsterSprite::shoot3BulletTwo,
	&MonsterSprite::shoot4BulletTwo
};

MonsterWeapon gMonsterWeapon3[5] =
{
	&MonsterSprite::shoot0BulletThree,
	&MonsterSprite::shoot1BulletThree,
	&MonsterSprite::shoot2BulletThree,
	&MonsterSprite::shoot3BulletThree,
	&MonsterSprite::shoot4BulletThree
};

#define MONSTERDIFF 300
MonsterSprite::MonsterSprite(int mId): m_monsterId(mId)
	, m_shootFrq(1.0f)
	, m_monsterData(NULL)
	, m_score(0)
	, m_shootEnabled(false)
	, m_shootFunId(-1)
	, m_bulletId(-1)
	, m_isBoss(false)
	, m_moveStart(true)
	,m_shootTag(0)
	,m_shootOrientation(false)
	,m_lowHp(0)
	,m_isPlayDownUp(true)
	,m_slowDown(false)
{
}

MonsterSprite::MonsterSprite(const CreateMInf& mInf): m_monsterId(mInf.mMonsterId)
	, m_shootFrq(1.0f)
	, m_monsterData(NULL)
	, m_score(0)
	, m_shootEnabled(false)
	, m_shootFunId(-1)
	, m_bulletId(-1)
	, m_isBoss(false)
	, m_actionId(-1)
	, m_moveStart(true)
	,m_shootTag(0)
	,m_shootOrientation(false)
	,m_lowHp(0)
	,m_isPlayDownUp(true)
	,m_slowDown(false)
{
	setMinf(mInf);
}

MonsterSprite::~MonsterSprite()
{
}

MonsterSprite *MonsterSprite::createMonsterById(int mId)
{
	MonsterSprite *mSprite = new MonsterSprite(mId);

	if(mSprite && mSprite->init())
	{
		mSprite->autorelease();
		return mSprite;
	}

	delete mSprite;
	return NULL;
}

MonsterSprite *MonsterSprite::createMonsterByInf(const CreateMInf& mIf)
{
	MonsterSprite *mSprite = new MonsterSprite(mIf);

	if(mSprite && mSprite->init())
	{
		mSprite->autorelease();
		return mSprite;
	}

	delete mSprite;
	return NULL;
}

bool MonsterSprite::init()
{
	int cur =GameConfigData::initialGameData().getCurLevel();
if(DataInitial::initialDataSource().getLvTag() == SUB_LEVEL )
	{
		cur = GameConfigData::initialGameData().getCurSubLevel();

		LevelUnLock *data =  DataInitial::initialDataSource().getSubLevelUnLockInf(cur);
		if(data!=NULL)
		{
			cur = data->mUnLockValue1;
		}
	}
	m_monsterData = DataInitial::initialDataSource().getMDataById(m_monsterId);
	m_fullhp = m_monsterData->mHp;
	m_hp = m_monsterData->mHp*(1.0+0.15*cur)*(1.0+0.15*cur);
	m_maxHp = m_monsterData->mHp;
	m_speed = m_monsterData->mSpeed;
	m_boundRadius = m_monsterData->mBoundRadius;
	m_boundRadiusInitState = true;
	m_shootFrq = m_monsterData->mShootFrq;
	m_score = m_monsterData->mScore;
	m_shootEnabled = m_monsterData->mShootEnabled;
	m_shootFunId = m_monsterData->mShootFunId;
	m_bulletId = m_monsterData->mBulletId ;
	m_isBoss = m_monsterData->mIsBoss;
	m_lowHp = m_monsterData->mLowHp;

	m_maxHp = m_hp;
	m_fullhp = m_hp;

	initWithAni();
	initWithFiles();

	if(m_isBoss)
	{
		scheduleShoot();
		addBloodBar();
	}
	return true;
}

void MonsterSprite::initWithAni()
{
	if(!m_isBoss)
	{
		char filename[33];

		if(m_monsterId<1000)
		{
			sprintf(filename, "monster%d_0.png", m_monsterId);
			this->initWithSpriteFrameName(filename);
		}
		else if(m_monsterId ==1007)
		{
			sprintf(filename, "monster%d_0.png", m_monsterId);
			this->initWithSpriteFrameName(filename);
			this->setScale(0.8f);
		}
		else
		{
			sprintf(filename, "monster/monster%d_0.png", m_monsterId);
			this->initWithFile(filename);
		}

		if(m_monsterId < 1000)
		{
			char aniKeyName[40];
			sprintf(aniKeyName, "monster%d_%%d.png", m_monsterId);
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			this->runAction(CCRepeatForever::create(ani));
		}
		else if(m_monsterId == 1007)
		{
			char aniKeyName[40];
			sprintf(aniKeyName, "monster1007_%%d.png");
			CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
			CCAnimate *ani = CCAnimate::create(animation);
			this->runAction(CCRepeatForever::create(ani));
		}
	}
	else
	{
		createBossById(m_monsterId);
		addBossEffectById(m_monsterId);
	}
}

void MonsterSprite::addBloodBar()
{
	CCSprite *bgSprite = CCSprite::create("monster/hp_back.png");
	bgSprite->setAnchorPoint(ccp(0.5, 0));
	bgSprite->setPosition(ccp(this->getContentSize().width / 2, 0));
	addChild(bgSprite, 1, 1);
	CCSprite *hpSprite = CCSprite::create("monster/hp_front.png");
	CCProgressTimer *progressTimer = CCProgressTimer::create(hpSprite);
	progressTimer->setAnchorPoint(ccp(0.5, 0));
	progressTimer->setPosition(bgSprite->getPosition()+ccp(0,3));
	progressTimer->setType(kCCProgressTimerTypeBar);
	progressTimer->setMidpoint(ccp(0, 0.5));
	progressTimer->setBarChangeRate(ccp(1, 0));
	progressTimer->setPercentage(100);
	addChild(progressTimer, 2, 2014719);
	schedule(schedule_selector(MonsterSprite::updateBloodBar), 1.0f / 9.0f);
}
void MonsterSprite::updateBloodBar(float t)
{
	if(this->getDeadState() == false)
	{
		double hp = this->getHp();
		double fullhp = m_fullhp;
		float percent = 100 * (float)hp / fullhp;
		((CCProgressTimer*)this->getChildByTag(2014719))->setPercentage(percent);
	}
}
void MonsterSprite::createBossById(int id)
{
	char filename[33];
	sprintf(filename, "monster/monster%d_0.png", id);
	this->initWithFile(filename);
	char shadowFile[33];
	sprintf(shadowFile, "monster/monster%d_1.png", id);
	CCSprite *shadow = CCSprite::create(shadowFile);
	shadow->setAnchorPoint(CCPointZero);
	this->addChild(shadow, -2);

	switch(id)
	{
	case 8:
	case 9:
	case 10:
		{
			char nmlname[33];
			sprintf(nmlname, "monster%d_00.png", id);
			char nmlname2[33];
			sprintf(nmlname2, "monster%d_0%%d.png", id);
			CCSprite *sprite1 = CCSprite::createWithSpriteFrameName(nmlname);
			sprite1->setAnchorPoint(CCPointZero);
			this->addChild(sprite1, -1);
			CCAnimation *animation1 = SpriteAniManager::getAniManager()->getSpriteAnimationByName(nmlname2);
			CCRepeatForever *ani1 = CCRepeatForever::create(CCAnimate::create(animation1));
			sprite1->runAction(ani1);
		}
		break;

	case 11:
	case 12:
		{
			char nmlname[33];
			sprintf(nmlname, "monster%d_00.png", id);
			CCSprite *sprite1 = CCSprite::createWithSpriteFrameName(nmlname);
			sprite1->setAnchorPoint(CCPointZero);
			this->addChild(sprite1, -1, 200 + id);
		}
		break;

	default:
		break;
	}
}

void MonsterSprite::addBossEffectById(int id)
{
	switch(id)
	{
	case 8:
		{
			char nmlname[33];
			sprintf(nmlname, "monster%d_10.png", id);
			char nmlname2[33];
			sprintf(nmlname2, "monster%d_1%%d.png", id);
			CCSprite *sprite1 = CCSprite::createWithSpriteFrameName(nmlname);
			sprite1->setAnchorPoint(CCPointZero);
			this->addChild(sprite1, -1);
			CCAnimation *animation1 = SpriteAniManager::getAniManager()->getSpriteAnimationByName(nmlname2);
			CCRepeatForever *ani1 = CCRepeatForever::create(CCAnimate::create(animation1));
			sprite1->runAction(ani1);
			break;
		}

	case 9:
	case 10:
		{
		}
		break;

	case 11:
	case 12:
		{
			char nmlname2[33];
			sprintf(nmlname2, "monster%d_0%%d.png", id);
			CCSprite * sprite1 = (CCSprite*)this->getChildByTag(200 + id);
			CCAnimation *animation1 = SpriteAniManager::getAniManager()->getSpriteAnimationByName(nmlname2);
			CCRepeatForever *ani1 = CCRepeatForever::create(CCAnimate::create(animation1));
			sprite1->runAction(ani1);
		}
		break;

	default:
		break;
	}
}
void MonsterSprite::initWithFiles()
{
}

int MonsterSprite::getMonsterId()
{
	return m_monsterId;
}

int MonsterSprite::getScore()
{
	return m_score;
}

void   MonsterSprite::setMinf(const CreateMInf & cInf)
{
	m_createMInf = cInf;
}

CreateMInf& MonsterSprite::getMInf()
{
	return m_createMInf;
}

void MonsterSprite::updatePosWithMap(float t)
{
	if(!m_moveStart)
	{
		return;
	}

	CCAssert(GameLayer::sharedGameLayer() != NULL, "MonsterSprite::updatePosWithMa must not be NULL");
	bool upState = GameLayer::sharedGameLayer()->getHeroSprite()->getSpeedUpState();
	float scale = 1.0;

	if(upState)
	{
		scale = DataInitial::initialDataSource().getMapNodeUpSpeed() / DataInitial::initialDataSource().getMapNodeSpeed();
	}

	this->setPositionY(this->getPositionY() - scale * m_createMInf.mActionSpeed * t);
}


void MonsterSprite::shootBullet(float t)
{
	if(m_isPlayDownUp)
	{
		if(m_hp<m_lowHp)
		{
			unschedule(schedule_selector(MonsterSprite::shootBullet));
			playMoveDownUp();
			m_isPlayDownUp = false;
		}
	}

	if(m_shootTag ==0)
	{
		(this->*gMonsterWeapon1[m_monsterId-8])();
	}
	else if(m_shootTag==1)
	{
		(this->*gMonsterWeapon2[m_monsterId-8])();
	}
	else if(m_shootTag ==2)
	{
		(this->*gMonsterWeapon3[m_monsterId-8])();
	}

	if(m_shootTag==0)
	{
		m_shootTag =1;
	}
	else if(m_shootTag ==1)
	{
		m_shootTag =2;
	}else if(m_shootTag ==2)
	{
		m_shootTag =0;
	}
}

void MonsterSprite::playMoveDownUp()
{
	this->stopAllActions();

	CCMoveTo *moveDown = CCMoveTo::create(0.5,ccp(this->getPositionX(),this->getContentSize().height/2));
	CCMoveTo *moveUp = CCMoveTo::create(0.5,ccp(this->getPositionX(),this->getPositionY()));
	this->runAction(CCSequence::create(moveDown,moveUp,CCCallFunc::create(this,callfunc_selector(MonsterSprite::animationEnd)),NULL));

}

void MonsterSprite::animationEnd()
{
	this->stopAllActions();
	scheduleShoot();
	bossMove(0);
}

void MonsterSprite::shoot0BulletOne()
{

	static bool isThree = false;

	CCPoint point[2] = {ccp(-50,-100*gScaleY),ccp(550*gScaleX,-100*gScaleY)};
	int id = m_monsterId;

	for(int i=0;i<2;i++)
	{
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());

		CCMoveTo *moveTo = CCMoveTo::create(0.8,point[i]);
		bullet->runAction(CCRepeatForever::create(moveTo));

	}

	if(isThree)
	{
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());

		CCMoveTo *moveTo = CCMoveTo::create(1.2,ccp(250*gScaleX,-100*gScaleY));
		bullet->runAction(CCRepeatForever::create(moveTo));
	}

	if(isThree)
	{
		isThree = false;
	}
	else
	{
		isThree = true;
	}

}

void MonsterSprite::shoot1BulletOne()
{
	CCPoint point[2] = {ccp(0,-100*gScaleY),ccp(450*gScaleX,-100*gScaleY)};

	for(int i=0;i<2;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		CCMoveTo *moveTo = CCMoveTo::create(0.8,point[i]);
		bullet->runAction(CCRepeatForever::create(moveTo));
	}
}

void MonsterSprite::shoot2BulletOne()
{
	int id = m_monsterId;
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition());
	CCMoveBy *moveBy = CCMoveBy::create(0.4f, ccp(0, -400*gScaleY));
	bullet->runAction(CCRepeatForever::create(moveBy));
}

void MonsterSprite::shoot3BulletOne()
{
	CCPoint point[2] = {ccp(0,-100*gScaleY),ccp(450*gScaleX,-100*gScaleY)};

	for(int i=0;i<2;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		CCMoveTo *moveTo = CCMoveTo::create(0.8,point[i]);
		bullet->runAction(CCRepeatForever::create(moveTo));
	}

}

void MonsterSprite::shoot4BulletOne()
{
	int id = m_monsterId;
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition());
	CCMoveBy *moveBy = CCMoveBy::create(0.4f, ccp(0, -400*gScaleY));
	bullet->runAction(CCRepeatForever::create(moveBy));
}

void MonsterSprite::shoot0BulletTwo()
{
	//GameLayer::sharedGameLayer()->createEnemy2ByBoss(this->getPosition());

	float offset = 50*gScaleY;

	CCPoint leftPoint[7] = {ccp(-200*gScaleX,900*gScaleY),ccp(-200*gScaleX,700*gScaleY),ccp(-200*gScaleX,500*gScaleY),ccp(-200*gScaleX,300*gScaleY),
		ccp(-100*gScaleX,100*gScaleY),
		ccp(0,-100*gScaleY),ccp(200*gScaleX,-100*gScaleY)};

	CCPoint rightPoint[7] = {ccp(600*gScaleX,900*gScaleY),ccp(600*gScaleX,700*gScaleY),ccp(600*gScaleX,500*gScaleY),ccp(600*gScaleX,300*gScaleY),
		ccp(300*gScaleX,-200*gScaleY),
		ccp(500,-100*gScaleY),ccp(650*gScaleX,100*gScaleY)};

	for(int i=0;i<7;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		if(m_shootOrientation)
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8, rightPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
		else
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8, leftPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
	}

	if(m_shootOrientation)
	{
		m_shootOrientation = false;
	}
	else
	{
		m_shootOrientation = true;
	}
}

void MonsterSprite::shoot1BulletTwo()
{

	int id = m_monsterId;
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition());
	CCMoveTo *moveTo = CCMoveTo::create(0.8,ccp(250,-100));
	bullet->runAction(CCRepeatForever::create(moveTo));

}

void MonsterSprite::shoot2BulletTwo()
{
	GameLayer::sharedGameLayer()->createEnemy1007ByBoss(this->getPosition());
}

void MonsterSprite::shoot3BulletTwo()
{
	CCPoint leftPoint[2] = {ccp(0,-100*gScaleY),ccp(200*gScaleX,-100*gScaleY)};
	CCPoint rightPoint[2] = {ccp(300,-100*gScaleY),ccp(500*gScaleX,-100*gScaleY)};

	for(int i=0;i<2;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		if(m_shootOrientation)
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8,rightPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
		else
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8,leftPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
	}

	if(m_shootOrientation)
	{
		m_shootOrientation = false;
	}
	else
	{
		m_shootOrientation = true;
	}
}

void MonsterSprite::shoot4BulletTwo()
{
	float offset = 100*gScaleY;

	CCPoint point[7] = {ccp(-200*gScaleX,this->getPositionY()),ccp(-300*gScaleX,this->getPositionY()-offset*4),ccp(-200*gScaleX,this->getPositionY()-offset*8),
		ccp(250*gScaleX,this->getPositionY()-offset*8),
		ccp(700*gScaleX,this->getPositionY()-offset*7),ccp(900*gScaleY,this->getPositionY()-offset*4),ccp(1100*gScaleX,this->getPositionY())
	};

	for(int i=0;i<7;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		CCMoveTo *moveTo = CCMoveTo::create(0.8,point[i]);
		bullet->runAction(CCRepeatForever::create(moveTo));
	}
}

void MonsterSprite::shoot0BulletThree()
{
	GameLayer::sharedGameLayer()->createEnemy1007ByBoss(this->getPosition());
}

void MonsterSprite::shoot1BulletThree()
{
	GameLayer::sharedGameLayer()->createEnemy2ByBoss(this->getPosition());
}

void MonsterSprite::shoot2BulletThree()
{
	GameLayer::sharedGameLayer()->createEnemy1007ByBoss(this->getPosition());
}

void MonsterSprite::shoot3BulletThree()
{
	CCPoint point[2] = {ccp(-50,-100*gScaleY),ccp(550*gScaleX,-100*gScaleY)};
	int id = m_monsterId;

	for(int i=0;i<2;i++)
	{
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());

		CCMoveTo *moveTo = CCMoveTo::create(0.8,point[i]);
		bullet->runAction(CCRepeatForever::create(moveTo));

	}

	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition());

	CCMoveTo *moveTo = CCMoveTo::create(1.2,ccp(250*gScaleX,-100*gScaleY));
	bullet->runAction(CCRepeatForever::create(moveTo));
}

void MonsterSprite::shoot4BulletThree()
{
	float offset = 50*gScaleY;

	CCPoint leftPoint[7] = {ccp(-200*gScaleX,900*gScaleY),ccp(-200*gScaleX,700*gScaleY),ccp(-200*gScaleX,500*gScaleY),ccp(-200*gScaleX,300*gScaleY),
		ccp(-100*gScaleX,100*gScaleY),
		ccp(0,-100*gScaleY),ccp(200*gScaleX,-100*gScaleY)};

	CCPoint rightPoint[7] = {ccp(600*gScaleX,900*gScaleY),ccp(600*gScaleX,700*gScaleY),ccp(600*gScaleX,500*gScaleY),ccp(600*gScaleX,300*gScaleY),
		ccp(300*gScaleX,-200*gScaleY),
		ccp(500,-100*gScaleY),ccp(650*gScaleX,100*gScaleY)};

	for(int i=0;i<7;i++)
	{
		int id = m_monsterId;
		MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
		BulletSprite *bullet = spManager->createMonsterBullet(MONSTERDIFF + id);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition());
		if(m_shootOrientation)
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8, rightPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
		else
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.8, leftPoint[i]);
			bullet->runAction(CCRepeatForever::create(moveTo));
		}
	}

	if(m_shootOrientation)
	{
		m_shootOrientation = false;
	}
	else
	{
		m_shootOrientation = true;
	}
}


bool MonsterSprite::isBoss()
{
	return m_isBoss;
}

void MonsterSprite::bossMove(float t)
{
	if(!m_moveStart)
	{
		return;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	double posY = 600;
	CCMoveTo *midToBe = CCMoveTo::create(1.0f, ccp(this->getContentSize().width * 0.6f / 2, posY));
	CCMoveTo *ToMid = CCMoveTo::create(1.0f, ccp(size.width / 2, posY));
	CCMoveTo *midToEnd = CCMoveTo::create(1.0f, ccp(size.width - this->getContentSize().width * 0.6f / 2, posY));
	CCSequence *sequence = CCSequence::create(midToBe, ToMid, midToEnd, ToMid, NULL);
	CCRepeatForever *repeat = CCRepeatForever::create(sequence);
	this->runAction(repeat); 
}

void MonsterSprite::scheduleShoot()
{
	schedule(schedule_selector(MonsterSprite::shootBullet), m_shootFrq);
}

int MonsterSprite::getActionId()
{
	return m_actionId;
}

void MonsterSprite::addMonsterTip()
{
	m_moveStart = false;
	GameLayer::sharedGameLayer()->addMTip(this->getMonsterId(), this->getPosition());
	this->runAction(CCSequence::create(CCDelayTime::create(1.5),CCCallFunc::create(this,callfunc_selector(MonsterSprite::tipEnd)),NULL));
}

void MonsterSprite::tipEnd()
{
	m_moveStart = true;
}

void MonsterSprite::slowDown()
{
	if(!m_slowDown)
	{
		//m_speed = m_speed*0.5;
		m_createMInf.mActionSpeed = m_createMInf.mActionSpeed*0.8;
		m_slowDown = true;
	}
}
