#ifndef GAMELAYER_H
#define GAMELAYER_H
#include "cocos2d.h"
#include "../SpriteNode/MapEventsNode.h"
#include "../ToolFun/JniCall.h"
using namespace cocos2d;

#define BULLET_ZORDER  100
#define  ITEM_ZORDER 90
#define  MONSTER_ZORDER 80
#define HERO_ZORDER 70
#define UI_ZORDER  10000
#define EFFECT_ZORDER 10
#define  PAUSE_ZORDER 100100
#define PETLEFTTAG 1407
#define PETRIGHTTAG 1408

class MapNode;
class HeroSprite;
class MySpriteManager;
class MonsterSprite;
class BulletSprite; 
class PetSprite;
class HeroBulletSprite;
class GameLayer : public CCLayer
{
public:
	GameLayer();
	~GameLayer();
	virtual bool init();
	CREATE_FUNC(GameLayer);
	void initUi();
	void initKernelModes();
	void updateProgress(float t);
	void scheduleStart();
	virtual void updateLayer(float t);
	virtual void updateMove(float t);
	void touchMenu(CCObject *pSender);
	void pauseGame(CCObject *pSender);
	void pause(float t=0);
	void resume();
	void useInvincibleItem(CCObject *pSender);
	void parseMapEvent(LevelEvent *event);
	void createEnemyByEvent(LevelEvent *event);
	void createItemByEvent(LevelEvent *event);

	void createEnemy2ByBoss(CCPoint point);
	void createEnemy1007ByBoss(CCPoint point);
	
	void addGameTip(LevelEvent *event);
	void pauseParseEvent(LevelEvent *event);
	void parseStoryEvent(LevelEvent *event);
	void showTipById(int tipId);
	void showStoryById(int sId);
	CCPoint getHeroPos();
	HeroSprite *getHeroSprite();
	PetSprite* getLeftPet();
	PetSprite* getRightPet();
	MySpriteManager *getMySpriteManager();
	void addBullet(BulletSprite *bullet,int zOrder);
	void addBullet(HeroBulletSprite *bullet,int zOrder );
	void resumeLevel(CCLayer *layer);
	void quitLevel();
	void releaseGameLayerData();
	void retryLevel();
	void reviveLevel(CCLayer *layer);
	void storyEnd(int ok);
	void mapSpeedUp();
	void mapSpeedUpEnd();
	void showReviveUI();
	void addMTip(int mId ,CCPoint tippos);
	void setInvincibleItem(bool enable);
	void showEndLine(LevelEvent *event);
	void endLineActionFinish(CCNode *obj);
	void updateEndLinePos(float dt);

	void registerNDK();
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void setReviveCount();

	int getKillMonTotal();


public:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void keyBackClicked();
	
public:
	static GameLayer *sharedGameLayer();
	void setallTouchEnabled(bool st);
	void gameOver(CCObject* pSender);
	void add_m_score(int value);
	void add_m_money(int value);
	void add_m_distance(int value);
	int getscore();
	int getmoney();
	int getdistance();
	void initRightPet();
	void initLeftPet();
	// 	void onEnter();
// 	void onExit();
	void updateMoneyLabel();
	void removeMTip();
	void showDeadthEffect(CCPoint dead_pos,int mId=0);
	void removeDeadEffect(CCNode *node );
	void updateDistanceLabel();
	void updateScoreLabel();
	int getCurStar();
	void set_m_Star(int value);
	void gameOverDelay(float t);
	void initHero();
	void showGiftbag(int id);
	void touchRewardMenu(CCObject *sender);
	void showActiveDialog();
	void showActiveDialogClose(CCObject *sender);
	void showActiveDialogSure(CCObject *sender);

	void show30luckyDialog();
	void show30luckyDialogClose(CCObject *sender);
	void show30luckyDialogSure(CCObject *sender);

private:
	CCLabelBMFont *m_gainScore;
	CCLabelBMFont *m_gainMoney;
	CCLabelBMFont *m_progressLength;
	CCLabelBMFont *m_reviveNumber;
	
	//add zh 
	unsigned int m_Score;
	unsigned int m_Money;
	unsigned int m_Distance;
	unsigned int m_CurStar;
	int moveLenght;
	bool m_levelPass;

	CCSprite *m_pHeadSprite;
	PetSprite *m_leftPet;
	PetSprite *m_rightPet;
	CCMenu *m_menu;
	MapNode *m_mapNode;
	HeroSprite *m_heroSprite;
	CCNode *m_storyNode;
	MySpriteManager *m_mySpriteManager;
	
	bool m_nodeMoved;
	CCPoint m_curPos;
	CCPoint m_lastPos;
	CCPoint m_offectPos;
	bool m_pasueState;
	int m_revive;
	bool m_isBossLevel;
	int m_killMosTotal;  //击杀的怪物数量
	int m_min;
	int m_max;
	int m_firstRevive;
	int m_speed;
	bool m_useProtect;
	bool m_reward;
};
#endif