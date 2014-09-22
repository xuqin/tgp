#include "StartLoading.h"
#include "MainMenu.h"
#include "../ToolFun/ToolsFun.h"
#include "LevelChoice.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "../GCData/GameConfigData.h"
#include "../ToolFun/HttpClientNode.h"

StartLoading::StartLoading():m_loadProgress(NULL)
	,m_loadedCount(0)
{
	m_vImageFiles.clear();
}

StartLoading::~StartLoading()
{
	
}

bool StartLoading::init()
{
	m_loadProgress = CCProgressTimer::create(CCSprite::create("ui/startLoading/role3.jpg"));
	
	m_loadProgress->setPosition(ToolsFun::sharedTools()->getSceneSize()/2);
	m_loadProgress->setType(kCCProgressTimerTypeBar);
    m_loadProgress->setPercentage(0);
    m_loadProgress->setMidpoint(ccp(0, 1));
	m_loadProgress->setBarChangeRate(ccp(1, 0));
	this->addChild(m_loadProgress,1);

	/**
	公共界面资源
	*/
	m_vImageFiles.push_back("ui/shared/bkg.png");
	m_vImageFiles.push_back("ui/shared/top.png");
	m_vImageFiles.push_back("ui/shared/coin.png");
	m_vImageFiles.push_back("ui/shared/coinplus.png");
	m_vImageFiles.push_back("ui/shared/back.png");
	m_vImageFiles.push_back("ui/shared/continueGame.png");
	m_vImageFiles.push_back("ui/shared/closeTipNml.png");
	m_vImageFiles.push_back("ui/shared/tipBkg0.png");
	m_vImageFiles.push_back("ui/shared/plus.png");
	m_vImageFiles.push_back("ui/shared/placeboard.png");
	m_vImageFiles.push_back("ui/shared/shop_item.png");
	m_vImageFiles.push_back("ui/shared/cancel.png");

	/**
	MainMenu界面资源
	*/
	m_vImageFiles.push_back("ui/mainUI/login_bg.png");
	m_vImageFiles.push_back("ui/mainUI/login_title.png");
	m_vImageFiles.push_back("ui/mainUI/login_start.png");
	m_vImageFiles.push_back("ui/mainUI/music.png");
	m_vImageFiles.push_back("ui/mainUI/sound.png");
	m_vImageFiles.push_back("ui/mainUI/on.png");
	m_vImageFiles.push_back("ui/mainUI/off.png");

	/**
	LevelChoice界面资源
	*/
	m_vImageFiles.push_back("ui/levelChoice/levelCLock.png");
	m_vImageFiles.push_back("ui/levelChoice/levelSLock.png");
	m_vImageFiles.push_back("ui/levelChoice/levelBossLock.png");
	m_vImageFiles.push_back("ui/levelChoice/level0CNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level0SNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level0BossNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level1CNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level1SNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level1BossNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level2CNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level2SNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level2BossNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level3CNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level3SNml.png");
	m_vImageFiles.push_back("ui/levelChoice/level3BossNml.png");
	m_vImageFiles.push_back("ui/levelChoice/reward0Nml.png");
	m_vImageFiles.push_back("ui/levelChoice/reward1Nml.png");
	m_vImageFiles.push_back("ui/levelChoice/reward2Nml.png");
	m_vImageFiles.push_back("ui/levelChoice/reward3Nml.png");
	m_vImageFiles.push_back("ui/levelChoice/bubble.png");
	m_vImageFiles.push_back("ui/levelChoice/base.png");
	m_vImageFiles.push_back("ui/levelChoice/help_btn.png");
	m_vImageFiles.push_back("ui/levelChoice/shop.png");
	m_vImageFiles.push_back("ui/levelChoice/hero.png");
	m_vImageFiles.push_back("ui/levelChoice/pet.png");
	m_vImageFiles.push_back("ui/levelChoice/levelChoice0.jpg");
	m_vImageFiles.push_back("ui/levelChoice/levelChoice1.jpg");
	m_vImageFiles.push_back("ui/levelChoice/levelChoice2.jpg");
	m_vImageFiles.push_back("ui/levelChoice/levelChoice3.jpg");
	m_vImageFiles.push_back("ui/levelChoice/levelRmbLock0.png");
	m_vImageFiles.push_back("ui/levelChoice/levelRmbLock1.png");
	m_vImageFiles.push_back("ui/levelChoice/levelRmbLock2.png");
	m_vImageFiles.push_back("ui/timedBuyUI/baseboard.png");
	m_vImageFiles.push_back("ui/tipUI/heroSmile.png");
	m_vImageFiles.push_back("ui/tipUI/gainStart.png");
	m_vImageFiles.push_back("ui/tipUI/tipbkg.png");

	for(int i=0;i!=4;++i)
	{
		char fileName[33];
		sprintf(fileName,"ui/tipUI/rwardIcon%d.png",i);
		m_vImageFiles.push_back(fileName);
	}

	/**
	DailyLogin界面资源
	*/
	m_vImageFiles.push_back("ui/dailyRewardUI/login_bkg.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day0.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day1.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day2.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day3.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day4.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day5.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/Login_day6.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/login_get0.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/bigSelect.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/smallSelect.png");
	m_vImageFiles.push_back("ui/dailyRewardUI/currentGain.png");

	/**
	herochoice界面资源
	*/
	m_vImageFiles.push_back("ui/herochoice/hero_bkgcube.png");
	m_vImageFiles.push_back("ui/herochoice/hero_describe.png");
	m_vImageFiles.push_back("ui/herochoice/battle.png");
	m_vImageFiles.push_back("ui/herochoice/role_buy.png");
	m_vImageFiles.push_back("ui/herochoice/hero_head.png");
	m_vImageFiles.push_back("ui/herochoice/hero_lock.png");
	m_vImageFiles.push_back("ui/herochoice/select.png");

	for(int i=0;i!=3;++i)
	{
		char fileName[33];
		sprintf(fileName,"ui/herochoice/heroPortrait_%d.png",i);
		m_vImageFiles.push_back(fileName);
		char headFileName[48];
		sprintf(headFileName,"ui/herochoice/heroHead_%d.png",i);
		m_vImageFiles.push_back(headFileName);
	}

	/**
	petchoice界面资源
	*/
	m_vImageFiles.push_back("ui/petUI/pet_head.png");
	m_vImageFiles.push_back("ui/petUI/pet_describe.png");

	m_vImageFiles.push_back("ui/petUI/pet_follow_0.png");
	m_vImageFiles.push_back("ui/petUI/pet_follow_1.png");

	m_vImageFiles.push_back("ui/petUI/pet_btn.png");
	m_vImageFiles.push_back("ui/petUI/battle.png");
	m_vImageFiles.push_back("ui/petUI/cancel_battle.png");

	for(int i=0;i!=6;++i)
	{
		char fileName[33];
		sprintf(fileName,"ui/petUI/pet%d_0.png",i);
		m_vImageFiles.push_back(fileName);
	}

	/**
	luckyspin界面资源
	*/
	m_vImageFiles.push_back("ui/luckyspin/luckybkg.jpg");
	m_vImageFiles.push_back("ui/luckyspin/Lucky_btn0.png");
	m_vImageFiles.push_back("ui/luckyspin/lastTime.png");
	m_vImageFiles.push_back("ui/luckyspin/sped_up.png");
	m_vImageFiles.push_back("ui/luckyspin/speed_sel.png");
	m_vImageFiles.push_back("ui/luckyspin/shielding.png");
	m_vImageFiles.push_back("ui/luckyspin/200.png");
	m_vImageFiles.push_back("ui/luckyspin/5000.png");
	m_vImageFiles.push_back("ui/luckyspin/1000.png");
	m_vImageFiles.push_back("ui/luckyspin/8888.png");
	m_vImageFiles.push_back("ui/luckyspin/500.png");
	m_vImageFiles.push_back("ui/luckyspin/again.png");
	m_vImageFiles.push_back("ui/luckyspin/gold.png");
	m_vImageFiles.push_back("ui/luckyspin/rewardSur.png");
	m_vImageFiles.push_back("ui/luckyspin/opacityMenu.png");

	for(int i=0;i!=3;++i)
	{
		char fileName[48];
		sprintf(fileName,"ui/luckyspin/buyGambleTime%d.png",i);
		m_vImageFiles.push_back(fileName);
	}

	/**
	prepare界面资源
	*/
	m_vImageFiles.push_back("ui/prepareUI/upgrade.png");
	m_vImageFiles.push_back("ui/prepareUI/mainbkg.png");
	m_vImageFiles.push_back("ui/prepareUI/recordbkg.png");
	m_vImageFiles.push_back("ui/prepareUI/head.png");
	m_vImageFiles.push_back("ui/prepareUI/level.png");
	m_vImageFiles.push_back("ui/prepareUI/changename.png");

	/**
	shop界面资源
	*/
	m_vImageFiles.push_back("ui/shopUI/shop_board.png");
	m_vImageFiles.push_back("ui/shopUI/shop_item.png");
	m_vImageFiles.push_back("ui/shopUI/shop_getbtn.png");
	m_vImageFiles.push_back("ui/prepareUI/level.png");

	for (int i =0;i!=6;++i)
	{
		char itemFile[33];
		sprintf(itemFile,"ui/shopUI/item_%d.png",i);
		m_vImageFiles.push_back(itemFile);

		char itemDesFile[33];
		sprintf(itemDesFile,"ui/shopUI/itemDes_%d.png",i);
		m_vImageFiles.push_back(itemDesFile);
	}
	scheduleOnce(schedule_selector(StartLoading::loadImage),0.2f);

	return true;
}

