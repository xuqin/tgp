#include "PetChoiceLayer.h"
#include "LevelChoice.h"
#include "ToolFun/ToolsFun.h"
#include "../GLayer/MainMenu.h"
#include "../GCData/DataInitial.h"
#include "../GCData/GameConfigData.h"
#include "ShopLayer.h"
#include "../ToolFun/StringKey.h"
#include "ShopLayer.h"
#include "ToolFun/JniCall.h"
#include "../ToolFun/HttpClientNode.h"
#include "../GCData/DataInitial.h"
#include "BackLayer.h"
#include "LuckySpinLayer.h"

#define  COIN_TAG 6111045
#define  LEFT_TAG 6111046
#define  RIGHT_TAG 6111047
#define  PET_GAIN_START_TAG 10011
#define  UPPET_START_TAG 10021
#define  PET_BATTLE_START_TAG 10031
#define  PET_CANCEL_START_TAG 10041

#define  POS_TAG 900000


PetChoiceLayer::PetChoiceLayer():m_beChoicePetId(-1)
	,m_target(NULL)
	,m_callFun(NULL)
	,m_UpPetid(0)
{

}

PetChoiceLayer::~PetChoiceLayer()
{

}

bool PetChoiceLayer::init()
{
	MyScrowLayer::init();

	m_maxHeight = 1580;
	setKeypadEnabled(true);

	return true;
}

void PetChoiceLayer::initMenu()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	setScissorRect(CCRectMake(0,0,size.width,size.height));
	initUI();
}

