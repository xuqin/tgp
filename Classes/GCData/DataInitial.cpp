#include "DataInitial.h"
#include "../ToolFun/ToolsFun.h"


DataInitial& DataInitial::initialDataSource()
{
	static DataInitial tempDatainitia;
	return tempDatainitia;
}

void DataInitial::initData()
{
	m_hero0EffectData.clear();
	m_hero1EffectData.clear();
	m_hero2EffectData.clear();
	/*怪物数据配置的初始化*/
	initMData();
	/*角色数据配置的初始化*/
	initHeroData();
	/*道具数据的初始化*/
	initItemData();
	/*子弹数据初始化*/
	initBulletData();
	/*关卡配置数据初始化*/
	initLevelUnLock();
	/*奖励的数据配置*/
	initRewardInf();
	/*宠物的数据配置*/
	initPetData();
	//初始化第一个英雄的子弹
	initHero0WeaponData();
	initHero1WeaponData();
	initHero2WeaponData();

	//初始化每一级子弹效果
	initHero0WeaponEffectData();
	initHero1WeaponEffectData();
	initHero2WeaponEffectData();
	initStarData();

	initHeroDamage();

	initPetUpgradeData();

	m_mapNodeSpeed = 200;
	m_mapNodeUpSpeed = 600;

	m_HeroShootFrq = 0.2f;
	m_PetShootFrq = 0.2f;
}

float DataInitial::getHeroShootFrq()
{
	return m_HeroShootFrq;
}

float DataInitial::getPetShootFrq()
{
	return m_PetShootFrq;
}

MonsterData * DataInitial::getMDataById(int mId)
{
	for(int i = 0; i != m_monsterDatas.size(); ++i)
	{
		if(m_monsterDatas[i]->mId == mId)
		{
			return m_monsterDatas[i];
		}
	}

	//ToolsFun::sharedTools()->writeErrLog("DataInitial::getMDataById");
	return NULL;
}

HeroData *DataInitial::getHDataById(int hId)
{
	for(int i = 0; i != m_heroDatas.size(); ++i)
	{
		if(m_heroDatas[i]->mHeroId == hId)
		{
			return m_heroDatas[i];
		}
	}

	//ToolsFun::sharedTools()->writeErrLog("DataInitial::getHDataById");
	return NULL;
}

HeroUpgradeData & DataInitial::getHeroUpgrateData(int heroId, int level)
{
	return m_heroDatas[heroId]->mUpgradeData[level];
}

ItemData *DataInitial::getItemDataById(int iId)
{
	for(int i = 0; i != m_itemDatas.size(); ++i)
	{
		if(m_itemDatas[i]->mItemId == iId)
		{
			return m_itemDatas[i];
		}
	}

	return NULL;
}

BulletData *DataInitial::getBulletDataById(int bId)
{
	for(int i = 0; i != m_BulletDatas.size(); ++i)
	{
		if(m_BulletDatas[i]->mBulletId == bId)
		{
			return m_BulletDatas[i];
		}
	}

	return NULL;
}


BulletData *DataInitial::getBulletDataByPIdandSub(int pId, int sub)
{
	for(int i = 0; i != m_BulletDatas.size(); ++i)
	{
		if(m_BulletDatas[i]->parentId == pId && m_BulletDatas[i]->mSub == sub)
		{
			return m_BulletDatas[i];
		}
	}

	//ToolsFun::sharedTools()->writeErrLog("DataInitial::getHDataById");
	return NULL;
}

PetData *DataInitial::getPetDataById(int pId)
{

	for(int i = 0; i != m_PetDatas.size(); ++i)
	{
		if(m_PetDatas[i]->mPetId == pId)
		{
			return m_PetDatas[i];
		}
	}

	return NULL;
}

PetUpgrateData& DataInitial::getPetUpgrateData(int pId, int level)
{
	return m_PetDatas[pId]->petUpgradeDatas[level];
}

LevelUnLock *DataInitial::getLeveUnLockInf(int levelId)
{
	for(int i = 0; i != m_unLockConfig.size(); ++i)
	{
		if(m_unLockConfig[i]->mLevelId == levelId)
		{
			return m_unLockConfig[i];
		}
	}

	return NULL;
}

LevelUnLock *DataInitial::getSubLevelUnLockInf(int levelId)
{
	for(int i = 0; i != m_subUnLockConfig.size(); ++i)
	{
		if(m_subUnLockConfig[i]->mLevelId == levelId)
		{
			return m_subUnLockConfig[i];
		}
	}

	return NULL;
}

RewardConfig *DataInitial::getRewardInf(int rewaredId)
{
	for(int i = 0; i != m_rewardConfig.size(); ++i)
	{
		if(m_rewardConfig[i]->mRewardId == rewaredId)
		{
			return m_rewardConfig[i];
		}
	}

	return NULL;
}

