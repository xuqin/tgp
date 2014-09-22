#include "GamePause.h"
#include "GameLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "MainMenu.h"
#include "../SpriteNode/HeroSprite.h"

#include "../GCData/GameConfigData.h"

GamePause::GamePause():m_menu(NULL)
{
	m_layer = GameLayer::sharedGameLayer();
}

GamePause::~GamePause()
{

}

bool GamePause::init()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	CCSprite* bkg = CCSprite::create("ui/pause/dialogbox.png");
	bkg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
	this->addChild(bkg);

	CCSprite* word=CCSprite::create("ui/pause/word.png");
	bkg->addChild(word);
	word->setPosition(bkg->getContentSize()/2+ccp(0,-60*gScaleY));


	m_menu = CCMenu::create();
	CCPoint posArray[10] = 
	{
		ccp(bkg->getContentSize().width/4,50*gScaleY),
		ccp(bkg->getContentSize().width*3/4-40*gScaleX,50*gScaleY),

	};
	const char *filename [2] = 
	{
		"ui/pause/confirm.png",
		"ui/shared/cancel.png",
	};

	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(22);
	//初始化resume retry quit 
	for (int i=0;i!=2;++i)
	{
		CCSprite *spriteNml = CCSprite::create(filename[i]);
		CCSprite *spriteSel = CCSprite::create(filename[i]);
		CCMenuItemSprite *spriteItem = CCMenuItemSprite::create(spriteNml,spriteSel,this,menu_selector(GamePause::menuCall));
		m_menu->addChild(spriteItem,10,100+i);
		spriteItem->setPosition(posArray[i]);
	}

	bkg->addChild(m_menu,1);
	m_menu->setPosition(CCPointZero);

	return true;
}

void GamePause::menuCall(CCObject *pSender)
{ToolsFun::sharedTools()->playEffect(BUTTON);
	//SimpleAudioEngine::sharedEngine()->playEffect("music/transition.mp3");
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag();
	switch (tag)
	{
	case 100:
		back(pSender);
		break;
	case 101:
		resume(pSender);
		break;

	default:
		break;
	}
	GameConfigData::initialGameData().writeData();
}

void GamePause::resume(CCObject *pSender)
{
	m_layer->resumeLevel(this);
}

void GamePause::back(CCObject *pSender)
{
	m_layer->quitLevel();
}

void GamePause::retryGame(CCObject *pSender)
{
	m_layer->retryLevel();
}

