#include "ToolsFun.h"
#include "../GCData/GameConfigData.h"
#if (CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32)
#include <windows.h>
#endif



static size_t writehtml(uint8_t* ptr, size_t size, size_t number, void *stream);
bool gTeachingType = false;
int gMapId = -1;
int CURRENT_LAYER = 0;
float PI = 3.1415926f;
float gScaleX = 1.0f;
float gScaleY = 1.0f;

const char* gMusicBkgName[BKG_END] =
{
	"music/login.ogg",
	"music/battle.ogg",


//	"music/login.mp3",
	//"music/battle.mp3",
};

const char* gMusicEffectName[EFFECT_END] =
{


	"music/Complete.ogg",
	"music/Failed.ogg",
	"music/button.ogg",
	"music/coin.ogg",
	"music/Dead1.ogg",
	"music/Dead2.ogg",
	"music/Dead3.ogg",
	"music/Dead4.ogg",
	"music/Dead5.ogg",
	"music/unbeatable.ogg",
	"music/slide.ogg",
	"music/sprint.ogg",
	"music/Dead1.ogg",
	"music/Under_attack1.ogg",
	"music/Under_attack2.ogg",
	"music/shoot.ogg",
	"music/attack1.ogg",
	"music/warning.ogg",
	"music/spinEffect.ogg",
	"music/lihua.ogg" 

/*
	"music/Complete.mp3",
	"music/Failed.mp3",
	"music/button.mp3",
	"music/coin.mp3",
	"music/Dead1.mp3",
	"music/Dead2.mp3",
	"music/Dead3.mp3",
	"music/Dead4.mp3",
	"music/Dead5.mp3",
	"music/unbeatable.mp3",
	"music/slide.mp3",
	"music/sprint.mp3",
	"music/Dead1.mp3",
	"music/Under_attack1.mp3",
	"music/Under_attack2.mp3",
	"music/shoot.mp3",
	"music/attack1.mp3",
	"music/warning.mp3",
	"music/spinEffect.mp3",
	"music/lihua.mp3"*/
};

ToolsFun * ToolsFun::m_singleObject = NULL;
ToolsFun::ToolsFun()
{
	m_mySceneSize = ccp(480, 800); //CCDirector::sharedDirector()->getWinSize();
}

ToolsFun:: ~ToolsFun()
{
}

ToolsFun *ToolsFun::sharedTools()
{
	if(!m_singleObject)
	{
		m_singleObject = new ToolsFun();
	}

	return m_singleObject;
}

void ToolsFun::releaseTools()
{
	if(m_singleObject)
	{
		delete m_singleObject;
		m_singleObject = NULL;
	}
}

CCSize ToolsFun::getSceneSize()
{
	return m_mySceneSize;
}

