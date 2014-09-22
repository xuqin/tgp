#include "MySpriteManager.h"

#include "../ToolFun/ToolsFun.h"
#include "MySprite.h"
#include "HeroSprite.h"
#include "ItemSprite.h"
#include "MonsterSprite.h"
#include "MapNode.h"
#include "../GLayer/GameLayer.h"
#include "BulletSprite.h"
#include "../GCData/GameConfigData.h"

const int WORLD_WIDTH = (480 + 120) * gScaleY;
const int WORLD_HEIGHT = (800+120) * gScaleY;
const int WORLD_OR_X = -60;
const int WORLD_OR_Y = -60;
static MySpriteManager *gSpriteManager = NULL;
static pthread_t gDetectionCollision;

pthread_mutex_t      gSpriteMutex;
pthread_mutex_t      gItemMutex;
pthread_mutex_t      gHeroBulletSpriteMutex;
pthread_mutex_t      gMonsterBulletSpriteMutex;
pthread_mutex_t      gPetBulletSpriteMutex;

static list<MonsterSprite*> *m_monsterLists = NULL;
static list<ItemSprite*> *m_itemLists = NULL;
static list<HeroBulletSprite*> *m_heroBulletLists = NULL;
static list<BulletSprite*> *m_monsterBulletLists = NULL;
static list<BulletSprite*> *m_petBulletLists = NULL;

static THREAD_STATE gThreadState = THREAD_END;

MySpriteManager::MySpriteManager()
{
	m_monsterLists = new list<MonsterSprite*>(0);
	m_itemLists = new list<ItemSprite*>(0);
	m_heroBulletLists = new list<HeroBulletSprite*>(0);
	m_monsterBulletLists =  new list<BulletSprite*>(0);
	m_petBulletLists = new list<BulletSprite*>(0);
	pthread_mutex_init(&gSpriteMutex, NULL);
	pthread_mutex_init(&gItemMutex, NULL);
	pthread_mutex_init(&gHeroBulletSpriteMutex, NULL);
	pthread_mutex_init(&gMonsterBulletSpriteMutex, NULL);
	pthread_mutex_init(&gPetBulletSpriteMutex, NULL);
	m_threadInit = false;
}

MySpriteManager::~MySpriteManager()
{
	LD("MySpriteManager::~MySpriteManager()");
	pthread_mutex_lock(&gItemMutex);
	m_itemLists->clear();
	delete m_itemLists;
	pthread_mutex_unlock(&gItemMutex);
	pthread_mutex_lock(&gSpriteMutex);
	m_monsterLists->clear();
	delete m_monsterLists;
	pthread_mutex_unlock(&gSpriteMutex);
	pthread_mutex_destroy(&gPetBulletSpriteMutex);
	pthread_mutex_destroy(&gMonsterBulletSpriteMutex);
	pthread_mutex_destroy(&gHeroBulletSpriteMutex);
	pthread_mutex_destroy(&gSpriteMutex);
	pthread_mutex_destroy(&gItemMutex);
}

MonsterSprite *MySpriteManager::createMonsterById(int monsterId)
{
	MonsterSprite *sprite = MonsterSprite::createMonsterById(monsterId);
	pthread_mutex_lock(&gSpriteMutex);
	m_monsterLists->push_back(sprite);
	pthread_mutex_unlock(&gSpriteMutex);
	return sprite;
}

MonsterSprite *MySpriteManager::createMonster(const CreateMInf& mInf)
{
	MonsterSprite *sprite = MonsterSprite::createMonsterByInf(mInf);
	pthread_mutex_lock(&gSpriteMutex);
	m_monsterLists->push_back(sprite);
	pthread_mutex_unlock(&gSpriteMutex);
	return sprite;
}

ItemSprite *MySpriteManager::createItemById(int itemId)
{
	ItemSprite *iSprite =  ItemSprite::createItemById(itemId);
	pthread_mutex_lock(&gItemMutex);
	m_itemLists->push_back(iSprite);
	pthread_mutex_unlock(&gItemMutex);
	return iSprite;
}

/*
BulletSprite *MySpriteManager::createHeroBullet(int bulletId)
{
	BulletSprite *iSprite =  BulletSprite::createBulletById(bulletId);
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	m_heroBulletLists->push_back(iSprite);
	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
	return iSprite;
}*/

