#ifndef DATAINITIAL_H
#define  DATAINITIAL_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "cocos2d.h"

#include "../ToolFun/ToolsFun.h"

using namespace std;
using namespace cocos2d;

const int   RMB_UNLOCK = 0;
const int 	HERO_LEVEL = 1;
const int 	START_COUT = 2;
const int   LEVEL = 0;
const int   SUB_LEVEL = 1;

const int HERO_MAXLEVEL = 30;
const int PET_MAXLEVEL=30;
const int MAXUNLOCKLEVEL=30;
struct BoundRadius
{
	/*相对图片中心点的偏移*/
	CCPoint rCenter;
	float radius;
	BoundRadius():radius(1)
	{

	}
	BoundRadius(CCPoint pCenter,float r):rCenter(pCenter),radius(r)
	{

	}
};

struct MonsterData
{
	int mId;
	int mHp;
	int mScore;
	float mSpeed;
	bool mShootEnabled;
	int mShootFunId;
	int mBulletId;
	bool mIsBoss;
	float mShootFrq;
	/*以上是xml中配置的数据*/
	vector<BoundRadius>  mBoundRadius;
	int mLowHp;;
	int mDefault1;
	MonsterData(int id,int hp,int score,float speed,bool shootE,int shootFId,int bulletId,bool isBoss,float shootFrq,vector<BoundRadius> vBoundR,int default0=0,int default1=0):mId(id)
		,mHp(hp)
		,mScore(score)
		,mSpeed(speed)
		,mShootEnabled(shootE)
		,mShootFunId(shootFId)
		,mBulletId(bulletId)
		,mIsBoss(isBoss)
		,mShootFrq(shootFrq)
	{
		mBoundRadius = vBoundR;
		mLowHp = default0;
		mDefault1 = default1;
	}
};

struct HeroUpgradeData
{
	int mHeroId;
	int mHeroLevel;
	int mlevelPrice;
	float mLevelValue1;
	float mLevelValue2;
	HeroUpgradeData(int mH,int mHlevel,int levelP,float levelV1,float leveV2):mHeroId(mH)
		,mHeroLevel(mHlevel)
		,mlevelPrice(levelP)
		,mLevelValue1(levelV1)
		,mLevelValue2(leveV2)
	{

	}
};

struct HeroData
{
	int mHeroId;
	int mMaxHp;
	int mCurHp;
	float mDamage;
	bool mMagneticEnable;
	vector<BoundRadius>  mBoundRadius;
	int mDefault0;
	int mDefault1;
	vector<HeroUpgradeData> mUpgradeData;
	HeroData(int heroId,int maxHp,int curHp,float damage,bool magneticEnable,vector<BoundRadius>  boundRadius,int d0=0,int d1=0):mHeroId(heroId)
		,mMaxHp(maxHp)
		,mCurHp(curHp)
		,mDamage(damage)
		,mMagneticEnable(magneticEnable)
		,mDefault0(d0)
		,mDefault1(d1)
	{
		mBoundRadius = boundRadius;
		mDefault0 = d0;
		mDefault1 = d1;
	}
	void pushUpData(const HeroUpgradeData& data)
	{
		mUpgradeData.push_back(data);
	}
};


struct ItemData
{
	int mItemId;
	int mItemValue;
	vector<BoundRadius>  mBoundRadius;
	int mD0;
	int mD1;
	ItemData(int mId,int itemValue,vector<BoundRadius>  boundRadius,int d0=0,int d1=0):mItemId(mId)
		,mItemValue(itemValue)
	{
		mBoundRadius = boundRadius;
		mD0 = d0;
		mD1 = d1;
	}
};

struct BulletData
{
	int mBulletId;
	float mBulletDamage;
	vector<BoundRadius> mBoundRadius;
	bool mIsPenetrability;
	int parentId;
	int mSub;
	
	BulletData(int bId,float damge,vector<BoundRadius> boundRadius,bool isPenetrability=false,int d0=-1,int d1=-1):mBulletId(bId),mBulletDamage(damge)
	{
		mBoundRadius = boundRadius; 
		mIsPenetrability = isPenetrability;
		mSub = d1;
		parentId = d0;
	}
};
//add zh

struct WeaponBulletData
{
	int bulletId;
	CCPoint posBuy;
	WeaponBulletData(int bbulletid , CCPoint subpos):bulletId(bbulletid),posBuy(subpos)
	{
		
	};
};
struct WeaponEffectData
{
	string m_bulletName;
	CCPoint m_pos;
	WeaponEffectData(string fileName,CCPoint pos):m_bulletName(fileName),m_pos(pos){}
};

struct WeaponLevelEffectData
{
	int m_levelId;
	vector<BoundRadius> mBoundRadius;
	vector<WeaponEffectData *> m_effectData;
	WeaponLevelEffectData(int id,vector<BoundRadius> boundRadius):m_levelId(id),mBoundRadius(boundRadius){}
};

struct WeaponData
{
	vector<WeaponBulletData> heroWeaponLevelData;
};

struct PetUpgrateData
{
	int level;
	int levelPrice;
	float levelValue;
	PetUpgrateData(int lv,int  lP,float lV):level(lv),levelPrice(lP),levelValue(lV)
	{
	}
};
struct PetData
{
	int mPetId;
	float mDamage;
	float mAttackLenglth;
	float mAttackFre;
	int mD0;
	int mD1;
	vector<PetUpgrateData> petUpgradeDatas;
	PetData(int pId,float dg,float fLength,float aFre,int d0=0,int d1=0):mPetId(pId)
		,mDamage(dg)
		,mAttackLenglth(fLength)
		,mAttackFre(aFre)
	{

	}
	void pushPetUgData(const PetUpgrateData& petData)
	{
		petUpgradeDatas.push_back(petData);
	}
};

