#include "HeroSprite.h"
#include "SpriteAniManager.h"
#include "BulletSprite.h"
#include "../GLayer/GameLayer.h"
#include "MySpriteManager.h"
#include "../GCData/GameConfigData.h"
#include "../GCData/DataInitial.h"

#define  MAGNETIC_EFFECT 1524
#define  DEFENCE_EFFECT 1534

// static HeroWeapon gHeroWeapon[3] = {
//  &HeroSprite::weaponId0,
//  &HeroSprite::weaponId1,
//  &HeroSprite::weaponId2,
// };
HeroSprite::HeroSprite():m_heroId(-1)
	,m_heroConfigData(NULL)
	,m_playerLevel(0)
	,m_levelDamage(0)
	,m_shootFrq(1)
	,m_magneticEnabled(false)
	,m_protectState(false)
	,m_invincible(false)
	,m_speedUpState(false)
	,m_pauseShoot(false)
	,m_doubleDamage(false)
{
}

HeroSprite::HeroSprite(int heroId): m_heroId(heroId)
	, m_heroConfigData(NULL)
	, m_playerLevel(0)
	, m_levelDamage(0)
	, m_shootFrq(5)
	, m_magneticEnabled(false)
	, m_protectState(false)
	, m_invincible(false)
	, m_speedUpState(false)
	, m_pauseShoot(false)
	,m_doubleDamage(false)
{
}

HeroSprite* HeroSprite::createHeroById(int heroId)
{
	HeroSprite *sprite = new HeroSprite(heroId);

	if(sprite && sprite->init())
	{
		sprite->autorelease();
		return sprite;
	}

	delete sprite;
	return NULL;
}

bool HeroSprite::init()
{
	m_heroConfigData = DataInitial::initialDataSource().getHDataById(m_heroId);
	m_boundRadiusInitState = true;
	m_boundRadius = m_heroConfigData->mBoundRadius;
	m_hp = m_heroConfigData->mCurHp;
	m_maxHp = m_heroConfigData->mMaxHp;

	m_magneticEnabled = m_heroConfigData->mMagneticEnable;

	m_playerLevel = GameConfigData::initialGameData().getCurHeroLevel();
	float shootTime = DataInitial::initialDataSource().getHeroShootFrq();
	schedule(schedule_selector(HeroSprite::shootBullet), shootTime);

	char filename[33];
	sprintf(filename, "hero%d_0.png", m_heroId);
	this->initWithSpriteFrameName(filename);

	char aniKeyName[64];
	sprintf(aniKeyName, "hero%d_%%d.png", m_heroId);
	CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName(aniKeyName);
	CCAnimate *ani = CCAnimate::create(animation);
	this->runAction(CCRepeatForever::create(ani));
	return true;
}


void HeroSprite::shootBullet(float t)
{
	if(m_pauseShoot)
	{
		return;
	}

	weaponId(m_heroId);
	ToolsFun::sharedTools()->playEffect(ATTACK1);
}