HeroBulletSprite *MySpriteManager::createHeroBullet(int bulletId)
{
	HeroBulletSprite *iSprite =  HeroBulletSprite::createBulletById(bulletId);
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	m_heroBulletLists->push_back(iSprite);
	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
	return iSprite;
}

/*
BulletSprite *MySpriteManager::createHeroSubBullet(int bulletId, int sub)
{
	BulletSprite *iSprite =  BulletSprite::createSubBulletById(bulletId, sub);


	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	m_heroBulletLists->push_back(iSprite);
 	pthread_mutex_unlock(&gHeroBulletSpriteMutex);

	return iSprite;
}*/

BulletSprite *MySpriteManager::createMonsterBullet(int bulletId)
{
	BulletSprite *iSprite =  BulletSprite::createBulletById(bulletId);
	pthread_mutex_lock(&gMonsterBulletSpriteMutex);
	m_monsterBulletLists->push_back(iSprite);
	pthread_mutex_unlock(&gMonsterBulletSpriteMutex);
	return iSprite;
}

BulletSprite *MySpriteManager::createPetBullet(int bulletId)
{
	BulletSprite *iSprite =  BulletSprite::createBulletById(bulletId);
	pthread_mutex_lock(&gPetBulletSpriteMutex);
	m_petBulletLists->push_back(iSprite);
	pthread_mutex_unlock(&gPetBulletSpriteMutex);
	return iSprite;
}
BulletSprite *MySpriteManager::createPetBulletWithPet(int bulletId, int petId)
{
	BulletSprite *iSprite =  BulletSprite::createBulletByIdWithPet(bulletId, petId);
	pthread_mutex_lock(&gPetBulletSpriteMutex);
	m_petBulletLists->push_back(iSprite);
	pthread_mutex_unlock(&gPetBulletSpriteMutex);
	return iSprite;
}
list<MonsterSprite*>* MySpriteManager::getMonsterSpriteList()
{
	return m_monsterLists;
}

list<ItemSprite*> *MySpriteManager::getItemSpriteList()
{
	return NULL;
}


//怪物死亡后添加金币
void MySpriteManager::monsterDeadOp(MonsterSprite *mSprite)
{
	int itemId = mSprite->getMInf().mDeadItemId;
	int percent = rand() % 100;
	GameLayer::sharedGameLayer()->showDeadthEffect(mSprite->getPosition(), mSprite->getMInf().mMonsterId);
	GameLayer::sharedGameLayer()->add_m_score(mSprite->getScore());
	if(mSprite->isBoss())
	{
		int level = GameConfigData::initialGameData().getCurLevel();

		int count = 30+(level%5)*10;
		int posY = mSprite->getPositionY();

		CCPoint pos[18] = {ccp(160,posY),ccp(140,posY+20),ccp(120,posY+20),ccp(130,posY+50),ccp(130,posY+50),ccp(180,posY+50),
						   ccp(110,posY+50),ccp(80,posY+20),ccp(70,posY+20),ccp(120,posY-20),ccp(90,posY-40),ccp(200,posY-80),
						   ccp(110,posY+50),ccp(180,posY+20),ccp(100,posY+20),ccp(120,posY-20),ccp(170,posY+40),ccp(190,posY+80)};

		for(int i=0;i<count;i++)
		{
			srand((unsigned)time(NULL));
			ItemSprite * itemSprite = this->createItemById(itemId);
			GameLayer::sharedGameLayer()->addChild(itemSprite, ITEM_ZORDER);
			itemSprite->setPosition(pos[i%18]+ccp(20+rand()%40,10+rand()%40));
			itemSprite->itemAction();
		}
	}
	else
	{
		int mId = mSprite->getMonsterId();

		if(mId>999)
		{
			itemId = 4;
		}


		ItemSprite * itemSprite = this->createItemById(itemId);
		GameLayer::sharedGameLayer()->addChild(itemSprite, ITEM_ZORDER);
		itemSprite->setPosition(mSprite->getPosition());
		itemSprite->itemAction();
	}
	//itemSprite->setPosition(mSprite->getPosition());
	//itemSprite->itemAction();
}

void MySpriteManager::detAllMonster()
{
	list<MonsterSprite*> deadMonsterSprite ;//= new list<MonsterSprite*>();
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		if(!(*iter)->isBoss())
			deadMonsterSprite.push_back(*iter);
	}

	for(list<MonsterSprite*>::iterator iter = deadMonsterSprite.begin(); iter != deadMonsterSprite.end(); ++iter)
	{
		(*iter)->removeFromParent();
		monsterList->remove(*iter);
	}

	if(deadMonsterSprite.size() > 0)
	{
		deadMonsterSprite.clear();
	}

	pthread_mutex_unlock(&gSpriteMutex);
}

