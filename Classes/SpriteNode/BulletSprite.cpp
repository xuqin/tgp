#include "BulletSprite.h"
#include "../GCData/GameConfigData.h"


BulletSprite::BulletSprite(int bulletId): m_bulletId(bulletId)
	, m_bulletDamage(0)
	, m_isPenetrability(false)
	, m_petId(-1)
	, isMax(false)
{
}

BulletSprite::BulletSprite(int bulletId, int petId): m_bulletId(bulletId)
	, m_bulletDamage(0)
	, m_isPenetrability(false)
	, m_petId(petId)
	, isMax(false)
{
}
BulletSprite::~BulletSprite()
{

}

BulletSprite * BulletSprite::createBulletById(int bulletId)
{

	BulletSprite *bulletSp = new BulletSprite(bulletId);

	if(bulletSp && bulletSp->init())
	{
		bulletSp->autorelease();
		return bulletSp;
	}

	delete bulletSp;
	return NULL;
}

BulletSprite * BulletSprite::createSubBulletById(int bulletId, int sub)
{
	BulletSprite *bulletSp = new BulletSprite(bulletId);

	if(bulletSp && bulletSp->init())
	{
		bulletSp->autorelease();
		BulletData *subBulletData = DataInitial::initialDataSource().getBulletDataByPIdandSub(bulletId, sub);
		int subBulletId = subBulletData->mBulletId;
		BulletSprite *subBulletLeft = new BulletSprite(subBulletId);
		BulletSprite *subBulletRight = new BulletSprite(subBulletId);

		if(subBulletLeft && subBulletLeft->init() && subBulletRight && subBulletRight->init())
		{

			subBulletLeft->autorelease();
			subBulletRight->autorelease();
			bulletSp->addChild(subBulletLeft,0,100);
			subBulletLeft->setPosition(ccp(0, bulletSp->getPositionY() / 2));

			bulletSp->addChild(subBulletRight,0,101);
			subBulletRight->setPosition(ccp(bulletSp->getContentSize().width, bulletSp->getPositionY() / 2));
			return bulletSp;
		}

		delete subBulletLeft;
		delete subBulletRight;
	}

	delete bulletSp;
	return NULL;
}


BulletSprite * BulletSprite::createBulletByIdWithPet(int bulletId, int petId)
{
	BulletSprite *bulletSp = new BulletSprite(bulletId, petId);

	if(bulletSp && bulletSp->init())
	{
		bulletSp->autorelease();
		return bulletSp;
	}

	delete bulletSp;
	return NULL;
}
bool BulletSprite::init()
{
	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();
	m_bulletData = DataInitial::initialDataSource().getBulletDataById(m_bulletId);
	m_boundRadius = m_bulletData->mBoundRadius;
	m_boundRadiusInitState = true;
	m_bulletDamage = m_bulletData->mBulletDamage;
	//m_bulletDamage = DataInitial::initialDataSource().getHeroDamage(heroLevel);
	m_isPenetrability = m_bulletData->mIsPenetrability;
	char filename[33];

	sprintf(filename, "bullet/bullet%d.png", m_bulletId);

	this->initWithFile(filename);
	return true;
}



void BulletSprite::setDamge(float damage)
{
	m_bulletDamage = damage;
}

float BulletSprite::getDamage()
{
	return m_bulletDamage;
}

void BulletSprite::setIsPenetrability(bool enable)
{
	m_isPenetrability = enable;
}

bool BulletSprite::getIsPenetrability()
{
	return m_isPenetrability;
}

void BulletSprite::setPetId(int petId)
{
	m_petId = petId;
}
int BulletSprite::getPetId()
{
	return m_petId;
}