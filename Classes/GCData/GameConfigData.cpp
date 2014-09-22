#include "GameConfigData.h"
#include "../ToolFun/ToolsFun.h"
#include "DataInitial.h"

//add zh




GameConfigData& GameConfigData::initialGameData()
{
	static GameConfigData data;
	return data;
}

GameConfigData::~GameConfigData()
{

}

GameConfigData::GameConfigData(): m_mapBkgId(0)
	, m_curLevel(0)
	, m_curSubLevel(0)
	, m_maxUnLockLevel(0)
	, m_moveSpeed(1)
	, m_musicState(1)
	, m_soundEffectState(1)
	, m_curHeroId(0)
	, m_curHeroLevel(1)
	, m_startCount(0)
	, m_petlv(0)
	, m_petpos(0)
	, m_totalcoin("3000")
	, m_totaldiamond("0")
	, m_totalRevive("3")
	, m_totalBomb("3")
	, lastDate("default")
	, lastMonth("default")
	, currentReIndex("0")
	, m_heightScores("0")
	, m_heightLength("0")
	, m_killTotal("0")
	, m_dailyPurchase(0)
	, m_activeState(0)
	, m_luckyCount(3)
	, m_userCount(0)
{

	// 关卡解锁状态
	vector<LevelUnLock*> unLockConfigs  = DataInitial::initialDataSource().getUnLockConfigs();

	for(int i = 0; i < unLockConfigs.size(); i++)
	{
		m_levelState.push_back("0");
		m_levelstar.push_back(0);
	}

	m_levelState[0] = "1";

	//支线关卡解锁状态
	vector<LevelUnLock*> subUnLockConfigs = DataInitial::initialDataSource().getSubUnLockConfigs();

	for(int i = 0; i < subUnLockConfigs.size(); i++)
	{
		m_sbuLevelState.push_back("0");
	}

	//奖励关卡解锁状态

	vector<RewardConfig*> rewardConfigs = DataInitial::initialDataSource().getRewardConfigs();

	for(int i = 0; i < rewardConfigs.size(); i++)
	{
		m_rewardState.push_back("0");
	}

	//宠物解锁状态
	vector<PetData*> petConfigs = DataInitial::initialDataSource().getPetData();

	for(int i = 0; i < petConfigs.size(); i++)
	{
		m_petState.push_back("0");
		m_petlv.push_back(1);
		m_petpos.push_back(-1);
	}

	//m_petState[0] = 1;
	m_petState.at(0) =string("1");


	//英雄解锁
	for(int i = 0; i != 3; ++i)
	{
		m_heroStates.push_back("0");
	}

	m_heroStates[0] = "1";
	//readGameConfigData();
	readData();


}