void PetChoiceLayer::initUI()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();

	//背景
	CCSprite * bkg = CCSprite::create("ui/shared/bkg.png");
	this->addChild(bkg,-4);
	bkg->setPosition(size/2);
	
	//头部
	CCSprite *spBkgTop = CCSprite::create("ui/shared/top.png");
	this->addChild(spBkgTop,0);
	spBkgTop->setPosition(ccp(size.width/2,size.height - spBkgTop->getContentSize().height/2));
	
	//金币放置最上方商城
	CCSprite *coinIcon = CCSprite::create("ui/shared/coin.png");
	this->addChild(coinIcon,2,1429);
	coinIcon->setPosition(ccp(280*gScaleX,size.height-30*gScaleY));

	m_coinLabel = CCLabelBMFont::create("10000000",GOLD_FONT);
	this->addChild(m_coinLabel,4);
	m_coinLabel->setAnchorPoint(ccp(0,0.5f));
	m_coinLabel->setPosition(ccp(300*gScaleX, size.height - 25*gScaleY));
	m_coinLabel->setScale(0.6F);
	setCoin();

	CCMenu *menu = CCMenu::create();
	this->addChild(menu,10);
	menu->setTouchPriority(-130);
	menu->setPosition(CCPointZero);
	CCSprite *spNml = CCSprite::create("ui/shared/back.png");
	CCSprite *spSel = CCSprite::create("ui/shared/back.png");
	spSel->setColor(ccc3(100,100,100));
	CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PetChoiceLayer::back));
	menu->addChild(spItem);
	spItem->setPosition(ccp(37*gScaleX,size.height-37*gScaleY));

	//show shop
	spNml = CCSprite::create("ui/shared/coinplus.png");
	spSel = CCSprite::create("ui/shared/coinplus.png");
	spSel->setColor(ccc3(100,100,100));
	spItem = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PetChoiceLayer::showShopLayer));
	menu->addChild(spItem);
	spItem->setPosition(ccp(430*gScaleX, size.height - 30*gScaleY));
	
	//初始化宠物界面
	m_menu = CCMenu::create();
	m_uiNode->addChild(m_menu,2);
	m_uiNode->setPosition(CCPointZero);
	m_menu->setPosition(CCPointZero);
	SK::StringKey nameKey[6] = {K.PET0_NAME,K.PET1_NAME,K.PET2_NAME,K.PET3_NAME,K.PET4_NAME,K.PET5_NAME};
	SK::StringKey desKey[6] = {K.PET0_DES,K.PET1_DES,K.PET2_DES,K.PET3_DES,K.PET4_DES,K.PET5_DES};

	for (int i =0;i!=6;++i)
	{
		m_petBkgs[i] = CCSprite::create("ui/petUI/pet_head.png");
		CCSprite *sprite = m_petBkgs[i];
		m_uiNode->addChild(sprite,1,14610+i);
		sprite->setPosition(ccp(240*gScaleX,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i));

		char petFile[33];
		sprintf(petFile,"ui/petUI/pet%d_0.png",i);
		CCSprite *petSprite  = CCSprite::create(petFile);
		m_petBkgs[i]->addChild(petSprite,1);
		petSprite->setPosition(ccp(170*gScaleX,130*gScaleY));

		CCSprite* petDes=CCSprite::create("ui/petUI/pet_describe.png");
		m_petBkgs[i]->addChild(petDes,1);
		petDes->setPosition(ccp(330*gScaleX,140*gScaleY));
		
		m_desFont[i] = CCLabelBMFont::create("10000000","font/pet_des.fnt",200);
		m_desFont[i]->setLineBreakWithoutSpace(true);
		m_petBkgs[i]->addChild(m_desFont[i],10);
		m_desFont[i]->setAnchorPoint(CCPointZero);

		if(i==5)
		{
			m_desFont[i]->setPosition(ccp(230*gScaleX,90*gScaleY));
		}
		else
		{
			m_desFont[i]->setPosition(ccp(230*gScaleX,105*gScaleY));
		}

		m_nameFont[i] = CCLabelBMFont::create("1000000","font/pet_name.fnt");
		sprite->addChild(m_nameFont[i],10);
		m_nameFont[i]->setAnchorPoint(CCPointZero);
		m_nameFont[i]->setPosition(ccp(230*gScaleX,150*gScaleY));


		m_levelFont[i] = CCLabelBMFont::create("1000000","font/level.fnt");
		sprite->addChild(m_levelFont[i],10);
		m_levelFont[i]->setPosition(ccp(400*gScaleX,170*gScaleY));
				
		CCLabelBMFont *nameLabel = m_nameFont[i];
		nameLabel->setScale(0.8f);
		nameLabel->setString(gGetLanguageDesc(nameKey[i]).c_str());
				
		CCLabelBMFont *desLabel = m_desFont[i];
		desLabel->setScale(0.8f);
		desLabel->setString(gGetLanguageDesc(desKey[i]).c_str());

		CCSprite *left = CCSprite::create("ui/petUI/pet_follow_0.png");
		left->setScale(0.8f);
		sprite->addChild(left,11,LEFT_TAG+i);
		left->setPosition(ccp(330*gScaleX,230*gScaleY));

		CCSprite *right = CCSprite::create("ui/petUI/pet_follow_1.png");
		right->setScale(0.8f);
		sprite->addChild(right,11,RIGHT_TAG+i);
		right->setPosition(ccp(330*gScaleX,230*gScaleY));

		CCSprite *buySel = NULL;

		if(i==5)
		{
			buySel = CCSprite::create("ui/petUI/pet_get.png");
			buySel->setColor(ccc3(100,100,100));
			m_buyPetItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/petUI/pet_get.png"),buySel,
				this,menu_selector(PetChoiceLayer::purchasePet));
			m_menu->addChild(m_buyPetItems[i],1,PET_GAIN_START_TAG+i);
			m_buyPetItems[i]->setPosition(ccp(320*gScaleX,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i - 80*gScaleY));
		}
		else
		{
			buySel = CCSprite::create("ui/petUI/pet_btn.png");
			buySel->setColor(ccc3(100,100,100));
			m_buyPetItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/petUI/pet_btn.png"),buySel,
				this,menu_selector(PetChoiceLayer::purchasePet));
			m_menu->addChild(m_buyPetItems[i],1,PET_GAIN_START_TAG+i);
			m_buyPetItems[i]->setPosition(ccp(320*gScaleX,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i - 80*gScaleY));

		}

		if(i!=5)
		{
			CCSprite *moneyIcon = CCSprite::create("ui/shared/coin.png");
			m_buyPetItems[i]->addChild(moneyIcon,13,COIN_TAG);
			moneyIcon->setPosition(ccp(buySel->getContentSize().width/5,buySel->getContentSize().height/2+10*gScaleY));
		}

		m_priceFont[i] = CCLabelBMFont::create("1000000","font/zl_1.fnt");
		m_buyPetItems[i]->addChild(m_priceFont[i],1);
		m_priceFont[i]->setPosition(ccp(buySel->getContentSize().width/3+25*gScaleX,buySel->getContentSize().height/2+10*gScaleY));

		if(i ==5)
		{
			m_priceFont[i]->setVisible(false);
		}


		CCSprite *upSel = CCSprite::create("ui/prepareUI/upgrade.png");
		upSel->setColor(ccc3(100,100,100));
		m_upPetItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/prepareUI/upgrade.png"),upSel,
			this,menu_selector(PetChoiceLayer::upPet));
		m_menu->addChild(m_upPetItems[i],1,UPPET_START_TAG+i);
		m_upPetItems[i]->setPosition(ccp(320*gScaleX,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i - 80*gScaleY));


		m_upPriceFont[i] = CCLabelBMFont::create("1000000","font/zl_1.fnt");
		m_upPetItems[i]->addChild(m_upPriceFont[i],1);
		m_upPriceFont[i]->setPosition(ccp(upSel->getContentSize().width/3,upSel->getContentSize().height/2+5*gScaleY));

		CCSprite *goBattle = CCSprite::create("ui/petUI/battle.png");
		goBattle->setColor(ccc3(100,100,100));
		m_goBattleItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/petUI/battle.png"),goBattle,
			this,menu_selector(PetChoiceLayer::showPetPosMenu));
		m_menu->addChild(m_goBattleItems[i],1,PET_BATTLE_START_TAG+i);
		m_goBattleItems[i]->setPosition(ccp(110,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i - 80*gScaleY));

		CCSprite *cancelBattle = CCSprite::create("ui/petUI/cancel_battle.png");
		cancelBattle->setColor(ccc3(100,100,100));
		m_cancelBattleItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/petUI/cancel_battle.png"),cancelBattle,
			this,menu_selector(PetChoiceLayer::cancelBattle));
		m_menu->addChild(m_cancelBattleItems[i],1,PET_CANCEL_START_TAG+i);
		m_cancelBattleItems[i]->setPosition(ccp(110,sprite->getContentSize().height/2 +  sprite->getContentSize().height * i - 80*gScaleY));
	}

	resetPetState();
	registerNDK();
}