struct  LevelUnLock
{
	int mLevelId;
	string mLevelNmlFile;
	string mLevelSelFile;
	string mLevelIconFile;
	int mUnLockType ;
	int mUnLockValue0;
	CCPoint mPos;
	int mUnLockValue1;
	LevelUnLock(int lId,string nmlFile,string selFile,string iconName,int unLockType,int unValue0,CCPoint pos,int unValue1 = -1):mLevelId(lId)
		,mLevelNmlFile(nmlFile)
		,mLevelSelFile(selFile)
		,mLevelIconFile(iconName)
		,mUnLockType(unLockType)
		,mUnLockValue0(unValue0)
		,mPos(pos)
		,mUnLockValue1(unValue1)
	{

	}

};

struct RewardConfig
{
	int mRewardId;
	string mRewardNmlFile;
	string mRewardSelFile;
	int mUnLockType;
	int mUnLockValue0;
	CCPoint mPos;
	int mRewardValue;
	int mUnLockValue1;
	RewardConfig(int lId,string nmlFile,string selFile,int unLockType,int unValue0,CCPoint pos,int rValue,int unValue1 = -1):mRewardId(lId)
		,mRewardNmlFile(nmlFile)
		,mRewardSelFile(selFile)
		,mUnLockType(unLockType)
		,mUnLockValue0(unValue0)
		,mPos(pos)
		,mRewardValue(rValue)
		,mUnLockValue1(unValue1)
	{

	}
};

struct StarConfig
{
	int m_min;
	int m_max;
	StarConfig(int min,int max):m_min(min),m_max(max)
	{

	}
};

struct UpgratePetData
{
	int m_petId;
	vector<float> m_damage;
	vector<int> m_price;
	UpgratePetData(int id):m_petId(id)
	{

	}
};

class DataInitial
{
public:
	static DataInitial& initialDataSource();
	void initData();
	MonsterData * getMDataById(int mId);
	HeroData *getHDataById(int hId);
	HeroUpgradeData & getHeroUpgrateData(int heroId,int level);
	ItemData *getItemDataById(int iId);
	BulletData *getBulletDataById(int bId);
	BulletData *getBulletDataByPIdandSub(int pId,int sub);
	PetData *getPetDataById(int pId);
	PetUpgrateData& getPetUpgrateData(int pId,int level);
	LevelUnLock *getLeveUnLockInf(int levelId);
	LevelUnLock *getSubLevelUnLockInf(int levelId);
	RewardConfig*getRewardInf(int rewaredId);
	StarConfig *getStarConfig(int leveId);

	~DataInitial();

	void initMData();
	void initHeroData();
	void initItemData();
	void initBulletData();
	void initPetData();
	void initLevelUnLock();
	void initRewardInf();
	void initStarData();
	void setLvTag(int tag);
	int getLvTag();

	void initHeroDamage();
	float getHeroDamage(int id);

	void initPetUpgradeData();
	UpgratePetData* getSubPetUpgrateData(int pet);

	vector<LevelUnLock*> & getUnLockConfigs();
	vector<LevelUnLock*> & getSubUnLockConfigs();
	vector<RewardConfig*> & getRewardConfigs();
	//add zh
	vector<PetData*> & getPetData();
	vector<WeaponData*> & gethero0WeaponData();
	vector<WeaponData*> & gethero1WeaponData();
	vector<WeaponData*> & gethero2WeaponData();
	vector<WeaponBulletData*> & getWeaponBulletData();
	void initHero0WeaponData();
	void initHero1WeaponData();
	void initHero2WeaponData();


	vector<WeaponLevelEffectData*> & getheroEffectData(int id);

	void initHero0WeaponEffectData();
	void initHero1WeaponEffectData();
	void initHero2WeaponEffectData();
	float getMapNodeSpeed();
	float getMapNodeUpSpeed();
	float getHeroShootFrq();
	float getPetShootFrq();

private:
	DataInitial();
	DataInitial(const DataInitial& data);
	DataInitial & operator=(const DataInitial& data);
	


private:
	int leveltag ;
	vector<MonsterData*> m_monsterDatas;
	vector<HeroData*> m_heroDatas;
	vector<ItemData*> m_itemDatas;
	vector<BulletData*> m_BulletDatas;

	vector<PetData*> m_PetDatas;
	vector<LevelUnLock*> m_unLockConfig;
	vector<LevelUnLock*> m_subUnLockConfig;
	vector<RewardConfig*> m_rewardConfig;
	//add
	vector<WeaponData*> hero0WeaponDatas;
	vector<WeaponData*> hero1WeaponDatas;
	vector<WeaponData*> hero2WeaponDatas;
	vector<WeaponLevelEffectData *> m_hero0EffectData;
	vector<WeaponLevelEffectData *> m_hero1EffectData;
	vector<WeaponLevelEffectData *> m_hero2EffectData;
	vector<StarConfig *> m_starConfig;

	vector<float> m_HeroDamage;
	vector<UpgratePetData*> m_petUpData;
	float m_HeroShootFrq;
	float m_PetShootFrq;

	float m_mapNodeSpeed;
	float m_mapNodeUpSpeed;
};


#endif
