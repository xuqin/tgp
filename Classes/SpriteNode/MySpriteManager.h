#ifndef MYSPRITEMANAGER_H
#define MYSPRITEMANAGER_H

#include <iostream>
#include <list>
#include <string>


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <pthread.h>
#else
#include "pthread/pthread.h"
#endif
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "HeroBulletSprite.h"
using namespace std;
using namespace cocos2d;
using namespace  CocosDenshion;

class MySprite;
class HeroSprite;
class ItemSprite;
class MonsterSprite;
class BulletSprite;
class HeroBulletSprite;

extern pthread_mutex_t      gSpriteMutex;
extern pthread_mutex_t      gItemMutex;


enum THREAD_STATE
{
	THREAD_RUNING,
	THREAD_END,
	THREAD_PAUSE,
};

struct CreateMInf;
class MySpriteManager
{
public:
	MySpriteManager();
	~MySpriteManager();
	MonsterSprite *createMonsterById(int monsterId);
	MonsterSprite *createMonster(const CreateMInf& mInf);
	ItemSprite *createItemById(int itemId);
//	BulletSprite *createHeroBullet(int bulletId);
HeroBulletSprite *createHeroBullet(int bulletId);
	BulletSprite *createMonsterBullet(int bulletId);
	BulletSprite *createPetBullet(int bulletId);
	BulletSprite *createPetBulletWithPet(int bulletId, int petId);
	list<MonsterSprite*> *getMonsterSpriteList();
	list<ItemSprite*> *getItemSpriteList();


//	BulletSprite *createHeroSubBullet(int bulletId, int sub);
	void monsterDeadOp(MonsterSprite *mSprite);
	void detMonster();
	void detItem();
	void deBullet();
	static bool collisionMonsterWithHero(MySprite *sprite1, HeroSprite *sprite2);
	static bool collisionDetectTwoSprite(MySprite *sp1, MySprite *sp2);
	static CCPoint getRotateCenterPosOnLayer(MySprite* rotateSprite, CCPoint curCenter); /*获取精灵配置的碰撞检测区域的中心点坐标*/
	void spriteCollisiion();
	void checkCollision(float t);
	//void pauseAllActionAndSchedule();
	void resumeAllActionAndSchedule();
	static void* collisionDetection(void *r);
	list<MonsterSprite *>* getBeAttackMonsterList(CCPoint petpos , float attacklength);
	void detAllMonster();
	static void monsterWithHeroCollision();
	static void itemWithHeroCollision();
	static void bulletWithHero();
	static void bulletWithMonster();
	void setThreadState(THREAD_STATE state);
	void updateMySprite(float t);
	void clearAllEnemy();
	void clearAllMoneyItem();
	void computeGameOver(int currentLevel, int bossId);

	MonsterSprite *getBeAttackMonster(CCPoint petpos , float attacklength);
private:
	bool m_threadInit;
};

#endif