void PetChoiceLayer::movePetPosition(int id)
{
	float pos[2] = {-265.0f,-513.0f};
	//m_uiNode->setPositionY(m_petBkgs[id]->getPositionY());
	m_uiNode->setPositionY(pos[id-3]);
}


void PetChoiceLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}


void PetChoiceLayer::back(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	if (m_target)
	{
		(m_target->*m_callFun)(pSender);
		GameConfigData::initialGameData().writeData();
		this->removeFromParent();
	}
	else
	{
		LevelChoice *menuLayer = LevelChoice::create();
		ToolsFun::sharedTools()->gotoscene(menuLayer);
	}
}

void PetChoiceLayer::showShopLayer(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	m_menu->setEnabled(false);
	ShopLayer *shopLayer = ShopLayer::create();
	this->addChild(shopLayer,10);

	shopLayer->setCloseCallBack(this,menu_selector(PetChoiceLayer::backFromShopLayer));
}
void PetChoiceLayer::backFromShopLayer(CCObject *pSender)
{
	m_menu->setEnabled(true);
}

void PetChoiceLayer::skillUp(int petid)
{	
	GameConfigData::initialGameData().writeData();
}

void PetChoiceLayer::petRest(UIButton* button)
{
	int pId = 0;
	GameConfigData::initialGameData().setPetState(pId,"1");
	GameConfigData::initialGameData().setPetPos(pId,-1);
	GameConfigData::initialGameData().writeData();
	scheduleOnce(schedule_selector(PetChoiceLayer::removeAddPetMenu),0.05f);
}