void MySpriteManager::detMonster()
{
	list<MonsterSprite*> deadMonsterSprite ;//= new list<MonsterSprite*>();
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		if((*iter)->getDeadState())
		{
			deadMonsterSprite.push_back(*iter);

			switch((*iter)->getDeadType())
			{
				case 0:
					//monsterDeadOp(*iter);
					break;

				case 1:
					/*角色撞死*/
					monsterDeadOp(*iter);
					break;

				case 2:
					/*子弹打死*/
					monsterDeadOp(*iter);
					break;

				default:
					break;
			}	
		}
	}

	for(list<MonsterSprite*>::iterator iter = deadMonsterSprite.begin(); iter != deadMonsterSprite.end(); ++iter)
	{
		(*iter)->removeFromParent();
		monsterList->remove(*iter);
	}

	if(deadMonsterSprite.size() > 0)
	{
		deadMonsterSprite.clear();
	}

	pthread_mutex_unlock(&gSpriteMutex);
}

void MySpriteManager::detItem()
{
	list<ItemSprite*> deadItemSprite;// = new list<MonsterSprite*>();
	pthread_mutex_lock(&gItemMutex);
	list<ItemSprite*> *itemList  = m_itemLists;

	for(list<ItemSprite*>::iterator iter = itemList->begin(); iter != itemList->end(); ++iter)
	{
		if((*iter)->getDeadState()) //detTwoSpriteCln(*iter,heroSprite))
		{
			deadItemSprite.push_back(*iter);
			int itemId = (*iter)->getItemId();

			int deadType = (*iter)->getDeadType();

			if(deadType)
			{
				///**角色吃到***//
				switch(itemId)
				{
					case 0:
						GameLayer::sharedGameLayer()->getHeroSprite()->runMagneticEffect();//*吸铁石**//
						break;

					case 1:
						GameLayer::sharedGameLayer()->getHeroSprite()->runSpeedUpEffect();//*加速**//
						break;

					case 2:
						GameLayer::sharedGameLayer()->getHeroSprite()->addProtectEffect();//**保护罩**//
						break;

					case 3:
						{
							GameLayer::sharedGameLayer()->getHeroSprite()->addAttackEffect();//*攻击效果加成*//
						}
						break;
					case 4:
						{
							ToolsFun::sharedTools()->playEffect(COIN);
							GameLayer::sharedGameLayer()->add_m_money((*iter)->getItemValue());
						}
						break;

					case 5:
						break;

					default:
						break;
				}
			}
		}
		else
		{
			GameLayer *layer = GameLayer::sharedGameLayer();

			if(layer && layer->getHeroSprite()->getMgneticState())
			{
				(*iter)->startMoveToPlayer();
			}
		}
	}

	for(list<ItemSprite*>::iterator iter = deadItemSprite.begin(); iter != deadItemSprite.end(); ++iter)
	{
		(*iter)->removeFromParentAndCleanup(true);
		itemList->remove(*iter);
	}

	if(deadItemSprite.size() > 0)
	{
		deadItemSprite.clear();
	}

	pthread_mutex_unlock(&gItemMutex);
}

