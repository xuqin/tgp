#ifndef MYSPRITE_H
#define  MYSPRITE_H


#include <vector>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

struct BoundRadius;

class MySprite : public CCSprite
{
public:
	MySprite();
	virtual ~MySprite();
	CREATE_FUNC(MySprite);
	virtual void draw();

	virtual void subHp(float damage);
	virtual void addHp(int aHp);
	void setSpeed(float speed);
	void setHp(float hp);
	float getSpeed();
	float getHp();
	vector<BoundRadius>& getBoundRadius();
	bool getDeadState();
	void setDeadState(bool state);
	void setDeadType(int type);
	int getDeadType();
protected:
	int m_hp;
	int m_maxHp;
	float m_speed;
	vector<BoundRadius> m_boundRadius;
	bool m_boundRadiusInitState;
	bool m_deadState;
	int m_deadType;
};
#endif