void PetChoiceLayer::setCloseCallBack(CCObject *pTarget,SEL_MenuHandler callBack)
{
	m_target = pTarget;
	m_callFun = callBack;
}

void PetChoiceLayer::getPet(int petId )
{
	if(petId ==1 || petId ==2)
	{
		int price[2] = {8888,128888};

		int curCoinVal=GameConfigData::initialGameData().getCoinValue();
		if (GameConfigData::initialGameData().getCoinValue() >= price[petId-1])
		{
			curCoinVal -= price[petId-1];
			GameConfigData::initialGameData().setCoinValue(curCoinVal);
			GameConfigData::initialGameData().setPetState(petId,"1");
			GameConfigData::initialGameData().setPetPos(petId,-1);
			GameConfigData::initialGameData().writeData();
			resetPetStateById(petId);
			setCoin();
		}
		else
		{
			showBuyCoin();
		}
	}
	else if(petId ==3 || petId ==4) //购买宠物
	{
		int priceIndex[2] = {2,3};

		JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseFailed));
		JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseSuccess));
		JniCall::sharedJniCall()->purchaseByIndex(priceIndex[petId-3]);
	}
	else if(petId ==5)
	{
		CCScene *sc = CCScene::create();
		LuckySpinLayer *ly = LuckySpinLayer::create();
		sc->addChild(ly);
		CCDirector::sharedDirector()->replaceScene(sc);
	}
}

void PetChoiceLayer::purchaseSuccess(const char* str)
{
	this->removeChildByTag(12313154);

	if(strcmp(str,"2") ==0)
	{
		this->removeChildByTag(12313133);

		GameConfigData::initialGameData().setPetState(3,"1");
		GameConfigData::initialGameData().setPetPos(3,-1);
		GameConfigData::initialGameData().writeData();
		resetPetStateById(3);
	}
	else if(strcmp(str,"3") ==0)
	{
		GameConfigData::initialGameData().setPetState(4,"1");
		GameConfigData::initialGameData().setPetPos(4,-1);
		GameConfigData::initialGameData().writeData();
		resetPetStateById(4);
	}else if(strcmp(str,"8")==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=6000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);

		this->removeChildByTag(12313133);
		GameConfigData::initialGameData().setpetLevel(m_UpPetid,PET_MAXLEVEL);
		GameConfigData::initialGameData().writeData();
		resetPetStateById(m_UpPetid);
	}
	else if(strcmp(str,"12")==0)
	{
		int moneyCount = GameConfigData::initialGameData().getCoinValue();
		moneyCount+=4000;
		GameConfigData::initialGameData().setCoinValue(moneyCount);
	}
	
	setCoin();
}
void PetChoiceLayer::purchaseFailed(const char* str)
{
}