void GameConfigData::readData()
{

	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "config.xml";

	/* 文件不存在，初始化 */
	if(! CCFileUtils::sharedFileUtils()->isFileExist(path))
	{
		writeData();
		return;
	}



	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* element = doc.RootElement();

	if(!element)
	{
		return;
	}

	element = element->FirstChildElement();
	m_maxUnLockLevel = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_moveSpeed = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_musicState = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_soundEffectState = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_curHeroId = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_curHeroLevel = element->IntAttribute("value");


	element = element->NextSiblingElement();
	m_startCount = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_totalcoin = element->Attribute("value");

	element = element->NextSiblingElement();
	m_totalRevive = element->Attribute("value");

	element = element->NextSiblingElement();
	m_totalBomb = element->Attribute("value");

	element = element->NextSiblingElement();
	lastDate = string(element->Attribute("value"));

	element = element->NextSiblingElement();
	lastMonth = string(element->Attribute("value"));

	element = element->NextSiblingElement();
	currentReIndex = string(element->Attribute("value"));


	element = element->NextSiblingElement();
	m_luckyCount = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_userCount = element->IntAttribute("value");
	//
	vector<LevelUnLock*> unLockConfigs  = DataInitial::initialDataSource().getUnLockConfigs();

	for(int i = 0; i < unLockConfigs.size(); i++)
	{

		element = element->NextSiblingElement();
		m_levelState[i] = element->Attribute("value");  //解锁主线关卡
		m_levelstar[i] = element->IntAttribute("starvalue");
	}

	vector<LevelUnLock*> subUnLockConfigs = DataInitial::initialDataSource().getSubUnLockConfigs();

	for(int i = 0; i < subUnLockConfigs.size(); i++)
	{

		element = element->NextSiblingElement();
		m_sbuLevelState[i] = element->Attribute("value"); //解锁支线关卡

	}

	vector<RewardConfig*> rewardConfigs = DataInitial::initialDataSource().getRewardConfigs();

	for(int i = 0; i < rewardConfigs.size(); i++)
	{

		element = element->NextSiblingElement();
		m_rewardState[i] = element->Attribute("value");

	}

	vector<PetData*> petConfigs = DataInitial::initialDataSource().getPetData();

	for(int i = 0; i < petConfigs.size(); i++)
	{
		element = element->NextSiblingElement();
		m_petState[i] = element->Attribute("value");
		m_petlv[i] = element->IntAttribute("petlv");
		m_petpos[i] = element->IntAttribute("petpos");
	}


	element = element->NextSiblingElement();
	m_heightScores = element->Attribute("value");

	element = element->NextSiblingElement();
	m_heightLength = element->Attribute("value");

	element = element->NextSiblingElement();
	m_killTotal = element->Attribute("value");

	element = element->NextSiblingElement();
	m_dailyPurchase = element->IntAttribute("value");

	element = element->NextSiblingElement();
	m_activeState = element->IntAttribute("value");

	for(int i = 0; i != m_heroStates.size(); ++i)
	{
		element = element->NextSiblingElement();
		m_heroStates[i] = element->Attribute("value");
	}

	//for(int i = 0;i<30;i++)
	//{
	//	element = element->NextSiblingElement();
	//	killTotal[i] = atoi(element->Attribute("value"));
	//}

}


/*
void  GameConfigData::setKillTotal(int index,int store)
{
}

int GameConfigData::getKillTotal(int index)
{
	return 0;
}*/