void MySpriteManager::deBullet()
{
	list<BulletSprite*> deadBullets;// = new list<MonsterSprite*>();
	list<HeroBulletSprite*> heroDeadBullets;
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
		list<HeroBulletSprite*> *heroBulletLists  = m_heroBulletLists;

	for(list<HeroBulletSprite*>::iterator iter = heroBulletLists->begin(); iter != heroBulletLists->end(); ++iter)
	{
		if((*iter)->getDeadState())
		{
				heroDeadBullets.push_back(*iter);
		}
	}

	for(list<HeroBulletSprite*>::iterator iter = heroDeadBullets.begin(); iter != heroDeadBullets.end(); ++iter)
	{
		(*iter)->removeFromParentAndCleanup(true);
		heroBulletLists->remove(*iter);
	}

	if(heroDeadBullets.size() > 0)
	{
		heroDeadBullets.clear();
	}

	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
	pthread_mutex_lock(&gMonsterBulletSpriteMutex);
	deadBullets.clear();
	list<BulletSprite*> *monsterBulletsLists  = m_monsterBulletLists;

	for(list<BulletSprite*>::iterator iter = monsterBulletsLists->begin(); iter != monsterBulletsLists->end(); ++iter)
	{
		if((*iter)->getDeadState())
		{
			deadBullets.push_back(*iter);
		}
	}

	for(list<BulletSprite*>::iterator iter = deadBullets.begin(); iter != deadBullets.end(); ++iter)
	{
		(*iter)->removeFromParentAndCleanup(true);
		monsterBulletsLists->remove(*iter);
	}

	if(deadBullets.size() > 0)
	{
		deadBullets.clear();
	}

	pthread_mutex_unlock(&gMonsterBulletSpriteMutex);
	pthread_mutex_lock(&gPetBulletSpriteMutex);
	deadBullets.clear();
	list<BulletSprite*> *petBulletsLists  = m_petBulletLists;

	for(list<BulletSprite*>::iterator iter = petBulletsLists->begin(); iter != petBulletsLists->end(); ++iter)
	{
		if((*iter)->getDeadState())
		{
			deadBullets.push_back(*iter);
		}
	}

	for(list<BulletSprite*>::iterator iter = deadBullets.begin(); iter != deadBullets.end(); ++iter)
	{
		(*iter)->removeFromParentAndCleanup(true);
		petBulletsLists->remove(*iter);
	}

	if(deadBullets.size() > 0)
	{
		deadBullets.clear();
	}

	pthread_mutex_unlock(&gPetBulletSpriteMutex);
}

bool MySpriteManager::collisionMonsterWithHero(MySprite *sprite1, HeroSprite *sprite2)
{
	vector<BoundRadius> sprite1Bounds = sprite1->getBoundRadius();
	vector<BoundRadius> sprite2Bounds = sprite2->getBoundRadius();
	CCPoint pos1 = sprite1->getPosition();
	CCPoint pos2 = sprite2->getPosition();
	float r1 = ccp(sprite1->getContentSize().width, sprite1->getContentSize().height).getLength();
	float r2 = ccp(sprite2->getContentSize().width, sprite2->getContentSize().height).getLength();

	if(sprite2->getDeadState())
	{
		//return false;
	}

	if(sprite2->getSpeedUpState() || sprite2-> getInvincible())
	{
		//**加速状态下，到固定Y值就是死亡了***//
		if(sprite1->getPositionY() < 180)
		{
			return true;
		}
	}
	else
	{
		if(sprite2->getDeadState())
		{
			return false;
		}
	}

	if(pos1.getDistance(pos2) > (r1 + r2) / 2)
	{
		return false;
	}

	for(vector<BoundRadius>::iterator iter1 = sprite1Bounds.begin(); iter1 != sprite1Bounds.end(); ++iter1)
	{
		for(vector<BoundRadius>::iterator iter2 = sprite2Bounds.begin(); iter2 != sprite2Bounds.end(); ++iter2)
		{
			pos1 = getRotateCenterPosOnLayer(sprite1, iter1->rCenter);

			if(sprite2->getProtectedState())
			{
				pos2 = sprite2->getPosition();
				iter2->radius+=30;
			}
			else
			{
				pos2 = getRotateCenterPosOnLayer(sprite2, iter2->rCenter);
			}

			if(sqrtf(ccpDistanceSQ(pos1, pos2)) < (iter1->radius  + iter2->radius))
			{
				/*两圆心距离小于等于两个圆圈的半径之和 即发生碰撞*/
				MonsterSprite *monsterSprite = (MonsterSprite *)sprite1;

				if(monsterSprite->getMonsterId()>=8 && monsterSprite->getMonsterId()<=12 &&sprite2->getProtectedState())
				{
					return false;
				}
				
				return true;
			}
		}
	}

	return false;
}


