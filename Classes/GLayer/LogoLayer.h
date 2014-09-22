#ifndef LOGOSCENE_H
#define LOGOSCENE_H
#include "cocos2d.h"
using namespace cocos2d;

class LogoLayer : public CCLayer
{
public:
	LogoLayer();
	~LogoLayer();
	static CCScene* scene();
	CREATE_FUNC(LogoLayer);
	virtual bool init();
	void logoEnd(float t);
	void loadingMiddle(float t);
private:
	CCSprite* m_logo;
};

#endif