DataInitial::~DataInitial()
{
	//LD("DataInitial::~DataInitial()");
	for(int i = 0; i != m_monsterDatas.size(); ++i)
	{
		delete m_monsterDatas[i];
		m_monsterDatas[i] = NULL;
	}

	m_monsterDatas.clear();


	for(int i = 0; i != m_heroDatas.size(); ++i)
	{
		delete m_heroDatas[i];
		m_heroDatas[i] = NULL;
	}

	m_heroDatas.clear();


	for(int i = 0; i != m_itemDatas.size(); ++i)
	{
		delete m_itemDatas[i];
		m_itemDatas[i] = NULL;
	}

	m_itemDatas.clear();

	for(int i = 0; i != m_BulletDatas.size(); ++i)
	{
		delete m_BulletDatas[i];
		m_BulletDatas[i] = NULL;
	}

	m_BulletDatas.clear();

	for(int i = 0; i != m_PetDatas.size(); ++i)
	{
		delete m_PetDatas[i];
		m_PetDatas[i] = NULL;
	}

	m_PetDatas.clear();

	for(int i = 0; i < m_petUpData.size(); i++)
	{
		delete m_petUpData[i];
		m_petUpData[i] = NULL;
	}

	m_petUpData.clear();
}

StarConfig *DataInitial::getStarConfig(int leveId)
{
	if(leveId < 0 || leveId >= 30)
		return NULL;

	return m_starConfig.at(leveId);
}

void DataInitial::initStarData()
{
	m_starConfig.push_back(new StarConfig(100, 250));
	m_starConfig.push_back(new StarConfig(150, 300));
	m_starConfig.push_back(new StarConfig(180, 350));
	m_starConfig.push_back(new StarConfig(250, 500));
	m_starConfig.push_back(new StarConfig(500, 850));

	m_starConfig.push_back(new StarConfig(750, 1100));
	m_starConfig.push_back(new StarConfig(600, 900));
	m_starConfig.push_back(new StarConfig(600, 900));
	m_starConfig.push_back(new StarConfig(1000, 1450));
	m_starConfig.push_back(new StarConfig(900, 1200));

	m_starConfig.push_back(new StarConfig(950, 1400));
	m_starConfig.push_back(new StarConfig(1250, 1700));
	m_starConfig.push_back(new StarConfig(1000, 1500));
	m_starConfig.push_back(new StarConfig(1350, 1900));
	m_starConfig.push_back(new StarConfig(1050, 1550));

	m_starConfig.push_back(new StarConfig(800, 1100));
	m_starConfig.push_back(new StarConfig(1000, 1300));
	m_starConfig.push_back(new StarConfig(1100, 1450));
	m_starConfig.push_back(new StarConfig(1050, 1480));
	m_starConfig.push_back(new StarConfig(1200, 1550));

	m_starConfig.push_back(new StarConfig(750, 1000));
	m_starConfig.push_back(new StarConfig(1500, 2000));
	m_starConfig.push_back(new StarConfig(1000, 1450));
	m_starConfig.push_back(new StarConfig(1500, 2100));
	m_starConfig.push_back(new StarConfig(1200, 1850));

	m_starConfig.push_back(new StarConfig(800, 1200));
	m_starConfig.push_back(new StarConfig(1000, 1450));
	m_starConfig.push_back(new StarConfig(1250, 1700));
	m_starConfig.push_back(new StarConfig(1100, 1500));
	m_starConfig.push_back(new StarConfig(1700, 2100));
}

void DataInitial::initHeroDamage()
{
	float damage[30] =  {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
	                    };

	for(int i = 0; i < 30; i++)
	{
		m_HeroDamage.push_back(damage[i]);
	}

}

float DataInitial::getHeroDamage(int id)
{
	if(id >= 0 && id < 31)
	{
		return m_HeroDamage.at(id);
	}

	return 1.0;
}


void DataInitial::initPetUpgradeData()
{
	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(0);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}

	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(1);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}

	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(2);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}

	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(3);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}

	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(4);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}

	{
		float damage[30] = {5.0, 6.0, 7.0, 8.0, 9.0,
	                     10.0, 11.5, 13.0, 14.5, 16.0,
	                     17.5, 19.0, 20.5, 22.0, 23.5,
	                     25.0, 27.5, 30.0, 32.5, 35.0,
	                     37.5, 40.0, 44.0, 48.0, 52.0,
	                     56.0, 60.0, 65.0, 70.0, 75.0,
		                   } ;

		int price[30] = {100, 300, 500, 700, 900,
		                 1100, 1500, 1900, 2300, 2700,
		                 3100, 3500, 4000, 4500, 5000,
		                 6000, 7000, 8000, 10000, 12000,
		                 14000, 16000, 18000, 20000, 25000,
		                 30000, 35000, 40000, 45000, 50000,
		                };

		UpgratePetData *data = new UpgratePetData(5);

		for(int i = 0; i < 30; i++)
		{
			data->m_damage.push_back(damage[i]);
			data->m_price.push_back(price[i]);
		}

		m_petUpData.push_back(data);
	}
}

UpgratePetData* DataInitial::getSubPetUpgrateData(int pet)
{
	if(pet >= 0 && pet < 6)
	{
		return m_petUpData.at(pet);
	}
	else
	{
		return NULL;
	}
}

