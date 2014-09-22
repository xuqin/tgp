#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <pthread.h>
#else
#include "pthread/pthread.h"
#endif
using namespace std;
using namespace cocos2d;

struct KEY_VALUE
{
	string keyName;
	string keyValue;
};

struct LevelEvent
{
	int posX;
	int posY;
	int eventType;
	string eventName;
	vector<KEY_VALUE>  keyValues;
	//map<string,int> mapKeyValue;
};

class MapEventsNode : public CCNode
{
public:
	MapEventsNode();
	~MapEventsNode();
	static MapEventsNode* create(const char *filename);
	bool initWithFile(const char *filename);
	void readEventsByThread();
	static void * readInThread(void *r);
	static void readEvent();
	static void readEventForDic(CCDictionary *pEvent);
	void resetMapSpeed(float fMapSpeed);
	int getMapPositionY();
	int getMapPositionX();
	void resumeMapAction();
	void pauseMapAction();
	LevelEvent *getIsAbledEvent();
	bool getMoveStarted();
	bool getParseOver();
	bool getPauseState();
	float getMovePercent();
	void  setMapSpeed(float fMapSpeed);
	static MapEventsNode* sharedMapEventsNode();
	static void freeMapEventsNode();
	void startMove();
	void updateEvent(float t);
	void retryMapEvent();

	void removeMapEventNode();
	vector<int> getMonsterIds();
	vector<int> getTipIds();
	vector<int> getStoryIds();
	vector<int> getItemIds();
protected:
	float m_speed;
	int m_eventsInex;
	float m_mapHt;
	float m_mapWi;
	bool m_startState;
	bool m_eventParseOver;
	bool m_pauseMove;

};

#endif
