#include "GameOver.h"
#include "GameLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "MainMenu.h"
#include "../SpriteNode/HeroSprite.h"
#include "../GCData/DataInitial.h"
#include "../GCData/GameConfigData.h"
#include "LevelChoice.h"

GameOver::GameOver():m_menu(NULL)
{
	m_MaxStar = 0;
	m_layer = GameLayer::sharedGameLayer();
	m_currentLevel = GameConfigData::initialGameData().getCurLevel();
}

GameOver::~GameOver()
{

}
#define STAR 245

bool GameOver::initWin()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	CCSprite* bkg = CCSprite::create("ui/gameover/end_box.png");
	bkg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);

	CCSprite* scorebkg = CCSprite::create("ui/gameover/end_score_bg.png");
	scorebkg->setPosition(ccp(bkg->getContentSize().width*5/7-10*gScaleX,bkg->getContentSize().height/2-5*gScaleY));

	CCSprite* text_pass = CCSprite::create("ui/gameover/text_pass.png");
	text_pass->setPosition(ccp(scorebkg->getContentSize().width/2,scorebkg->getContentSize().height));

	
	for (int i=1;i<=3;i++)
	{
		CCSprite *star = CCSprite::create("ui/gameover/star.png");
		star->setPosition(ccp(scorebkg->getContentSize().width*i/4,scorebkg->getContentSize().height-80*gScaleY));
		star->setTag(STAR + i);
		scorebkg->addChild(star);
		star->setVisible(false);
	}
	
	//根据星级显示星星
 	int curstar = GameLayer::sharedGameLayer()->getCurStar();
	if (curstar >= m_MaxStar)
	{
		m_MaxStar = curstar;
	}
	for (int j =1; j < curstar + 1; j++ )
	{	
		if (curstar == 0)
		{
		}
		else
		{
			CCScaleTo * fin =CCScaleTo::create(1.0f,1.5);
			CCScaleTo * fin1 = CCScaleTo::create(1.0f,1);
			CCArray *arr = CCArray::create();
			arr->addObject(fin);
			arr->addObject(fin1);
			CCSequence * seq = CCSequence::create(arr);
			scorebkg->getChildByTag(STAR + j )->setVisible(true);
			scorebkg->getChildByTag(STAR + j )->runAction(seq);
		}		
	}

	bkg->addChild(scorebkg,1);
	scorebkg->addChild(text_pass);

	int scorevalue = GameLayer::sharedGameLayer()->getscore();
	char scoreStr[33];
	sprintf(scoreStr,"%d 分",scorevalue);

	int store = atoi(GameConfigData::initialGameData().getHScores().c_str());

	if(scorevalue>store)
	{
		GameConfigData::initialGameData().setHScores(scorevalue);
	}

	CCLabelBMFont *m_gainScore = CCLabelBMFont::create("0"  ,"font/white.fnt");
	m_gainScore->setPosition(ccp(scorebkg->getContentSize().width*2/4-20*gScaleX,scorebkg->getContentSize().height-130*gScaleY));
	m_gainScore->setString(scoreStr);
	scorebkg->addChild(m_gainScore,UI_ZORDER);


