#ifndef HEROSPRITE_H
#define HEROSPRITE_H
#include "cocos2d.h"
#include "GCData/DataInitial.h"
#include "MySprite.h"
using namespace cocos2d;


class HeroSprite : public MySprite
{
public:
	HeroSprite();
	HeroSprite(int heroId);
	static HeroSprite* createHeroById(int heroId);
	virtual bool init();
	void shootBullet(float t);
// 	void weaponId0(float t);
// 	void weaponId1(float t);
// 	void weaponId2(float t);
	void weaponId(int m_heroId);
	void setProtectedState(bool state);
	bool getProtectedState();

	void setInvincible(bool state);
	bool getInvincible();

	void runMagneticEffect();
	void magneticEffectEnd(CCNode *node);

	void runSpeedUpEffect();
	void speedUpEffectEnd(float t);

	void addProtectEffect();
	void proEffectEnd(float t);

	void addAttackEffect();
	void endAttackEffect(CCNode *node);

	virtual void subHp(float damage);
	bool getSpeedUpState();
	void setSpeedUpState(bool state);
	bool getMgneticState();
	void setMagneticState(bool state);

	void pauseShoot();
	void resumeShoot();

	void addInvincibleEffect();
	void end0Invincible(CCNode *node);
	void end1Invincible(CCNode *node);
	void end2Invincible(CCNode *node);
	void endInvincible(CCNode* node);
	void creatBulletsPos();
	
private:
	int m_heroId;
	HeroData *m_heroConfigData;
	int m_playerLevel;
	float m_levelDamage;
	float m_shootFrq;
	bool m_magneticEnabled;
	bool m_protectState;
	bool m_invincible;
	bool m_speedUpState;
	bool m_pauseShoot;
	bool m_doubleDamage;
	vector <CCPoint> hero_bullet_posarr;

};
typedef void (HeroSprite::*HeroWeapon)(float t);
#endif