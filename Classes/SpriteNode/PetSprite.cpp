#include "PetSprite.h"
#include "SpriteAniManager.h"
#include "../GCData/DataInitial.h"
#include "BulletSprite.h"
#include "../GLayer/GameLayer.h"
#include "MySpriteManager.h"
#include "MonsterSprite.h"
#include "ItemSprite.h"
#include "../ToolFun/ToolsFun.h"
#include "../GLayer/GameLayer.h"
#include "../GCData/GameConfigData.h"
#include "../GCData/DataInitial.h"


#define PETDIFF 200
#define PETMAXDIFF 1200
PetWeapon gPetWeapon[6] =
{
	&PetSprite::pet0Attack,
	&PetSprite::pet1Attack,
	&PetSprite::pet2Attack,
	&PetSprite::pet3Attack,
	&PetSprite::pet4Attack,
	&PetSprite::pet5Attack,
};

PetWeapon gPetFullWeapon[6] =
{
	&PetSprite::pet0FullAttack,
	&PetSprite::pet1FullAttack,
	&PetSprite::pet2FullAttack,
	&PetSprite::pet3FullAttack,
	&PetSprite::pet4FullAttack,
	&PetSprite::pet5FullAttack,
};

PetSprite::PetSprite(): m_petId(0), m_petData(NULL), m_petPos(0),m_petLevel(0)
{
}

PetSprite::PetSprite(int petId): m_petId(petId), m_petData(NULL), m_petPos(0),m_petLevel(0)
{
}

PetSprite::PetSprite(bool boundRadius )
{
	m_boundRadiusInitState = false;
}

PetSprite::~PetSprite()
{
}

bool PetSprite::init()
{
	m_petData = DataInitial::initialDataSource().getPetDataById(m_petId);
	//schedule(schedule_selector(PetSprite::petAttack,1/m_petData->mAttackFre));
	int level = GameConfigData::initialGameData().getpetLevelById(m_petId);

	m_petLevel = level;

	float shootTime = DataInitial::initialDataSource().getPetShootFrq();

	if(level < PET_MAXLEVEL)
	{
		schedule(schedule_selector(PetSprite::petAttack), shootTime);
	}
	else
	{
		schedule(schedule_selector(PetSprite::petFullAttack), shootTime);
	}

	char filename[33];
	sprintf(filename, "petAni/petAni%d.png", m_petId);
	this->initWithFile(filename);

	float  posx = this->getContentSize().width;
	float  posy = this->getContentSize().height;

	if(m_petId == 0 || m_petId == 1)
	{
		CCSprite *sp1 = CCSprite::create();
		this->addChild(sp1);
		sp1->setPosition(this->getContentSize() / 3 - ccp(0, 40));
		CCSprite *sp2 = CCSprite::create();
		this->addChild(sp2);
		sp2->setPosition(ccp(posx * 2 / 3, posy / 3) - ccp(0, 40));
		char aniName[40];
		sprintf(aniName, "petAni/petAni%d_0_%%d.png", 0);

		CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName, 2, 0.15f);
		CCAnimate *ani = CCAnimate::create(animation);
		CCAnimate *ani_1 = CCAnimate::create(animation);
		sp1->runAction(CCRepeatForever::create(ani));
		sp2->runAction(CCRepeatForever::create(ani_1));
	}

	if(m_petId == 2 || m_petId == 3)
	{
		CCPoint rightPetPos[6] = {ccp(0,0),ccp(0,0),ccp(0,20),
			ccp(0,20),ccp(0,0),ccp(0,0)};

		CCAction * move = CCMoveTo::create(0.5, ccp(this->getPositionX(), this->getPositionY() + 90+ rightPetPos[m_petId].y));
		CCAction * move1 = CCMoveTo::create(0.5, ccp(this->getPositionX(), this->getPositionY() + 70+ rightPetPos[m_petId].y));
		CCArray * arr = CCArray::create();
		arr->addObject(move);
		arr->addObject(move1);
		CCSequence * sep = CCSequence::create(arr);
		this->runAction(CCRepeatForever::create(sep));
	}
	if(m_petId == 4 || m_petId == 5)
	{
		CCSprite *sp1 = CCSprite::create();
		this->addChild(sp1);
		sp1->setPosition(this->getContentSize() / 2 - ccp(180, 0));
		CCSprite *sp2 = CCSprite::create();
		this->addChild(sp2);
		sp2->setPosition(this->getContentSize() / 2 + ccp(180, 0));
		char aniName[40];

		sprintf(aniName,"petAni/petAni%d_0_%%d.png",m_petId);
		CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles(aniName,2,0.12f);

		CCAnimate *ani = CCAnimate::create(animation);
		CCAnimate *ani_1 = CCAnimate::create(animation);
		sp1->runAction(CCRepeatForever::create(ani));
		sp2->runAction(CCRepeatForever::create(ani_1));
		sp1->setFlipX(true);
	}

	this->setScale(0.2f);
	return true;
}


