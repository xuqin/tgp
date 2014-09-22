#include "ShopLayer.h"
#include "ToolFun/ToolsFun.h"
#include "ToolFun/StringKey.h"
#include "LevelChoice.h"
#include "../ToolFun/HttpClientNode.h"
#include "../ToolFun/JniCall.h"
#include "BackLayer.h"
#include "LuckySpinLayer.h"

#include "../GCData/GameConfigData.h"

#define  BUY_TAG 6000
#define  PRICE_TAG 7000


ShopLayer::ShopLayer():m_scrowPanel(NULL)
	,m_rootLayer(NULL)
	,m_coinLabel(NULL)
	,m_target(NULL)
	,m_callFun(NULL)
{

}

ShopLayer::~ShopLayer()
{

}

bool ShopLayer::init()
{
	MyScrowLayer::init();
	//initMenu();
	m_isUpdating = 0;
	m_maxHeight = 920*gScaleY;
	m_pauseMove = true;

	setKeypadEnabled(true);

	return true;
}

void ShopLayer::initMenu()
{	
	CCSize size = ToolsFun::sharedTools()->getSceneSize();
	setScissorRect(CCRectMake(0,0,size.width,size.height));
	initUI();
}

void ShopLayer::initUI()
{
	this->setTouchEnabled(true);
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
	m_coinLabel->setPosition(ccp(300*gScaleX,size.height-25*gScaleY));
	m_coinLabel->setScale(0.6F);
	setCoin();

	CCMenu *menu = CCMenu::create();
	this->addChild(menu,10);
	menu->setTouchPriority(-230);
	menu->setPosition(CCPointZero);
	CCSprite *spNml = CCSprite::create("ui/shared/back.png");
	CCSprite *spSel = CCSprite::create("ui/shared/back.png");
	spSel->setColor(ccc3(100,100,100));
	CCMenuItemSprite *spItem = CCMenuItemSprite::create(spNml,spSel,this,menu_selector(ShopLayer::back));
	menu->addChild(spItem);
	spItem->setPosition(ccp(37*gScaleX,size.height-37*gScaleY));

	m_menu = CCMenu::create();
	m_menu->setTouchPriority(-230);
	m_uiNode->addChild(m_menu,2);
	m_uiNode->setPosition(CCPointZero);
	m_menu->setPosition(CCPointZero);

	int itemPicId[5] = {3,2,1,0};
	int pricePicId[5] = {1,0,2,1};

	for (int i =0;i!=4;i++)
	{
		m_itemBkgs[i] = CCSprite::create("ui/shopUI/shop_board.png");
		CCSprite *sprite = m_itemBkgs[i];
		m_uiNode->addChild(sprite,1,14610+i);
		sprite->setPosition(ccp(245*gScaleX,sprite->getContentSize().height/2 + ( sprite->getContentSize().height+40) * i));

		char itemFile[33];
		sprintf(itemFile,"ui/shopUI/item_%d.png",itemPicId[i]);
		CCSprite *itemSprite  = CCSprite::create(itemFile);
		m_itemBkgs[i]->addChild(itemSprite,2);
		itemSprite->setPosition(sprite->getContentSize()/2+ccp(-175*gScaleX,5*gScaleY));

		char itemDesFile[33];
		sprintf(itemDesFile,"ui/shopUI/itemDes_%d.png",itemPicId[i]);
		CCSprite* itemDes=CCSprite::create(itemDesFile);
		m_itemBkgs[i]->addChild(itemDes,1);
		itemDes->setPosition(sprite->getContentSize()/2+ccp(5*gScaleX,-10*gScaleY));

		CCSprite *buySel = CCSprite::create("ui/shopUI/shop_getbtn.png");
		buySel->setColor(ccc3(100,100,100));
		m_buyItems[i] = CCMenuItemSprite::create(CCSprite::create("ui/shopUI/shop_getbtn.png"),buySel,this,menu_selector(ShopLayer::purchase));
		m_menu->addChild(m_buyItems[i],1,BUY_TAG+i);
		m_buyItems[i]->setPosition(ccp(400*gScaleX,sprite->getContentSize().height/2 +  (sprite->getContentSize().height+40) * i -10*gScaleY));

	
		char priceFile[32];
		sprintf(priceFile,"ui/shopUI/shop_RMB_%d.png",pricePicId[i]);
		CCSprite *moneyIcon = CCSprite::create(priceFile);
		m_buyItems[i]->addChild(moneyIcon,13);
		if(i ==4)
		{
			moneyIcon->setPosition(ccp(buySel->getContentSize().width/5+35*gScaleY,buySel->getContentSize().height/2));
		}
		else
		{
			moneyIcon->setPosition(ccp(buySel->getContentSize().width/5+25*gScaleX,buySel->getContentSize().height/2));
		}

	}

	setCoin();
	initItem();
	registerNDK();
}

