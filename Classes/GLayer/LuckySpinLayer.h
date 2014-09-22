#ifndef LuckySpinLayer_h
#define LuckySpinLayer_h

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
using namespace std;

using namespace cocos2d::extension;
struct LuckyCofig
{
	int rewardType;
	string rewartMoney;
	int rewardIndex;
	LuckyCofig():rewardType(0),rewartMoney("0"),rewardIndex(0)
	{
	}
	LuckyCofig(int type,string str):rewardType(type),rewartMoney(str),rewardIndex(0)
	{

	}
};

class LuckySpinLayer : public CCLayer{
public:
	LuckySpinLayer();
	~LuckySpinLayer();
	virtual bool init();


	CREATE_FUNC(LuckySpinLayer);
	void initUI();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void setCallBackTarget(CCObject* target,SEL_CallFunc fun);
	void rotateEndGetReward(CCNode *node);
	void registerNDK();

	void purchaseGameCount();
	void purchaseCountSuc(const char *str);
	void playGainRewardEffect();
	void endAddCoinEffect(float t);
	void addCoin(float t);
	void removeCoin(CCNode *node);
	void randSpriteMove();
	void updateSpinSpritePos(float t);
	CCPoint getPosByIndex(int index);
	void playSoundEffect(CCNode *node);

	void rightMoveEnd(CCNode *node);
	void downMoveEnd(CCNode *node);
	void leftMoveEnd(CCNode *node);
	void upMoveEnd(CCNode *node);
	void addTipSprite();
	void buyCount(CCObject *pSender);

	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);


	void playParticleEffec(float t);
	void endParticle(float t);
	void canSpeedUp(CCObject *pSender);
	void coutinue(CCObject *pSender);

	void menuEnable(CCObject *obj);
	void setCoin();
	void showShopLayer(CCObject *pSender);
	void backFromShopLayer(CCObject *pSender);
	void back(CCObject *pSender);
	void setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack);


	void addUpSucEffect(int type);
	void animationEnd(CCNode *obj);
	void endUpEffect(CCNode *node);

	void keyBackClicked();
	void goGamble();
private:
	LuckyCofig computeReward();
	int getIndexByGambleType(int type,float value);
	float  getRewardIndex(int iCount);
	int getRewardTypeByIndex(int index);
	void startSpin(CCObject* obj);
	void close(CCObject* obj);
	void enableBtn();
	CCMenu *m_menu;
	CCSprite* m_spinSprite;
	int m_rewardIndex;
	int m_spinCount;
	CCObject* m_pTarget;
	SEL_CallFunc m_fun;
	CCLabelBMFont *m_lastSpinCount;
	CCLabelBMFont *m_diamondLabel;
	CCLabelBMFont *m_coinLabel;
	LuckyCofig m_data;
	CCMenu *m_buyTimeMenu;
	bool m_speedUp;
	int m_rotateCount;
	int m_playCoinAndDiaType;
	SEL_MenuHandler m_callFun;
};
#endif