#ifndef HELPLAYER_H
#define HELPLAYER_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyScissorLayer.h"
#include "MyScrowLayer.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class HelpLayer : public MyScrowLayer
{
public:
	HelpLayer();
	virtual ~HelpLayer();

	virtual bool init();
	virtual void initMenu();
	CREATE_FUNC(HelpLayer);
	void back(CCObject *pSender);
	void initUI();
	void setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack);
	void showShopLayer(CCObject *pSender);
	void backFromShopLayer(CCObject *pSender);
	void setCoin();

	void keyBackClicked();
	void goGamble();

private:
	//CCSprite *m_petBkgs[6];
	CCLabelBMFont *m_desFont[6];
	CCLabelBMFont *m_nameFont[6];	
	CCLabelBMFont *m_coinLabel;	
	CCObject *m_target;
	SEL_MenuHandler m_callFun;
};
#endif