void PetChoiceLayer::showPetPosMenu(CCObject* pSender)
{   	
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCLayerColor *layer = CCLayerColor::create(ccc4(112,128,144,160),480,800);
	this->addChild(layer,5,POS_TAG);
	m_pauseMove = true;
	this->m_menu->setEnabled(false);
	int newChoicePetId = ((CCNode*)pSender)->getTag() - PET_BATTLE_START_TAG;
	m_beChoicePetId = newChoicePetId;

	CCMenu *menu = CCMenu::create();
	menu->setPosition(CCPointZero);

	char filename[64];
	sprintf(filename,"ui/shared/placeboard.png");
	petPos =CCSprite::create(filename);
	layer->addChild(petPos,5,POS_TAG);

	petPos->setVisible(true);
		//petPos->setPosition(m_petBkgs[newChoicePetId]->getPosition() + ccp(0,50*gScaleY));
	petPos->setPosition(ccp(240,400));
	petPos->addChild(menu);

	CCPoint petPosition[6] = {ccp(0*gScaleX,0),ccp(0*gScaleX,0),ccp(10*gScaleX,0),
		ccp(10*gScaleX,0),ccp(10*gScaleX,0),ccp(10*gScaleX,0)};

	float scale[6] = {0.25f,0.25f,0.3f,0.3f,0.3f,0.3f};

	for (int i=0;i!=2;++i)
	{
		CCSprite *spNml = CCSprite::create("ui/shared/shop_item.png");
		CCSprite *spSel = CCSprite::create("ui/shared/shop_item.png");
		spSel->setColor(ccc3(100,100,100));
		CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PetChoiceLayer::addPet));
		menu->addChild(spItem,1,100+i);
		CCSprite *sp=NULL;
		for (int petId =0;petId !=6; ++petId )
		{
			if (GameConfigData::initialGameData().getPetStateById(petId) == "2")
			{
				if (GameConfigData::initialGameData().getPetPosById(petId) == i)
				{
					char petIconName[48];		
					sprintf(petIconName,"ui/petUI/pet%d_0.png",petId);
					sp= CCSprite::create(petIconName);
					sp->setScale(scale[petId]);
					spItem->addChild(sp,1,1234);
					sp->setPosition(spItem->getContentSize()/2+petPosition[petId]);
				}
			} 
		}

		if (!sp)
		{
			sp = CCSprite::create("ui/shared/plus.png");	
			spItem->addChild(sp,0,1234);
			sp->setPosition(spItem->getContentSize()/2);
		}
		spItem->setPosition(ccp(petPos->getContentSize().width/4 + petPos->getContentSize().width*3/8 *i,petPos->getContentSize().height/4)+ccp(14*gScaleX,17*gScaleY));
	}
}

void PetChoiceLayer::addPet(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	CCNode *node = (CCNode*)pSender;
	int tag = node->getTag();
	if (GameConfigData::initialGameData().getPetStateById(m_beChoicePetId) != "2"){


		int leftOrRight = 0;
		if (tag == 100)
		{
			leftOrRight = 0;
		}
		else
		{
			leftOrRight = 1;
		}

		GameConfigData::initialGameData().setPetPos(m_beChoicePetId,leftOrRight);
		if(node->getChildByTag(1234))
		{
			CCSprite * sp = (CCSprite*)(node->getChildByTag(1234));
			char petIconName[48];
			sprintf(petIconName,"ui/petUI/pet%d_0.png",m_beChoicePetId);
			sp->initWithFile(petIconName);
			sp->setScale(0.3f);
			int battlePetId = -1;
			for (int petId =0;petId !=6; ++petId )
			{
				if (GameConfigData::initialGameData().getPetStateById(petId) == "2")
				{
					if (GameConfigData::initialGameData().getPetPosById(petId) == leftOrRight)
					{
						GameConfigData::initialGameData().setPetState(petId,"1");

						/**修改之前出战的宠物的状态和对应的****/
						GameConfigData::initialGameData().setPetPos(petId,-1);
					}
				}
			}

		}
		else
		{
			for (int petId =0;petId !=6; ++petId )
			{
				if (GameConfigData::initialGameData().getPetStateById(petId) == "2" && petId == m_beChoicePetId)
				{
					//*删除同一位置的当前出战的宠物*//
					node->getParent()->getChildByTag(101 + leftOrRight * -1)->removeChildByTag(1234);
				}	
			}
			char petIconName[48];
			sprintf(petIconName,"ui/petUI/pet%d_0.png",m_beChoicePetId);
			CCSprite * sp = CCSprite::create(petIconName);
			node->addChild(sp,1,1234);
			sp->setPosition(node->getContentSize()/2);
		}
		GameConfigData::initialGameData().setPetState(m_beChoicePetId,"2");
		//initPetLayer();
		GameConfigData::initialGameData().writeData();
	}
	scheduleOnce(schedule_selector(PetChoiceLayer::removeAddPetMenu),0.01f);
	resetPetState();
}

