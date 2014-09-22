#include "DailyLogIn.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
DailyLogin::DailyLogin()
{

}

DailyLogin::~DailyLogin()
{

}

bool DailyLogin::init()
{
	setTouchEnabled(true);
	CCSize size=CCDirector::sharedDirector()->getWinSize();

	//背景
	CCSprite *bkg = CCSprite::create("ui/shared/bkg.png");
	this->addChild(bkg,-1);
	bkg->setPosition(CCDirector::sharedDirector()->getWinSize()/2  );

	//每日登陆面板
	CCSprite *login_bkg = CCSprite::create("ui/dailyRewardUI/login_bkg.png");
	this->addChild(login_bkg,0);
	login_bkg->setPosition(ccp(size.width/2,size.height-300*gScaleY));

	//每日登陆图标
	m_menu = CCMenu::create();
	this->addChild(m_menu);
	m_menu->setPosition(CCPointZero);
	const char *fileArray[7] = {"ui/dailyRewardUI/Login_day0.png",
		"ui/dailyRewardUI/Login_day1.png",
		"ui/dailyRewardUI/Login_day2.png",
		"ui/dailyRewardUI/Login_day3.png",
		"ui/dailyRewardUI/Login_day4.png",
		"ui/dailyRewardUI/Login_day5.png",
		"ui/dailyRewardUI/Login_day6.png"
	};
	const char*  moneyReward[7] = {"60"
		,"100"
		,"150"
		,"30"
		,"50"
		,"80"
		,"50"
	};
	CCPoint posArray[7] = {ccp(86*gScaleX,540*gScaleY),ccp(186*gScaleX,540*gScaleY),ccp(286*gScaleX,540*gScaleY),ccp(386*gScaleX,540*gScaleY),ccp(86*gScaleX,380*gScaleY),ccp(186*gScaleX,380*gScaleY),ccp(335*gScaleX,380*gScaleY)};
	for (int i=0;i!=7;++i)
	{
		CCSprite *spriteNml = CCSprite::create(fileArray[i]);
		CCSprite *spriteSel = CCSprite::create(fileArray[i]);
		CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml,NULL,NULL);
		m_menu->addChild(itemSprite,1,100+i);
		itemSprite->setPosition(posArray[i]);
		itemSprite->setEnabled(false);
	}
	//领取按钮
	CCSprite *spriteNml = CCSprite::create("ui/dailyRewardUI/login_get0.png");
	CCSprite *spriteSel = CCSprite::create("ui/dailyRewardUI/login_get0.png");
	spriteSel->setColor(ccc3(100,100,100));
	CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml,spriteSel,this,menu_selector(DailyLogin::gainReward));
	m_menu->addChild(itemSprite,1);
	m_menu->setTouchPriority(-201);
	itemSprite->setPosition(ccp(login_bkg->getPositionX(),login_bkg->getPositionY()-230*gScaleY));

	//标志当前可获取
	setCurrentReward();

	//标记已获取奖励
	setcurrentGain();
	return true;
}


void DailyLogin::setCurrentReward()
{
	int curindex = GameConfigData::initialGameData().getcurrentReIndex();
	CCMenuItemSprite *itemSprite = (CCMenuItemSprite*)m_menu->getChildByTag(curindex + 100);
	itemSprite->setEnabled(true);
	m_menu->setTag(100 + curindex);

	if(curindex==6)
	{  
		CCSprite *sLight = CCSprite::create("ui/dailyRewardUI/bigSelect.png");
		itemSprite->addChild(sLight,280);
		sLight->setPosition(itemSprite->getContentSize()/2);
	}
	else
	{
		CCSprite *sLight = CCSprite::create("ui/dailyRewardUI/smallSelect.png");
		itemSprite->addChild(sLight,280);
		sLight->setPosition(itemSprite->getContentSize()/2 );
	}

}

void DailyLogin::gainReward(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	int curindex = GameConfigData::initialGameData().getcurrentReIndex();
		const char*  moneyReward[6] = {
			"500"
			,"800"
			,"1200"
			,"2000"
			,"3000"
			,"4000"
	};
	int index = m_menu->getTag() - 100;
	int money = atoi(moneyReward[index]);
	if (index < 6)
	{
		int addMoney = GameConfigData::initialGameData().getCoinValue() + money;
		GameConfigData::initialGameData().setCoinValue(addMoney);
	}
	else
	{
		int reviveCount =GameConfigData::initialGameData().getReviveValue()+3;
		GameConfigData::initialGameData().setReviveValue(reviveCount);

		int addMoney = GameConfigData::initialGameData().getCoinValue() + 5000;
		GameConfigData::initialGameData().setCoinValue(addMoney);
		
	}
	GameConfigData::initialGameData().setlastDate(ToolsFun::sharedTools()->getSystemData());
	GameConfigData::initialGameData().setlastMonth(ToolsFun::sharedTools()->getSystemMonth());
	if(curindex==2||curindex==4||curindex==6){
		GameConfigData::initialGameData().setDailyPurchase(1);
	}
	else{
		GameConfigData::initialGameData().setDailyPurchase(0);
	}
	curindex++;
	if (curindex==7)
	{
		curindex=0;
	}
	ToolsFun::sharedTools()->playEffect(BUTTON);
	GameConfigData::initialGameData().setcurrentReIndex(curindex);
	GameConfigData::initialGameData().writeData();

	//mainLayer->backFromDailyLogin();
	if(m_target)
	{
		(m_target->*m_callFun)(pSender);
		this->removeFromParent();
	}
}

void DailyLogin::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, true);
}

bool DailyLogin::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

void DailyLogin::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void DailyLogin::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void DailyLogin::setcurrentGain()
{
	int curindex = GameConfigData::initialGameData().getcurrentReIndex()-1;
	m_menu->setTag(100 + curindex+1);
	int i=0;
	while(i<=curindex)
	{
		CCSprite *sprite = CCSprite::create("ui/dailyRewardUI/currentGain.png");
		CCMenuItemSprite *itemSprite = (CCMenuItemSprite*)m_menu->getChildByTag(i + 100);
		sprite->setScale(0.6f);
		itemSprite->addChild(sprite,281);
		if(curindex==6)
		{
			sprite->setPosition(itemSprite->getContentSize()/2 +ccp(79*gScaleX,46*gScaleY));
		}
		else{
			sprite->setPosition(itemSprite->getContentSize()/2 +ccp(31*gScaleX,47*gScaleY));
		}
		i++;
	}
}

void DailyLogin::setCloseCallBack(CCObject *pSender,SEL_MenuHandler callFun)
{
	m_target = pSender;
	m_callFun = callFun;
}