// 	CCLabelBMFont *m_Score = CCLabelBMFont::create("n","font/white.fnt");
// 	m_Score->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-130));
// 	scorebkg->addChild(m_Score,UI_ZORDER);
// 	
// 	CCLabelBMFont *m_gainLen = CCLabelBMFont::create("0","font/white.fnt");
// 	m_gainLen->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-160));
// 	scorebkg->addChild(m_gainLen,UI_ZORDER);
	int length_value = GameLayer::sharedGameLayer()->getdistance();
	char lenStr[33];
	sprintf(lenStr,"%d m",length_value);
	CCLabelBMFont *m_len = CCLabelBMFont::create("0","font/white.fnt");
	m_len->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-160*gScaleY));
	m_len->setString(lenStr);
	scorebkg->addChild(m_len,UI_ZORDER);
	
	int dis_length = atoi(GameConfigData::initialGameData().getHLenght().c_str());
	if(length_value>dis_length)
	{
		GameConfigData::initialGameData().setHLength(length_value);
	}

	int money_value = GameLayer::sharedGameLayer()->getmoney();
	char moneyStr[33];
	sprintf(moneyStr,"%d m",money_value);
	CCLabelBMFont *m_gainIcon = CCLabelBMFont::create("0","font/gold_big.fnt");
	m_gainIcon->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-200*gScaleY));
	m_gainIcon->setString(moneyStr);
	scorebkg->addChild(m_gainIcon,UI_ZORDER);

	int curLevel = GameConfigData::initialGameData().getCurLevel();
	int curKillTotal = GameLayer::sharedGameLayer()->getKillMonTotal();
	int killTotal = atoi(GameConfigData::initialGameData().getKillTotal().c_str());

	GameConfigData::initialGameData().setKillTotal(curKillTotal+killTotal);

	int count = GameConfigData::initialGameData().getCoinValue()+GameLayer::sharedGameLayer()->getmoney();
	GameConfigData::initialGameData().setCoinValue(count);
	
// 	CCLabelBMFont *m_icon = CCLabelBMFont::create("分","font/white.fnt");
// 	m_icon->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-200));
// 	scorebkg->addChild(m_icon,UI_ZORDER);

	this->addChild(bkg);

	int curHeroId=GameConfigData::initialGameData().getCurHeroId();
	char heroStr[33];
	sprintf(heroStr,"ui/herochoice/heroPortrait_%d.png",curHeroId);
	CCSprite* hero=CCSprite::create(heroStr);
	hero->setScale(0.7f);
	bkg->addChild(hero,0);
	hero->setPosition(scorebkg->getPosition()+ccp(-170*gScaleX,0));
	
	/*解锁相关关卡 */
	if(DataInitial::initialDataSource().getLvTag() == LEVEL)
	{
	int level = GameConfigData::initialGameData().getCurLevel();
	int max_level = GameConfigData::initialGameData().getMaxUnLockLevel();
	if(level>=0&&level<=max_level)
	{
		//&&GameConfigData::initialGameData().getStartCout()>=DataInitial::initialDataSource().getLeveUnLockInf(leve)
		if (GameConfigData::initialGameData().getLevelStateByLevel(level+1)=="0")
		{
			GameConfigData::initialGameData().setMaxUnLockLevel();
		}		
		//此处的 5  应该是个最大星数
		GameConfigData::initialGameData().setLevelStar(level,m_MaxStar);
		GameConfigData::initialGameData().setLevelState(level,"2");
		GameConfigData::initialGameData().setCurLevel(++level);
		GameConfigData::initialGameData().setLevelState(level,"1");
}
	}
	//支线关卡解锁

	//change
	int starnum = GameConfigData::initialGameData().getStartCout();
	vector<LevelUnLock*> sublv = DataInitial::initialDataSource().getSubUnLockConfigs();
	vector<LevelUnLock*>::iterator it ;
	/*int rewardpos[] = {1,5,8,11,14,17,20,23};*/
	for (it = sublv.begin() ;it != sublv.end();it++)
	{
		int unvalue = (*it)->mUnLockValue1;
		int prelvstate = atoi(GameConfigData::initialGameData().getLevelStateByLevel(unvalue).c_str());
		if( prelvstate  && starnum  >= (*it)->mUnLockValue0 )
		{			
				GameConfigData::initialGameData().setSubLevelState((*it)->mLevelId,"1");
				GameConfigData::initialGameData().setCurSubLevel((*it)->mLevelId);
				//GameConfigData::initialGameData().setSubLevelState((*it)->mLevelId,"2");	
		}
	}
	int curlv_id = GameConfigData::initialGameData().getCurSubLevel();
	if (DataInitial::initialDataSource().getLvTag() && GameConfigData::initialGameData().getSubLevelStateByLevel(curlv_id) == "1")
	{
			GameConfigData::initialGameData().setSubLevelState(curlv_id,"2");
	}
	
	//奖励关卡解锁
	vector<RewardConfig*> reward = DataInitial::initialDataSource().getRewardConfigs();
	vector<RewardConfig*>::iterator iter ;
	/*int rewardpos[] = {1,5,8,11,14,17,20,23};*/
	
	 for (iter = reward.begin() ;iter != reward.end();iter++)
	 {
		 int unrewardvalue = (*iter)->mUnLockValue0;
		 if (GameConfigData::initialGameData().getRewardStateByReward((*iter)->mRewardId) != "2")
		 {
			 if((*iter)->mUnLockType == LEVEL  && GameConfigData::initialGameData().getMaxUnLockLevel()>unrewardvalue)
			 {
				 GameConfigData::initialGameData().setRewardState((*iter)->mRewardId,"1");
			 }
			 if (unrewardvalue<3)
			 {
				 if((*iter)->mUnLockType == SUB_LEVEL && GameConfigData::initialGameData().getSubLevelStateByLevel(unrewardvalue) == "2")
				 {
					 GameConfigData::initialGameData().setRewardState((*iter)->mRewardId,"1");
				 }
			 }
		 }
	 }
	GameConfigData::initialGameData().writeData();
		
	//按钮事件
	m_menu = CCMenu::create();
	CCPoint posArray[10] = {ccp(bkg->getContentSize().width/4,20*gScaleY),ccp(bkg->getContentSize().width/2,20*gScaleY),ccp(bkg->getContentSize().width*3/4,20*gScaleY)};
	const char *filename [10] = {"ui/gameover/goBackNml.png","ui/gameover/retryNml.png","ui/gameover/resumeNml.png"};
	const char *filename2 [10] = {"ui/gameover/goBackNml.png","ui/gameover/retryNml.png","ui/gameover/resumeNml.png"};

	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(22);

	for (int i=0;i!=3;++i)
	{
		CCSprite *spriteNml = CCSprite::create(filename[i]);
		CCSprite *spriteSel = CCSprite::create(filename2[i]);
		CCMenuItemSprite *spriteItem = CCMenuItemSprite::create(spriteNml,spriteSel,this,menu_selector(GameOver::menuCall));
		m_menu->addChild(spriteItem,10,100+i);
		spriteItem->setPosition(posArray[i]);
	}
	bkg->addChild(m_menu,1);
	m_menu->setPosition(CCPointZero);

	return true;
}