//此处修改金币碰撞
bool MySpriteManager::collisionDetectTwoSprite(MySprite *sp1, MySprite *sp2)
{
	vector<BoundRadius> sprite1Bounds = sp1->getBoundRadius();
	vector<BoundRadius> sprite2Bounds = sp2->getBoundRadius();
	CCPoint pos1 = sp1->getPosition();
	CCPoint pos2 = sp2->getPosition();
	float r1 = ccp(sp1->getContentSize().width, sp1->getContentSize().height).getLength();

	for(vector<BoundRadius>::iterator iter1 = sprite1Bounds.begin(); iter1 != sprite1Bounds.end(); ++iter1)
	{
		pos1 = getRotateCenterPosOnLayer(sp1, iter1->rCenter);

		for(vector<BoundRadius>::iterator iter2 = sprite2Bounds.begin(); iter2 != sprite2Bounds.end(); ++iter2)
		{
			pos2 = getRotateCenterPosOnLayer(sp2, iter2->rCenter);

			 ItemSprite *item = (ItemSprite *)sp1;

			 if(sqrtf(ccpDistanceSQ(pos1, pos2)) < (iter1->radius + iter2->radius) || ToolsFun::sharedTools()->floatEquals(sqrtf(ccpDistanceSQ(pos1, pos2)), (iter1->radius +  iter2->radius)))
			 {
				 /*两圆心距离小于等于两个圆圈的半径之和 即发生碰撞*/
				 return true;
			 }
			 
		}
	}

	return false;
}

CCPoint MySpriteManager::getRotateCenterPosOnLayer(MySprite* rotateSprite, CCPoint curCenter)
{
	float oldX = curCenter.x;
	float oldY = curCenter.y;
	float rotateAngle =  360 - rotateSprite->getRotation();
	float r = sqrtf(oldX * oldX + oldY * oldY);
	CCPoint posCenter;

	if(ToolsFun::sharedTools()->floatEquals(0, r))
	{
		return rotateSprite->getPosition();
	}
	else
	{
		float angleInit = asin(oldY / r);

		if(oldX < 0)
		{
			angleInit = 3.1415926f - angleInit;
		}

		float angle = angleInit + rotateAngle * 3.1415926f / 180;
		float newX = r * cos(angle);
		float newY = r * sin(angle);
		posCenter = rotateSprite->getPosition() + ccp(newX, newY);
		return posCenter;
	}
}

void MySpriteManager::spriteCollisiion()
{
	return;
	if (m_threadInit == false)
	{
		m_threadInit = true;
		setThreadState(THREAD_RUNING);
		pthread_create(&gDetectionCollision, NULL, collisionDetection, NULL);
	}
}

void MySpriteManager::checkCollision(float t)
{
	monsterWithHeroCollision();
	itemWithHeroCollision();
	bulletWithHero();
	bulletWithMonster();
}

#if 0
void MySpriteManager::pauseAllActionAndSchedule()
{
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		(*iter)->pauseSchedulerAndActions();
	}

	pthread_mutex_unlock(&gSpriteMutex);
	pthread_mutex_lock(&gItemMutex);
	list<ItemSprite*> *itemList  = m_itemLists;

	for(list<ItemSprite*>::iterator iter = itemList->begin(); iter != itemList->end(); ++iter)
	{
		(*iter)->pauseSchedulerAndActions();
	}

	pthread_mutex_unlock(&gItemMutex);
	pthread_mutex_lock(&gMonsterBulletSpriteMutex);
	list<BulletSprite*> *monsterBulletLists  = m_monsterBulletLists;

	for(list<BulletSprite*>::iterator bulletIter = monsterBulletLists->begin(); bulletIter != monsterBulletLists->end(); ++bulletIter)
	{
		(*bulletIter)->pauseSchedulerAndActions();
	}

	pthread_mutex_unlock(&gMonsterBulletSpriteMutex);
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	list<BulletSprite*> *heroBullets  = m_heroBulletLists;

	/*检测出界*/
	for(list<BulletSprite*>::iterator bulletIter = heroBullets->begin(); bulletIter != heroBullets->end(); ++bulletIter)
	{
		(*bulletIter)->pauseSchedulerAndActions();
	}

	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
}

#endif

void MySpriteManager::resumeAllActionAndSchedule()
{
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		(*iter)->resumeSchedulerAndActions();
	}

	pthread_mutex_unlock(&gSpriteMutex);
	pthread_mutex_lock(&gItemMutex);
	list<ItemSprite*> *itemList  = m_itemLists;

	for(list<ItemSprite*>::iterator iter = itemList->begin(); iter != itemList->end(); ++iter)
	{
		(*iter)->resumeSchedulerAndActions();
	}

	pthread_mutex_unlock(&gItemMutex);
	pthread_mutex_lock(&gMonsterBulletSpriteMutex);
	list<BulletSprite*> *monsterBulletLists  = m_monsterBulletLists;

	for(list<BulletSprite*>::iterator bulletIter = monsterBulletLists->begin(); bulletIter != monsterBulletLists->end(); ++bulletIter)
	{
		(*bulletIter)->resumeSchedulerAndActions();
	}

	pthread_mutex_unlock(&gMonsterBulletSpriteMutex);
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	list<HeroBulletSprite*> *heroBullets  = m_heroBulletLists;

	/*检测出界*/
	for(list<HeroBulletSprite*>::iterator bulletIter = heroBullets->begin(); bulletIter != heroBullets->end(); ++bulletIter)
	{
		(*bulletIter)->resumeSchedulerAndActions();
	}

	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
}