PetSprite * PetSprite::createPetById(int petId)
{
	PetSprite *petSp = new PetSprite(petId);

	if(petSp->init())
	{
		petSp->autorelease();
		return petSp;
	}

	delete petSp;
	return NULL;
}

void  PetSprite::petAttack(float t)
{
	(this->*gPetWeapon[m_petId])(t);
}

void  PetSprite::petFullAttack(float t)
{
	(this->*gPetFullWeapon[m_petId])(t);
}
/*
对附近的某个敌人进行放电攻击
*/
void  PetSprite::pet0Attack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[1] = {ccp(-40, 20)};
	MonsterSprite *monster = spManager->getBeAttackMonster(this->getPosition(), 2 * this->m_petData->mAttackLenglth);

	if(monster)
	{
/*		monster->setDeadState(true);
		monster->setDeadType(2);*/
		CCPoint mPos = monster->getPosition();
		CCPoint subPos = mPos - this->getPosition();
		float angel = subPos.getAngle();
		float angel360 = ToolsFun::sharedTools()->piToAngle360(angel);
		BulletSprite *sp = BulletSprite::createBulletByIdWithPet(PETDIFF + m_petId, m_petId);
		
		resetDamage(sp);

monster->subHp(sp->getDamage());

		if(monster->getHp()<0)
		{
			monster->setDeadState(true);
			monster->setDeadType(2);
		}
		GameLayer::sharedGameLayer()->addChild(sp, BULLET_ZORDER);
		sp->setPosition(this->getPosition());
		sp->setRotation(90 - angel360);
		sp->setAnchorPoint(CCPointZero);
		float scY = subPos.getLength() / sp->getContentSize().height;
		CCScaleTo *sc = CCScaleTo::create(0.1f, 1.0f, scY);
		CCSequence *seq = CCSequence::create(sc
		                                     , CCDelayTime::create(0.4f)
		                                     , CCCallFuncN::create(this, callfuncN_selector(PetSprite::removeSprite))
		                                     , NULL);
		sp->runAction(seq);
	}
}
/*
按照某个频率发射会减慢速度的冰弹
*/

void  PetSprite::pet1Attack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[1] = {ccp(0, 30)};
	BulletSprite *bullet = spManager->createPetBulletWithPet(PETDIFF + m_petId, m_petId);
	//bullet->setScale(0.5f);

	resetDamage(bullet);

	char buf[30];
	sprintf(buf, "pet%dbullet", m_petId);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition() + posSub[0]);
	CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
	bullet->runAction(CCRepeatForever::create(moveBy));
}


/*
按照某个频率发射能穿透怪物的子弹
*/
void  PetSprite::pet2Attack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();

	CCPoint posSub[1] = {ccp(0, 30)};

	BulletSprite *bullet = spManager->createPetBulletWithPet(PETDIFF + m_petId, m_petId);

	resetDamage(bullet);

	bullet->setIsPenetrability(true);
	//bullet->setScale(0.5f);
	char buf[30];
	sprintf(buf, "pet%dbullet", m_petId);
	////LD(buf);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition() + posSub[0]);
	CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
	bullet->runAction(CCRepeatForever::create(moveBy));
}

//按照某个频率发射范围攻击的炸弹
void  PetSprite::pet3Attack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[1] = {ccp(0, 30)};
	list<MonsterSprite *> *monsterlist = spManager->getBeAttackMonsterList(this->getPosition(), 2 * this->m_petData->mAttackLenglth);

	for(list<MonsterSprite *>::iterator iter = monsterlist->begin(); iter != monsterlist->end(); ++iter)
	{
/*		(*iter)->setDeadState(true);
		(*iter)->setDeadType(2);*/
		CCPoint mPos = (*iter)->getPosition();
		CCPoint subPos = mPos - this->getPosition();
		float angel = subPos.getAngle();
		float angel360 = ToolsFun::sharedTools()->piToAngle360(angel);
		BulletSprite *sp = BulletSprite::createBulletByIdWithPet(PETDIFF + m_petId, m_petId);
		resetDamage(sp);
(*iter)->subHp(sp->getDamage());

		if((*iter)->getHp()<0)
		{
			(*iter)->setDeadState(true);
			(*iter)->setDeadType(2);
		}

		//sp->setScale(0.2f);
		GameLayer::sharedGameLayer()->addChild(sp, BULLET_ZORDER);
		sp->setPosition(this->getPosition());

		CCMoveTo *moveBy = CCMoveTo::create(0.3f, mPos);
		CCSequence *seq = CCSequence::create(moveBy
		                                     , CCCallFuncN::create(this, callfuncN_selector(PetSprite::removeSprite))
		                                     , NULL);
		sp->runAction(seq);
	}
}

