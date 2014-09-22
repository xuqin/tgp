#include "AppDelegate.h"
#include "GLayer/LogoLayer.h"
#include "SimpleAudioEngine.h"
#include "ToolFun/ToolsFun.h"
#include "GCData/GameConfigData.h"

using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
	GameConfigData::initialGameData().writeData();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	pDirector->setOpenGLView(pEGLView);

	// turn on display FPS
	pDirector->setDisplayStats(false);
	pEGLView->setDesignResolutionSize(480, 800, kResolutionExactFit);
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	CCScene *pScene = CCScene::create();
	LogoLayer *layer = LogoLayer::create();
	pScene->addChild(layer);
	// run
	pDirector->runWithScene(pScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
GameConfigData::initialGameData().writeData();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	if(ToolsFun::sharedTools()->getMusicBkgEnable())
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}
