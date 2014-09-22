#include "MainMenu.h"
#include "StartGameLoading.h"
#include "../ToolFun/ToolsFun.h"
#include "LevelChoice.h"
#include "PetChoiceLayer.h"
#include "ShopLayer.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "LuckySpinLayer.h"
#include "SpriteNode/SystemTipNode.h"
#include "GCData/GameConfigData.h"
#include "BackLayer.h"

#define MUSIC_SETTING 101
#define SOUND_SETTING 102
#define STARTGAME 100
#define GAMBLE 204

MainMenu::MainMenu()
{
	LD("MainMenu::MainMenu()");
}

MainMenu::~MainMenu()
{
	LD("MainMenu::~MainMenu()");
}

bool MainMenu::init()
{
	ToolsFun::sharedTools()->initMusicAndEffectEnable();
	ToolsFun::sharedTools()->playBkgMusic(LOGIN, true);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *spriteBkg = CCSprite::create("ui/mainUI/login_bg.png");
	this->addChild(spriteBkg, -1);
	spriteBkg->setPosition(size / 2);

	CCSprite *title = CCSprite::create("ui/mainUI/login_title.png");
	this->addChild(title, 0);
	title->setPosition(size / 2 + ccp(0, 200*gScaleY));

	CCMenu *menu = CCMenu::create();
	this->addChild(menu);
	menu->setPosition(CCPointZero);
	CCSprite *startNml = CCSprite::create("ui/mainUI/login_start.png");
	CCSprite *startSel = CCSprite::create("ui/mainUI/login_start.png");
	startSel->setColor(ccc3(100, 100, 100));
	startSel->setOpacity(100);
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(startNml, startSel,
	                           this, menu_selector(MainMenu::touchMenu));
	menu->addChild(itemSp, 1, STARTGAME);
	itemSp->setPosition(ccp(size.width / 2, 230*gScaleY));
	CCSequence *seq = CCSequence::create(CCScaleTo::create(0.4f, 1.3f), CCScaleTo::create(0.4f, 1.0f), NULL);
	menu->getChildByTag(100)->runAction(CCRepeatForever::create(seq));

	CCSprite* music = CCSprite::create("ui/mainUI/music.png");

	CCSprite* spriteNml = NULL;
	CCSprite *spriteSel = NULL;

	if(GameConfigData::initialGameData().getCurMusicState())
	{
		spriteNml = CCSprite::create("ui/mainUI/on.png");
		spriteSel = CCSprite::create("ui/mainUI/on.png");
	}
	else
	{
		spriteNml = CCSprite::create("ui/mainUI/off.png");
		spriteSel = CCSprite::create("ui/mainUI/off.png");
	}

	spriteSel->setColor(ccc3(80, 80, 80));
	CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel,
	                               this, menu_selector(MainMenu::touchMenu));
	itemSprite->addChild(music, -1);
	music->setPositionX(-60*gScaleY);
	menu->addChild(itemSprite, 5, MUSIC_SETTING);
	itemSprite->setPosition(ccp(440*gScaleX, 750*gScaleY));
	music->setAnchorPoint(ccp(0, 0));

	CCSprite* spriteNml1 = NULL;
	CCSprite *spriteSel1 = NULL;
	CCSprite* sound = CCSprite::create("ui/mainUI/sound.png");

	if(GameConfigData::initialGameData().getCurSoundEffectState())
	{
		spriteNml1 = CCSprite::create("ui/mainUI/on.png");
		spriteSel1 = CCSprite::create("ui/mainUI/on.png");
	}
	else
	{
		spriteNml1 = CCSprite::create("ui/mainUI/off.png");
		spriteSel1 = CCSprite::create("ui/mainUI/off.png");
	}

	spriteSel->setColor(ccc3(80, 80, 80));
	CCMenuItemSprite *itemSprite1 = CCMenuItemSprite::create(spriteNml1, spriteSel1,
	                                this, menu_selector(MainMenu::touchMenu));
	itemSprite1->addChild(sound, -1);
	sound->setPositionX(-60*gScaleX);
	menu->addChild(itemSprite1, 5, SOUND_SETTING);
	itemSprite1->setPosition(ccp(440*gScaleX, 710*gScaleY));
	sound->setAnchorPoint(ccp(0, 0));

	CCSprite *spSel = CCSprite::create("ui/luckyspin/opacityMenu.png", CCRect(0, 0, 81, 89));
	spSel->setColor(ccc3(80, 80, 80));
	CCMenuItemSprite* goGambleItem = CCMenuItemSprite::create(CCSprite::create("ui/luckyspin/opacityMenu.png", CCRect(0, 0, 81, 89)), spSel, NULL,
	                                 this, menu_selector(MainMenu::touchMenu));
	goGambleItem->setPosition(ccp(goGambleItem->getContentSize().width / 2 + 15*gScaleX, 79*gScaleY));
	menu->addChild(goGambleItem, 1, GAMBLE);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/startLucky.plist");
	CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("gambleStart%d.png", 2, 0.2f);
	CCSprite *spriteGoL = CCSprite::createWithSpriteFrameName("gambleStart0.png");
	goGambleItem->addChild(spriteGoL);
	spriteGoL->setPosition(goGambleItem->getContentSize() / 2);
	spriteGoL->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
	bool g0First = CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble0", false);
	bool g1First = CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble1", false);
	bool g2First = CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble2", false);

	if(!(g0First && g1First && g2First))
	{
		CCArray *array = CCArray::create();

		for(int i = 0; i < 2; i++)
		{
			char buf[64];
			sprintf(buf, "firstGet%d.png", i);
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
			array->addObject(frame);
		}

		CCAnimation *firstGetArrayAni = CCAnimation::createWithSpriteFrames(array, 0.1f);
		CCSprite *spriteGet = CCSprite::createWithSpriteFrameName("firstGet0.png");
		this->addChild(spriteGet, 121, 3344);
		spriteGet->setPosition(ccp(55*gScaleX, 140*gScaleY));

		if(firstGetArrayAni != NULL)
		{
			spriteGet->runAction(CCRepeatForever::create(CCAnimate::create(firstGetArrayAni)));
		}
	}

	SystemTipNode *node = SystemTipNode::create();
	this->addChild(node, 1113);
	node->setParentType(0);

	setKeypadEnabled(true);

	return true;
}