//按照某个频率发射子弹，每发射一颗增加一个金币
void  PetSprite::pet4Attack(float t)
{
	//加金币
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();

	CCPoint posSub[1] = {ccp(0, 30)};
	BulletSprite *bullet = spManager->createPetBulletWithPet(PETDIFF + m_petId, m_petId);
	//bullet->setScale(0.5f);

	resetDamage(bullet);

	ItemSprite * itemSprite = spManager->createItemById(4);
	GameLayer::sharedGameLayer()->addChild(itemSprite, ITEM_ZORDER);
	itemSprite->setPosition(this->getPosition() + ccp(0, 500));
	itemSprite->itemAction();
	char buf[30];
	sprintf(buf, "pet%dbullet", m_petId);
	//LD(buf);
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);

	bullet->setPosition(this->getPosition() + posSub[0]);
	CCMoveBy *moveBy = CCMoveBy::create(0.5f, ccp(0, 500));
	bullet->runAction(CCRepeatForever::create(moveBy));
}
//对附近的敌人进行范围冲击波打击
void  PetSprite::pet5Attack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();

	CCPoint posSub[1] = {ccp(-40, 20)};
	MonsterSprite *monster = spManager->getBeAttackMonster(this->getPosition(), 2 * this->m_petData->mAttackLenglth);

	if(monster)
	{
/*		monster->setDeadState(true);
		monster->setDeadType(2);*/
		CCPoint mPos = monster->getPosition();
		CCPoint subPos = mPos - this->getPosition();
		float angel = subPos.getAngle();
		float angel360 = ToolsFun::sharedTools()->piToAngle360(angel);
		BulletSprite *sp = BulletSprite::createBulletByIdWithPet(PETDIFF + m_petId, m_petId);

		resetDamage(sp);
monster->subHp(sp->getDamage());

		if(monster->getHp()<0)
		{
			monster->setDeadState(true);
			monster->setDeadType(2);
		}

		//sp->setScale(0.5f);
		char buf[30];
		sprintf(buf, "pet%dbullet", m_petId);
		//LD(buf);
		GameLayer::sharedGameLayer()->addChild(sp, BULLET_ZORDER);
		sp->setPosition(this->getPosition());
		sp->setRotation(90 - angel360);
		sp->setAnchorPoint(CCPointZero);
		float scY = subPos.getLength() / sp->getContentSize().height;
		CCScaleTo *sc = CCScaleTo::create(0.1f, 1.0f, scY);
		CCSequence *seq = CCSequence::create(sc
		                                     , CCDelayTime::create(0.2f)
		                                     , CCCallFuncN::create(this, callfuncN_selector(PetSprite::removeSprite))
		                                     , NULL);
		sp->runAction(seq);
	}
}
/*
发射多道闪电
*/
void  PetSprite::pet0FullAttack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();

	CCPoint posSub[1] = {ccp(0, 30)};
	list<MonsterSprite *> *monsterlist = spManager->getBeAttackMonsterList(this->getPosition(), 2 * this->m_petData->mAttackLenglth);

	for(list<MonsterSprite *>::iterator iter = monsterlist->begin(); iter != monsterlist->end(); ++iter)
	{
/*		(*iter)->setDeadState(true);
		(*iter)->setDeadType(2);*/
		CCPoint mPos = (*iter)->getPosition();
		CCPoint subPos = mPos - this->getPosition();
		float angel = subPos.getAngle();
		float angel360 = ToolsFun::sharedTools()->piToAngle360(angel);
		BulletSprite *sp = BulletSprite::createBulletByIdWithPet(PETMAXDIFF + m_petId, m_petId);

		resetDamage(sp);

(*iter)->subHp(sp->getDamage());

		if((*iter)->getHp()<0)
		{
			(*iter)->setDeadState(true);
			(*iter)->setDeadType(2);
		}
		GameLayer::sharedGameLayer()->addChild(sp, BULLET_ZORDER);
		sp->setPosition(this->getPosition() + posSub[0]);
		sp->setRotation(90 - angel360);
		sp->setAnchorPoint(CCPointZero);
		float scY = subPos.getLength() / sp->getContentSize().height;
		CCScaleTo *sc = CCScaleTo::create(0.1f, 1.0f, scY);
		CCSequence *seq = CCSequence::create(sc
		                                     , CCDelayTime::create(0.2f)
		                                     , CCCallFuncN::create(this, callfuncN_selector(PetSprite::removeSprite))
		                                     , NULL);
		sp->runAction(seq);
	}
}

