#ifndef STARTLOADINGLAYER_H
#define STARTLOADINGLAYER_H
#include <iostream>
#include <vector>
#include <string>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class StartLoading : public CCLayer
{
public:
	StartLoading();
	~StartLoading();
	virtual bool init();
	CREATE_FUNC(StartLoading);
	void loadImage(float t);
	void loadMusic();
	void loadingFiles();
	void loadingEnd(float t);
	void loadImageCallBack(CCObject *pSender);
	void loadConfigData();
    void releaseFiles();
private:
	CCProgressTimer *m_loadProgress;
	vector<string> m_vImageFiles;
	int m_loadedCount;
};
#endif