void DataInitial::initMData()
{
	/*怪物的配置数据初始化*/
	/*依次：角色id
	，血量hp
	，分数score
	，速度Speed
	，射击否sE,射击类型函数id
	，子弹id
	，是否是BOSS mIsBoss;
	，射击频率mShootFrq;
	，碰撞检测范围数组
	，低血量是做俯冲动作
	，预留值1
	，默认为0*/
	//MonsterData(int id,int hp,int score,float speed,bool shootE,int shootFId,int bulletId,bool isBoss,float shootFrq,vector<BoundRadius> vBoundR,int default0=0,int default1=0):
	float radius = 45;
	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(ccp(0, 33), 25));
	bound0.push_back(BoundRadius(ccp(0, -18), 25));
	m_monsterDatas.push_back(new MonsterData(0, 5, 5, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0, 33), 25));
	bound0.push_back(BoundRadius(ccp(0, -18), 25));
	m_monsterDatas.push_back(new MonsterData(1, 7, 7, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 30));
	m_monsterDatas.push_back(new MonsterData(2, 5, 5, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 30));
	m_monsterDatas.push_back(new MonsterData(3, 6, 6, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(5, 33), 25));
	bound0.push_back(BoundRadius(ccp(5, -18), 30));
	m_monsterDatas.push_back(new MonsterData(4, 10, 4, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 25));
	m_monsterDatas.push_back(new MonsterData(5, 5, 5, 50, false, -1, -1, false, 1, bound0));


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(-5, 33), 25));
	bound0.push_back(BoundRadius(ccp(0, -18), 25));
	m_monsterDatas.push_back(new MonsterData(6, 8, 8, 50, false, -1, -1, false, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_monsterDatas.push_back(new MonsterData(7, 8, 8, 50, false, -1, -1, false, 1, bound0));

	//boss怪物ID从8~12
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 130));
	m_monsterDatas.push_back(new MonsterData(8, 500, 500, 50, false, -1, -1, true, 1, bound0, 10));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 130));
	m_monsterDatas.push_back(new MonsterData(9, 300, 300, 50, false, -1, -1, true, 1, bound0, 10));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 130));
	m_monsterDatas.push_back(new MonsterData(10, 400, 400, 50, false, -1, -1, true, 1, bound0, 10));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 130));
	m_monsterDatas.push_back(new MonsterData(11, 250, 250, 50, false, -1, -1, true, 1, bound0, 10));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 130));
	m_monsterDatas.push_back(new MonsterData(12, 150, 150, 50, false, -1, -1, true, 1, bound0, 10));

	//**不可被子弹打死的怪物，ID从1000编号****//
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 60));
	m_monsterDatas.push_back(new MonsterData(1000, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 40));
	m_monsterDatas.push_back(new MonsterData(1001, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1002, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1003, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1004, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1005, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1006, 10, 1, 50, false, -1, -1, false, 1, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 45));
	m_monsterDatas.push_back(new MonsterData(1007, 10, 1, 50, false, -1, -1, false, 1, bound0));
}

void DataInitial::initHeroData()
{
	/*角色属性配置
	角色id
	，最大血量maxHp
	，当前血量curhp
	，攻击力damage
	，具有吸引力magneticEnable
	，碰撞检测区域数组
	，预留值d0(默认为0)
	，预留值d1(默认为0)
	*/
	/*HeroData(int heroId,int maxHp,int curHp,float damage,bool magneticEnable,vector<BoundRadius>  boundRadius,int d0=0,int d1=0);*/
	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(ccp(-5, 30), 30));
	bound0.push_back(BoundRadius(ccp(-5, -30), 25));
	HeroData *data = new HeroData(0, 1, 1, 1, false, bound0);

	int price[30] = {100, 300, 500, 700, 900,
		1100, 1500, 1900, 2300, 2700,
		3100, 3500, 4000, 4500, 5000,
		6000, 7000, 8000, 10000, 12000,
		14000, 16000, 18000, 20000, 25000,
		30000, 35000, 40000, 45000, 50000,
	};

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		data->pushUpData(HeroUpgradeData(0, i, price[i], 1, 1));
	}

	m_heroDatas.push_back(data);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(-5, 30), 30));
	bound0.push_back(BoundRadius(ccp(-5, -30), 25));
	HeroData *data1 = new  HeroData(1, 1, 1, 1, true, bound0);

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		data1->pushUpData(HeroUpgradeData(1, i, price[i], 1, 1));
	}

	m_heroDatas.push_back(data1);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(5, 30), 30));
	bound0.push_back(BoundRadius(ccp(5, -30), 25));
	HeroData *data2 = new HeroData(2, 1, 2, 1, false, bound0);

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		data2->pushUpData(HeroUpgradeData(2, i, price[i], 1, 1));
	}

	m_heroDatas.push_back(data2);
}