bool ToolsFun::floatEquals(float f1, float f2)
{
	if(fabs(f1 - f2) > -1e-5 && fabs(f1 - f2) < 1e-5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float ToolsFun::piToAngle360(float pi)
{
	pi = pi * 180 / PI;
	return pi;
}

void ToolsFun::angle360ToPi(float angle360)
{

}

string ToolsFun::utf8ToGbk(const char *strUTF8)
{
#if (CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32)
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
#else
	return string(strUTF8);
#endif
}

string ToolsFun::gbkToUtf8(const char *gbkStr)
{
#if (CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32)
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, gbkStr, -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, gbkStr, -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
#else
	return string(gbkStr);
#endif
}

string ToolsFun::getLanguageValue(SK::StringKey key)
{
	string value = "defaultTest";

	if(string(LANGUE_VERSON) == "CHINES")
	{
		if(m_mapChinesString.find(gLangueValueKey[key]) != m_mapChinesString.end())
		{
			value = m_mapChinesString.find(gLangueValueKey[key])->second.c_str();
		}
	}
	else
	{
		if(m_mapEnglishString.find(gLangueValueKey[key]) != m_mapEnglishString.end())
		{
			value = m_mapEnglishString.find(gLangueValueKey[key])->second.c_str();
		}
		else
		{
			if(m_mapChinesString.find(gLangueValueKey[key]) != m_mapChinesString.end())
			{
				value = m_mapChinesString.find(gLangueValueKey[key])->second.c_str();
			}
		}
	}

	return value;
}

string ToolsFun::getLanguageValueByString(string skey)
{
	string value = "defaultTest";

	if(string(LANGUE_VERSON) == "CHINES")
	{
		if(m_mapChinesString.find(skey) != m_mapChinesString.end())
		{
			value = m_mapChinesString.find(skey)->second.c_str();
		}
	}
	else
	{
		if(m_mapEnglishString.find(skey) != m_mapEnglishString.end())
		{
			value = m_mapEnglishString.find(skey)->second.c_str();
		}
		else
		{
			if(m_mapChinesString.find(skey) != m_mapChinesString.end())
			{
				value = m_mapChinesString.find(skey)->second.c_str();
			}
		}
	}

	return value;
}

void ToolsFun::readLangueValue()
{
	//writeChinesValue();
	readChinesValue();
	readEnglishValue();
	readXingMingValues();
}

void ToolsFun::readChinesValue()
{
	/*补充一下，tinyXml2更适合用于手机应用的开发，速度更快 ，更节省内存，所以更建议大家采用tinyXml2，
	新版的cocos2d-x已经集成了tinyxml2.h 与.cpp*/
	string documentPath =  "languageXml/langueChineseValue.xml";
	tinyxml2::XMLDocument *myDocument = new tinyxml2::XMLDocument();//"WriteTest.xml");
	unsigned long nSize;
	const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(documentPath.c_str(), "rb", &nSize);

	if(NULL == pXmlBuffer)
	{
		CCLOG("can not read xml file");
		return;
	}

	myDocument->Parse(pXmlBuffer, nSize);
	delete[] pXmlBuffer;
	tinyxml2::XMLElement *RootElement = myDocument->RootElement();/*读取根节点*/

	if(!RootElement)
	{
		return;
	}

	tinyxml2::XMLElement *FirstPerson = RootElement->FirstChildElement();/*读取根节点中的第*/

	while(FirstPerson)
	{
		string childName = string(FirstPerson->Value());
		//LD("kNameCHS  %s",childName.c_str());
		if (FirstPerson->GetText() == NULL)
		{
			//FirstPerson = FirstPerson->NextSiblingElement();
			m_mapChinesString.insert(map<string, string>::value_type(FirstPerson->Value(), "default")); //FirstPerson->GetText()));
		}
		else
		{
			m_mapChinesString.insert(map<string, string>::value_type(FirstPerson->Value(), FirstPerson->GetText()));
		}

		FirstPerson = FirstPerson->NextSiblingElement();
	}
}

void ToolsFun::readEnglishValue()
{
	string documentPath =  "languageXml/langueEnglishValue.xml";
	tinyxml2::XMLDocument *myDocument = new tinyxml2::XMLDocument();//"WriteTest.xml");
	unsigned long nSize;
	const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(documentPath.c_str(), "rb", &nSize);

	if(NULL == pXmlBuffer)
	{
		CCLOG("can not read xml file");
		return;
	}

	myDocument->Parse(pXmlBuffer, nSize);
	delete[] pXmlBuffer;
	tinyxml2::XMLElement *RootElement = myDocument->RootElement();/*读取根节点*/

	if(!RootElement)
	{
		return;
	}

	tinyxml2::XMLElement *FirstPerson = RootElement->FirstChildElement();/*读取根节点中的第*/

	while(FirstPerson)
	{
		string childName = string(FirstPerson->Value());
		//LD("kNameENG  %s",childName.c_str());
		if (FirstPerson->GetText() == NULL)
		{
			//FirstPerson = FirstPerson->NextSiblingElement();
			m_mapEnglishString.insert(map<string, string>::value_type(FirstPerson->Value(), "default")); //FirstPerson->GetText()));
		}
		else
		{
			m_mapEnglishString.insert(map<string, string>::value_type(FirstPerson->Value(), FirstPerson->GetText()));
		}

		FirstPerson = FirstPerson->NextSiblingElement();
	}
}

string gLgeValue(SK::StringKey key)
{
	return (ToolsFun::sharedTools()->getLanguageValue(key));
}

struct tm* getTime()
{
	struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	time_t timep;
	time(&timep);
	tm = localtime(&timep);
#else
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	tm = localtime(&now.tv_sec);
#endif
	return tm;
}

float ToolsFun::getAngleByPos(CCPoint pos)
{
	float angle = pos.getAngle();

	if((pos.x > 0 || floatEquals(pos.x, 0))  && pos.y > 0)
	{
		angle = angle;
	}
	else if(pos.x < 0 && (floatEquals(pos.y, 0) || pos.y < 0))
	{
		angle = angle;
	}
	else if(pos.x < 0  && (floatEquals(pos.y, 0) && pos.y > 0))
	{
		angle = PI + angle;
	}
	else if((pos.x < 0 || floatEquals(pos.x, 0)) && pos.y < 0)
	{
		/*if (pos.x < 0 && (pos.y < 0 || floatEquals(pos.y,0)))*/
		angle = -PI - angle;
	}

	angle = angle * 180 / PI;
	return angle;
}


string gGetLanguageDesc(SK::StringKey key)
{
	return (ToolsFun::sharedTools()->getLanguageValue(key));
}


void ToolsFun::gotoscene(CCLayer *layer)
{
	CCScene *scene = CCScene::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->replaceScene(scene);

}

string ToolsFun::intToString(int i)
{
	char str[48];
	sprintf(str, "%d", i);
	return string(str);
}

int ToolsFun::stringToInt(string str)
{
	int i = atoi(str.c_str());
	return i;
}

string ToolsFun::getSystemData()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	struct tm *tm;
	tm = localtime((time_t*)&now.tv_sec);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
#else
	struct tm *tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int minute = tm->tm_min;
	int second = tm->tm_sec;
#endif
	char data[33];
	sprintf(data, "%d", day);
	return string(data);
}

string ToolsFun::getSystemMonth()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	struct tm *tm;
	tm = localtime((time_t*)&now.tv_sec);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
#else
	struct tm *tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int minute = tm->tm_min;
	int second = tm->tm_sec;
#endif
	char data[39];
	sprintf(data, "%d", month);
	return string(data);
}


