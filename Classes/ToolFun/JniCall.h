#ifndef __JniCall_H__
#define __JniCall_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//com.joymetec.llqnr
#define PACKAGE_PATH "com/joymetec/llqnr/llqn"
#endif


struct InPurchaseData
{
	string itemIndex;
	string priceValue;
	string rewardValue;
	string itemCodeValue;
	InPurchaseData(string sIndex,string sPrice,string rValue="de",string iValue="de"):itemIndex(sIndex)
		,priceValue(sPrice)
		,rewardValue(rValue)
		,itemCodeValue(iValue)
	{

	}
	InPurchaseData()
	{

	}
};

typedef void (CCObject::*SEL_CallFunStr)(const char* itemCode);

#define callFunStr_selector(_SELECTOR) (SEL_CallFunStr)(&_SELECTOR)

class JniCall : public CCNode
{
public:
	JniCall();
	~JniCall();

	static JniCall* sharedJniCall();
	static void release();
	void exit();
	string getSystemTime();
	void purchaseByIndex(int index);
	void purchaseSuccess(CCNode* node, void *data);
	void purchaseFailed(CCNode* node, void *data);
	void purchaseFailed(const char *itemCode);
	void setBuyCompleteCallBack(CCObject *pObject,SEL_CallFunStr callFun);
	void setBuyFailedCallBack(CCObject *pObject,SEL_CallFunStr callFun);
	void addSelector();
	void removeSelector();
	string getDeviceInf();
	string getSystemVersion();
	string getLocation();
	string getGameVersionCode();
	string getPhone();
	string getCorp();
private:
	CCObject* m_pTarget;
	SEL_CallFunStr m_fun;
	CCObject* m_FailedpTarget;
	SEL_CallFunStr m_Failedfun;
	static JniCall *m_jniCall;
};
extern InPurchaseData gInPurchaseData[34];
#endif