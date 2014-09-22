//
//  DialogLayer.h
//  ModalDialogDemo


#ifndef ModalDialogDemo_DialogLayer_h
#define ModalDialogDemo_DialogLayer_h

#include "cocos2d.h"


USING_NS_CC;
typedef void (CCObject::*SEL_CallFuncInt)(int);
#define callfuncInt_selector(_SELECTOR) (SEL_CallFuncInt)(&_SELECTOR)
class DialogLayer: public CCLayer
{
private:   // 模态对话框菜单
	static DialogLayer *sharedDialogLayer();
    CCMenu *m_pMenu;
    // 记录菜单点击
    bool m_bTouchedMenu;
    
public:
    DialogLayer();
    ~DialogLayer();
    
    virtual bool init();
	
    // 初始化对话框内容
    void initDialog();
    void initDialog2(int tipid);
    CREATE_FUNC(DialogLayer);
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    
    void okMenuItemCallback(CCObject *pSender);
    void cancelMenuItemCallback(CCObject *pSender);

	void setCallBackFun(CCObject *pOb,SEL_CallFuncInt fun);
	void registerTouch(float t);
	

private:
	CCObject *m_target;
	SEL_CallFuncInt m_funCallBack;
	//menu_selector
};

#endif
