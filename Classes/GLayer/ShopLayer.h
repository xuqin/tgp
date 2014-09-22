#ifndef SHOPLAYER_H
#define SHOPLAYER_H
#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyScissorLayer.h"
#include "MyScrowLayer.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class ShopLayer : public MyScrowLayer
{
public:
	ShopLayer();
	virtual ~ShopLayer();
	virtual bool init();
	virtual void initMenu();
	void initUI();
	CREATE_FUNC(ShopLayer);
	virtual void touchMenu(CCObject *pSender);
	void back(CCObject* obj);
	void setCloseCallBack(CCObject *pSender,SEL_MenuHandler callFun);
	void setCoin();
	void initItem();
	void initItemById(int id);
	void purchase(CCObject* obj);
	void purchaseSuccess(CCNode* node, void *data);
	void purchaseSuccess(const char* str);
	void purchaseFailed(const char* str);
	void setIsUpdating(int i);
	void registerNDK();
	void updateItem(int id);

	void keyBackClicked();
	void goGamble();

private:
	UIPanel *m_scrowPanel;
	UILayer *m_rootLayer;
	CCLabelBMFont *m_coinLabel;
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
	CCMenu* m_menu;
	CCSprite *m_itemBkgs[5];
	CCLabelBMFont *m_priceFont[5];
	CCLabelBMFont *m_desFont[5];
	CCLabelBMFont *m_nameFont[5];
	CCMenuItemSprite *m_buyItems[5];
	CCLabelBMFont *m_reviveNum;
	CCLabelBMFont *m_bombNum;
	int m_isUpdating;
};
#endif