#ifndef TOOLSFUN_H
#define TOOLSFUN_H
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyLog.h"
#include "StringKey.h"
#include "SimpleAudioEngine.h"
#include "curl/curl.h"
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;
USING_NS_CC_EXT;
//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
#define LANGUE_VERSON  "CHINES"
#define NEED_PAUSE_GAME_TAG_LAYER  1
#define DONOT_NEED_PAUSE 0
extern float PI;
extern bool gTeachingType;
extern string gGetLanguageDesc(SK::StringKey key);
extern struct tm* getTime();
extern int gMapId;
extern int CURRENT_LAYER;
extern float gScaleX;
extern float gScaleY;
#define GOLD_FONT "font/level_big.fnt"


enum BKG_MUSIC
{
	LOGIN,
	BATTLE,
	BKG_END
};
enum EFFECT_NAME
{
	GAME_COMPLETE,/**过关成功**/
	GAME_FAILED,/**失败**/
	BUTTON,
	COIN,
	DEAD_0,
	DEAD_1,
	DEAD_2,
	DEAD_3,
	DEAD_4,
	UNBEATABLE,
	SLIDE,
	SPRINT,
	JUMP,
	UNDER_ATTACK0,
	UNDER_ATTACK1,
	SHOOT,
	ATTACK1,
	GAME_WARNING,
	SPIN_EFFECT,
	LI_HUA,
	EFFECT_END
};


extern const char* gMusicEffectName[EFFECT_END];
extern const char* gMusicBkgName[BKG_END];


class ToolsFun : public CCNode
{
public:
	ToolsFun();
	~ToolsFun();
	static ToolsFun *sharedTools();
	static void releaseTools();
	CCSize getSceneSize();
	bool floatEquals(float f1, float f2);
	float piToAngle360(float pi);
	void angle360ToPi(float angle360);
	string utf8ToGbk(const char *strUTF8);
	string gbkToUtf8(const char *gbkStr);
	string getLanguageValue(SK::StringKey key);
	string getLanguageValueByString(string skey);
	void readLangueValue();
	void readChinesValue();
	void readEnglishValue();
	float getAngleByPos(CCPoint pos);

	string getSystemTime();
	void  gotoscene(CCLayer *layer);
	string intToString(int i);
	int stringToInt(string str);
	string getSystemMonth();
	string getSystemData();
	//add zh
	void initMusicAndEffectEnable();
	void preLoadMusicAndEffect();
	void playEffect(EFFECT_NAME name, bool loop = false);
	void playBkgMusic(BKG_MUSIC name, bool loop = true);
	bool getEffectEnable();
	void setEffectEnable(bool sE);
	bool getMusicBkgEnable();
	void setMusicEnable(bool mE);
	void readXmlData();
	void readXingMingValues();
	string getRandXing();
	string getRandMing();

private:
	static ToolsFun *m_singleObject;
	CCSize m_mySceneSize;
	map<string, string> m_mapEnglishString;
	map<string, string> m_mapChinesString;
	bool m_effectEnable;
	bool m_bkgEnable;
	vector<string> m_xings;
	vector<string> m_mings;
};



#endif