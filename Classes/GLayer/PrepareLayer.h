#ifndef PREPARELAYER_H
#define PREPARELAYER_H

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

struct HeroUpgradeData;
class HeroSprite;
class GameLayer;
class PrepareLayer : public CCLayer
{
public:
	PrepareLayer();
	~PrepareLayer();
	CREATE_FUNC(PrepareLayer);
	virtual bool init();
	void initPet(int leftOrRight);
	void initItems();
	void menuTouch(CCObject *pSender);
	void startGame(CCObject *pSender);
	void back(CCObject *pSender);
	void showShopLayer(CCObject *pSender);
	void showPetLayer(CCObject *pSender);
	void showHeroLayer(CCObject *pSender);
	void heroUp(CCObject *pSender);
	void playUpEffect(int level,bool upEffect = true);
	void backFromPetLayer(CCObject *pSender);
	void backFromHeroLayer(CCObject *pSender);
	void backFromShopLayer(CCObject *pSender);
	void setStartGameEnable(bool enable=true);
	void initHeroSp(int mId);
	void initItem(int leftOrRight);

	void buyCoin(CCObject *pSender);
	void purchaseSp(int index);
	void purchaseSuc(int index);
	void setCoin();

	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void shotBullet(float dt);
	void shotBulletPetLeft(float dt);
	void shotBulletPetRight(float dt);

	void showHeroPet(int leftOrRight,int petId);
	void registerNDK();

	void showFullUpgrade(int type =0);
	void showFullUpgradeClose(CCObject *sender);
	void openBuyCoin(CCObject *sender);
	void showFullUpgradeSure(CCObject *sender);

	void showBuyCoin();
	void showBuyCoinClose(CCObject *sender);
	void showBuyCoinSure(CCObject *sender);
private:
	CCLabelBMFont *m_moneyLabel;
	CCLabelBMFont *m_diamondLabel;
	CCLabelBMFont *m_nextLevelPriceLabel;
	CCLabelBMFont *m_curHeroLevel;
	CCSprite *m_heroHeadSp;
	CCSprite *m_heroSprite;
	CCMenu *m_menu;
	int m_curShowHeroId;
	int m_leftPitId;
	int m_rightPetId;
	CCLabelBMFont *m_heroNameLabel;
};
#endif