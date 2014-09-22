#include "MapEventsNode.h"
#include "../GLayer/GameLayer.h"
#include "../ToolFun/ToolsFun.h"
static pthread_t gReadEvents;
static CCTMXTiledMap *m_tmxTiledMap  = NULL;
static vector<LevelEvent*> m_levelEvents;
static float m_floatMaxPosY = 0;
static MapEventsNode *gMapEventsNode = NULL;

static vector<int> m_monsterIds;
static vector<int> m_tipIds;
static vector<int> m_storyIds;
static vector<int> m_itemIds;
MapEventsNode::MapEventsNode(): m_speed(90.0f)
	, m_eventsInex(0)
	, m_mapHt(0)
	, m_mapWi(0)
	, m_startState(false)
	, m_eventParseOver(false)
	, m_pauseMove(true)
{
	LD("MapEventsNode::MapEventsNode()");
	gMapEventsNode = this;
	m_monsterIds.clear();
	m_tipIds.clear();
	m_storyIds.clear();
	m_itemIds.clear();
}

MapEventsNode* MapEventsNode::create(const char *filename)
{
	MapEventsNode *mapNode = new MapEventsNode();

	if(mapNode && mapNode->initWithFile(filename))
	{
		return mapNode;
	}

	delete mapNode;
	return NULL;
}

bool MapEventsNode::initWithFile(const char *filename)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	float scale = size.height / 800.0f;
	//CCLog("MapEventsNode::initWithFile");
	m_tmxTiledMap = CCTMXTiledMap::create(filename);
	CCSize mapSize = m_tmxTiledMap->getMapSize();
	CCSize tileSize = m_tmxTiledMap->getTileSize();
	m_mapHt = mapSize.height * tileSize.height;
	m_mapWi = mapSize.width * tileSize.width;
	m_mapHt *= scale;
	this->addChild(m_tmxTiledMap, 1);
	m_tmxTiledMap->setPosition(ccp(0, size.height));
	//readEventsByThread();
	//readEvent();
	return true;
}

void MapEventsNode::setMapSpeed(float mapSpeed)
{
	m_speed = mapSpeed;
}

void MapEventsNode::resumeMapAction()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	float scale = size.height / 800.0f;
	m_tmxTiledMap->stopActionByTag(11123);
	m_pauseMove = false; 
	float time1 = m_mapHt/(m_speed);/*地图移动完所需时间*/
	CCMoveTo *moveTo = CCMoveTo::create(time1 * 1.0f/scale, ccp(0,-m_mapHt*1.0f/scale));
	moveTo->setTag(11123);
	m_tmxTiledMap->runAction(moveTo);
}

void MapEventsNode::pauseMapAction()
{
	m_tmxTiledMap->stopActionByTag(11123);
	//m_tmxTiledMap->pauseSchedulerAndActions();
	m_pauseMove = true;
	//m_mapNode->rem
}

void MapEventsNode::resetMapSpeed(float mapSpeed)
{
	pauseMapAction();
	m_speed = mapSpeed;
	resumeMapAction();
}

int MapEventsNode::getMapPositionY()
{
	//int posY =  m_mapNode->getPosition().y;
	return m_tmxTiledMap->getPosition().y;
}

int MapEventsNode::getMapPositionX()
{
	return m_tmxTiledMap->getPosition().x;
}

