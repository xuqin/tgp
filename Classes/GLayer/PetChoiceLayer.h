#ifndef PETCHOICELAYER_H
#define PETCHOICELAYER_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyScissorLayer.h"
#include "MyScrowLayer.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class PetChoiceLayer : public MyScrowLayer
{
public:
	PetChoiceLayer();
	virtual ~PetChoiceLayer();

	virtual bool init();
	virtual void initMenu();
	CREATE_FUNC(PetChoiceLayer);
	void back(CCObject *pSender);
	void getPet(int petId );
	void initUI();
	void showPetPosMenu(CCObject* pSender);
	void addPet(CCObject *pSender);

	void removeAddPetMenu(float t);
	void petRest(UIButton* button);
	void skillUp(int petid);

	void setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack);
	void showShopLayer(CCObject *pSender);
	void backFromShopLayer(CCObject *pSender);
	void purchasePet(CCObject *pSender);
	void upPet(CCObject *pSender);
	void cancelBattle(CCObject *pSender);

	void resetPetState();
	void resetPetStateById(int pId);
	void setCoin();
	void registerNDK();
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);

	void keyBackClicked();
	void goGamble();

	void showFullUpgrade(int type =0);
	void showFullUpgradeClose(CCObject *sender);
	void openBuyCoin(CCObject *sender);
	void showFullUpgradeSure(CCObject *sender);

	void showBuyCoin();
	void showBuyCoinClose(CCObject *sender);
	void showBuyCoinSure(CCObject *sender);

	void movePetPosition(int id);

private:
	CCSprite *m_petBkgs[6];
	CCLabelBMFont *m_priceFont[6];
	CCLabelBMFont *m_upPriceFont[6];
	CCLabelBMFont *m_desFont[6];
	CCLabelBMFont *m_nameFont[6];
	CCLabelBMFont *m_levelFont[6];
	CCMenuItemSprite *m_buyPetItems[6];
	CCMenuItemSprite *m_upPetItems[6];
	CCMenuItemSprite *m_goBattleItems[6];
	CCMenuItemSprite *m_cancelBattleItems[6];
	int m_beChoicePetId;
	int m_UpPetid;
	CCLabelBMFont *m_coinLabel;
	CCSprite* petPos;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};
#endif