void* MySpriteManager::collisionDetection(void *r)
{
	while(true)
	{
		if(gThreadState == THREAD_END)
		{
			break;
		}
		else if(gThreadState == THREAD_PAUSE)
		{
			//LD("dafa");
			continue;
		}
		else
		{
			monsterWithHeroCollision();
			itemWithHeroCollision();
			bulletWithHero();
			bulletWithMonster();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
			usleep(30);
#else
			Sleep(30);
#endif
		}
	}

	return 0;
}
//add zh


MonsterSprite * MySpriteManager::getBeAttackMonster(CCPoint petpos , float attacklength)
{
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite *> *monsterlist = m_monsterLists;

	for(list<MonsterSprite *>::iterator iter = monsterlist->begin(); iter != monsterlist->end(); ++iter)
	{
		if((*iter)->getMonsterId() <  1000)
		{
			CCPoint posSub = (*iter)->getPosition() - petpos;

			if(posSub.getLength() < attacklength)
			{
				pthread_mutex_unlock(&gSpriteMutex);
				return (*iter);
			}
		}
	}

	pthread_mutex_unlock(&gSpriteMutex);
	return NULL;
}



list<MonsterSprite *>* MySpriteManager::getBeAttackMonsterList(CCPoint petpos , float attacklength)
{
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite *> *monsterlist = m_monsterLists;
	list<MonsterSprite *> *attackMonsterList = new list<MonsterSprite*>();

	for(list<MonsterSprite *>::iterator iter = monsterlist->begin(); iter != monsterlist->end(); ++iter)
	{
		if((*iter)->getMonsterId() <  1000)
		{
			CCPoint posSub = (*iter)->getPosition() - petpos;

			if(posSub.getLength() < attacklength)
			{
				pthread_mutex_unlock(&gSpriteMutex);
				attackMonsterList->push_back(*iter);
			}
		}
	}

	pthread_mutex_unlock(&gSpriteMutex);
	return attackMonsterList;
}


//end

void MySpriteManager::monsterWithHeroCollision()
{
	CCRect  worldRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH, WORLD_HEIGHT);
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		if(!(*iter)->getDeadState() && collisionMonsterWithHero(*iter, heroSprite))
		{
			if((*iter)->getMonsterId() < 1000 || heroSprite->getProtectedState() || heroSprite->getSpeedUpState() || heroSprite->getInvincible())
			{

				if((*iter)->getMonsterId()<=7 || (*iter)->getMonsterId()>=13)
				{
					(*iter)->setDeadState(true);
					(*iter)->setDeadType(1);
				}
			}
		

			if(heroSprite->getProtectedState() && !heroSprite->getSpeedUpState() && !heroSprite->getInvincible())
			{
				heroSprite->proEffectEnd(NULL);
			}
			else
			{

				heroSprite->subHp(2.0f);

				if(heroSprite->getHp() < 0)
				{
					heroSprite->setDeadState(true);
					heroSprite->setDeadType(2);
				}
			}
		}
		else
		{
			CCRect rect = (*iter)->boundingBox();

			if(!(worldRect.intersectsRect(rect)))
			{
				(*iter)->setDeadState(true);
				(*iter)->setDeadType(0);
			}
		}
	}

	pthread_mutex_unlock(&gSpriteMutex);
}

void MySpriteManager::itemWithHeroCollision()
{
	pthread_mutex_lock(&gItemMutex);
	list<ItemSprite*> *itemList  = m_itemLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<ItemSprite*>::iterator iter = itemList->begin(); iter != itemList->end(); ++iter)
	{

		CCPoint pos1 = (*iter)->getPosition();
		CCPoint pos2 = heroSprite->getPosition();

		if(pos1.getDistance(pos2)<90.0f)
		{
			(*iter)->setDeadState(true);
			(*iter)->setDeadType(1);
		}
		else
		{
			CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

			if(!(itemRect.intersectsRect((*iter)->boundingBox())))
			{
				(*iter)->setDeadState(true);
				(*iter)->setDeadType(0);
			}
		}

		/*
		if(collisionDetectTwoSprite(*iter, heroSprite))
		{
			(*iter)->setDeadState(true);
			(*iter)->setDeadType(1);
		}*/
	}

	pthread_mutex_unlock(&gItemMutex);
}

