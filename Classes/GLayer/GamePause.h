#ifndef GAMEPAUSE_H
#define GAMEPAUSE_H
#include "cocos2d.h"

using namespace cocos2d;

class GameLayer;
class GamePause : public CCLayer
{
public:
	GamePause();
	~GamePause();
	CREATE_FUNC(GamePause);
	virtual bool init();
	void menuCall(CCObject *pSender);
	void resume(CCObject *pSender);
	void back(CCObject *pSender);
	void retryGame(CCObject *pSender);
private:
	GameLayer *m_layer;
	CCMenu *m_menu;	
};
#endif