void HeroSprite::weaponId(int  t)
{
	MySpriteManager *spManager = GameLayer::sharedGameLayer()->getMySpriteManager();
	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();
	float damage = DataInitial::initialDataSource().getHeroDamage(heroLevel-1);  

	vector<WeaponData*> mWeaponDatas;

	switch(t)
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
	//for (int i=0;i!=bulletDatas.size();++i)
	{


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

		if(!m_doubleDamage)
		{
				HeroBulletSprite* bullet = spManager->createHeroBullet(bulletId);
			bullet->setDamge(damage);

			vector<WeaponLevelEffectData* > levelEffectData = DataInitial::initialDataSource().getheroEffectData(m_heroId);

			if(levelEffectData.at(m_playerLevel-1))
			{
				WeaponLevelEffectData *data = levelEffectData.at(m_playerLevel-1);

				if(data)
				{
					int size = data->m_effectData.size();

					for(int i=0;i<size;i++)
					{
						WeaponEffectData *effectData = data->m_effectData.at(i);
						if(effectData)
						{
							CCSprite *sprite = CCSprite::create(effectData->m_bulletName.c_str());
							sprite->setPosition(effectData->m_pos);
							bullet->addChild(sprite);
						}
					}
				}
			}

			GameLayer::sharedGameLayer()->addBullet(bullet, BULLET_ZORDER);

			bullet->setPosition(this->getPosition() + bulletDatas[0].posBuy+ccp(0,this->getContentSize().height/2));
			CCMoveBy *moveBy = CCMoveBy::create(0.3f, ccp(0, 400));
			bullet->runAction(CCRepeatForever::create(moveBy));
		}
		else
		{
				HeroBulletSprite* bullet1 = spManager->createHeroBullet(bulletId);
			//bullet1->setScale(scale);
			HeroBulletSprite* bullet2 = spManager->createHeroBullet(bulletId);
			//bullet2->setScale(scale);
			bullet1->setDamge(damage);
			bullet2->setDamge(damage);

			vector<WeaponLevelEffectData* > levelEffectData = DataInitial::initialDataSource().getheroEffectData(m_heroId);

			if(levelEffectData.at(m_playerLevel-1))
			{
				WeaponLevelEffectData *data = levelEffectData.at(m_playerLevel-1);

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
							bullet1->addChild(sprite1);

							CCSprite *sprite2 = CCSprite::create(effectData->m_bulletName.c_str());
							sprite2->setPosition(effectData->m_pos);
							bullet2->addChild(sprite2);
						}
					}
				}
			}
			GameLayer::sharedGameLayer()->addBullet(bullet1, BULLET_ZORDER);
			GameLayer::sharedGameLayer()->addBullet(bullet2, BULLET_ZORDER);
			bullet1->setPosition(this->getPosition() + bulletDatas[0].posBuy+ccp(-40,0)+ccp(0,this->getContentSize().height/2));
			bullet2->setPosition(this->getPosition() + bulletDatas[0].posBuy+ccp(40,0)+ccp(0,this->getContentSize().height/2));
			CCMoveBy *moveBy1 = CCMoveBy::create(0.3f, ccp(0, 400));
			bullet1->runAction(CCRepeatForever::create(moveBy1));
			CCMoveBy *moveBy2 = CCMoveBy::create(0.3f, ccp(0, 400));
			bullet2->runAction(CCRepeatForever::create(moveBy2));

		}
	}

}

void HeroSprite::setProtectedState(bool state)
{
	m_protectState = state;
}

bool HeroSprite::getProtectedState()
{
	return m_protectState;
}

void HeroSprite::setInvincible(bool state)
{
	m_invincible = state;
}

bool HeroSprite::getInvincible()
{
	return m_invincible;
}

void HeroSprite::runMagneticEffect()
{
	if(m_magneticEnabled)
	{
		return;
	}

	m_magneticEnabled = true;
	CCSprite *spMagEffect0 = CCSprite::create("hero/magEffect0.png");
	this->addChild(spMagEffect0, 1, MAGNETIC_EFFECT);
	spMagEffect0->setPosition(this->getContentSize() / 2);
	CCScaleTo *scaleTo0 = CCScaleTo::create(1.0f, 0.1f);
	CCScaleTo *scaleTo1 = CCScaleTo::create(1.0f, 1.0f);
	CCSequence *seq1 = CCSequence::create(scaleTo0, scaleTo1, NULL);
	CCRepeat *scaleToRepeat = CCRepeat::create(seq1, 5);
	spMagEffect0->runAction(scaleToRepeat);
	CCSprite *spManEffect1 = CCSprite::create("hero/magEffect1.png");
	this->addChild(spManEffect1, 0, MAGNETIC_EFFECT + 1);
	spManEffect1->setPosition(this->getContentSize() / 2);
	CCSequence *seq = CCSequence::create(CCRotateBy::create(5, 540), CCCallFuncN::create(this, callfuncN_selector(HeroSprite::magneticEffectEnd)), NULL);
	spManEffect1->runAction(seq);
}