void MySpriteManager::bulletWithHero()
{
	pthread_mutex_lock(&gMonsterBulletSpriteMutex);
	list<BulletSprite*> *monsterBulletLists  = m_monsterBulletLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<BulletSprite*>::iterator bulletIter = monsterBulletLists->begin(); bulletIter != monsterBulletLists->end(); ++bulletIter)
	{
		if(!(*bulletIter)->getDeadState() &&  collisionMonsterWithHero(*bulletIter, heroSprite))
		{
			if(!(*bulletIter)->getIsPenetrability())
			{
				(*bulletIter)->setDeadState(true);
				(*bulletIter)->setDeadType(1);
			}

	if(heroSprite->getProtectedState() && !heroSprite->getSpeedUpState() && !heroSprite->getInvincible())
			{
				heroSprite->proEffectEnd(NULL);
			}
			else
{
			MySprite *hSprite = dynamic_cast<MySprite*>(heroSprite);
			hSprite->subHp((*bulletIter)->getDamage());

			if(hSprite->getHp() < 0)
			{
				hSprite->setDeadState(true);
				hSprite->setDeadType(2);
}

			}
		}
	}

	for(list<BulletSprite*>::iterator bulletIter = monsterBulletLists->begin(); bulletIter != monsterBulletLists->end(); ++bulletIter)
	{
		CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

		if(!(itemRect.intersectsRect((*bulletIter)->boundingBox())))
		{
			(*bulletIter)->setDeadState(true);
			(*bulletIter)->setDeadType(0);
		}
	}

	pthread_mutex_unlock(&gMonsterBulletSpriteMutex);
}

void MySpriteManager::bulletWithMonster()
{
	pthread_mutex_lock(&gSpriteMutex);
	pthread_mutex_lock(&gHeroBulletSpriteMutex);
	pthread_mutex_lock(&gPetBulletSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;
	list<HeroBulletSprite*> *heroBullets  = m_heroBulletLists;
	list<BulletSprite*> *petBullets  = m_petBulletLists;

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		for(list<HeroBulletSprite*>::iterator bulletIter = heroBullets->begin(); bulletIter != heroBullets->end(); ++bulletIter)
		{
			if(!(*iter)->getDeadState() && collisionDetectTwoSprite(*iter, *bulletIter))
			{
				/*检测角色子弹和怪物的碰撞*/
				if(!(*bulletIter)->getIsPenetrability())
				{
					(*bulletIter)->setDeadState(true);
					(*bulletIter)->setDeadType(1);
				}

				if((*iter)->getMonsterId() < 1000)
				{
					ToolsFun::sharedTools()->playEffect(UNDER_ATTACK0);
					(*iter)->subHp((*bulletIter)->getDamage());
				}

				if((*iter)->getMonsterId() > 1000 - 1)
				{
					ToolsFun::sharedTools()->playEffect(UNDER_ATTACK1);
				}

				//(*iter)->subHp((*bulletIter)->getDamage());

				if((*iter)->getHp() < 0)
				{
					(*iter)->setDeadState(true);
					(*iter)->setDeadType(2);
				}
			}
			else
			{
				/*检测出界*/
				CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

				if(!(itemRect.intersectsRect((*bulletIter)->boundingBox())))
				{
					(*bulletIter)->setDeadState(true);
					(*bulletIter)->setDeadType(0);
				}
			}
		}

		for(list<BulletSprite*>::iterator petbulletIter = petBullets->begin(); petbulletIter != petBullets->end(); ++petbulletIter)
		{
			int petId = (*petbulletIter)->getPetId();
			int petLevel = 0;

			if(petId != -1)
			{
				petLevel = GameConfigData::initialGameData().getpetLevelById(petId);
			}

			if(!(*iter)->getDeadState() && collisionDetectTwoSprite(*iter, *petbulletIter))
			{
				/*检测宠物子弹和怪物的碰撞*/
				if(!(*petbulletIter)->getIsPenetrability())
				{
					(*petbulletIter)->setDeadState(true);
					(*petbulletIter)->setDeadType(1);
				}

				if((*iter)->getMonsterId() < 1000 || (petId == 1 && petLevel == PET_MAXLEVEL))
				{
					if(petId ==1)  //宠物发出的冰弹，减速作用
					{
	if((*iter)->getMonsterId() < 1000)
						{
						(*iter)->setColor(ccc3(0,153,204));
						(*iter)->slowDown();
						(*iter)->subHp((*petbulletIter)->getDamage());
}
					}
					else
					{
						ToolsFun::sharedTools()->playEffect(UNDER_ATTACK0);
						(*iter)->subHp((*petbulletIter)->getDamage());
					}
				}
				else
				{
					ToolsFun::sharedTools()->playEffect(UNDER_ATTACK1);
				}

				if((*iter)->getMonsterId() > 999 && (*iter)->getMonsterId() !=1007)
				{
					if(petId == 4 && petLevel == PET_MAXLEVEL)
					{
						(*iter)->setDeadState(true);
						(*iter)->setDeadType(2);
						
					}
				}


				if((*iter)->getHp() < 0)
				{
					(*iter)->setDeadState(true);
					(*iter)->setDeadType(2);
				}
			}
			else
			{
				/*检测出界*/
				CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

				if(!(itemRect.intersectsRect((*petbulletIter)->boundingBox())))
				{
					(*petbulletIter)->setDeadState(true);
					(*petbulletIter)->setDeadType(0);
				}
			}
		}
	}

	/*检测出界*/
	for(list<HeroBulletSprite*>::iterator bulletIter = heroBullets->begin(); bulletIter != heroBullets->end(); ++bulletIter)
	{
		CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

		if(!(itemRect.intersectsRect((*bulletIter)->boundingBox())))
		{
			(*bulletIter)->setDeadState(true);
			(*bulletIter)->setDeadType(0);
		}
	}

	/*检测宠物子弹出界*/
	for(list<BulletSprite*>::iterator petbulletIter = petBullets->begin(); petbulletIter != petBullets->end(); ++petbulletIter)
	{
		CCRect itemRect = CCRectMake(WORLD_OR_X, WORLD_OR_Y, WORLD_WIDTH , WORLD_HEIGHT);

		if(!(itemRect.intersectsRect((*petbulletIter)->boundingBox())))
		{
			(*petbulletIter)->setDeadState(true);
			(*petbulletIter)->setDeadType(0);
		}
	}

	pthread_mutex_unlock(&gPetBulletSpriteMutex);
	pthread_mutex_unlock(&gHeroBulletSpriteMutex);
	pthread_mutex_unlock(&gSpriteMutex);
}