void DataInitial::initItemData()
{
	/*
	道具属性配置
	道具id
	，道具数值（金币的数值）
	，碰撞检测范围数组
	，预留值d0
	，预留值d1
	*/
	//ItemData(int mId,int itemValue,vector<BoundRadius>  boundRadius,int d0,int d1):mItemId(mId)
	/*
	默认四个道具：
	0，磁铁吸附道具
	1，罐头道具增加攻击
	2，冲刺道具
	3，保护罩道具
	*/
	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(CCPointZero, 60));
	m_itemDatas.push_back(new ItemData(0, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 60));
	m_itemDatas.push_back(new ItemData(1, 1, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 60));
	m_itemDatas.push_back(new ItemData(2, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 30));
	m_itemDatas.push_back(new ItemData(3, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 20));
	m_itemDatas.push_back(new ItemData(4, 1, bound0));
}

void DataInitial::initBulletData()
{
	/*
	子弹属性配置
	子弹id
	，子弹伤害值
	，碰撞检测范围数组
	，isPenetrability可穿透的属性，默认为false
	，预留值d0
	，预留值d1
	*/
	//BulletData(int bId,float damge,vector<BoundRadius> boundRadius.bool isPenetrability,int d0,int d1):mBulletId(bId),mBulletDamage(damge)
	float radius = 35;
	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(CCPointZero, 25));
	m_BulletDatas.push_back(new BulletData(0, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 25));
	m_BulletDatas.push_back(new BulletData(1, 2, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, 25));
	m_BulletDatas.push_back(new BulletData(2, 2, bound0));


	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(10, 2, bound0, false, 0, 0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(11, 2, bound0, false, 0, 1));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(12, 2, bound0, false, 1, 0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(20, 2, bound0, false, 1, 1));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(21, 2, bound0, false, 2, 0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(22, 2, bound0, false, 2, 1));


	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1000, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1001, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1002, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(200, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(201, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(202, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(203, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(204, 2, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(205, 2, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1200, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1201, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1202, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1203, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1204, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(1205, 20, bound0));

	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(308, 5, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(309, 5, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(310, 5, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(311, 5, bound0));
	bound0.clear();
	bound0.push_back(BoundRadius(CCPointZero, radius));
	m_BulletDatas.push_back(new BulletData(312, 5, bound0));
}

void DataInitial::initHero0WeaponData()
{
	WeaponData *level1;

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		level1 = new WeaponData();
		level1->heroWeaponLevelData.push_back(WeaponBulletData(0, ccp(0, 0)));
		hero0WeaponDatas.push_back(level1);

	}
}

void DataInitial::initHero0WeaponEffectData()
{

	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(ccp(0,15), 20*gScaleY));
	WeaponLevelEffectData *levelEffectData = new WeaponLevelEffectData(1,bound0);
	WeaponEffectData *effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,45));

	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 25*gScaleY));
	levelEffectData = new WeaponLevelEffectData(2,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(38.5,28.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(50.5,28.5));
	levelEffectData->m_effectData.push_back(effectData);
	
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(3,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(44.5,39.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(36.5,30.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(52.5,30.5));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(4,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,45)); 
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(39.5,37.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(36.5,30.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(52.5,30.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(49.5,37.5));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(5,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(38,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(52,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);
	
	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 25*gScaleY));
	levelEffectData = new WeaponLevelEffectData(6,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(38,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(52,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(44.5,52.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(7,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(38,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(52,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,45.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(59.5,45.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(8,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(38,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(52,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,45.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(59.5,45.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(44.5,44.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(9,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(38,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(52,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,45.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(59.5,45.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(40.5,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(48.5,44.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(10,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,51));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(35,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(55,39));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(11,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,51));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(35,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(55,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(38.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(50.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(12,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,51));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(35,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(55,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(38.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(50.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(29.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(60.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 45*gScaleY));
	levelEffectData = new WeaponLevelEffectData(13,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,51));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(31,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(59,39));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(25.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(33.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(40.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(48.5,50.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(56.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(64.5,40.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(14,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(15,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(39.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(49.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(16,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(39.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(49.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(26.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(63.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(17,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(39.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(49.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(26.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(63.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(32.5,35.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(56.5,35.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(18,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,39));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(19,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,39));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(34.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(54.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(20,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,39));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(34.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(54.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(26.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(63.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(21,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(45,39));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(34.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(54.5,42.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(26.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(63.5,29.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,37.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(58.5,37.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(22,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(30,31));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(60,31));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(23,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(30,31));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(60,31));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(19.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(69.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(24,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(30,31));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(60,31));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(19.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(69.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(39.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(49.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(25,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(31,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(59,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(26,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(31,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(59,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,47.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(58.5,47.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(27,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(31,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(59,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(30.5,47.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(58.5,47.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(12.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(77.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(28,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(31,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(59,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(31,35));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(59,35));
	levelEffectData->m_effectData.push_back(effectData);  
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(29,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(31,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_3.png",ccp(59,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(31,35));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_1.png",ccp(59,35));
	levelEffectData->m_effectData.push_back(effectData);  
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(14.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_0_2.png",ccp(75.5,34.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero0EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(30,bound0);
	effectData = new WeaponEffectData("bullet/bullet_0_4.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero0EffectData.push_back(levelEffectData);
}

void DataInitial::initHero1WeaponEffectData()
{

	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(ccp(0,5), 20*gScaleY));
	WeaponLevelEffectData *levelEffectData = new WeaponLevelEffectData(1,bound0);
	WeaponEffectData *effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,45));

	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 25*gScaleY));
	levelEffectData = new WeaponLevelEffectData(2,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(34.5,38)); 
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(55.5,38));
	levelEffectData->m_effectData.push_back(effectData);

	m_hero1EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(3,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(44.5,39.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(36.5,30.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(52.5,30.5));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(4,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(34.5,38));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(55.5,38));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(38.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(50.5,45));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(5,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(53,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 25*gScaleY));
	levelEffectData = new WeaponLevelEffectData(6,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(53,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(7,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(53,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(27.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(61.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(8,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(53,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(27.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(61.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(45,41));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(9,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(53,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(27.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(61.5,41));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(39.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(49.5,41));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(10,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(32,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(58,42));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(11,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(32,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(58,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(12,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(32,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(58,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(26.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(63.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 45*gScaleY));
	levelEffectData = new WeaponLevelEffectData(13,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(32,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(58,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(26.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(63.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(31.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(57.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	
	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(14,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(15,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(53.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(16,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(53.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(65.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(17,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(53.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(65.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(30.5,39));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(60.5,39));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(18,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,44));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(19,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(32.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(57.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(20,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(32.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(57.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(64.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(21,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(45,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(32.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(57.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(64.5,37));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(28.5,54));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(61.5,54));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(22,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(55,44));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(23,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(37,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(55,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(65.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(24,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(33,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(58,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(25.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(65.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(40.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(49.5,41));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(25,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(27.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(62.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(26,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(27.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(62.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(27.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(62.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(27,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(27.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(62.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(14.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(39.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(76.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(51.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(28,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(27.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(62.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(27,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(63,44));
	levelEffectData->m_effectData.push_back(effectData);  
	m_hero1EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(29,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(27.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_3.png",ccp(62.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(27,44));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_1.png",ccp(63,44));
	levelEffectData->m_effectData.push_back(effectData);  
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(15.5,39));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_1_2.png",ccp(74.5,39));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero1EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(30,bound0);
	effectData = new WeaponEffectData("bullet/bullet_1_4.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero1EffectData.push_back(levelEffectData); 
}

void DataInitial::initHero2WeaponEffectData()
{
	vector<BoundRadius> bound0;
	bound0.push_back(BoundRadius(ccp(0,5), 20*gScaleY));
	WeaponLevelEffectData *levelEffectData = new WeaponLevelEffectData(1,bound0);
	WeaponEffectData *effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,45));

	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(2,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(32,35.5)); 
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(57,35.5));
	levelEffectData->m_effectData.push_back(effectData);

	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(3,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(32,35.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(57,35.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(45,42.5));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(4,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(32,35.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(57,35.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,41.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(51,41.5));
	levelEffectData->m_effectData.push_back(effectData); 

	m_hero2EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(5,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(6,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(45,44.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(7,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(60,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 30*gScaleY));
	levelEffectData = new WeaponLevelEffectData(8,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(60,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(45,44.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(9,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(37.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(52.5,45));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(60,39.5));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(39,47.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(51,47.5));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(10,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(32.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,10), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(11,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(32.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 45*gScaleY));
	levelEffectData = new WeaponLevelEffectData(12,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(32.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(22,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(68,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 45*gScaleY));
	levelEffectData = new WeaponLevelEffectData(13,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,48));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(32.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,42));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(22,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(68,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,38.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(61,38.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(14,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(15,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(16,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(32,33.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(58,33.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(17,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(52,39.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(32,33.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(58,33.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(26,26.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(64,26.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(18,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,42));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(19,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,42));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(36,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(53,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(20,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,42));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(36,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(53,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(58,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 35*gScaleY));
	levelEffectData = new WeaponLevelEffectData(21,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(45,42));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(36,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(53,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(30,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(58,36.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(24,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(65,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(22,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(33.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(23,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(33.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(26,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(64,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);


	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 40*gScaleY));
	levelEffectData = new WeaponLevelEffectData(24,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(33.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(56.5,36));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(26,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(64,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(38,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(51,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(25,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(31.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(59.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(26,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(31.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(59.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(37,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(53,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(27,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(31.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(59.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(37,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(53,44.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(16,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(75,30.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,5), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(28,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(31.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(59.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(31.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(59.5,43));
	levelEffectData->m_effectData.push_back(effectData);  
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(29,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(31.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_3.png",ccp(59.5,45));
	levelEffectData->m_effectData.push_back(effectData);
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(31.5,43));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_1.png",ccp(59.5,43));
	levelEffectData->m_effectData.push_back(effectData);  
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(16,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	effectData = new WeaponEffectData("bullet/bullet_2_2.png",ccp(75,32.5));
	levelEffectData->m_effectData.push_back(effectData); 
	m_hero2EffectData.push_back(levelEffectData);

	bound0.clear();
	bound0.push_back(BoundRadius(ccp(0,0), 50*gScaleY));
	levelEffectData = new WeaponLevelEffectData(30,bound0);
	effectData = new WeaponEffectData("bullet/bullet_2_4.png",ccp(45,45));
	levelEffectData->m_effectData.push_back(effectData);
	m_hero2EffectData.push_back(levelEffectData);  
}
void DataInitial::initHero1WeaponData()
{
	WeaponData *level1;

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		level1 = new WeaponData();
		level1->heroWeaponLevelData.push_back(WeaponBulletData(1, ccp(0, 0)));
		hero1WeaponDatas.push_back(level1);

	}
}

void DataInitial::initHero2WeaponData()
{
	WeaponData *level1;

	for(int i = 0; i != HERO_MAXLEVEL; ++i)
	{
		level1 = new WeaponData();
		level1->heroWeaponLevelData.push_back(WeaponBulletData(2, ccp(0, 0)));
		hero2WeaponDatas.push_back(level1);

	}
}


void DataInitial::initPetData()
{
	/*
	宠物数据配置
	宠物id
	，伤害值dg
	，攻击范围fLength
	，攻击频率aFre
	*/
	//PetData(int pId,float dg,float fLength,float aFre,int d0=0,int d1=0)
	float petDamage[6] = {1, 1, 1, 1, 1, 1};
	float petAttackLenght[6] = {150, 300, 150, 150, 150, 150};
	float petFrequence[6] = {2, 2, 2, 2, 2, 2};

	for(int j = 0; j != 6; ++j)
	{
		PetData *data = new PetData(j, petDamage[j], petAttackLenght[j], petFrequence[j]);

		for(int i = 0; i != 30; ++i)
		{
			data->pushPetUgData(PetUpgrateData(i, 100 + i * 200, 2));
		}

		m_PetDatas.push_back(data);
	}
}


void DataInitial::initLevelUnLock()
{
	/*关卡id
	，关卡图片Nml
	，关卡图片Sel
	，关卡icon
	，解锁类型
	，解锁值
	，关卡图标位置
	*/
	//LevelUnLock(int lId,string nmlFile,string selFile,string iconName,int unLockType,int unValue,CCPoint pos):mLevelId(lId)

	string LEVEL_CLOCK = "ui/levelChoice/levelCLock.png";
	string LEVEL_SLOCK = "ui/levelChoice/levelSLock.png";
	string LEVEL_BOSS_LOCK = "ui/levelChoice/levelBossLock.png";

	string LEVEL0_UNLOCK_CNML = "ui/levelChoice/level0CNml.png";
	string LEVEL0_UNLOCK_CSEL = "ui/levelChoice/level0CNml.png";
	string LEVEL0_UNLOCK_SNML = "ui/levelChoice/level0SNml.png";
	string LEVEL0_UNLOCK_SSEL = "ui/levelChoice/level0SNml.png";
	string LEVEL0_UNLOCK_BOSS_NML = "ui/levelChoice/level0BossNml.png";
	string LEVEL0_UNLOCK_BOSS_SEL = "ui/levelChoice/level0BossNml.png";

	string LEVEL1_UNLOCK_CNML = "ui/levelChoice/level1CNml.png";
	string LEVEL1_UNLOCK_CSEL = "ui/levelChoice/level1CNml.png";
	string LEVEL1_UNLOCK_SNML = "ui/levelChoice/level1SNml.png";
	string LEVEL1_UNLOCK_SSEL = "ui/levelChoice/level1SNml.png";
	string LEVEL1_UNLOCK_BOSS_NML = "ui/levelChoice/level1BossNml.png";
	string LEVEL1_UNLOCK_BOSS_SEL = "ui/levelChoice/level1BossNml.png";

	string LEVEL2_UNLOCK_CNML = "ui/levelChoice/level2CNml.png";
	string LEVEL2_UNLOCK_CSEL = "ui/levelChoice/level2CNml.png";
	string LEVEL2_UNLOCK_SNML = "ui/levelChoice/level2SNml.png";
	string LEVEL2_UNLOCK_SSEL = "ui/levelChoice/level2SNml.png";
	string LEVEL2_UNLOCK_BOSS_NML = "ui/levelChoice/level2BossNml.png";
	string LEVEL2_UNLOCK_BOSS_SEL = "ui/levelChoice/level2BossNml.png";

	string LEVEL3_UNLOCK_CNML = "ui/levelChoice/level3CNml.png";
	string LEVEL3_UNLOCK_CSEL = "ui/levelChoice/level3CNml.png";

	string LEVEL3_UNLOCK_SNML = "ui/levelChoice/level3SNml.png";
	string LEVEL3_UNLOCK_SSEL = "ui/levelChoice/level3SNml.png";
	string LEVEL3_UNLOCK_BOSS_NML = "ui/levelChoice/level3BossNml.png";
	string LEVEL3_UNLOCK_BOSS_SEL = "ui/levelChoice/level3BossNml.png";


	m_unLockConfig.push_back(new LevelUnLock(0, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 1, ccp(296.5 * gScaleX, 137.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(1, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 2, ccp(226.5 * gScaleX, 181.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(2, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 3, ccp(296.5 * gScaleX, 298.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(3, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 4, ccp(233.5 * gScaleX, 399.5 * gScaleY), 1));
	m_unLockConfig.push_back(new LevelUnLock(4, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 5, ccp(216.5 * gScaleX, 517.5 * gScaleY)));

	m_unLockConfig.push_back(new LevelUnLock(5, LEVEL0_UNLOCK_BOSS_NML, LEVEL0_UNLOCK_BOSS_SEL, LEVEL_BOSS_LOCK, HERO_LEVEL, 6, ccp(296 * gScaleX, 637 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(6, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 7, ccp(290.5 * gScaleX, 762.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(7, LEVEL0_UNLOCK_CNML, LEVEL0_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 8, ccp(253.5 * gScaleX, 847.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(8, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 9, ccp(334.5 * gScaleX, 932.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(9, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 10, ccp(146.5 * gScaleX, 1102.5 * gScaleY)));

	m_unLockConfig.push_back(new LevelUnLock(10, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 11, ccp(176.5 * gScaleX, 1192.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(11, LEVEL0_UNLOCK_BOSS_NML, LEVEL0_UNLOCK_BOSS_SEL, LEVEL_CLOCK, HERO_LEVEL, 12, ccp(272 * gScaleX, 1277 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(12, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 13, ccp(253.5 * gScaleY, 1417.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(13, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 14, ccp(176.5 * gScaleX, 1497.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(14, LEVEL1_UNLOCK_CNML, LEVEL1_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 15, ccp(233.5 * gScaleX, 1607.5 * gScaleY)));

	m_unLockConfig.push_back(new LevelUnLock(15, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 16, ccp(401.5 * gScaleX, 1757.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(16, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 17, ccp(277.5 * gScaleX, 1787.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(17, LEVEL0_UNLOCK_BOSS_NML, LEVEL0_UNLOCK_BOSS_SEL, LEVEL_BOSS_LOCK, HERO_LEVEL, 18, ccp(186 * gScaleX, 1887 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(18, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 19, ccp(417.5 * gScaleX, 2023.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(19, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 20, ccp(314.5 * gScaleX, 2092.5 * gScaleY)));

	m_unLockConfig.push_back(new LevelUnLock(20, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 21, ccp(185.5 * gScaleX, 2127.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(21, LEVEL2_UNLOCK_CNML, LEVEL2_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 22, ccp(346.5 * gScaleX, 2297.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(22, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 23, ccp(334.5 * gScaleX, 2402.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(23, LEVEL0_UNLOCK_BOSS_NML, LEVEL0_UNLOCK_BOSS_SEL, LEVEL_CLOCK, HERO_LEVEL, 24, ccp(245 * gScaleX, 2513 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(24, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 25, ccp(283.5 * gScaleX, 2742.5 * gScaleY)));

	m_unLockConfig.push_back(new LevelUnLock(25, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 26, ccp(193.5 * gScaleX, 2812.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(26, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 27, ccp(217.5 * gScaleY, 2917.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(27, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 28, ccp(326.5 * gScaleY, 2947.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(28, LEVEL3_UNLOCK_CNML, LEVEL3_UNLOCK_CSEL, LEVEL_CLOCK, HERO_LEVEL, 29, ccp(314.5 * gScaleY, 3042.5 * gScaleY)));
	m_unLockConfig.push_back(new LevelUnLock(29, LEVEL0_UNLOCK_BOSS_NML, LEVEL0_UNLOCK_BOSS_SEL, LEVEL_CLOCK, HERO_LEVEL, 30, ccp(98 * gScaleY, 2993 * gScaleY)));

	//
	/*初始化支线关卡数据*/

	m_subUnLockConfig.push_back(new LevelUnLock(0, LEVEL0_UNLOCK_SNML, LEVEL0_UNLOCK_SSEL, LEVEL_SLOCK, START_COUT, 21, ccp(133.5 * gScaleX, 664 * gScaleY), 6));
	m_subUnLockConfig.push_back(new LevelUnLock(1, LEVEL1_UNLOCK_SNML, LEVEL1_UNLOCK_SSEL, LEVEL_SLOCK, START_COUT, 33, ccp(396.5 * gScaleX, 1200 * gScaleY), 12));
	m_subUnLockConfig.push_back(new LevelUnLock(2, LEVEL2_UNLOCK_SNML, LEVEL2_UNLOCK_SSEL, LEVEL_SLOCK, START_COUT, 45, ccp(116.5 * gScaleX, 2024 * gScaleY), 18));

}

void DataInitial::initRewardInf()
{
	/*
	初始化关卡配置信息
	lId奖励id
	，奖励图片nml
	，奖励图片sel
	，解锁类型
	，解锁值0
	，位置
	，奖励值
	，解锁值1
	*/
	//RewardConfig(int lId,string nmlFile,string selFile,int unLockType,int unValue0,CCPoint pos,int rValue,int unValue1 = -1)
	string REWARD_FILE0Nml = "ui/levelChoice/reward0Nml.png";
	string REWARD_FILE0Sel = "ui/levelChoice/reward0Nml.png";

	string REWARD_FILE1Nml = "ui/levelChoice/reward1Nml.png";
	string REWARD_FILE1Sel = "ui/levelChoice/reward1Nml.png";

	string REWARD_FILE2Nml = "ui/levelChoice/reward2Nml.png";
	string REWARD_FILE2Sel = "ui/levelChoice/reward2Nml.png";

	string REWARD_FILE3Nml = "ui/levelChoice/reward3Nml.png";
	string REWARD_FILE3Sel = "ui/levelChoice/reward3Nml.png";

	m_rewardConfig.push_back(new RewardConfig(0, REWARD_FILE2Nml, REWARD_FILE2Sel, LEVEL, 1, ccp(158 * gScaleX, 267.5 * gScaleY), 1));
	m_rewardConfig.push_back(new RewardConfig(1, REWARD_FILE2Nml, REWARD_FILE2Sel, LEVEL, 5, ccp(418 * gScaleX, 693.5 * gScaleY), 3));
	m_rewardConfig.push_back(new RewardConfig(2, REWARD_FILE3Nml, REWARD_FILE3Sel, SUB_LEVEL, 0, ccp(79.5 * gScaleX, 788 * gScaleY), 2000));
	m_rewardConfig.push_back(new RewardConfig(3, REWARD_FILE3Nml, REWARD_FILE3Sel, LEVEL, 8, ccp(247.5 * gScaleX, 1032 * gScaleY), 5000));

	m_rewardConfig.push_back(new RewardConfig(4, REWARD_FILE2Nml, REWARD_FILE2Sel, LEVEL, 11, ccp(388 * gScaleX, 1343.5 * gScaleY), 2));
	m_rewardConfig.push_back(new RewardConfig(5, REWARD_FILE3Nml, REWARD_FILE3Sel, SUB_LEVEL, 1, ccp(417.5 * gScaleX, 1068 * gScaleY), 3000));


	m_rewardConfig.push_back(new RewardConfig(6, REWARD_FILE3Nml, REWARD_FILE3Sel, LEVEL, 14, ccp(352.5 * gScaleX, 1658 * gScaleY), 5000));
	m_rewardConfig.push_back(new RewardConfig(7, REWARD_FILE3Nml, REWARD_FILE3Sel, SUB_LEVEL, 2, ccp(49.5 * gScaleX, 2132 * gScaleY), 8000));


	m_rewardConfig.push_back(new RewardConfig(8, REWARD_FILE2Nml, REWARD_FILE2Sel, LEVEL, 17, ccp(306 * gScaleX, 1956.5 * gScaleY), 3));

	m_rewardConfig.push_back(new RewardConfig(9, REWARD_FILE3Nml, REWARD_FILE3Sel, LEVEL, 20, ccp(222.5 * gScaleX, 2238 * gScaleY), 10000));
	m_rewardConfig.push_back(new RewardConfig(10, REWARD_FILE2Nml, REWARD_FILE2Sel, LEVEL, 23, ccp(255 * gScaleX, 2643.5 * gScaleY), 5));
}

vector<LevelUnLock*> & DataInitial::getUnLockConfigs()
{
	return m_unLockConfig;
}

vector<LevelUnLock*> & DataInitial::getSubUnLockConfigs()
{
	return m_subUnLockConfig;
}

vector<RewardConfig*> & DataInitial::getRewardConfigs()
{
	return m_rewardConfig;
}
//add zh
vector<PetData*> & DataInitial::getPetData()
{
	return m_PetDatas;
}

vector<WeaponData*> & DataInitial::gethero0WeaponData()
{
	return hero0WeaponDatas;
}

vector<WeaponData*> & DataInitial::gethero1WeaponData()
{
	return hero1WeaponDatas;
}

vector<WeaponData*> & DataInitial::gethero2WeaponData()
{
	return hero2WeaponDatas;
}

vector<WeaponLevelEffectData*> & DataInitial::getheroEffectData(int id)
{
	if(id ==0)
	{
		return m_hero0EffectData;
	}
	else  if(id ==1)
	{
		return m_hero1EffectData;
	}
	else if(id==2)
	{
		return m_hero2EffectData;
	}
	
}

float DataInitial::getMapNodeSpeed()
{
	return m_mapNodeSpeed;
}

float DataInitial::getMapNodeUpSpeed()
{
	return m_mapNodeUpSpeed;
}

DataInitial::DataInitial()
{
	initData();
}

void DataInitial::setLvTag(int tag)
{

	leveltag = tag;
}
int DataInitial::getLvTag()
{
	return  leveltag;
}