void PetChoiceLayer::removeAddPetMenu(float t)
{
if(this->getChildByTag(POS_TAG)!=NULL)
	{
		CCNode* petPos=this->getChildByTag(POS_TAG);
		petPos->setVisible(false);
		this->removeChildByTag(POS_TAG);
		this->m_menu->setEnabled(true);
		m_pauseMove = false;
	}
}

void PetChoiceLayer::purchasePet(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	if (m_nodeMoved)
	{
		return;
	}

	int petId = ((CCNode*)pSender)->getTag() - PET_GAIN_START_TAG;
	LD("petId = %d",petId);
	getPet(petId);
}

void PetChoiceLayer::upPet(CCObject *pSender)
{
	ToolsFun::sharedTools()->playEffect(BUTTON);
	if (m_nodeMoved)
	{
		return;
	}


	int petId = ((CCNode*)pSender)->getTag() - UPPET_START_TAG;
	//宠物的现有等级
	
	m_UpPetid = petId;

	int curLevel=GameConfigData::initialGameData().getpetLevelById(petId);
	int curCoinVal=GameConfigData::initialGameData().getCoinValue();
	int price = 300;

	char buf[64];
	sprintf(buf,"pet%dUp",petId);

	int upCount = CCUserDefault::sharedUserDefault()->getIntegerForKey(buf,0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(buf,upCount+1);

	if(upCount %3==0)
	{
		showFullUpgrade();
	}
	else
	{
		UpgratePetData *data = DataInitial::initialDataSource().getSubPetUpgrateData(petId);
		if(data!=NULL)
		{
			price = data->m_price[curLevel];
		}

		if (curCoinVal >= price) //DataInitial::initialDataSource().getPetUpgrateData(petId,GameConfigData::initialGameData().getpetLevelById(petId)).levelPrice)
		{
			curCoinVal -= price;  //DataInitial::initialDataSource().getPetUpgrateData(petId,GameConfigData::initialGameData().getpetLevelById(petId)).levelPrice;
			curLevel++;
			GameConfigData::initialGameData().setpetLevel(petId,curLevel);
			GameConfigData::initialGameData().setCoinValue(curCoinVal);
			GameConfigData::initialGameData().writeData();
			resetPetStateById(petId);
			setCoin();
		}
		else
		{
			showFullUpgrade(1);
		}
	}
}

void PetChoiceLayer::showFullUpgrade(int type)
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,200,12313133);
	bkg->setPosition(ccp(size.width/2,size.height/2));

	CCSprite *tip1 = CCSprite::create("DayReward/fullUpgrade.png");
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));

	CCMenuItemSprite *itemClose  = NULL;

	if(type ==1)
	{
		itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PetChoiceLayer::openBuyCoin));
	}
	else
	{
		itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PetChoiceLayer::showFullUpgradeClose));
	}
	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PetChoiceLayer::showFullUpgradeSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp);
	m_menu->setEnabled(false);
	m_pauseMove = true;
}

void PetChoiceLayer::openBuyCoin(CCObject *sender)
{
	this->removeChildByTag(12313133);
	showBuyCoin();
}

