#ifndef HERR_BULLET_SPRITE_H
#define HERR_BULLET_SPRITE_H
#include "MySprite.h"
#include "cocos2d.h"
#include "../GCData/DataInitial.h"
using namespace cocos2d;

class HeroBulletSprite : public MySprite
{
public:
	HeroBulletSprite(int bulletId);
	HeroBulletSprite(int bulletId, int petId);

	~HeroBulletSprite();
	static HeroBulletSprite * createBulletById(int bulletId);
	virtual bool init();

	void setDamge(float damage);
	float getDamage();

	void setIsPenetrability(bool enable);
	bool getIsPenetrability();
	void setPetId(int petId);
	int getPetId();
private:
	int m_bulletId;
	int m_petId;
	bool isMax;
	BulletData *m_bulletData;
	float m_bulletDamage;
	bool m_isPenetrability;
};
#endif