void MySpriteManager::setThreadState(THREAD_STATE state)
{
	gThreadState = state;

	if(gThreadState == THREAD_END)
	{
		m_threadInit = false;
	}
}

void MySpriteManager::updateMySprite(float t)
{
	pthread_mutex_lock(&gSpriteMutex);

	for(list<MonsterSprite*>::iterator iter = m_monsterLists->begin(); iter != m_monsterLists->end(); ++iter)
	{
		if((*iter)->getPositionY() > 600 || (*iter)->isBoss() == false)
		{
			(*iter)->updatePosWithMap(t);
		}
	}

	pthread_mutex_unlock(&gSpriteMutex);
	pthread_mutex_lock(&gItemMutex);

	for(list<ItemSprite*>::iterator iter = m_itemLists->begin(); iter != m_itemLists->end(); ++iter)
	{
		(*iter)->updateItemPosWithMap(t);
	}

	GameLayer::sharedGameLayer()->updateEndLinePos(t);

	pthread_mutex_unlock(&gItemMutex);
}

void MySpriteManager::clearAllEnemy()
{
	pthread_mutex_lock(&gSpriteMutex);
	list<MonsterSprite*> *monsterList  = m_monsterLists;
	HeroSprite *heroSprite = GameLayer::sharedGameLayer()->getHeroSprite();

	for(list<MonsterSprite*>::iterator iter = monsterList->begin(); iter != monsterList->end(); ++iter)
	{
		if((*iter)->isVisible() && (*iter)->getDeadState() == false)  // && heroSprite->getDeadState() == false)
		{
			float damage = 50;
			(*iter)->subHp(damage);

			if((*iter)->getHp() < 0 || (ToolsFun::sharedTools()->floatEquals((*iter)->getHp(), 0)))
			{
				(*iter)->setDeadState(true);
				(*iter)->setDeadType(3);
			}
		}
	}

	pthread_mutex_unlock(&gSpriteMutex);
}