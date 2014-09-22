#ifndef PETSPRITE_H
#define  PETSPRITE_H
#include "MySprite.h"

class MonsterSprite;
class BulletSprite;

struct PetData;
class PetSprite : public MySprite
{
public:
	PetSprite();
	PetSprite(bool boundRadius );
	PetSprite(int petId);
	~PetSprite();
	virtual bool init();
	static PetSprite * createPetById(int petId);

	void pet0Attack(float t);
	void pet1Attack(float t);
	void pet2Attack(float t);
	void pet3Attack(float t);
	void pet4Attack(float t);
	void pet5Attack(float t);
	void petAttack(float t);

	void pet0FullAttack(float t);
	void pet1FullAttack(float t);
	void pet2FullAttack(float t);
	void pet3FullAttack(float t);
	void pet4FullAttack(float t);
	void pet5FullAttack(float t);

	int getPetId();
	int getPetLevel();
	void petFullAttack(float t);
	void removeSprite(CCNode *node);
	void pauseShoot();
	void resumeShoot();
	void resetDamage(BulletSprite *sprite);
	void pet1ShootBullet(float dt);
private:
	int m_petId;
	bool m_pauseShoot;
	PetData *m_petData;
	int m_petPos;
	int m_petLevel;
	float m_levelDamage;

};

typedef void (PetSprite::*PetWeapon)(float t);
#endif