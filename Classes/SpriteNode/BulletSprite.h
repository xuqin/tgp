#ifndef BULLETSPRITE_H
#define BULLETSPRITE_H
#include "MySprite.h"
#include "cocos2d.h"
#include "../GCData/DataInitial.h"
using namespace cocos2d;

class BulletSprite : public MySprite
{
public:
	BulletSprite(int bulletId);
	BulletSprite(int bulletId, int petId);

	~BulletSprite();
	static BulletSprite * createBulletById(int bulletId);
	static BulletSprite * createSubBulletById(int bulletId, int sub);
	static BulletSprite * createBulletByIdWithPet(int bulletId, int petId);
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