bool GameOver::initLose()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	CCSprite* bkg = CCSprite::create("ui/gameover/end_box.png");
	bkg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);

	CCSprite* scorebkg = CCSprite::create("ui/gameover/end_score_bg.png");
	scorebkg->setPosition(ccp(bkg->getContentSize().width*5/7-10*gScaleX,bkg->getContentSize().height/2-5*gScaleY));

	CCLabelBMFont *tip = CCLabelBMFont::create(gGetLanguageDesc(K.STORE_TIP).c_str(),"font/white.fnt");
	tip->setAnchorPoint(CCPointZero);
	scorebkg->addChild(tip);
	tip->setPosition(ccp(50*gScaleX,250*gScaleY));

	/*
	CCLabelBMFont *m_gainScoreLabel = CCLabelBMFont::create("得分"  ,"font/white.fnt");
	m_gainScoreLabel->setPosition(ccp(scorebkg->getContentSize().width*2/4-20*gScaleX,scorebkg->getContentSize().height-80*gScaleY));
	scorebkg->addChild(m_gainScoreLabel,UI_ZORDER);*/

	int scorevalue = GameLayer::sharedGameLayer()->getscore();
	char scoreStr[33];
	sprintf(scoreStr,"%d",scorevalue);

	CCLabelBMFont *m_gainScore = CCLabelBMFont::create("0"  ,"font/white.fnt");
	m_gainScore->setPosition(ccp(scorebkg->getContentSize().width*2/4-20*gScaleX,scorebkg->getContentSize().height-130*gScaleY));
	m_gainScore->setString(scoreStr);
	scorebkg->addChild(m_gainScore,UI_ZORDER);

	int length_value = GameLayer::sharedGameLayer()->getdistance();
	char lenStr[33];
	sprintf(lenStr,"%d m",length_value);
	CCLabelBMFont *m_len = CCLabelBMFont::create("0","font/white.fnt");
	m_len->setPosition(ccp(scorebkg->getContentSize().width*2/4,scorebkg->getContentSize().height-170*gScaleY));
	m_len->setString(lenStr);
	scorebkg->addChild(m_len,UI_ZORDER);


	int money_value = GameLayer::sharedGameLayer()->getmoney();
	char moneyStr[33];
	sprintf(moneyStr,"%d",money_value);
	CCLabelBMFont *m_gainIcon = CCLabelBMFont::create("0","font/gold_big.fnt");
	m_gainIcon->setPosition(ccp(scorebkg->getContentSize().width*2/4-10*gScaleX,scorebkg->getContentSize().height-220*gScaleY));
	m_gainIcon->setString(moneyStr);
	scorebkg->addChild(m_gainIcon,UI_ZORDER);

	CCSprite *coin  =CCSprite::create("ui/shared/coin.png");
	scorebkg->addChild(coin);
	coin->setPosition(m_gainIcon->getPosition()+ccp(m_gainIcon->getContentSize().width/2+10*gScaleX,-3*gScaleY));

	bkg->addChild(scorebkg,1);
	this->addChild(bkg);

	CCLabelBMFont *m_tip = CCLabelBMFont::create("","font/white.fnt");
	m_tip->setPosition(scorebkg->getPosition()+ccp(-200*gScaleX,130*gScaleY));
	m_tip->setString(gGetLanguageDesc(K.FAILED_TIP).c_str());
	bkg->addChild(m_tip,0);
	m_tip->setScale(0.6f);

	CCLabelBMFont *m_tipContent = CCLabelBMFont::create("","font/white.fnt",150);
	m_tipContent->setLineBreakWithoutSpace(true);
	m_tipContent->setAnchorPoint(CCPointZero);
	m_tipContent->setPosition(scorebkg->getPosition()+ccp(-260*gScaleX,40*gScaleY));
	m_tipContent->setString(gGetLanguageDesc(K.FAILED_CONTENT).c_str());
	bkg->addChild(m_tipContent,0);
	m_tipContent->setScale(0.6f);


	//按钮事件
	m_menu = CCMenu::create();
	CCPoint posArray[10] = {ccp(bkg->getContentSize().width/3,20*gScaleY),ccp(bkg->getContentSize().width*2/3,20*gScaleY)};
	const char *filename [10] = {"ui/gameover/goBackNml.png","ui/gameover/retryNml.png"};
	const char *filename2 [10] = {"ui/gameover/goBackNml.png","ui/gameover/retryNml.png"};

	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(22);

	for (int i=0;i!=2;++i)
	{
		CCSprite *spriteNml = CCSprite::create(filename[i]);
		CCSprite *spriteSel = CCSprite::create(filename2[i]);
		CCMenuItemSprite *spriteItem = CCMenuItemSprite::create(spriteNml,spriteSel,this,menu_selector(GameOver::menuCall));
		m_menu->addChild(spriteItem,10,100+i);
		spriteItem->setPosition(posArray[i]);
	}
	bkg->addChild(m_menu,1);
	m_menu->setPosition(CCPointZero);

	return true;
}

void GameOver::menuCall(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag();
	switch (tag)
	{
	case 100:
		back(pSender);
		break;
	case 101:
		retryGame(pSender);
		break;
	case 102:
		resume(pSender);
		break;
	default:
		break;
	}
}

void GameOver::resume(CCObject *pSender)
{
	m_layer->quitLevel();
}

void GameOver::back(CCObject *pSender)
{
	m_layer->quitLevel();
}

void GameOver::retryGame(CCObject *pSender)
{
	GameConfigData::initialGameData().setCurLevel(m_currentLevel);
	m_layer->retryLevel();
}

