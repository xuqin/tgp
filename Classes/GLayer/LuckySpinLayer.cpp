#include "LuckySpinLayer.h"

#include "../SpriteNode/HeroSprite.h"
#include "ShopLayer.h"
#include "../GLayer/MainMenu.h"
#include "../NDKHelper/NDKHelper.h"
#include "../ToolFun/StringKey.h"
#include "../ToolFun/ToolsFun.h"
#include "../ToolFun/JniCall.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "../GCData/GameConfigData.h"
#include "BackLayer.h"

float gValuePercent[18] =
{
	0.20f, 12.38f, 2.06f, 6.88f, 1.37f, /*305*/
	12.38f, 0.34f, 8.25f, 12.38f, 3.44f, /*447*/
	3.44f, 6.88f, 10.32f, 1.37f, 8.25f, /*507*/
	0.68f, 10.32f, 2.06f /*458*/
};

string gValueRewardGolden[18] =
{
	"true", "500", "10000", "1000", "88888",
	"500", "true", "1", "500", "true",
	"5000", "1000", "800", "88888", "1",
	"6", "800", "10000"
};

LuckySpinLayer::LuckySpinLayer(): m_rotateCount(4), m_playCoinAndDiaType(0), m_pTarget(NULL), m_fun(NULL)
{
	m_data = LuckyCofig(0, "0");
}

LuckySpinLayer::~LuckySpinLayer()
{
}

bool LuckySpinLayer::init()
{
	m_spinCount = GameConfigData::initialGameData().getUserCount();
	int luckycount = GameConfigData::initialGameData().getLuckyCount();
	//CCLog("usercount:"+luckycount);
	m_menu = NULL;
	m_rewardIndex = -1;
	m_pTarget = NULL;
	m_fun = NULL;
	m_rewardIndex = 0;
	m_callFun = NULL;
	this->initUI();

	return true;
}

