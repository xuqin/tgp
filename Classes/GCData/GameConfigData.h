#ifndef GAMECONFIGDATA_H
#define GAMECONFIGDATA_H
#include <iostream>
#include <vector>
#include <string>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class GameConfigData
{
public:
	static GameConfigData& initialGameData();
	~GameConfigData();

	void readGameConfigData();
	void writeGameConfigData();
	void writeData();
	void readData();

	int getMapBkgId();
	void setCurLevel(int cLevel);
	int getCurLevel();

	void setMaxUnLockLevel();
	int getMaxUnLockLevel();

	void setCurHeroId(int heroId);
	int getCurHeroId();

	
	void setStartCount(int sCout);
	int getStartCout();

	void setLevelState(int levelId,string state);
	vector<string> getLevelState();
	string getLevelStateByLevel(int level);

	void setSubLevelState(int levelId,string state);
	vector<string> getSubLevelState();
	string getSubLevelStateByLevel(int level);

	void setRewardState(int rewardId,string state);
	vector<string> getRewardState();
	string getRewardStateByReward(int rewardId);	
	void setCoinValue(int value);
	int getCoinValue();
	string getCoinValueStr();
	void setDiamondValue(int value);
	int getDiamondValue();
	void setReviveValue(int value);
	int getReviveValue();
	void setBombValue(int value);
	int getBombValue();
	int getLevelStarByLevel(int level);
	void setLevelStar(int levelId,int levelstarnum);
	int getCurSubLevel();
	void setCurSubLevel(int cLevel);

	void setPetState(int petId,string state);
	vector<string> getPetState();
	string getPetStateById(int petId);
	// 1代表做，2代表右
	void setPetPos(int petId,int pos);
	vector<int> getPetPos();
	int getPetPosById(int petId);
	void setpetLevel(int petid ,int cLevel);
	
	int getpetLevelById(int petid);

	string getHScores();
	string getHLenght();
	string getKillTotal();
	void setHScores(int sc);
	void setHLength(int lenght);
	void setKillTotal(int total);
	void setCurHeroLevel(int herolevel);
	int getCurHeroLevel();
	string getHeroStateStr(int heroId);
	int getHeroStateInt(int heroId);
	vector<string> getHeroStates();
	void setHeroStateInt(int heroId ,string state);
	void setcurrentReIndex(int value);
	int getcurrentReIndex();
	int getLuckyCount();
	void setLuckyCount(int value);
	int getUserCount();
	void setUserCount(int value);
	void setlastDate(string value);
	void setlastMonth(string value);
	string getlastDate();
	string getlastMonth();
	void setCurSpeedId(int SpeedId);
	int getCurSpeedId();

	int getCurMusicState();
	void setMusicState(int state);
	int  getDailyPurchase();
	void setDailyPurchase(int isDailyPurchase);
	int getActiveState();
	void setActiveState(int active);
	int getCurSoundEffectState();
	void setCurSoundEffectState(int state);
	//void setKillTotal(int index,int store);
	//int getKillTotal(int index);

private:
	GameConfigData();
	GameConfigData(const GameConfigData& data);
	GameConfigData & operator=(const GameConfigData& data);
	int m_dailyPurchase;
	int m_mapBkgId;
	int m_curLevel;
	int m_leveltype;
	int m_maxUnLockLevel;
	int m_moveSpeed;
	int m_musicState;
	int m_soundEffectState;
	int m_curHeroId;
	int m_curHeroLevel;
	int m_startCount;
	int m_luckyCount;  //抽奖次数
	int m_userCount; //已经抽奖次数
	int m_activeState;
	//add zh
	string m_totalcoin;
	string m_totaldiamond;
	string m_totalRevive;
	string m_totalBomb;
	string lastDate;
	string lastMonth;
	string currentReIndex;

	int m_curSubLevel;
	
	/*bool m_tipvalue;*/

	vector <int> m_levelstar;
	vector <int> m_petlv; 
	vector <int> m_petpos;
	vector<string> m_levelState;
	vector<string> m_sbuLevelState;
	vector<string> m_rewardState;
	vector<string> m_petState;


	string m_heightScores;
	string m_heightLength;
	string m_killTotal;

	vector<string> m_heroStates;
};

#endif