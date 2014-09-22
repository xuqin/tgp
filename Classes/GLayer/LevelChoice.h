#ifndef LEVELCHOICE_H
#define LEVELCHOICE_H

#include "cocos2d.h"

using namespace cocos2d;
class GameLayer;

#include "../GCData/GameConfigData.h"
class LevelChoice : public CCLayer
{
public:
	LevelChoice();
	~LevelChoice();
	virtual bool init();
	CREATE_FUNC(LevelChoice);
	void choiceLevel(CCObject *pSender);
	void choiceSubLevel(CCObject *pSender);
	void choiceReward(CCObject *pSender);
	void setCurrentLevel(int level);
	void startGame();
	void startGame2();  //开始支线关卡战斗
	void updateTipPos(int pos);
	void showLevelUnLockTip(int level);
	void initUnLockLevel();
	void initUnLockSubLevel();
	void initRewardLevel();
	void getReward(CCObject *pSender);
	void showActiveUI();
	void showLevelTip(int level);
	void showStartCountUI(int level);
	void showRewardUI(int rewardId, int m_tag);
	void registerNDK();

	void activeTouch(CCObject *pSender);
	void levelTipTouch(CCObject *pSender);
	void subLevelTouch(CCObject *pSender);
	void setAllTouchEnabled(bool state);
	void tipMoveEnd(CCNode *node);

	void initMenuItem();
	void gotoPetInfo();
	void gotoHelp();
	void gotoHeroInfo();
	void gotoShop();
	void gotoDailyLogIn();
	void backFromDailyLogIn(CCObject *pSender);
	void back();
	void dailyPurchase();

	void hideDailyPurchase(CCObject *obj);
	void closeDailyPurchase(CCObject *obj);

	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);

	void keyBackClicked();
	void goGamble();

	void showGiftBag();
	void showGiftBag(int id);
	void showGiftBagSure(CCObject *sender);
	void showGiftBagClose(CCObject *sender);
	int getGiftBag();

public:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void initTipIcon();
	void endSlid(float t);
	void updatePosTouchEnd(float t);
	void menuCloseCallback(CCObject* pSender);
	void showHelp();
	void setCoin();

private:
	CCNode *m_uiNode;

	bool m_nodeMoved;
	bool m_pauseMove;
	CCPoint m_curPos;
	CCPoint m_lastPos;
	CCMenu *m_menu;
	long m_curTime;
	float m_lastPosY;
	float m_curEndSpeed;
	CCMenu *m_menuNormal;
	CCLabelBMFont *m_coinLabel;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};
#endif