void PetChoiceLayer::showBuyCoin()
{
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	CCSprite *bkg = CCSprite::create("DayReward/bkg.png");
	this->addChild(bkg,200,12313154);
	bkg->setPosition(ccp(size.width/2,size.height/2));

	CCSprite *tip1 = CCSprite::create("DayReward/50000coin.png");
	bkg->addChild(tip1);
	tip1->setPosition(ccp(bkg->getContentSize().width/2,bkg->getContentSize().height/2-50*gScaleY));

	CCMenu *menu = CCMenu::create();
	bkg->addChild(menu);
	menu->setPosition(CCPointZero);

	CCSprite *closeNml = CCSprite::create("ui/backUI/close0.png");
	CCSprite *closeSel = CCSprite::create("ui/backUI/close0.png");
	closeSel->setColor(ccc3(80,80,80));

	CCMenuItemSprite *itemClose = CCMenuItemSprite::create(closeNml,closeSel,NULL,this,menu_selector(PetChoiceLayer::showBuyCoinClose));

	itemClose->setPosition(ccp(bkg->getContentSize().width-40*gScaleX ,bkg->getContentSize().height-60*gScaleY));
	menu->addChild(itemClose);

	CCSprite *spNml = CCSprite::create("DayReward/button.png");
	CCSprite *spSel = CCSprite::create("DayReward/button.png");
	spSel->setColor(ccc3(80,80,80));
	CCMenuItemSprite *itemSp = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(PetChoiceLayer::showBuyCoinSure));
	itemSp->setPosition(ccp(bkg->getContentSize().width/2 ,35*gScaleY));
	menu->addChild(itemSp);
}

void PetChoiceLayer::showBuyCoinClose(CCObject *sender)
{
	this->removeChildByTag(12313154);
	m_menu->setEnabled(true);
	m_pauseMove = false;
}

void PetChoiceLayer::showBuyCoinSure(CCObject *sender)
{
	//this->removeChildByTag(12313154);

	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(12);
	m_menu->setEnabled(true);
	m_pauseMove = false;
}

void PetChoiceLayer::showFullUpgradeClose(CCObject *sender)
{
	this->removeChildByTag(12313133);
	m_menu->setEnabled(true);
	m_pauseMove = false;
}

