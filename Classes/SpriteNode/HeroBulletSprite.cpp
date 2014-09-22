#include "HeroBulletSprite.h"
#include "../GCData/GameConfigData.h"


HeroBulletSprite::HeroBulletSprite(int bulletId): m_bulletId(bulletId)
	, m_bulletDamage(0)
	, m_isPenetrability(false)
	, m_petId(-1)
	, isMax(false)
{
}

HeroBulletSprite::HeroBulletSprite(int bulletId, int petId): m_bulletId(bulletId)
	, m_bulletDamage(0)
	, m_isPenetrability(false)
	, m_petId(petId)
	, isMax(false)
{
}
HeroBulletSprite::~HeroBulletSprite()
{

}

HeroBulletSprite * HeroBulletSprite::createBulletById(int bulletId)
{

	HeroBulletSprite *bulletSp = new HeroBulletSprite(bulletId);

	if(bulletSp && bulletSp->init())
	{
		bulletSp->autorelease();
		return bulletSp;
	}

	delete bulletSp;
	return NULL;
}

bool HeroBulletSprite::init()
{
	int heroLevel = GameConfigData::initialGameData().getCurHeroLevel();
	m_bulletData = DataInitial::initialDataSource().getBulletDataById(m_bulletId);
	//m_boundRadius = m_bulletData->mBoundRadius;
	m_boundRadiusInitState = true;
	m_bulletDamage = m_bulletData->mBulletDamage;
	m_isPenetrability = m_bulletData->mIsPenetrability;
	char filename[33];

	int heroId = GameConfigData::initialGameData().getCurHeroId();
	int level = GameConfigData::initialGameData().getCurHeroLevel();

	this->initWithFile("bullet/bullet.png");

	int playLevel = GameConfigData::initialGameData().getCurHeroLevel();

	vector<WeaponLevelEffectData* > levelEffectData = DataInitial::initialDataSource().getheroEffectData(heroId);

	if(levelEffectData.at(playLevel-1))
	{
		WeaponLevelEffectData *data = levelEffectData.at(playLevel-1);

		if(data)
		{
			m_boundRadius = data->mBoundRadius;
		}
	} 

	return true;
}



void HeroBulletSprite::setDamge(float damage)
{
	m_bulletDamage = damage;
}

float HeroBulletSprite::getDamage()
{
	return m_bulletDamage;
}

void HeroBulletSprite::setIsPenetrability(bool enable)
{
	m_isPenetrability = enable;
}

bool HeroBulletSprite::getIsPenetrability()
{
	return m_isPenetrability;
}

void HeroBulletSprite::setPetId(int petId)
{
	m_petId = petId;
}
int HeroBulletSprite::getPetId()
{
	return m_petId;
}