void ToolsFun::initMusicAndEffectEnable()
{
	m_effectEnable = GameConfigData::initialGameData().getCurSoundEffectState();
	m_bkgEnable = GameConfigData::initialGameData().getCurMusicState();
	setEffectEnable(m_effectEnable);
	setMusicEnable(m_bkgEnable);
}

bool ToolsFun::getEffectEnable()
{
	return m_effectEnable;
}

void ToolsFun::setEffectEnable(bool sE)
{
	m_effectEnable = sE;

	if(sE)
	{
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
		//SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}
}

bool ToolsFun::getMusicBkgEnable()
{
	return m_bkgEnable;
}
void ToolsFun::setMusicEnable(bool mE)
{
	m_bkgEnable = mE;

	if(mE)
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void ToolsFun::readXmlData()
{
	readLangueValue();
}

void ToolsFun::readXingMingValues()
{
	string documentPath =  "languageXml/nameConfig.xml";
	tinyxml2::XMLDocument *myDocument = new tinyxml2::XMLDocument();//"WriteTest.xml");
	unsigned long nSize;
	const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(documentPath.c_str(), "rb", &nSize);

	if(NULL == pXmlBuffer)
	{
		CCLOG("can not read xml file");
		return;
	}

	myDocument->Parse(pXmlBuffer, nSize);
	delete[] pXmlBuffer;
	tinyxml2::XMLElement *RootElement = myDocument->RootElement();/*读取根节点*/

	if(!RootElement)
	{
		return;
	}

	tinyxml2::XMLElement *FirstPerson = RootElement->FirstChildElement();/*读取根节点中的第*/

	while(FirstPerson)
	{
		string textName = FirstPerson->Name();
		tinyxml2::XMLElement *fChild = FirstPerson->FirstChildElement();

		while(fChild)
		{
			string childName = string(fChild->Value());

			//LD("kName  %s",childName.c_str());
			if(fChild->GetText() != NULL)
			{
				string text = fChild->GetText();
				//LD("text = %s",text.c_str());
				if (textName == "xing")
				{
					m_xings.push_back(fChild->GetText());
				}
				else
				{
					m_mings.push_back(fChild->GetText());
				}
			}

			fChild = fChild->NextSiblingElement();
		}

		FirstPerson = FirstPerson->NextSiblingElement();
	}

	delete myDocument;
}

void ToolsFun::preLoadMusicAndEffect()
{
	for(int i = 0; i != EFFECT_END; ++i)
	{
		SimpleAudioEngine::sharedEngine()->preloadEffect(gMusicEffectName[i]);
	}

	for(int i = 0; i != BKG_END; ++i)
	{
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(gMusicBkgName[i]);
	}
}

string ToolsFun::getRandXing()
{
	int index = rand() % (int)m_xings.size();
	return m_xings[index];
}

string ToolsFun::getRandMing()
{
	int index = rand() % (int)m_mings.size();
	return m_mings[index];
}

void ToolsFun::playEffect(EFFECT_NAME name, bool loop)
{
	if(m_effectEnable)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(gMusicEffectName[name],loop);
	}
}

void ToolsFun::playBkgMusic(BKG_MUSIC name, bool loop)
{
	if(m_bkgEnable)
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(gMusicBkgName[name], loop);
	}
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())    //判断当前是否播放背景音乐  
	{  
		LD("background music is playing");  
	}  
	else  
	{  
		LD("background music is not playing");  
	}  
}