/*
发射多个冰弹
*/
void  PetSprite::pet1FullAttack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[3] = {ccp(-60, 20), ccp(60, 20)};

	for(int i = 0; i != 2; i++)
	{
		BulletSprite *bullet = spManager->createPetBulletWithPet(PETMAXDIFF + m_petId, m_petId);

		resetDamage(bullet);

		bullet->setScale(0.5f);
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition() + posSub[i]);
		CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
		bullet->runAction(CCRepeatForever::create(moveBy));
	}

	scheduleOnce(schedule_selector(PetSprite::pet1ShootBullet),0.1f);
}

void PetSprite::pet1ShootBullet(float dt)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	BulletSprite *bullet = spManager->createPetBulletWithPet(PETMAXDIFF + m_petId, m_petId);
	resetDamage(bullet);
	bullet->setScale(0.5f);

	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition() + ccp(0, 20*gScaleY));
	CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
	bullet->runAction(CCRepeatForever::create(moveBy));
}

/*
发射多个射穿弹
*/
void  PetSprite::pet2FullAttack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[2] = {ccp(-40, 20), ccp(40, 20)};

	for(int i = 0; i != 2; i++)
	{
		BulletSprite *bullet = spManager->createPetBulletWithPet(PETMAXDIFF + m_petId, m_petId);

		resetDamage(bullet);

		bullet->setIsPenetrability(true);
		//  bullet->setScale(0.5f);
		CCLog("bullet1");
		GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
		bullet->setPosition(this->getPosition() + posSub[i]);
		CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
		bullet->runAction(CCRepeatForever::create(moveBy));
	}
}
//击毁障碍物

void  PetSprite::pet3FullAttack(float t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[3] = {ccp(-40, 20), ccp(0, 20), ccp(40, 20)};
	//for(int i=0;i!=2;i++){
	BulletSprite *bullet = spManager->createPetBulletWithPet(PETMAXDIFF + m_petId, m_petId);

	resetDamage(bullet);

	//bullet->setScale(0.5f);
	//CCLog("bullet1");
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition() + posSub[1]);
	CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
	bullet->runAction(CCRepeatForever::create(moveBy));
	//}
}

//捡到金币翻倍
void  PetSprite::pet4FullAttack(float t)
{
	//加金币
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	CCPoint posSub[1] = {ccp(0, 20)};
	BulletSprite *bullet = spManager->createPetBulletWithPet(PETMAXDIFF + m_petId, m_petId);

	resetDamage(bullet);

//	CCLog("bullet4");
	GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);
	bullet->setPosition(this->getPosition() + posSub[0]);
	CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
	bullet->runAction(CCRepeatForever::create(moveBy));
}

//每一局之前为角色提供一个保护罩
void  PetSprite::pet5FullAttack(float t)
{
	pet5Attack(t);
	//GameLayer::sharedGameLayer()->getHeroSprite()->addProtectEffect();//**保护罩**//
}

void PetSprite::removeSprite(CCNode *node)
{
	node->removeFromParent();
}

void PetSprite::pauseShoot()
{
	pauseSchedulerAndActions();
	m_pauseShoot = true;
}

void PetSprite::resumeShoot()
{
	resumeSchedulerAndActions();
	m_pauseShoot = false;
}

int PetSprite::getPetId()
{
	return m_petId;
}

int PetSprite::getPetLevel()
{
	return m_petLevel;
}

//根据宠物和重复等级计算伤害值
void PetSprite::resetDamage(BulletSprite *sprite)
{
	int curLevel = GameConfigData::initialGameData().getpetLevelById(m_petId)-1;
	UpgratePetData *data = DataInitial::initialDataSource().getSubPetUpgrateData(m_petId);

	if(curLevel<0)
	{
		curLevel =1;
	}

	if(data!=NULL)
	{
		sprite->setDamge(data->m_damage.at(curLevel));
	}
}