void MainMenu::touchMenu(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag();

	switch(tag)
	{
		case STARTGAME:
			toLevelChoice(pSender);
			break;

		case MUSIC_SETTING:
			toMusicSetting(pSender);
			break;

		case SOUND_SETTING:
			toSoundSetting(pSender);
			break;

		case GAMBLE:
			goGamble(pSender);

		default:
			break;
	}
}

void MainMenu::toLevelChoice(CCObject *pSender)
{
	CCScene *scene = CCScene::create();
	LevelChoice *gLayer = LevelChoice::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void MainMenu::toSetting(CCObject *pSender)
{

}

void MainMenu::toMusicSetting(CCObject *pSender)
{
	if(GameConfigData::initialGameData().getCurMusicState())
	{
		GameConfigData::initialGameData().setMusicState(false);
		ToolsFun::sharedTools()->setMusicEnable(false);
	}
	else
	{
		GameConfigData::initialGameData().setMusicState(true);
		ToolsFun::sharedTools()->setMusicEnable(true);
	}

	CCSprite *spriteNml = NULL;
	CCSprite *spriteSel = NULL;

	if(GameConfigData::initialGameData().getCurMusicState())
	{
		spriteNml = CCSprite::create("ui/mainUI/on.png");
		spriteSel = CCSprite::create("ui/mainUI/on.png");
	}
	else
	{
		spriteNml = CCSprite::create("ui/mainUI/off.png");
		spriteSel = CCSprite::create("ui/mainUI/off.png");
	}

	CCMenuItemSprite *item = (CCMenuItemSprite*)pSender;
	item->setNormalImage(spriteNml);
	item->setSelectedImage(spriteSel);
	GameConfigData::initialGameData().writeData();
}
void MainMenu::toSoundSetting(CCObject *pSender)
{
	if(GameConfigData::initialGameData().getCurSoundEffectState())
	{
		GameConfigData::initialGameData().setCurSoundEffectState(false);
		ToolsFun::sharedTools()->setEffectEnable(false);
	}
	else
	{
		GameConfigData::initialGameData().setCurSoundEffectState(true);
		ToolsFun::sharedTools()->setEffectEnable(true);
	}

	CCSprite *spriteNml = NULL;
	CCSprite *spriteSel = NULL;

	if(GameConfigData::initialGameData().getCurSoundEffectState())
	{
		spriteNml = CCSprite::create("ui/mainUI/on.png");
		spriteSel = CCSprite::create("ui/mainUI/on.png");
	}
	else
	{
		spriteNml = CCSprite::create("ui/mainUI/off.png");
		spriteSel = CCSprite::create("ui/mainUI/off.png");
	}

	CCMenuItemSprite *item = (CCMenuItemSprite*)pSender;
	item->setNormalImage(spriteNml);
	item->setSelectedImage(spriteSel);
	GameConfigData::initialGameData().writeData();
}

void MainMenu::toHelp(CCObject *pSender)
{

}

void MainMenu::goGamble(CCObject *pSender)
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void MainMenu::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(MainMenu::goGamble));
	this->addChild(layer,10000);
}

void MainMenu::goGamble()
{
	goGamble(NULL);
}