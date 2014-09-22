#include "JniCall.h"
#include "ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "HttpClientNode.h"
#include "../NDKHelper/NDKHelper.h"

JniCall* JniCall::m_jniCall = NULL;

InPurchaseData gInPurchaseData[34] =
{
	/*
	内置购买的下标字符，
	价格，
	奖励值，
	描述
	InPurchaseData(string sItem,string sPrice,string rValue,string iDes):itemCode(sItem)
	,priceValue(sPrice)
	,rewardValue(rValue)
	,itemDes(iDes)
	*/

	/*  1-6 商城界面购买
	    7-8 购买人物
	    9 购买金币
	    10-12  连抽购买
	    13 每日礼包
	    14  复活
	    15 激活
		16 无敌
	*/

	InPurchaseData("0","6","30000843028001"), //主角萌妹子
	InPurchaseData("1","8","30000843028002"), //主角狂战士
	InPurchaseData("2","6","30000843028003"), //宠物烈焰勇士 
	InPurchaseData("3","8","30000843028004"), //炽热精灵 
	InPurchaseData("4","4","30000843028005"), //十连抽 

	InPurchaseData("5","6","30000843028006"),//	二十连抽
	InPurchaseData("6","8","30000843028007"),//三十连抽 		
	InPurchaseData("7","6","30000843028008"),//	复活 
	InPurchaseData("8","12","30000843028009"),//一键满级
	InPurchaseData("9","4","30000843028010"),//	每日礼包
	InPurchaseData("10","6","30000843028011"),//正版激活 
	InPurchaseData("11","8","30000843028012"),//八星八钻礼包
	InPurchaseData("12","12","30000843028013"),//钻石金币包 
	InPurchaseData("13","8","30000843028014"),//大型金币包 
	InPurchaseData("14","6","30000843028015"),//无敌礼包 
};
JniCall::JniCall(): m_pTarget(NULL)
	, m_fun(NULL)
	, m_FailedpTarget(NULL)
	, m_Failedfun(NULL)
{
	NDKHelper::AddSelector("JniCall", "purchaseSuccess", callfuncND_selector(JniCall::purchaseSuccess), this);
	NDKHelper::AddSelector("JniCallFaile", "purchaseFailed", callfuncND_selector(JniCall::purchaseFailed), this);
}

JniCall::~JniCall()
{
	NDKHelper::RemoveSelectorsInGroup("JniCall");
	NDKHelper::RemoveSelectorsInGroup("JniCallFaile");
}

JniCall* JniCall::sharedJniCall()
{
	if(!m_jniCall)
	{
		m_jniCall = new JniCall();
		m_jniCall->addSelector();
	}

	return m_jniCall;
}

void  JniCall::release()
{
	if(m_jniCall)
	{
		delete m_jniCall;
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
	//"com/joymetec/llqnr/llqn
	void Java_com_joymetec_llqnr_llqn_nativeCloseApp(JNIEnv*  env, jobject thiz)
	{
		LD(" Game exit!!!");
		CCDirector::sharedDirector()->end();
	}
}
#endif

void JniCall::exit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "quitGame", "()V"))
	{
		LD("JniCall::exit");
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}

#endif
}

string JniCall::getSystemTime()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getSystemDate", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

string JniCall::getDeviceInf()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getDeviceInf", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

string JniCall::getSystemVersion()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getSystemVersion", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}
string JniCall::getGameVersionCode()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getGameVersionCode", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

string JniCall::getPhone()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getPhone", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

string JniCall::getCorp()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getCorp", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

void JniCall::purchaseByIndex(int index)
{
	InPurchaseData data = gInPurchaseData[index];
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

	CCDictionary* dic = CCDictionary::create();
	CCString* itemCode = CCString::create(data.itemIndex);
	CCString *priceValue = CCString::create(data.priceValue);
CCString *payAlias = CCString::create(data.rewardValue);
	dic->setObject(itemCode, "itemCode");
	dic->setObject(priceValue, "price");
	dic->setObject(payAlias,"payAlias");
	SendMessageWithParams("purchase", dic);
#else
	CCDictionary *dic = CCDictionary::create();
	dic->setObject(CCString::createWithFormat("%d", atoi(data.itemIndex.c_str())), "itemCode");
	dic->setObject(CCString::create(data.priceValue), "price");
	purchaseSuccess(NULL, dic);
#endif

}