void LuckySpinLayer::initUI()
{
	this->setTouchEnabled(true);
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	//背景
	CCSprite * bkg = CCSprite::create("ui/luckyspin/luckybkg.jpg");
	this->addChild(bkg, -4);
	bkg->setPosition(size / 2);

	//头部
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop, 0);
	spBkgTop->setPosition(ccp(size.width / 2, size.height - spBkgTop->getContentSize().height / 2));


	//金币放置最上方商城
	CCSprite *coinIcon = CCSprite::create("ui/shared/coin.png");
	this->addChild(coinIcon, 2, 1429);
	coinIcon->setPosition(ccp(280 * gScaleX, size.height - 30 * gScaleY));
	m_coinLabel = CCLabelBMFont::create("10000000",GOLD_FONT);
	this->addChild(m_coinLabel, 4);
	m_coinLabel->setAnchorPoint(ccp(0, 0.5f));
	m_coinLabel->setPosition(ccp(300 * gScaleX, size.height - 25 * gScaleY));
	m_coinLabel->setScale(0.6F * gScaleX);

	setCoin();

	m_menu = CCMenu::create();
	m_menu->setTouchPriority(-130);
	m_menu->setPosition(CCPointZero);
	CCSprite *spNml = CCSprite::create("ui/shared/back.png");
	CCSprite *spSel = CCSprite::create("ui/shared/back.png");
	spSel->setColor(ccc3(100, 100, 100));
	CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml, spSel,
	                           this, menu_selector(LuckySpinLayer::back));
	m_menu->addChild(spItem);
	spItem->setPosition(ccp(37 * gScaleX, size.height - 37 * gScaleY));

	//show shop
	spNml = CCSprite::create("ui/shared/coinplus.png");
	spSel = CCSprite::create("ui/shared/coinplus.png");
	spSel->setColor(ccc3(100, 100, 100));
	spItem = CCMenuItemSprite::create(spNml, spSel,
	                                  this, menu_selector(LuckySpinLayer::showShopLayer));
	m_menu->addChild(spItem);
	spItem->setPosition(ccp(430 * gScaleX, size.height - 30 * gScaleY));

	m_rewardIndex = atoi(CCUserDefault::sharedUserDefault()->getStringForKey("rewardIndex", "0").c_str());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/sprinPointer.plist");
	CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("sprinPointer%d.png", 0, 0.2f);
	m_spinSprite = CCSprite::createWithSpriteFrameName("sprinPointer0.png");
	this->addChild(m_spinSprite, 5);
	m_spinSprite->setPosition(getPosByIndex(m_rewardIndex) - ccp(1 * gScaleX, 0));
	m_spinSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

	const char *fileArrayNml[2] = {"ui/luckyspin/Lucky_btn0.png"
	                              };

	const char *fileArraySel[2] = {"ui/luckyspin/Lucky_btn0.png"
	                              };

	this->addChild(m_menu, 3);
	m_menu->setPosition(CCPointZero);
	m_menu->setTouchPriority(-201);

	CCPoint posArray[1] = {ccp(235 * gScaleX, 430 * gScaleY)};

	for(int i = 0; i != 1; ++i)
	{
		CCSprite *spriteNml = CCSprite::create(fileArrayNml[i]);
		CCSprite *spriteSel = CCSprite::create(fileArraySel[i]);
		spriteSel->setColor(ccc3(100, 100, 100));
		CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel,
		                               this, menu_selector(LuckySpinLayer::startSpin));
		m_menu->addChild(itemSprite, 1, 100 + i);
		itemSprite->setPosition(posArray[i]);
	}

	CCSprite *lastTimeBkg = CCSprite::create("ui/luckyspin/lastTime.png");
	this->addChild(lastTimeBkg, 1);
	lastTimeBkg->setPosition(ccp(210 * gScaleX, 500 * gScaleY));

	CCSprite *speUpNml = CCSprite::create("ui/luckyspin/sped_up.png");
	CCSprite *speUpSel = CCSprite::create("ui/luckyspin/sped_up.png");
	spSel->setColor(ccc3(100, 100, 100));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(speUpNml, speUpSel,
	                           this, menu_selector(LuckySpinLayer::canSpeedUp));


	CCSprite *itemSel = CCSprite::create("ui/luckyspin/speed_sel.png");
	itemSp->addChild(itemSel, 1, 1234);
	itemSel->setScale(0.5);
	itemSel->setPosition(ccp(58 * gScaleX, 16 * gScaleY));
	m_menu->addChild(itemSp);
	itemSp->setPosition(ccp(280 * gScaleX, 560 * gScaleY));

	int count = GameConfigData::initialGameData().getLuckyCount();
	char buf[32];
	sprintf(buf, "%d", count);

	m_lastSpinCount = CCLabelBMFont::create("100000000", GOLD_FONT);
	this->addChild(m_lastSpinCount, 3);
	m_lastSpinCount->setString(buf);
	m_lastSpinCount->setPosition(ccp(277 * gScaleX, 505 * gScaleY));
	m_lastSpinCount->setAnchorPoint(ccp(0, 0.5f));

	for(int i = 0; i != 18; ++i)
	{
		CCPoint pos = getPosByIndex(i);
		CCSprite *sp = NULL;

		if(i == 0)
		{
			sp = CCSprite::create("ui/luckyspin/hero2.png");
		}
		else if(i == 6)
		{
			sp = CCSprite::create("ui/luckyspin/hero1.png");
		}
		else if(i == 9)
		{
			sp = CCSprite::create("ui/luckyspin/pet5.png");
		}
		else if(i == 15)
		{
			sp = CCSprite::create("ui/luckyspin/shielding.png");
		}
		else if(i == 1 || i == 5 || i==8)
		{
			sp = CCSprite::create("ui/luckyspin/500.png");
		}
		else if(i == 2 || i==17)
		{
			sp = CCSprite::create("ui/luckyspin/10000.png");
		}
		else if(i == 3 || i == 11)
		{
			sp = CCSprite::create("ui/luckyspin/1000.png");
		}
		else if(i == 4 || i == 13)
		{
			sp = CCSprite::create("ui/luckyspin/88888.png");
		}
		else if(i == 12 || i == 16 )
		{
			sp = CCSprite::create("ui/luckyspin/800.png");
		}
		else if(i ==7 || i==14)
		{
			sp = CCSprite::create("ui/luckyspin/again.png");
		}
		else if(i ==10)
		{
			sp = CCSprite::create("ui/luckyspin/5000.png");
		}

		this->addChild(sp, 2);

		sp->setPosition(pos);
	}

	m_buyTimeMenu = CCMenu::create();
	this->addChild(m_buyTimeMenu, 1);
	m_buyTimeMenu->setPosition(CCPointZero);
	m_buyTimeMenu->setTouchPriority(-201);
	CCPoint posArray1[3] = {ccp(240 * gScaleX, 350 * gScaleY), ccp(240 * gScaleX, 290 * gScaleY), ccp(240 * gScaleX, 230 * gScaleY)};
	//int price[3] = {4, 6, 8};

	for(int i = 0; i != 3; ++i)
	{
		char filenames[33];
		sprintf(filenames, "ui/luckyspin/buyGambleTime%d.png", i);
		CCSprite *spriteNml = CCSprite::create(filenames);
		CCSprite *spriteSel = CCSprite::create(filenames);
		spriteSel->setColor(ccc3(100, 100, 100));
		CCMenuItemSprite *itemSprite = CCMenuItemSprite::create(spriteNml, spriteSel, this, menu_selector(LuckySpinLayer::buyCount));
		m_buyTimeMenu->addChild(itemSprite, 1, 100 + i);
		itemSprite->setPosition(posArray1[i]);

		/*char buf[32];
		sprintf(buf,"%s%d",gLgeValue(K.RMB).c_str(),price[i]);
		CCLabelTTF *label = CCLabelTTF::create(buf,"微软雅黑",38);
		itemSprite->addChild(label);
		label->setScale(0.6f);
		label->setColor(ccc3(220 ,220 ,220));
		label->setPosition(ccp(itemSprite->getContentSize().width/2,itemSprite->getContentSize().height + 7));*/

	}

	m_buyTimeMenu->setEnabled(false);
	m_buyTimeMenu->setVisible(false);

	if(count <= 0)
	{
		m_buyTimeMenu->setEnabled(true);
		m_buyTimeMenu->setVisible(true);
		CCNode * node = m_menu->getChildByTag(100);
		CCMenuItemSprite *item = (CCMenuItemSprite*)node;
		item->setVisible(false);
		item->setEnabled(false);
	}

	bool firstStart = CCUserDefault::sharedUserDefault()->getBoolForKey("firstStar", true);

	if(firstStart)
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("firstStar", false);
	}

	m_speedUp = CCUserDefault::sharedUserDefault()->getBoolForKey("speedUp", false);

	if(m_speedUp)
	{
		CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.5f);
		itemSel->setVisible(true);
		m_rotateCount = 1;
	}
	else
	{
		itemSel->setVisible(false);
	}

	setKeypadEnabled(true);

	registerNDK();
}

void LuckySpinLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();

	if(jniNode->getParent())
	{
		jniNode->removeFromParent();
	}

	this->addChild(jniNode);
}

void LuckySpinLayer::buyCount(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	int i = ((CCNode*)pSender)->getTag() - 100;

	int priceArray[3] =  {4, 5, 6};

	int purchaseIndex = priceArray[i];
	JniCall::sharedJniCall()->setBuyFailedCallBack(this, callFunStr_selector(LuckySpinLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this, callFunStr_selector(LuckySpinLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(purchaseIndex);
}

LuckyCofig LuckySpinLayer::computeReward()
{
	if(CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble0", false) == false)
	{
		/*第一次抽奖 */
		CCUserDefault::sharedUserDefault()->setBoolForKey("firstGameble0", true);
		LuckyCofig data = LuckyCofig(1, "1000"); /*0 奖励金币 */
		data.rewardIndex = 3;
		return data;
	}
	else if(CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble1", false) == false)
	{
		/*第二次抽奖 */
		CCUserDefault::sharedUserDefault()->setBoolForKey("firstGameble1", true);
		LuckyCofig data = LuckyCofig(1, "5000"); /*1 奖励金币 */
		data.rewardIndex = 10;
		return data;
	}
	else
	{
		if(CCUserDefault::sharedUserDefault()->getBoolForKey("firstGameble2", false) == false)
		{
			/*第三次抽奖*/
			CCUserDefault::sharedUserDefault()->setBoolForKey("firstGameble2", true);
			LuckyCofig data = LuckyCofig(15, "6");
			data.rewardIndex = 15;
			return data;
		}
		else
		{
			m_spinCount++;
			int randInt = rand() % atoi("100");
			float randDot = (rand() % 100) / 100.0f;
			float randValue = randInt + randDot;

			GameConfigData::initialGameData().setUserCount(m_spinCount);

			int index = getIndexByGambleType(1, randValue);
			LuckyCofig data;

			if(index >= 0)
			{
				if(index == 0 || index == 6 || index == 15 || index ==9)
				{
					if(m_spinCount < 50)
					{
						index += 1;
					}
				}
				else
				{
					if(m_spinCount == 200)
					{
						//*必定中奖*//
						m_spinCount = 50;
						index = 9;
					}
				}

				int type = getRewardTypeByIndex(index);
				data = LuckyCofig(type, gValueRewardGolden[index]);
				data.rewardIndex = index;
			}

			return data;
		}
	}
}

float  LuckySpinLayer::getRewardIndex(int iCount)
{
	float valueCount = 0;

	for(int i = 0; i != iCount; ++i)
	{
		valueCount += gValuePercent[i];
	}

	return valueCount;
}

int LuckySpinLayer::getRewardTypeByIndex(int index)
{

	//*奖励类型*//
	if(index == 0 || index == 6 || index == 15 || index ==9 )
	{
		/**四个特殊道具**/
		return index;
	}
	else if(index == 7 || index == 14)
	{
		return 3;
	}
	else
	{
		return 1;
	}

}

int LuckySpinLayer::getIndexByGambleType(int type, float value)
{
	for(int i = 0; i != 18; ++i)
	{
		//if((value >getRewardIndex(i) || ToolsFun::sharedTools()->floatEquals(value,getRewardIndex(i)) )&& value <getRewardIndex(i+1))
		if((value > getRewardIndex(i)) && value < getRewardIndex(i + 1))
		{
			return i;
		}
	}

	return -1;
}


void LuckySpinLayer::startSpin(CCObject* obj)
{
	CCNode *node = (CCNode*)obj;
	int tag = node->getTag();

	switch(tag)
	{
		case 100:
			{
				int  count = GameConfigData::initialGameData().getLuckyCount();

				if(count <= 0)
				{
				}
				else
				{
					m_menu->setEnabled(false);

					if(this->getChildByTag(2252251))
					{
						this->removeChildByTag(2252251);
					}

					--count;
					char gameBoxCountStr[33];
					sprintf(gameBoxCountStr, "%d", count);
					GameConfigData::initialGameData().setLuckyCount(count);
					m_lastSpinCount->setString(gameBoxCountStr);
					m_data = computeReward();
					randSpriteMove();

					if(m_speedUp)
					{
						CCDirector::sharedDirector()->getScheduler()->setTimeScale(4.0f);
					}
				}
			}
			break;

		case 101:
			close(NULL);
			break;

		default:
			break;
	}

}

void LuckySpinLayer::close(CCObject* obj)
{

	if(m_pTarget)
	{
		(m_pTarget->*m_fun)();
	}

	GameConfigData::initialGameData().writeData();
	CCUserDefault::sharedUserDefault()->setBoolForKey("speedUp", m_speedUp);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	this->removeAllChildren();
	this->removeFromParentAndCleanup(true);

}

void LuckySpinLayer::enableBtn()
{

}

void LuckySpinLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, true);
}

bool LuckySpinLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void LuckySpinLayer::setCallBackTarget(CCObject* target, SEL_CallFunc fun)
{
	m_pTarget = target;
	m_fun = fun;
}

void LuckySpinLayer::rotateEndGetReward(CCNode *node)
{
	//addTipSprite();
	CCPoint endPos = getPosByIndex(m_data.rewardIndex);
	m_spinSprite->setPosition(endPos - ccp(1*gScaleX, 2.5f*gScaleY));
	m_menu->setEnabled(true);

	m_rewardIndex = m_data.rewardIndex;
	char strRindex[33];
	sprintf(strRindex, "%d", m_data.rewardIndex);
	CCUserDefault::sharedUserDefault()->setStringForKey("rewardIndex", strRindex);

	switch(m_data.rewardType)
	{
		case 0: //狂战士
			{
				GameConfigData::initialGameData().setHeroStateInt(2 , "1");
				break;
			}

		case 1: //金币
			{
				CCLog("%d\n", atoi(m_data.rewartMoney.c_str()));
				GameConfigData::initialGameData().setCoinValue(GameConfigData::initialGameData().getCoinValue() + atoi(m_data.rewartMoney.c_str()));

				char buf[32];
				sprintf(buf, "%d", GameConfigData::initialGameData().getCoinValue());
				m_coinLabel->setString(buf);
				break;
			}

		case 3: //再来一次
			{
				int count = GameConfigData::initialGameData().getLuckyCount();
				char buf[32];
				sprintf(buf, "%d", count + 1);
				m_lastSpinCount->setString(buf);

				GameConfigData::initialGameData().setLuckyCount(count + 1);

				break;
			}

		case 6://萌妹子
			{
				GameConfigData::initialGameData().setHeroStateInt(1 , "1");
				break;
			}

		case 9: //蔚蓝精灵
			{
				GameConfigData::initialGameData().setPetState(5,"1");
				GameConfigData::initialGameData().setPetPos(5,-1);
				GameConfigData::initialGameData().writeData();
				break;
			}

		case 15: //保护罩
			{
				int reviveCount = GameConfigData::initialGameData().getReviveValue();
				GameConfigData::initialGameData().setReviveValue(reviveCount+6);
			}
			break;

		default:
			break;
	}

	char spinCount[33];
	sprintf(spinCount, "%d", m_spinCount);

	playGainRewardEffect();

	int count = GameConfigData::initialGameData().getLuckyCount();

	if(count <= 0)
	{
		m_buyTimeMenu->setEnabled(true);
		m_buyTimeMenu->setVisible(true);
		CCNode * node = m_menu->getChildByTag(100);
		CCMenuItemSprite *item = (CCMenuItemSprite*)node;
		item->setVisible(false);
		item->setEnabled(false);
	}

	this->setCoin();
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
}

void LuckySpinLayer::playGainRewardEffect()
{
	m_playCoinAndDiaType = m_data.rewardType;

	if(m_data.rewardType == 2 || m_data.rewardType == 1)
	{
		scheduleOnce(schedule_selector(LuckySpinLayer::endAddCoinEffect), 2.0f);
		schedule(schedule_selector(LuckySpinLayer::addCoin), 1 / 20.0f);
	}
	else if(m_data.rewardType == 0 || m_data.rewardType == 6 || m_data.rewardType == 9 || m_data.rewardType == 15)
	{
		addTipSprite();
	}
}

void LuckySpinLayer::endAddCoinEffect(float t)
{
	unschedule(schedule_selector(LuckySpinLayer::addCoin));
}

void LuckySpinLayer::addCoin(float t)
{
//	ToolsFun::sharedTools()->playEffect(COIN);

	for(int i = 0; i != 3; ++i)
	{
		CCPoint pos;
		pos.x = rand() % 800*gScaleY;
		pos.y = 800 * gScaleY + rand() % 160 * gScaleY; // - 80 + rand()%160;
		CCSprite* spriteCoin = NULL;//= CCSprite::createWithSpriteFrameName("coin_0.png");

		if(m_playCoinAndDiaType == 2)
		{
			/***钻石***/
			spriteCoin = CCSprite::create("ui/luckyspin/gold.png");
			spriteCoin->setScale(0.8f);
		}
		else if(m_playCoinAndDiaType == 1)
		{
			/***金币**/
			//spriteCoin = CCSprite::createWithSpriteFrameName("coin_0.png");
			spriteCoin = CCSprite::create("ui/luckyspin/gold.png");
			spriteCoin->setScale(0.75f);
		}
		else
		{
			unschedule(schedule_selector(LuckySpinLayer::endAddCoinEffect));
			unschedule(schedule_selector(LuckySpinLayer::addCoin));
			return;
		}

		spriteCoin->setPosition(pos);
		CCPoint posEnd = pos + ccp(0, -1000 * gScaleX);
		float timeSpeed = (pos - posEnd).getLength() / 300.0f;
		this->addChild(spriteCoin, this->getZOrder() + 1000);

		if(m_playCoinAndDiaType == 1)
		{
		}

		spriteCoin->runAction(CCSequence::create(CCEaseIn::create(CCMoveTo::create(0.6f * timeSpeed, posEnd), 4)
		                      , CCCallFuncN::create(this, callfuncN_selector(LuckySpinLayer::removeCoin)),
		                      NULL));
	}
}

void LuckySpinLayer::removeCoin(CCNode *node)
{
//	ToolsFun::sharedTools()->playEffect(COIN);
	node->removeFromParent();
}

void LuckySpinLayer::randSpriteMove()
{
	schedule(schedule_selector(LuckySpinLayer::updateSpinSpritePos));
}

void LuckySpinLayer::updateSpinSpritePos(float t)
{
	static int radius = 0;
	static int setPosCount = 0;
	static float upPosTime = 0;
	upPosTime += t;
	static float intervalTime = 0.07f;

	if(upPosTime > intervalTime)
	{
		upPosTime = 0;
		m_rewardIndex++;
		setPosCount++;

		if(radius == 0)
		{
			//*第零圈加速慢*//
			if(setPosCount % 3 == 0)
			{
				intervalTime -= 0.004f;

				if(intervalTime < 0.01f)
				{
					intervalTime = 0.01f;
				}
			}
		}
		else if(radius == 1)
		{
			//*第二圈加速快*//
			if(setPosCount % 3 == 0)
			{
				intervalTime -= 0.005f;

				if(intervalTime < 0.001f)
				{
					intervalTime = 0.001f;
				}
			}
		}
		else if(radius == 2)
		{
			//*第二圈加速快*//
			if(setPosCount % 3 == 0)
			{
				intervalTime += 0.004f;

				if(intervalTime > 0.02f)
				{
					intervalTime = 0.02f;
				}
			}
		}
		else if(radius == 3)
		{
			//*第三圈加速更快*//
			if(m_data.rewardIndex - m_rewardIndex <= 8)
			{
				if(m_data.rewardIndex - m_rewardIndex > 0)
				{
					intervalTime += (8 - m_data.rewardIndex + m_rewardIndex) * 0.02f;

					if(intervalTime > 0.4f)
					{
						intervalTime = 0.4f;
					}
				}
				else
				{
					if(m_data.rewardIndex + 17 - m_rewardIndex <= 8)
					{
						intervalTime += (8 - 17 - m_data.rewardIndex + m_rewardIndex) * 0.02f;

						if(intervalTime > 0.4f)
						{
							intervalTime = 0.4f;
						}
					}
				}
			}
		}

		if(m_rewardIndex >= 18)
		{
			m_rewardIndex = 0;
		}

		if(m_rewardIndex == m_data.rewardIndex)
		{
			radius += 1;
		}

		if(radius >= m_rotateCount)
		{
			if(m_rewardIndex == m_data.rewardIndex)
			{
				ToolsFun::sharedTools()->playEffect(SPIN_EFFECT);
				radius = 0;
				intervalTime = 0.07f;
				upPosTime = 0;
				rotateEndGetReward(NULL);
				setPosCount = 0;
				unschedule(schedule_selector(LuckySpinLayer::updateSpinSpritePos));
				return;
			}
		}

		CCPoint pos = getPosByIndex(m_rewardIndex);
		m_spinSprite->setPosition(pos - ccp(1*gScaleX, 2.5f*gScaleY));
		ToolsFun::sharedTools()->playEffect(SPIN_EFFECT);
	}
}

CCPoint LuckySpinLayer::getPosByIndex(int index)
{
	CCPoint pos;
	float leftX = 75*gScaleX;
	float downY = 690*gScaleY;
	float subX = 3.0f;
	float subY = 3.0f;


	if(index >= 0 && index <= 3)
	{
		pos.x = leftX +(105 * index*gScaleY + subX * index*gScaleY);
		pos.y = downY - 100 / 2*gScaleY;
	}
	else if(index >= 4 && index <= 9)
	{
		int subIndex = index - 3;
		pos.x = leftX +(105 * 3*gScaleY + subY * 3*gScaleX);
		pos.y = downY - (88/ 2.0f*gScaleY + 90 * subIndex*gScaleX+ subIndex * subY*gScaleX);
	} 
	else if(index >= 10 && index <= 12)
	{
		int subIndex = index - 9;
		pos.x=leftX + 105 * 3*gScaleY + subY * 3*gScaleY - (105 * subIndex*gScaleY + subX * subIndex*gScaleY);
		pos.y = downY - (88 / 2.0f*gScaleY + 90 * 6*gScaleY+ 6 * subY*gScaleY);
	}
	else if(index >= 13 && index <= 17)
	{
		int subIndex = index - 12;
		pos.x = leftX ;
		pos.y = downY - (84 * 7*gScaleY + 7 * subY*gScaleY) + (88 / 2.0f*gScaleY + 88 * subIndex*gScaleY + subIndex * subY*gScaleY)-35;
	} 

	return pos;  
}

void LuckySpinLayer::playSoundEffect(CCNode *node)
{

}

void LuckySpinLayer::rightMoveEnd(CCNode *node)
{
	CCPoint pos = getPosByIndex(9);
	node->setPosition(pos);
}

void LuckySpinLayer::downMoveEnd(CCNode *node)
{
	CCPoint pos = getPosByIndex(7);
	node->setPosition(pos);
}

void LuckySpinLayer::leftMoveEnd(CCNode *node)
{
	CCPoint pos = getPosByIndex(18);
	node->setPosition(pos);
}

void LuckySpinLayer::upMoveEnd(CCNode *node)
{
	CCPoint pos = getPosByIndex(16);
	node->setPosition(pos);
}

void LuckySpinLayer::addTipSprite()
{
	addUpSucEffect(0);

	scheduleOnce(schedule_selector(LuckySpinLayer::endParticle), 1.5f);
	schedule(schedule_selector(LuckySpinLayer::playParticleEffec), 0.08f);
	ToolsFun::sharedTools()->playEffect(LI_HUA);

}

void LuckySpinLayer::addUpSucEffect(int type)
{
	if(this->getChildByTag(2211623))
	{
		this->removeChildByTag(2211623);
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCNode *node = CCNode::create();
	this->addChild(node, 11, 2211623);
	node->setPosition(ccp(size.width/2, size.height/2));
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteEffect/upgrade.plist");
	CCSprite *sprite = CCSprite::createWithSpriteFrameName("upgrade_0.png");
	node->addChild(sprite);

	{
		CCSprite *sprite = CCSprite::create("ui/luckyspin/rewardSur.png");
		sprite->setScale(0.6f);
		sprite->setTag(2211644);
		sprite->setVisible(false);
		node->addChild(sprite);
	}

	
	CCAnimation *animation = SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("upgrade_%d.png", 3, 0.12f);
	CCAnimate *animate = CCAnimate::create(animation);
	sprite->runAction(CCSequence::create(animate
	                                     , CCCallFuncN::create(this, callfuncN_selector(LuckySpinLayer::animationEnd))
	                                     , CCDelayTime::create(0.6f)
	                                     , CCCallFuncN::create(this, callfuncN_selector(LuckySpinLayer::endUpEffect))
	                                     , NULL)); 
}

void LuckySpinLayer::animationEnd(CCNode *obj)
{
	CCNode *node = this->getChildByTag(2211623);

	CCSprite *sprite = (CCSprite *)this->getChildByTag(2211623)->getChildByTag(2211644);
	sprite->setVisible(true);

	sprite->runAction(CCSequence::create(CCDelayTime::create(1), CCCallFuncN::create(this, callfuncN_selector(LuckySpinLayer::endUpEffect)) , NULL));
}

void LuckySpinLayer::endUpEffect(CCNode *node)
{
	this->removeChildByTag(2211623);
}


void LuckySpinLayer::playParticleEffec(float t)
{
	CCPoint pos = ccp(100*gScaleX, 60*gScaleY);
	pos.x = pos.x + rand() % 700*gScaleY;
	pos.y = pos.y + rand() % 400*gScaleX;
	CCParticleSystemQuad *partilc = CCParticleSystemQuad::create("spriteEffect/reParticle.plist");
	this->addChild(partilc, 1000011, 201405131);
	partilc->setPosition(pos);
	CCSequence *seq = CCSequence::create(CCDelayTime::create(1.2f), CCCallFuncN::create(this, callfuncN_selector(LuckySpinLayer::removeChild)), NULL);
	partilc->runAction(seq);
	ToolsFun::sharedTools()->playEffect(LI_HUA);

}

void LuckySpinLayer::endParticle(float t)
{
	unschedule(schedule_selector(LuckySpinLayer::playParticleEffec));
}

void LuckySpinLayer::canSpeedUp(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	m_speedUp = !m_speedUp;

	if(m_speedUp)
	{
		char   buf[32];
		snprintf(buf,   sizeof(buf),   "%f ",   node->getChildByTag(1234)->getPositionX());
		///CCLog(s);
		node->getChildByTag(1234)->setVisible(true);
		m_rotateCount = 1;
	}
	else
	{
		node->getChildByTag(1234)->setVisible(false);
		m_rotateCount = 4;
	}
}

void LuckySpinLayer::coutinue(CCObject *pSender)
{
	//ToolsFun::sharedTools()->playEffect(BUTTON);
	this->removeChildByTag(5141124);
	m_menu->setEnabled(true);
}

void LuckySpinLayer::purchaseGameCount()
{
}

void LuckySpinLayer::purchaseCountSuc(const char *str)
{
#if 0
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);

	int i = atoi(str) - 14;
	const char *strDes[3] = {"10 times", "20 times", "30 times"};

	if(i >= 0 && i < 3)
	{
		HttpClientNode::sharedHttpClient()->eventSend(7, strDes[i]);
	}

	const char *strCount = ConfigData::sharedConfigData()->gameBoxCount.c_str();
	m_lastSpinCount->setString(strCount);
	CCNode * node = m_menu->getChildByTag(100);
	CCMenuItemSprite *item = (CCMenuItemSprite*)node;
	item->setVisible(true);
	item->setEnabled(true);
	m_buyTimeMenu->setEnabled(false);
	m_buyTimeMenu->setVisible(false);
#endif
}

void LuckySpinLayer::menuEnable(CCObject *obj)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "test");
	m_menu->setEnabled(true);
}

void LuckySpinLayer::purchaseSuccess(const char* str)
{
	int count = 0;

	if(strcmp(str, "4") == 0)
	{
		count = 10;
	}
	else if(strcmp(str, "5") == 0)
	{
		count = 20;
	}
	else if(strcmp(str, "6") == 0)
	{
		count = 30;
	}

	char buf[32];
	sprintf(buf, "%d", count);
	m_lastSpinCount->setString(buf);

	GameConfigData::initialGameData().setLuckyCount(count);

	if(count > 0)
	{
		m_buyTimeMenu->setEnabled(false);
		m_buyTimeMenu->setVisible(false);
		CCNode * node = m_menu->getChildByTag(100);
		CCMenuItemSprite *item = (CCMenuItemSprite*)node;
		item->setVisible(true);
		item->setEnabled(true);
		m_menu->setEnabled(true);
	}
}

void LuckySpinLayer::back(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);

	if(m_pTarget)
	{
		(m_pTarget->*m_fun)();
	}

	GameConfigData::initialGameData().writeData();
	CCUserDefault::sharedUserDefault()->setBoolForKey("speedUp", m_speedUp);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	this->removeAllChildren();
	this->removeFromParentAndCleanup(true);
	MainMenu *menuLayer = MainMenu::create();
	ToolsFun::sharedTools()->gotoscene(menuLayer);
}

void LuckySpinLayer::showShopLayer(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	this->m_menu->setEnabled(false);
	this->setTouchEnabled(false);
	ShopLayer *shopLayer = ShopLayer::create();
	this->addChild(shopLayer, 16);
	shopLayer->setCloseCallBack(this, menu_selector(LuckySpinLayer::backFromShopLayer));
}
void LuckySpinLayer::backFromShopLayer(CCObject *pSender)
{
	this->m_menu->setEnabled(true);
	this->setTouchEnabled(true);
}

void LuckySpinLayer::setCloseCallBack(CCObject *pTarget, SEL_MenuHandler callBack)
{

	m_pTarget = pTarget;
	m_callFun = callBack;
}
void LuckySpinLayer::purchaseFailed(const char* str)
{
}

void LuckySpinLayer::setCoin()
{
	int coinValue = GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);
	m_coinLabel->setString(coinStr.c_str());
}

void LuckySpinLayer::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(LuckySpinLayer::goGamble));
	this->addChild(layer,10000);
}

void LuckySpinLayer::goGamble()
{
	int count = GameConfigData::initialGameData().getLuckyCount();
	char buf[32];
	sprintf(buf, "%d", count);

	m_lastSpinCount->setString(buf);
}