void PetChoiceLayer::showFullUpgradeSure(CCObject *sender)
{
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(PetChoiceLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(8);
	m_menu->setEnabled(true);
	m_pauseMove = false;
}


void PetChoiceLayer::cancelBattle(CCObject *pSender){
	ToolsFun::sharedTools()->playEffect(BUTTON);
	if (m_nodeMoved)
	{
		return;
	}

	int newChoicePetId = ((CCNode*)pSender)->getTag() - PET_CANCEL_START_TAG;

	m_beChoicePetId = newChoicePetId;

	GameConfigData::initialGameData().setPetState(m_beChoicePetId,"1");
	GameConfigData::initialGameData().setPetPos(m_beChoicePetId,-1);
	//initPetLayer();
	GameConfigData::initialGameData().writeData();
	resetPetStateById(m_beChoicePetId);
}

void PetChoiceLayer::resetPetState()
{
	for (int i=0;i!=6;++i)
	{
		resetPetStateById(i);
	}
}

void PetChoiceLayer::resetPetStateById(int pId)
{
	//初始化领养按钮
	if (pId >=6)
	{
		return;
	}
	int i = pId;

	CCLabelBMFont *levelLabel = m_levelFont[i];
	levelLabel->setScale(0.8f);
	char levelstring[10];
	sprintf(levelstring, "lv.%d",GameConfigData::initialGameData().getpetLevelById(i)); 
	levelLabel->setString(levelstring);

	if (GameConfigData::initialGameData().getPetStateById(i) == "0")
	{
		m_buyPetItems[i]->setEnabled(true);
		m_buyPetItems[i]->setVisible(true);
		m_upPetItems[i]->setVisible(false);
		m_upPetItems[i]->setEnabled(false);

		m_goBattleItems[i]->setEnabled(false);
		m_goBattleItems[i]->setVisible(false);

		m_cancelBattleItems[i]->setEnabled(false);
		m_cancelBattleItems[i]->setVisible(false);

		//升级金币标签
		CCLabelBMFont *label = m_priceFont[i];
		label->setScale(0.8f);
		char labelstring[128];

		if(i ==1)
		{
			sprintf(labelstring,"%d",8888);
		}
		else if(i==2)
		{
			sprintf(labelstring,"%d",128888);
		}else if(i==3)
		{
			sprintf(labelstring,"%s",gGetLanguageDesc(SK::PET3_PRICE).c_str());
		}
		else if(i ==4)
		{
			sprintf(labelstring,"%s",gGetLanguageDesc(SK::PET4_PRICE).c_str());
		}
		else if(i==5)
		{
			sprintf(labelstring,"%s",gGetLanguageDesc(SK::PET4_PRICE).c_str());
		}


		label->setString(labelstring);
		//label->setVisible(true);
	}
	else
	{
		m_buyPetItems[i]->setEnabled(false);
		m_buyPetItems[i]->setVisible(false);
		m_upPetItems[i]->setVisible(true);
		m_upPetItems[i]->setEnabled(true);
		//初始化出战按钮，与休息按钮

		if (GameConfigData::initialGameData().getPetStateById(i) == "1")
		{
			m_goBattleItems[i]->setEnabled(true);
			m_goBattleItems[i]->setVisible(true);

			m_cancelBattleItems[i]->setEnabled(false);
			m_cancelBattleItems[i]->setVisible(false);
		}
		else if (GameConfigData::initialGameData().getPetStateById(i) == "2")
		{
			m_goBattleItems[i]->setEnabled(false);
			m_goBattleItems[i]->setVisible(false);

			m_cancelBattleItems[i]->setEnabled(true);
			m_cancelBattleItems[i]->setVisible(true);
		}

		vector<PetData*> petConfigs = DataInitial::initialDataSource().getPetData();
		//升级金币标签
		CCLabelBMFont *upLabel = m_upPriceFont[i];
		upLabel->setScale(0.8f);
		char upLabelstring[128];
		if (GameConfigData::initialGameData().getpetLevelById(i) < 30)
		{
			int curLevel = GameConfigData::initialGameData().getpetLevelById(i);
			UpgratePetData *data = DataInitial::initialDataSource().getSubPetUpgrateData(pId);

			sprintf(upLabelstring,"%d",data->m_price.at(curLevel));
			upLabel->setString(upLabelstring);
			upLabel->setVisible(true);
		}
		else
		{
			upLabel->setVisible(false);

			m_upPetItems[i]->setVisible(false);
			m_upPetItems[i]->setEnabled(false);
		}
	}
	//左右边icon
	int leftOrRight = GameConfigData::initialGameData().getPetPosById(i);
	if (leftOrRight == -1)
	{
		m_petBkgs[i]->getChildByTag(LEFT_TAG+i)->setVisible(false);
		m_petBkgs[i]->getChildByTag(RIGHT_TAG+i)->setVisible(false);
	}
	else if (leftOrRight == 0)
	{
		m_petBkgs[i]->getChildByTag(LEFT_TAG+i)->setVisible(true);
		m_petBkgs[i]->getChildByTag(RIGHT_TAG+i)->setVisible(false);
	}
	else
	{
		m_petBkgs[i]->getChildByTag(LEFT_TAG+i)->setVisible(false);
		m_petBkgs[i]->getChildByTag(RIGHT_TAG+i)->setVisible(true);
	}
}

void PetChoiceLayer::setCoin()
{
	int coinValue=GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);

	m_coinLabel->setString(coinStr.c_str());
}

void PetChoiceLayer::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(PetChoiceLayer::goGamble));
	this->addChild(layer,10000);
}
void PetChoiceLayer::goGamble()
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}