void StartLoading::loadImage(float t)
{
	for(int i=0;i!=m_vImageFiles.size();++i)
	{
		CCTextureCache::sharedTextureCache()->addImageAsync(m_vImageFiles[i].c_str(),this,callfuncO_selector(StartLoading::loadImageCallBack));
	}
}

void StartLoading::loadMusic()
{

}

void StartLoading::loadingFiles()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/firstGet.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/sprinPointer.plist");
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/sprint.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/startLucky.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/upgrade.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("item/item_4.plist");

	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("firstGet%d.png",2,0.12f);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("sprinPointer%d.png",2,0.12f);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("upgrade_%d.png",6,0.12f);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("gambleStart%d.png",2,0.12f);

	srand( (unsigned int)time( NULL ));
	CCUserDefault::sharedUserDefault()->setBoolForKey("inLuckyFirst",true);

	///*干点事情 */
	//HttpClientNode *node = HttpClientNode::sharedHttpClient();
	//this->addChild(node);
	//node->requestUserId();
	//node->removeFromParent();
	//node->eventSend(32,ConfigData::sharedConfigData()->coinNum);
	//node->eventSend(33,ConfigData::sharedConfigData()->diamondNum);
	//CCLog("start loading init end");
	HttpClientNode *node = HttpClientNode::sharedHttpClient();
	this->addChild(node);
	node->requestUserId();
	node->removeFromParent();
	loadConfigData();
	loadingEnd(0);
}

void StartLoading::loadingEnd(float t)
{
	CCScene *scene = CCScene::create();
	MainMenu *layer = MainMenu::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void StartLoading::loadImageCallBack(CCObject *pSender)
{
	m_loadedCount++;
	float percent = 100.0f* (float)m_loadedCount / m_vImageFiles.size();
	m_loadProgress->setPercentage(percent);
	if (m_loadedCount == m_vImageFiles.size())
	{
		loadingFiles();
	}
}

void StartLoading::loadConfigData()
{
	ToolsFun::sharedTools()->readXmlData();
	ToolsFun::sharedTools()->preLoadMusicAndEffect();
	ToolsFun::sharedTools()->initMusicAndEffectEnable();
}

void  StartLoading::releaseFiles()
{   CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	for(int i=0;i!=m_vImageFiles.size();++i)
	{
		CCTextureCache::sharedTextureCache()->removeTextureForKey(m_vImageFiles[i].c_str());
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	SpriteAniManager::getAniManager()->releaseAllAni();
}