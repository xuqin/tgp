#ifndef MONSTERENEMY_H
#define MONSTERENEMY_H

#include "MySprite.h"
#include "../GCData/DataInitial.h"
#include "cocos2d.h"
using namespace cocos2d;

class HeroSprite;

struct CreateMInf
{
	int mMonsterId;
	int mPosX;
	int mPosY;
	float  mActionSpeed;
	int mDeadItemId;/*死后的道具id*/
	int mDeadMId;/*死后需要创建怪物的id*/
	int mDeadMActionId;
	int mItemPercent;
	CreateMInf(): mMonsterId(0)
		, mPosX(0)
		, mPosY(0)
		, mActionSpeed(-1)
		, mDeadItemId(-1)
		, mDeadMId(-1)
		, mDeadMActionId(-1)
	{
	}
};


class MonsterSprite : public MySprite
{
public:
	MonsterSprite(int mId);
	MonsterSprite(const CreateMInf& mInf);
	virtual ~MonsterSprite();
	static MonsterSprite *createMonsterById(int mId);
	static MonsterSprite *createMonsterByInf(const CreateMInf& mIf);
	virtual bool init();
	void initWithAni();
	void initWithFiles();
	int getMonsterId();
	int getScore();
	void initMonsterData(MonsterData* mData);
	void   setMinf(const CreateMInf & cInf);
	CreateMInf& getMInf();
	void updatePosWithMap(float t);
	int getActionId();
	void addMonsterTip();
	void createBossById(int id);
	void addBossEffectById(int id);
	bool isBoss();
	void bossMove(float t);
	//void tipEnd(float t);
	void tipEnd();
	void addBloodBar();
	void updateBloodBar(float t);
	void playMoveDownUp();
	void animationEnd(); 
	void slowDown();

public:
	virtual void shootBullet(float t);
	virtual void scheduleShoot();
	void shoot0BulletOne();
	void shoot1BulletOne();
	void shoot2BulletOne();
	void shoot3BulletOne();
	void shoot4BulletOne();

	void shoot0BulletTwo();
	void shoot1BulletTwo();
	void shoot2BulletTwo();
	void shoot3BulletTwo();
	void shoot4BulletTwo();


	void shoot0BulletThree();
	void shoot1BulletThree();
	void shoot2BulletThree();
	void shoot3BulletThree();
	void shoot4BulletThree();

private:
	int m_monsterId;
	MonsterData *m_monsterData;
	float m_shootFrq;
	int m_score;
	bool m_shootEnabled;
	int m_shootFunId;
	int m_bulletId;
	bool m_isBoss;
	CreateMInf m_createMInf;
	int m_actionId;
	bool m_moveStart;
	float m_fullhp;
	int m_shootTag;
	bool m_shootOrientation;
	bool m_isPlayDownUp;
	float m_lowHp;
	bool m_slowDown;
};

typedef void (MonsterSprite::*MonsterWeapon)();
#endif