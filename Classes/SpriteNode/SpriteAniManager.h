#ifndef SPRITEEFFECTANIMANAGEER_H
#define SPRITEEFFECTANIMANAGEER_H
#include <iostream>
#include <list>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
struct MyAnimation
{
	CCAnimation *animation;
	bool isRelease;
	MyAnimation()
	{
		animation = NULL;
		isRelease = false;
	}
	~MyAnimation()
	{
		CC_SAFE_RELEASE(animation);
	}


};
class SpriteAniManager
{
public:
	SpriteAniManager();
	~SpriteAniManager();

	static SpriteAniManager* getAniManager();
	CCAnimation* addSpriteEffectAniBypList(string spriteFrameName, int frameCount, float t, bool isInside = false);
	CCAnimation* addSpriteEffectAniByFiles(string spriteFilename, int frameCount, float t, bool isInside = false);
	CCAnimation *getSpriteAnimationByName(string firstFrameName);
	static void releaseAniManager();
	void releaseAniByName(string firstFrameName);
	void releaseAllAni();
	void releaseAllByInstande(bool inside);
private:
	static SpriteAniManager* m_aniManager;
	map<string, MyAnimation*> m_effectAniMap;
};
#endif