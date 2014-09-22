#ifndef STARTGAMELOADINGLAYER_H
#define STARTGAMELOADINGLAYER_H
#include <iostream>
#include <vector>
#include <string>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class StartGameLoading : public CCLayer
{
public:
	StartGameLoading();
	~StartGameLoading();
	virtual bool init();

	CREATE_FUNC(StartGameLoading);
	void initUI();
	void initBeLoadedFiles();

	void loadImage();
	void loadingFiles();
	void loadingEnd(float t);
	void loadImageCallBack(CCObject *pSender);
	static void releaseImageInGame();
	void initTmx();
	void initTmx1();
	void addLoadPlist();
	//void update(float dt);
private:
	CCProgressTimer *m_loadProgress;
};
#endif