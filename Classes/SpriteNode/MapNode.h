#ifndef MAPNODE_H
#define MAPNODE_H

#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "../ToolFun/ToolsFun.h"
using namespace std;
using namespace cocos2d;

class MonsterSprite;
class HeroSprite;
class ItemSprite;
class MapEvents;
struct LevelEvent;
struct BkgNode
{
	CCNode *bkgNode;
	int speed;
	int initSpeed;
	BkgNode(): bkgNode(NULL), speed(0), initSpeed(0)
	{

	}
	~BkgNode()
	{
		bkgNode = NULL;
		LD("~BkgNode()");
	}
};

class MapNode : public CCNode
{
public:
	MapNode(int mapId);
	virtual ~MapNode();
	static MapNode * create(int mapId);
	virtual bool init();
	virtual void updateBkgPos(float t);
	void setBkgSpeed(float speed);
	void moveInit();
	void startMove();

	void mapSpeedUp();
	void mapSpeedEnd();

	void pauseMove();
	void resumeMove();
private:
	vector<BkgNode*> m_bkgs;
	int m_bkgId;
	bool m_pauseMove;
};
#endif