string JniCall::getLocation()
{
	string ret = "pc";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if(JniHelper::getStaticMethodInfo(t, PACKAGE_PATH, "getLocation", "()Ljava/lang/String;"))
	{
		jstring jStrRet = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ret = JniHelper::jstring2string(jStrRet);
		t.env->DeleteLocalRef(jStrRet);
	}

#endif
	return ret;
}

void JniCall::purchaseSuccess(CCNode* node, void *data)
{
	CCDictionary *dic = (CCDictionary *)data;
	CCString* s = (CCString*)dic->objectForKey("itemCode");
	CCString *price = (CCString *)dic->objectForKey("price");

	int curcoin = GameConfigData::initialGameData().getCoinValue();
	int currevive =  GameConfigData::initialGameData().getReviveValue();
	int curbomb = GameConfigData::initialGameData().getBombValue();

	int itemCode = atoi(s->getCString());

	switch(itemCode)
	{
		case 0:
			{
				
			}
			break;
		case 1:
			{
				
			}
			break;

		case 2:
			{
				
			}
			break;
		case 3:
			{
				
			}
			break;
		case 4:
			{
				
			}
			break;
		case 5:
			{
				
			}
			break;
		case 6:
			{
				
			}
			break;
		case 7:
			{

			}
			break;

		case 8:
			{

			}
			break;

		case 9:
			{

			}
			break;
		case 10:
			{

			}
			break;

		case 11:
			{
				curcoin+=30000;
				currevive+=3;
			}
			break;
		case 12:
			{
				curcoin+=100000;
			}
			break;

		case 13:
			{
				curcoin+=50000;
			}
			break;
		case 14:
			{
				currevive+=3;
			}
			break;

		default:
			break;
	}

	GameConfigData::initialGameData().setCoinValue(curcoin);
	GameConfigData::initialGameData().setBombValue(curbomb);
	GameConfigData::initialGameData().setReviveValue(currevive);

	if(itemCode != -1)
	{
		char buf[32];
		sprintf(buf, "%d", itemCode );

		char priceBuf[32];
		sprintf(priceBuf, "%s", price->getCString());

		HttpClientNode::sharedHttpClient()->eventSend(27, buf, "", priceBuf);

		//  CCLog("%s\t%s\t%s\n",__FILE__,s->getCString(),price->getCString());

		if(m_pTarget)
		{
			(m_pTarget->*m_fun)(s->getCString());
			m_pTarget = NULL;
			m_fun = NULL;
			m_FailedpTarget = NULL;
			m_Failedfun = NULL;
		}
	}
}

void JniCall::purchaseFailed(CCNode* node, void *data)
{

	CCDictionary *dic = (CCDictionary *)data;
	CCString* s = (CCString*)dic->objectForKey("itemCode");
	CCString *price = (CCString *)dic->objectForKey("price");

	// CCLog("%s\t%s\t%s\n",__FILE__,s->getCString(),price->getCString());

	if(m_FailedpTarget)
	{
		(m_FailedpTarget->*m_Failedfun)(s->getCString());
		m_pTarget = NULL;
		m_fun = NULL;
		m_FailedpTarget = NULL;
		m_Failedfun = NULL;
	}
}


void JniCall::setBuyCompleteCallBack(CCObject *pObject, SEL_CallFunStr callFun)
{
	m_pTarget = pObject;
	m_fun = callFun;
}

void JniCall::setBuyFailedCallBack(CCObject *pObject, SEL_CallFunStr callFun)
{
	m_FailedpTarget = pObject;
	m_Failedfun = callFun;
}

void JniCall::purchaseFailed(const char *itemCode)
{
	if(m_FailedpTarget)
	{
		(m_FailedpTarget->*m_Failedfun)(itemCode);
		m_FailedpTarget = NULL;
		m_Failedfun = NULL;
	}
}

void JniCall::addSelector()
{
	NDKHelper::AddSelector("JniCall", "purchaseSuccess", callfuncND_selector(JniCall::purchaseSuccess), this);
}

void JniCall::removeSelector()
{
	NDKHelper::RemoveSelectorsInGroup("JniCall");
}