void HeroSprite::magneticEffectEnd(CCNode *node)
{
	this->removeChildByTag(MAGNETIC_EFFECT);
	this->removeChildByTag(MAGNETIC_EFFECT + 1);
	m_magneticEnabled = false;
}

void HeroSprite::runSpeedUpEffect()
{
	ToolsFun::sharedTools()->playEffect(SPRINT);
	if (m_speedUpState)
	{
		return;
	}
	else
	{
		m_speedUpState = true;
		GameLayer::sharedGameLayer()->mapSpeedUp();
		CCSprite *charge_eff = CCSprite::create("ui/gameLayerUI/charge_eff.png");
		this->addChild(charge_eff);

		CCSprite *wave_cirlcle = CCSprite::create("ui/gameLayerUI/wave_cirlcle.png");
		this->addChild(wave_cirlcle);
		wave_cirlcle->setPosition(this->getContentSize() / 2 + ccp(0, 70));
		CCFadeIn *act1 = CCFadeIn::create(0.1);
		CCMoveBy *act2 = CCMoveBy::create(0.2, ccp(0, -150));
		CCFadeOut *act3 = CCFadeOut::create(0.1);
		CCMoveBy *act4 = CCMoveBy::create(0.1, ccp(0, 150));
		CCArray *arr = CCArray::create();
		arr->addObject(act1);
		arr->addObject(act2);
		arr->addObject(act3);
		arr->addObject(act4);
		CCSequence *act = CCSequence::create(arr);
		wave_cirlcle->runAction(CCRepeatForever::create(act));
		wave_cirlcle->setTag(3538);

		charge_eff->setTag(3527);
		charge_eff->setPosition(this->getContentSize() / 2 + ccp(0, 100));
		CCMoveTo *charge_act1 = CCMoveTo::create(0.2, charge_eff->getPosition() + ccp(0, 10));
		CCMoveTo *charge_act2 = CCMoveTo::create(0.2, charge_eff->getPosition() + ccp(0, -10));
		CCArray *arr_charge = CCArray::create();
		arr_charge->addObject(charge_act1);
		arr_charge->addObject(charge_act2);
		CCSequence *charge_act = CCSequence::create(arr_charge);
		charge_eff->runAction(CCRepeatForever::create(charge_act));

		scheduleOnce(schedule_selector(HeroSprite::speedUpEffectEnd), 5.0f);
	}
}

void HeroSprite::speedUpEffectEnd(float t)
{
	this->removeChildByTag(3538);
	this->removeChildByTag(3527);
	GameLayer::sharedGameLayer()->mapSpeedUpEnd();
	m_speedUpState = false;
}

void HeroSprite::addProtectEffect()
{
if(!m_protectState)
	{
	m_protectState = true;
	CCSprite *sp = CCSprite::createWithSpriteFrameName("mHeroEffect3_0.png");
	this->addChild(sp, 1, DEFENCE_EFFECT);
	sp->setPosition(this->getContentSize() / 2+ccp(0,15*gScaleY));
	//CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniByFiles("hero/defenceEffect%d.png", 2, 0.12f);
	CCAnimation *animation  =SpriteAniManager::getAniManager()->getSpriteAnimationByName("mHeroEffect3_%d.png");
	CCAnimate *ani  = CCAnimate::create(animation);
	sp->runAction(CCRepeatForever::create(ani));
}

	//	scheduleOnce(schedule_selector(HeroSprite::proEffectEnd), 5.0f);
}

void HeroSprite::proEffectEnd(float t)
{
	m_protectState = false;

	if(this->getChildByTag(DEFENCE_EFFECT))
	{
		this->removeChildByTag(DEFENCE_EFFECT);
	}
}

void HeroSprite::addAttackEffect()
{
	m_doubleDamage = true;
	this->runAction(CCSequence::create(CCDelayTime::create(3.0f),CCCallFuncN::create(this,callfuncN_selector(HeroSprite::endAttackEffect)),NULL));
}

void HeroSprite::endAttackEffect(CCNode *node)
{
	m_doubleDamage = false;
}

void HeroSprite::subHp(float damage)
{
	if(m_protectState || m_invincible || m_speedUpState)
	{
		return;
	}

	MySprite::subHp(damage);
}

