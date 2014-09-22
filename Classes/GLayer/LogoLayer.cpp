#include "LogoLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "StartLoading.h"

LogoLayer::LogoLayer()
{

}

LogoLayer::~LogoLayer()
{

}

CCScene* LogoLayer::scene(){
	CCScene* scene = CCScene::create();
	scene->addChild(LogoLayer::create());
	return scene;
}

bool LogoLayer::init()
{
	CCSize sceneSize = CCDirector::sharedDirector()->getWinSize();
	m_logo = CCSprite::create("ui/logoUi/JoyMe.png");
	m_logo->setOpacity(0);
	m_logo->runAction(CCFadeIn::create(1));
	m_logo->setPosition(ccp(sceneSize.width/2, sceneSize.height/2));
	this->addChild(m_logo,0);
	m_logo->setScale(0.5f);
	scheduleOnce(schedule_selector(LogoLayer::loadingMiddle),1.2f);
	return true;
}

void LogoLayer::loadingMiddle(float t)
{
	//NetworkLayer* nl = NetworkLayer::create();
	//this->addChild(nl);
	//scheduleOnce(schedule_selector(LogoLayer::loadingEnd),0.5);
	logoEnd(0);
}

void LogoLayer::logoEnd(float t)
{
	CCScene *scene = CCScene::create();
	StartLoading *layer = StartLoading::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->replaceScene(scene);
}