bool ComparePosByY(CCDictionary*  firstDic, CCDictionary*  secondDic)
{
	string sKey = "y";

	int y1  = ((CCString*)firstDic->objectForKey(sKey))->intValue();
	int y2 = ((CCString*)secondDic->objectForKey(sKey))->intValue();
	sKey = "x";
	int x1 = ((CCString*)(firstDic->objectForKey(sKey)))->intValue();
	int x2 = ((CCString*)(secondDic->objectForKey(sKey)))->intValue();

	if(y1 < y2)
	{
		return true;
	}
	else if(y1 == y2 && x1 < x2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ComparePosByX(CCDictionary*  firstDic, CCDictionary*  secondDic)
{
	string sKey = "y";
	int y1  = ((CCString*)firstDic->objectForKey(sKey))->intValue();
	int y2 = ((CCString*)secondDic->objectForKey(sKey))->intValue();
	sKey = "x";
	int x1 = ((CCString*)(firstDic->objectForKey(sKey)))->intValue();
	int x2 = ((CCString*)(secondDic->objectForKey(sKey)))->intValue();

	if(x1 < x2)
	{
		return true;
	}
	else if(x1 == x2 && y1 < y2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MapEventsNode::readEventsByThread()
{
	pthread_create(&gReadEvents, NULL, readInThread, NULL);
	pthread_join(gReadEvents, NULL);
}

void MapEventsNode::readEvent()
{
	CCTMXObjectGroup *eventGroup = m_tmxTiledMap->objectGroupNamed("object1");
	CCArray *eventObjects = eventGroup->getObjects();
	CCDictionary* dict;
	list<CCDictionary*> listMapEvent;

	for(int i = 0;  i != eventObjects->count(); i++)
	{
		dict = (CCDictionary*)eventObjects->objectAtIndex(i);

		if(!dict)
		{
			break;
		}

		listMapEvent.push_back(dict);
	}

	listMapEvent.sort(ComparePosByY);

	for(list<CCDictionary*>::iterator iter = listMapEvent.begin() ; iter != listMapEvent.end(); iter++)
	{
		readEventForDic(*iter);
	}
}

void * MapEventsNode::readInThread(void *r)
{
	readEvent();
	return NULL;
}

void MapEventsNode::readEventForDic(CCDictionary *pEvent)
{
	CCArray*  eventKeys = pEvent->allKeys();
	LevelEvent  *event = new LevelEvent;
	event->posX = ((CCString*)pEvent->objectForKey("x"))->intValue();
	event->posY = ((CCString*)pEvent->objectForKey("y"))->intValue();
	//LD("event->posX = %d",event->posX);
	//LD("event->posY= %d",event->posY);
	event->eventType = ((CCString*)pEvent->objectForKey("type"))->intValue();
	m_floatMaxPosY = event->posY;
	event->eventName = string(pEvent->valueForKey("name")->getCString());
	//LD("event->eventName = %s",event->eventName.c_str());
	vector<KEY_VALUE>  keyValues;

	//LD("arrayCount = %d",eventKeys->count());
	for(int i = 0;  i != eventKeys->count(); ++i)
	{
		//LD("i = %d",i);
		KEY_VALUE keyValue;
		keyValue.keyName = ((CCString*)eventKeys->objectAtIndex(i))->getCString();
		keyValue.keyValue = ((CCString*)pEvent->objectForKey(((CCString*)eventKeys->objectAtIndex(i))->getCString()))->getCString();
		keyValues.push_back(keyValue);
		event->keyValues = keyValues;
	}

	if(event->eventName == "cE")
	{
		if(event->eventType >= 0)
		{
			m_monsterIds.push_back(event->eventType);
		}
		else
		{
			//LD("cE event error!");
		}
	}
	else if(event->eventName == "cI")
	{
		if(event->eventType >= 0)
		{
			m_itemIds.push_back(event->eventType);
		}
		else
		{
			//LD("cE event error!");
		}
	}
	else if(event->eventName == "addTip")
	{
		CCString* valueString = (CCString*)pEvent->objectForKey("tipId");

		if(valueString)
		{
			int mId = valueString->intValue();
			m_tipIds.push_back(mId);
		}
		else
		{
			//LD("addTip event error!");
		}
	}
	else if(event->eventName == "addStory")
	{
		CCString* valueString = (CCString*)pEvent->objectForKey("storyId");

		if(valueString)
		{
			int mId = valueString->intValue();
			m_storyIds.push_back(mId);
		}
		else
		{
			//LD("addStory event error!");
		}
	}

	m_levelEvents.push_back(event);
}

LevelEvent *MapEventsNode::getIsAbledEvent()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	if(!(m_eventsInex  < (int)m_levelEvents.size()))
	{
		m_eventParseOver = true;
		return NULL;
	}

	LevelEvent *event = m_levelEvents[m_eventsInex];
	int y = event->posY;

	if(y + getMapPositionY() < winSize.height)
	{
		m_eventsInex++;
		return event;
	}

	return NULL;
}

MapEventsNode::~MapEventsNode()
{
	LD("MapEventsNode::~MapEventsNode()");
	unschedule(schedule_selector(MapEventsNode::updateEvent));

	for(vector<LevelEvent*>::iterator iter = m_levelEvents.begin(); iter != m_levelEvents.end(); ++iter)
	{
		if(*iter)
		{
			delete *iter;
			*iter = NULL;
		}
	}

	m_levelEvents.clear();
	m_tmxTiledMap = NULL;
	gMapEventsNode = NULL;
}

float MapEventsNode::getMovePercent()
{
	float percentage = (CCDirector::sharedDirector()->getWinSize().height - m_tmxTiledMap->getPositionY()) / (m_floatMaxPosY);
	return (percentage > 1.0) ? 1.0 : percentage;
}

bool MapEventsNode::getMoveStarted()
{
	return m_startState;
}

bool MapEventsNode::getParseOver()
{
	return m_eventParseOver;
}

bool MapEventsNode::getPauseState()
{
	return m_pauseMove;
}

MapEventsNode* MapEventsNode::sharedMapEventsNode()
{
	return gMapEventsNode;
}

void MapEventsNode::freeMapEventsNode()
{
	if(gMapEventsNode)
	{
		delete gMapEventsNode;
		gMapEventsNode = NULL;
	}
}

void MapEventsNode::startMove()
{
	resumeMapAction();
	schedule(schedule_selector(MapEventsNode::updateEvent));
}

void MapEventsNode::updateEvent(float t)
{
	LevelEvent *levelEvent = getIsAbledEvent();

	if(!getPauseState() && levelEvent)
	{
		GameLayer::sharedGameLayer()->parseMapEvent(levelEvent);
	}
}

void MapEventsNode::retryMapEvent()
{
	this->stopAllActions();
	unschedule(schedule_selector(MapEventsNode::updateEvent));
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	m_tmxTiledMap->setPosition(ccp(0, size.height));
	m_eventsInex = 0;
	m_pauseMove = true;
	m_eventParseOver = false;
}


void MapEventsNode::removeMapEventNode()
{
	CCNode::removeFromParent();
	MapEventsNode::freeMapEventsNode();
}



vector<int> MapEventsNode::getMonsterIds()
{
	return m_monsterIds;
}

vector<int> MapEventsNode::getTipIds()
{
	return m_tipIds;;
}

vector<int> MapEventsNode::getStoryIds()
{
	return m_storyIds;
}

vector<int> MapEventsNode::getItemIds()
{
	return m_itemIds;
}