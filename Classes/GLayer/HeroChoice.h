#ifndef HEROCHOICE_H
#define HEROCHOICE_H

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;


class HeroSprite;

class HeroChoiceLayer : public CCLayer
{
public:
	HeroChoiceLayer();
	~HeroChoiceLayer();
	CREATE_FUNC(HeroChoiceLayer);
	virtual bool init();
	void initPet(int leftOrRight);
	void menuTouch(CCObject *pSender);
	void back(CCObject* obj);
	void setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack);
	void initHeroSp(int mId);
	void initUnLockHero();
	void updateSelection();
	void showShopLayer(CCObject *pSender);
	void backFromShopLayer(CCObject *pSender);
    void registerNDK();
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void shotBullet(float dt);
	void showHeroPet(int leftOrRight,int petId);

	void keyBackClicked();
	void goGamble();

private:
	CCLabelBMFont *m_coinLabel;
	CCLabelBMFont *m_heroPriceLabel;
	CCSprite *m_heroHeadSp;
	CCSprite *m_heroSprite;
	CCSprite *m_rightHero;
	CCLabelBMFont *heroNameLabel;
	CCLabelBMFont *heroDesLabel;
	CCLabelBMFont *m_heroSkillLabel;
	CCMenu *m_menu;
	int curSelectId;
	int m_curHeroId;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};
#endif