bool HeroSprite::getSpeedUpState()
{
	return m_speedUpState;
}

void HeroSprite::setSpeedUpState(bool state)
{
	m_speedUpState = state;
}

bool HeroSprite::getMgneticState()
{
	return m_magneticEnabled;
}

void HeroSprite::setMagneticState(bool state)
{
	m_magneticEnabled = state;
}

void HeroSprite::pauseShoot()
{
	pauseSchedulerAndActions();
	m_pauseShoot = true;
}

void HeroSprite::resumeShoot()
{
	resumeSchedulerAndActions();
	m_pauseShoot = false;
}

//ÎÞµÐ×´Ì¬
void HeroSprite::addInvincibleEffect()
{
	ToolsFun::sharedTools()->playEffect(UNBEATABLE);

	CCSprite *sp1 = CCSprite::createWithSpriteFrameName("mHeroEffect0_0.png");
	this->addChild(sp1, 0, 1738);
	sp1->setPosition(this->getContentSize() / 2);

	CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName("mHeroEffect0_%d.png");
	CCAnimate *animate = CCAnimate::create(animation);

	sp1->runAction(CCSequence::create(animate,CCCallFuncN::create(this,callfuncN_selector(HeroSprite::end0Invincible)),NULL));

	m_invincible = true;

}

void HeroSprite::end0Invincible(CCNode *node)
{
	CCSprite *sp1 =(CCSprite *) this->getChildByTag(1738);
	CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName("mHeroEffect4_%d.png");
	CCAnimate *animate = CCAnimate::create(animation);
	sp1->runAction(CCSequence::create(animate,CCCallFuncN::create(this,callfuncN_selector(HeroSprite::end2Invincible)),NULL));


	CCSprite *sp2 = CCSprite::create("ui/gameLayerUI/god_stat_ring.png");
	this->addChild(sp2, 0, 1739);
	sp2->setPosition(this->getContentSize() / 2);
	CCArray * arr2 = CCArray::create();
	arr2->addObject(CCScaleTo::create(0.5, 1.2));
	arr2->addObject(CCScaleTo::create(0.01, 0.0));
	arr2->addObject(CCScaleTo::create(0.5, 0.5));
	CCSequence *seq_2 = CCSequence::create(arr2);
	sp2->runAction(CCRepeatForever::create(seq_2));

	CCSprite *sp3 = CCSprite::create("ui/gameLayerUI/shine.png");
	this->addChild(sp3, -10, 1741);
	sp3->setPosition(this->getContentSize() / 2);
	CCSequence *seq_3 = CCSequence::create(CCRotateBy::create(4.0, 700), CCCallFuncN::create(this, NULL), NULL);
	sp3->runAction(seq_3);

	sp2->runAction(CCSequence::create(CCDelayTime::create(4.0),CCCallFuncN::create(this,callfuncN_selector(HeroSprite::end1Invincible)),NULL));
}

void HeroSprite::end2Invincible(CCNode *node)
{
	this->removeChildByTag(1738);
}

void HeroSprite::end1Invincible(CCNode *node)
{
	this->removeChildByTag(1739);
	this->removeChildByTag(1741);

	CCSprite *sp1 = CCSprite::createWithSpriteFrameName("mHeroEffect1_0.png");
	this->addChild(sp1, 0, 1740);
	sp1->setPosition(this->getContentSize() / 2);

	CCAnimation *animation = SpriteAniManager::getAniManager()->getSpriteAnimationByName("mHeroEffect2_%d.png");
	CCAnimate *animate = CCAnimate::create(animation);

	sp1->runAction(CCSequence::create(animate,CCCallFuncN::create(this,callfuncN_selector(HeroSprite::endInvincible)),NULL));

}

void HeroSprite::endInvincible(CCNode* node)
{
	//this->removeChildByTag(1738);
	//this->removeChildByTag(1739);
	this->removeChildByTag(1740);

	m_invincible = false;
	GameLayer::sharedGameLayer()->setInvincibleItem(true);
}