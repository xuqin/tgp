

#include "DialogLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "GameLayer.h"



DialogLayer::DialogLayer():m_target(NULL)
	,m_funCallBack(NULL)
{
	
}


DialogLayer::~DialogLayer()
{
}

bool DialogLayer::init()
{
 
    CCLayerColor *bkgLayer = CCLayerColor::create(ccc4(0, 0, 0, 125));
        this->setTouchEnabled(false);
	this->addChild(bkgLayer,-1);
    return true;
}


void DialogLayer::initDialog()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	char filename[66];
	sprintf(filename,"ui/gameStory/story%d.png",0);
	CCSprite *spriteStory = CCSprite::create(filename);
	this->addChild(spriteStory,10000);
	spriteStory->setPosition(ccp(240*gScaleX,480*gScaleY));

	CCBlink *blinkTip = CCBlink::create(2,4);
	// 	touchBegan->runAction(CCRepeatForever::create(blinkTip));


    CCLabelTTF *label = CCLabelTTF::create("Are u sure exit?", "", 64*gScaleY);
    label->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50*gScaleY));
    this->addChild(label);
    
    CCMenuItemFont *okMenuItem = CCMenuItemFont::create(gGetLanguageDesc(K.OK).c_str(), this, menu_selector(DialogLayer::okMenuItemCallback));
    okMenuItem->setPosition(ccp(winSize.width / 2 - 100*gScaleX, winSize.height / 2 - 50*gScaleY));
    okMenuItem->runAction(CCRepeatForever::create(blinkTip));
    CCMenuItemFont *cancelMenuItem = CCMenuItemFont::create("Cancel", this, menu_selector(DialogLayer::cancelMenuItemCallback));
    cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100*gScaleX, winSize.height / 2 - 50*gScaleY));
    
    m_pMenu = CCMenu::create(okMenuItem, cancelMenuItem, NULL);
    m_pMenu->setPosition(CCPointZero);
    this->addChild(m_pMenu);
}
void DialogLayer::initDialog2(int tipid)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	char filename[66];
	sprintf(filename,"ui/gameStory/story%d.png",tipid);
	CCSprite *spriteStory = CCSprite::create(filename);
	this->addChild(spriteStory,10000);
	spriteStory->setPosition(ccp(240*gScaleX,480*gScaleY));

	CCBlink *blinkTip = CCBlink::create(2,4);
	CCSequence *seq = CCSequence::create(CCScaleTo::create(0.4f,1.3),CCScaleTo::create(0.4,1.0),NULL);

	this->setTouchEnabled(false);
	
	this->scheduleOnce(schedule_selector(DialogLayer::registerTouch),0.5f);
}

void DialogLayer::registerTouch(float t)
{	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSequence *seq = CCSequence::create(CCScaleTo::create(0.4f,1.3),CCScaleTo::create(0.4,1.0),NULL);
	CCLabelTTF *label = CCLabelTTF::create(gGetLanguageDesc(K.CLICK).c_str(), "", 64);
	label->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 100*gScaleY));
	label->runAction(CCRepeatForever::create(seq));
	this->addChild(label);
	this->setTouchEnabled(true);
}

void DialogLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, true);
}

bool DialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_target)
	{
		(m_target->*m_funCallBack)(0);
	}
	this->removeFromParentAndCleanup(false);
    MapEventsNode::sharedMapEventsNode()->resumeMapAction();
    return true;
}

void DialogLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void DialogLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}



void DialogLayer::okMenuItemCallback(CCObject *pSender)
{
}

void DialogLayer::cancelMenuItemCallback(CCObject *pSender)
{

}

void DialogLayer::setCallBackFun(CCObject *pOb,SEL_CallFuncInt fun)
{
	m_target = pOb;
	m_funCallBack = fun;
}