void GameConfigData::writeData()
{

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration *declare = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	doc.LinkEndChild(declare);

	tinyxml2::XMLElement* rootElement = doc.NewElement("config");
	doc.LinkEndChild(rootElement);

	tinyxml2::XMLElement* element = doc.NewElement("unLockMaxLevel");
	element->SetAttribute("value", m_maxUnLockLevel);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("MoveSpeed");
	element->SetAttribute("value", m_moveSpeed);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("Music");
	element->SetAttribute("value", m_musicState);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("SoundEffect");
	element->SetAttribute("value", m_soundEffectState);
	rootElement->LinkEndChild(element);


	element = doc.NewElement("CurHeroId");
	element->SetAttribute("value", m_curHeroId);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("CurHeroLevel");
	element->SetAttribute("value", m_curHeroLevel);
	rootElement->LinkEndChild(element);


	element = doc.NewElement("startCount");
	element->SetAttribute("value", m_startCount);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("TotalCoin");
	element->SetAttribute("value", m_totalcoin.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("TotalRevive");
	element->SetAttribute("value", m_totalRevive.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("TotalBomb");
	element->SetAttribute("value", m_totalBomb.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("lastDate");
	element->SetAttribute("value", lastDate.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("lastMonth");
	element->SetAttribute("value", lastMonth.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("currentReIndex");
	element->SetAttribute("value", currentReIndex.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("luckyCount");
	element->SetAttribute("value", this->m_luckyCount);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("userCount");
	element->SetAttribute("value", this->m_userCount);
	rootElement->LinkEndChild(element);
	/*初始化主线上每一关卡的状态
	，通过为3
	，满足第二条件解锁为2（第二条件包括，RMB和星星等级人物等级的限制）
	，满足第一条件解锁为1（前一关解锁，这个条件就满足）
	，未解锁为0*/

	vector<LevelUnLock*> unLockConfigs  = DataInitial::initialDataSource().getUnLockConfigs();

	for(int i = 0; i < unLockConfigs.size(); ++i)
	{
		char tmp[50];
		sprintf(tmp, "level%dState", i);
		element = doc.NewElement(tmp);
		element->SetAttribute("value", m_levelState[i].c_str());
		element->SetAttribute("starvalue", m_levelstar[i]);
		rootElement->LinkEndChild(element);
		//  m_levelState.push_back(tempStr);
	}

	/*初始化子线关卡状态，0为未解锁，1为解锁，2为通关*/
	vector<LevelUnLock*> subUnLockConfigs = DataInitial::initialDataSource().getSubUnLockConfigs();

	for(int i = 0; i != subUnLockConfigs.size(); ++i)
	{
		char levelState[33];
		sprintf(levelState, "subLevel%dState", i);
		element = doc.NewElement(levelState);
		element->SetAttribute("value", m_sbuLevelState[i].c_str());
		rootElement->LinkEndChild(element);
		//m_sbuLevelState.push_back("0");
	}

	/*初始化奖励状态，0为未完成，1为完成*/

	vector<RewardConfig*> rewardConfigs = DataInitial::initialDataSource().getRewardConfigs();

	for(int i = 0; i != rewardConfigs.size(); ++i)
	{
		char rewardState[33];
		sprintf(rewardState, "reward%dState", i);
		element = doc.NewElement(rewardState);
		element->SetAttribute("value", m_rewardState[i].c_str());
		rootElement->LinkEndChild(element);
		//m_rewardState.push_back("0");
	}

	//add zh
	/*初始化奖励状态，0为未领养，1为出战，2为休息   petlv为宠物级别*/

	vector<PetData*> petConfigs = DataInitial::initialDataSource().getPetData();

	for(int i = 0; i != petConfigs.size(); ++i)
	{
		char petState[33];
		sprintf(petState, "Pet%dState", i);
		element = doc.NewElement(petState);
		element->SetAttribute("value", m_petState[i].c_str());
		element->SetAttribute("petlv", m_petlv[i]);
		element->SetAttribute("petpos", m_petpos[i]);
		rootElement->LinkEndChild(element);
	}


	element = doc.NewElement("hScores");
	element->SetAttribute("value", m_heightScores.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("hLength");
	element->SetAttribute("value", m_heightLength.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("killTotal");
	element->SetAttribute("value", m_killTotal.c_str());
	rootElement->LinkEndChild(element);

	element = doc.NewElement("dailyPurchase");
	element->SetAttribute("value", this->m_dailyPurchase);
	rootElement->LinkEndChild(element);

	element = doc.NewElement("activeState");
	element->SetAttribute("value", this->m_activeState);
	rootElement->LinkEndChild(element);

	for(int i = 0; i != m_heroStates.size(); ++i)
	{
		char heroState[48];
		sprintf(heroState, "hero%dstate", i);
		element = doc.NewElement(heroState);
		element->SetAttribute("value", m_heroStates[i].c_str());
		rootElement->LinkEndChild(element);
	}

	/*
	for(int i = 0;i<30;i++)
	{
		char buf[48];
		sprintf(buf, "killTotal%d", i);
		element = doc.NewElement(buf);

		char buf1[64];
		sprintf(buf1,"%d",killTotal[i]);
		element->SetAttribute("value",buf1);
		rootElement->LinkEndChild(element);
	} */

	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "config.xml";
	doc.SaveFile(path.c_str());
}

void GameConfigData::writeGameConfigData()
{

}

int GameConfigData::getLuckyCount()
{
	return m_luckyCount;
}

void GameConfigData::setLuckyCount(int value)
{
	m_luckyCount = value;
}

int GameConfigData::getUserCount()
{
	return m_userCount;
}

void GameConfigData::setUserCount(int value)
{
	m_userCount = value;
}
int GameConfigData::getActiveState()
{
	return m_activeState;
}
void GameConfigData::setActiveState(int active)
{
	m_activeState = active;
}
int GameConfigData::getMapBkgId()
{
	m_mapBkgId = m_curLevel / 6 + 1;
	return m_mapBkgId;
}

void GameConfigData::setCurLevel(int cLevel)
{
	if(cLevel >=30)
	{
		m_curLevel = cLevel-1;
	}
	else
	{
		m_curLevel = cLevel;
	}
}

int GameConfigData::getCurLevel()
{
	return m_curLevel;
}

void GameConfigData::setMaxUnLockLevel()
{
	int level = m_maxUnLockLevel + 1;

	if(level < MAXUNLOCKLEVEL)
	{
		m_maxUnLockLevel = level;
	}
}

int GameConfigData::getMaxUnLockLevel()
{
	return m_maxUnLockLevel;
}

void GameConfigData::setCurSpeedId(int SpeedId)
{
	m_moveSpeed = SpeedId;
}

int GameConfigData::getCurMusicState()
{
	return m_musicState;
}

void GameConfigData::setMusicState(int state)
{
	m_musicState = state;
}

int GameConfigData::getCurSoundEffectState()
{
	return m_soundEffectState;
}

void GameConfigData::setCurSoundEffectState(int state)
{
	m_soundEffectState = state;
}


int GameConfigData::getCurSpeedId()
{
	return m_moveSpeed;
}

void GameConfigData::setCurHeroId(int heroId)
{
	m_curHeroId = heroId;
}

int GameConfigData::getCurHeroId()
{
	return m_curHeroId;
}

void GameConfigData::setCurHeroLevel(int herolevel)
{
	m_curHeroLevel = herolevel;
}

int GameConfigData::getCurHeroLevel()
{
	return m_curHeroLevel;
}

string GameConfigData::getHeroStateStr(int heroId)
{
	return m_heroStates[heroId];
}

int GameConfigData::getHeroStateInt(int heroId)
{
	return atoi(m_heroStates[heroId].c_str());
}
void GameConfigData::setHeroStateInt(int heroId , string state)
{
	m_heroStates[heroId] = state;
}

vector<string> GameConfigData::getHeroStates()
{
	return m_heroStates;
}

void GameConfigData::setLevelState(int levelId, string state)
{
	if(levelId >=30)
	{
		m_levelState[levelId-1] = state;
	}
	else
	{
		m_levelState[levelId] = state;
	}
}

// void GameConfigData::setTipValue(bool tipvalue)
// {
//  m_tipvalue = tipvalue;
// }
//
//
// bool GameConfigData::getTipValue()
// {
//  return m_tipvalue;
// }

vector<string> GameConfigData::getLevelState()
{
	return m_levelState;
}

string GameConfigData::getLevelStateByLevel(int level)
{
	if(level >=30)
		return m_levelState[level-1];
		
	return m_levelState[level];
}


void GameConfigData::setStartCount(int sCout)
{
	m_startCount = sCout;
}

int GameConfigData::getStartCout()
{
	int total = 0;

	for(int i = 0; i < DataInitial::initialDataSource().getUnLockConfigs().size(); i++)
	{
		total += m_levelstar[i];
	}

	m_startCount = total;
	return m_startCount;
}

int GameConfigData::getLevelStarByLevel(int level)
{

	return m_levelstar[level];
}
void GameConfigData::setLevelStar(int levelId, int levelstarnum)
{
		if(levelId >=30)
	{
		m_levelstar[levelId-1] = levelstarnum;
	}
	else
	{
		m_levelstar[levelId] = levelstarnum;
	}
}


void GameConfigData::setCoinValue(int value)
{
	char diaStr[33];
	sprintf(diaStr, "%d", value);
	m_totalcoin = string(diaStr);
}

int GameConfigData::getCoinValue()
{
	return atoi(m_totalcoin.c_str());
}

string GameConfigData::getCoinValueStr()
{
	return m_totalcoin;
}

void GameConfigData::setDiamondValue(int value)
{
	char diaStr[33];
	sprintf(diaStr, "%d", value);
	m_totaldiamond = string(diaStr);
}

int GameConfigData::getDiamondValue()
{
	return atoi(m_totaldiamond.c_str());
}

void GameConfigData::setReviveValue(int value)
{
	char diaStr[33];
	sprintf(diaStr, "%d", value);
	m_totalRevive = string(diaStr);
}

int GameConfigData::getReviveValue()
{
	return atoi(m_totalRevive.c_str());
}

void GameConfigData::setBombValue(int value)
{
	char diaStr[33];
	sprintf(diaStr, "%d", value);
	m_totalBomb = string(diaStr);
}

int GameConfigData::getBombValue()
{
	return atoi(m_totalBomb.c_str());
}

void GameConfigData::setcurrentReIndex(int value)
{
	char diaStr[33];
	sprintf(diaStr, "%d", value);
	currentReIndex = string(diaStr);
}

void GameConfigData::setlastDate(string value)
{

	lastDate = value;
}
void GameConfigData::setlastMonth(string value)
{
	lastMonth = value;
}

string GameConfigData::getlastDate()
{

	return lastDate;
}
string GameConfigData::getlastMonth()
{
	return lastMonth;
}

int GameConfigData::getcurrentReIndex()
{
	return atoi(currentReIndex.c_str());
}


void GameConfigData::setCurSubLevel(int cLevel)
{
	m_curSubLevel = cLevel;
}


int GameConfigData::getCurSubLevel()
{
	return m_curSubLevel;
}

void GameConfigData::setSubLevelState(int levelId, string state)
{
	m_sbuLevelState[levelId] = state;
}

vector<string> GameConfigData::getSubLevelState()
{
	return m_sbuLevelState;
}

string GameConfigData::getSubLevelStateByLevel(int level)
{
	return m_sbuLevelState[level];
}

void  GameConfigData::setRewardState(int rewardId, string state)
{
	m_rewardState[rewardId] = state;
}

vector<string>  GameConfigData::getRewardState()
{
	return m_rewardState;
}

string  GameConfigData::getRewardStateByReward(int rewardId)
{
	return m_rewardState[rewardId];
}

void  GameConfigData::setPetState(int petId, string state)
{
	m_petState[petId] = state;
}

vector<string>  GameConfigData::getPetState()
{
	return m_petState;
}

string  GameConfigData::getPetStateById(int petId)
{
	return m_petState[petId];
}

void GameConfigData::setpetLevel(int petid , int cLevel)
{
	m_petlv[petid] = cLevel;
}

int GameConfigData::getpetLevelById(int petid)
{
	return  m_petlv[petid] ;
}


string GameConfigData::getHScores()
{
	return m_heightScores;
}
string GameConfigData::getHLenght()
{
	return m_heightLength;
}

string GameConfigData::getKillTotal()
{
	return m_killTotal;
}

void GameConfigData::setHScores(int sc)
{
	m_heightScores = ToolsFun::sharedTools()->intToString(sc);
}

void GameConfigData::setHLength(int lenght)
{
	m_heightLength = ToolsFun::sharedTools()->intToString(lenght);
}

void GameConfigData::setKillTotal(int total)
{
	m_killTotal = ToolsFun::sharedTools()->intToString(total);
}

void  GameConfigData::setPetPos(int petId, int pos)
{
	m_petpos[petId] = pos;
}

vector<int>  GameConfigData::getPetPos()
{
	return m_petpos;
}

int  GameConfigData::getPetPosById(int petId)
{
	return m_petpos[petId];
}

int GameConfigData::getDailyPurchase()
{
	return m_dailyPurchase;
}

void GameConfigData::setDailyPurchase(int isDailyPurchase)
{
	m_dailyPurchase = isDailyPurchase;
}