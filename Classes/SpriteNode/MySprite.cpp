#include "MySprite.h"
#include "../GCData/DataInitial.h"

MySprite::MySprite(): m_hp(0)
	, m_speed(0)
	, m_boundRadiusInitState(false)
	, m_deadState(false)
	, m_deadType(-1)
{

}

MySprite::~MySprite()
{

}

void MySprite::draw()
{
	CCSprite::draw();

	if(m_boundRadiusInitState)
	{
		CCSize size = this->getContentSize();
		glLineWidth(1);
		ccDrawColor4B(0, 255, 0, 100);

		/*
		for(int i = 0; i != m_boundRadius.size(); ++i)
		{
			ccDrawCircle(m_boundRadius[i].rCenter + size / 2, m_boundRadius[i].radius, 0, 36, true);
		} */

		/*CCPoint posArray[4] = {ccp(0, 0), ccp(0, size.height), size, ccp(size.width, 0)};
		ccDrawColor4B(255, 0, 0, 100);
		ccDrawPoly(posArray, 4, true);*/
	}
}

void MySprite::subHp(float damage)
{
	m_hp -= damage;
}

void MySprite::addHp(int aHp)
{
	m_hp += aHp;
}

void MySprite::setSpeed(float speed)
{
	m_speed = speed;
}

void MySprite::setHp(float hp)
{
	m_hp = hp;
}

float MySprite::getSpeed()
{
	return m_speed;
}

float MySprite::getHp()
{
	return m_hp;
}

vector<BoundRadius>& MySprite::getBoundRadius()
{
	return m_boundRadius;
}

bool MySprite::getDeadState()
{
	return m_deadState;
}

void MySprite::setDeadState(bool state)
{
	m_deadState = state;
}

void MySprite::setDeadType(int type)
{
	m_deadType = type;
}

int MySprite::getDeadType()
{
	return m_deadType;
}
