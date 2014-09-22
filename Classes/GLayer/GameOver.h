#ifndef GAMEOVER_H
#define GAMEOVER_H
#include "cocos2d.h"

using namespace cocos2d;

class GameLayer;
class GameOver : public CCLayer
{
public:
	GameOver();
	~GameOver();
	CREATE_FUNC(GameOver);
	void menuCall(CCObject *pSender);
	void resume(CCObject *pSender);
	void back(CCObject *pSender);
	void retryGame(CCObject *pSender);
	bool initWin();
	bool initLose();
private:
	GameLayer *m_layer;
	CCMenu *m_menu;
	unsigned int m_MaxStar;
	int m_currentLevel;
};
#endif