void ShopLayer::registerNDK()
{
	JniCall *jniNode = JniCall::sharedJniCall();
	if (jniNode->getParent())
	{
		jniNode->removeFromParent();
	}
	this->addChild(jniNode);
}


void ShopLayer::touchMenu(CCObject *pSender)
{
	if (m_nodeMoved)
	{
		return;
	}
}

void ShopLayer::back(CCObject* obj)
{ToolsFun::sharedTools()->playEffect(BUTTON);
	if(m_target)
	{
		(m_target->*m_callFun)(obj);
		this->removeFromParent();
	}
	else
	{
		LevelChoice *menuLayer = LevelChoice::create();
		ToolsFun::sharedTools()->gotoscene(menuLayer);
	}
}


void ShopLayer::purchase(CCObject* pSender)
{	ToolsFun::sharedTools()->playEffect(BUTTON);
	int tag =((CCNode*)pSender)->getTag();
	int index = tag - BUY_TAG;
	int itemCodes[5] = {13,12,14,11};
	JniCall::sharedJniCall()->setBuyFailedCallBack(this,callFunStr_selector(ShopLayer::purchaseFailed));
	JniCall::sharedJniCall()->setBuyCompleteCallBack(this,callFunStr_selector(ShopLayer::purchaseSuccess));
	JniCall::sharedJniCall()->purchaseByIndex(itemCodes[index]);
}


void ShopLayer::purchaseSuccess(const char* str)
{
	//HttpClientNode::sharedHttpClient()->eventSend(13,str);
	GameConfigData::initialGameData().writeData();
	setCoin();
	updateItem(2);
	updateItem(3);
}

void ShopLayer::purchaseFailed(const char* str)
{
}

void ShopLayer::setIsUpdating(int i)
{
	m_isUpdating = i;
}

void ShopLayer::setCloseCallBack(CCObject *pSender,SEL_MenuHandler callFun)
{
	m_target = pSender;
	m_callFun = callFun;
}

void ShopLayer::setCoin()
{
	int coinValue = GameConfigData::initialGameData().getCoinValue();
	string coinStr = ToolsFun::sharedTools()->intToString(coinValue);
	m_coinLabel->setString(coinStr.c_str());
}
void ShopLayer::initItem(){
	for (int i=0;i!=4;++i)
	{
		initItemById(i);
	}
}
void ShopLayer::initItemById(int pId)
{
	if (pId >=5)
	{
		return;
	}

	int i = pId;

#if 0
	int i = pId;
	switch(i)
	{
	case 0:{
		   }
	case 1:{
		break;
		   }
	case 2:
		{
		}
		break;
	case 3:
		{
		}
		break;
	case 4:		
	case 5:
		break;
	default:
		break;
	}	

#endif

	switch(i)
	{
	case 3:{
		if(i==3)
		{
			int itemCount = GameConfigData::initialGameData().getBombValue();
			CCSprite *spriteItem =m_itemBkgs[i];
			string countStr = ToolsFun::sharedTools()->intToString(itemCount);
			m_bombNum = CCLabelBMFont::create(countStr.c_str(),"font/white_24.fnt");
			spriteItem->addChild(m_bombNum,10);
			m_bombNum->setPosition(ccp(100*gScaleX,-150*gScaleY));
			m_bombNum->setScale(0.8f);
		}
		   }
	case 4:{
		CCMenuItemSprite *spriteItem =m_buyItems[i];
		CCSprite *hotSale=CCSprite::create("ui/shopUI/shop_hot.png");
		spriteItem->addChild(hotSale,BUY_TAG+50);
		hotSale->setPosition(ccp(0*gScaleX,80*gScaleY));
		   }
		   break;
	}
}

void ShopLayer::updateItem(int pId)
{
	if (pId >=5)
	{
		return;
	}
	int i = pId;
	switch(i)
	{
		case 2:
		{
		}
		break;
	case 3:
		{
			int itemCount = GameConfigData::initialGameData().getBombValue();
			string countStr = ToolsFun::sharedTools()->intToString(itemCount);
			if(m_bombNum)
			{
				m_bombNum->setString(countStr.c_str());
			}
			else
			{
			CCSprite *spriteItem =m_itemBkgs[i];
			m_bombNum = CCLabelBMFont::create(countStr.c_str(),"font/white_24.fnt");
			spriteItem->addChild(m_bombNum,10);
			m_bombNum->setPosition(ccp(70*gScaleX,35*gScaleY));
			m_bombNum->setScale(0.6f);
			}
		}
		break;
	default:
		break;
	}	
}


void ShopLayer::keyBackClicked()
{
	this->setKeypadEnabled(false);
	BackLayer *layer = BackLayer::create();
	layer->setCallback(this,callFun_selector(ShopLayer::goGamble));
	this->addChild(layer,10000);
}
void ShopLayer::goGamble()
{
	CCScene *scene = CCScene::create();
	LuckySpinLayer *gLayer = LuckySpinLayer::create();
	scene->addChild(gLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}