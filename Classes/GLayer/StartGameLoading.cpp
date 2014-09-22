#include "StartGameLoading.h"
#include "GameLayer.h"
#include "../ToolFun/ToolsFun.h"
#include "../GCData/GameConfigData.h"
#include "../SpriteNode/SpriteAniManager.h"
#include "MainMenu.h"
#include "../GCData/DataInitial.h"
static vector<string> gInGameImageFiles;
static vector<string> gLoadPlists;

static int gLoadedCount;

#define HEROMAXDIFF 1000
#define MONSTERDIFF 300
#define PETDIFF 200
#define PETMAXDIFF 1200
#define subDiff 10

StartGameLoading::StartGameLoading()
{
    gLoadedCount = 0;
    gInGameImageFiles.clear();
    m_loadProgress = NULL;
}

StartGameLoading::~StartGameLoading()
{
}

bool StartGameLoading::init()
{
    initUI();
    initBeLoadedFiles();
    loadImage();
    return true;
}
void StartGameLoading::initUI()
{
    m_loadProgress = CCProgressTimer::create(CCSprite::create("ui/startLoading/role3.jpg"));
    m_loadProgress->setPosition(ToolsFun::sharedTools()->getSceneSize() / 2);
    m_loadProgress->setType(kCCProgressTimerTypeBar);
    m_loadProgress->setPercentage(100);
    m_loadProgress->setMidpoint(ccp(0, 1));
    m_loadProgress->setBarChangeRate(ccp(1, 0));
    this->addChild(m_loadProgress, 1);

    if(!DataInitial::initialDataSource().getLvTag())
    {
        this->initTmx();
    }
    else
    {
        this->initTmx1();
    }
}

void StartGameLoading::initTmx()
{
	/*加载地图事件文件*/
	char tmp[100];
	sprintf(tmp, "levelTmx/level%d.tmx",GameConfigData::initialGameData().getCurLevel());// GameConfigData::initialGameData().getCurLevel());
	
	LD("-----------%s",tmp);
	MapEventsNode *node = MapEventsNode::create(tmp);
	node->readEventsByThread();
	m_loadProgress->setPercentage(10);
}
void StartGameLoading::initTmx1()
{
	    /*加载地图事件文件*/
	char tmp[100];
	sprintf(tmp, "levelTmx/level_sub%d.tmx", GameConfigData::initialGameData().getCurSubLevel()); // GameConfigData::initialGameData().getCurLevel());
	MapEventsNode *node = MapEventsNode::create(tmp);
	node->readEventsByThread();
	m_loadProgress->setPercentage(10);
}

void StartGameLoading::initBeLoadedFiles()
{
    /*加载UI文件*/
    int heroId = GameConfigData::initialGameData().getCurHeroId();
    LD("START LOAD");
    gInGameImageFiles.push_back("ui/gameLayerUI/coin.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/pause.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/pg_dst.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/text_score1.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/wave_cirlcle.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/charge_eff.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/shine.png");
    gInGameImageFiles.push_back("ui/gameLayerUI/god_stat_ring.png");

    gInGameImageFiles.push_back("ui/pause/dialogbox.png");
    gInGameImageFiles.push_back("ui/pause/word.png");
    gInGameImageFiles.push_back("ui/pause/confirm.png");
    gInGameImageFiles.push_back("ui/gameover/end_box.png");
    gInGameImageFiles.push_back("ui/gameover/end_score_bg.png");
    gInGameImageFiles.push_back("ui/gameover/text_pass.png");
    gInGameImageFiles.push_back("ui/gameover/star.png");
    gInGameImageFiles.push_back("ui/gameover/goBackNml.png");
    gInGameImageFiles.push_back("ui/gameover/retryNml.png");
    gInGameImageFiles.push_back("ui/gameover/resumeNml.png");
    /*加载地图文件*/
    int mapId = GameConfigData::initialGameData().getMapBkgId();
    char filename[40];
    sprintf(filename, "mapBkg/0%d.jpg", mapId);
    gInGameImageFiles.push_back(filename);
    char flushName[33];
    sprintf(flushName, "mapBkg/map%d_blur.jpg", mapId);
    gInGameImageFiles.push_back(flushName);

	char heroName[64];
	sprintf(heroName, "hero/hero_%d.png", heroId);
	gInGameImageFiles.push_back(heroName);

    gInGameImageFiles.push_back("hero/magEffect0.png");
    gInGameImageFiles.push_back("hero/magEffect1.png");
	gInGameImageFiles.push_back("hero/mHeroEffect3.png");
	gInGameImageFiles.push_back("hero/mHeroEffect4.png");


	for(int i = 0;i<7;i++)
	{
		char filename[64];
		sprintf(filename, "monster/monster%d.png", i);
		gInGameImageFiles.push_back(filename);
	}

	for(int i = 8;i<13;i++)
	{
		for(int j= 0;i<2;i++)
		{
			char filename[64];
			sprintf(filename, "monster/monster%d_%d.png", i,j);
			gInGameImageFiles.push_back(filename);
		}
	}

	for(int i = 0;i<7;i++)
	{
		char filename[64];
		sprintf(filename, "monster/monster100%d_0.png", i);
		gInGameImageFiles.push_back(filename);
	}

	gInGameImageFiles.push_back("monster/monster1007.png");
	gInGameImageFiles.push_back("monster/smoke.png");
	gInGameImageFiles.push_back("monster/attach.png");
	gInGameImageFiles.push_back("monster/blast.png");
	gInGameImageFiles.push_back("monster/blood.png");
	gInGameImageFiles.push_back("item/item_4.png");
	gInGameImageFiles.push_back("hero/mHeroEffect0.png");
	gInGameImageFiles.push_back("hero/mHeroEffect1.png");
	gInGameImageFiles.push_back("hero/mHeroEffect2.png");

	//int id = GameConfigData::initialGameData().getCurHeroId();

    /*加载提示资源文件*/
    vector<int> tipIds = MapEventsNode::sharedMapEventsNode()->getTipIds();

    for(int i = 0; i != tipIds.size(); ++i)
    {
    }

    /*加载剧情资源文件*/
    vector<int> storyIds = MapEventsNode::sharedMapEventsNode()->getStoryIds();

    for(int i = 0; i != storyIds.size(); ++i)
    {
    }

    /*加载道具资源文件*/
    int itemIds[1] = { 0};

    for(int i = 0; i != 4; ++i)
    {
        for(int j = 0; j != 1; ++j)
        {
            char filename[64];
            sprintf(filename, "item/item%d_%d.png", i, j);
            gInGameImageFiles.push_back(filename);
        }
    }

    /*加载子弹资源*/
    //hero
    for(int i = 0; i != 3; ++i)
    {
    /*    char filename[33];
        sprintf(filename, "bullet/bullet%d.png", i);
        gInGameImageFiles.push_back(filename);
        char maxfilename[33];
        sprintf(maxfilename, "bullet/bullet%d.png", i + HEROMAXDIFF);
        gInGameImageFiles.push_back(maxfilename);
        char sub0filename[33];
        sprintf(sub0filename, "bullet/bullet%d.png", 10+i);
        gInGameImageFiles.push_back(sub0filename);
        char sub1filename[33];
        sprintf(sub1filename, "bullet/bullet%d.png", 20+i);
        gInGameImageFiles.push_back(sub1filename); */
    }

    //pet
    for(int i = 0; i != 6; ++i)
    {
        char filename[33];
        sprintf(filename, "bullet/bullet%d.png", i + PETDIFF);
        gInGameImageFiles.push_back(filename);
        char maxfilename[33];
        sprintf(maxfilename, "bullet/bullet%d.png", i + PETMAXDIFF);
        gInGameImageFiles.push_back(maxfilename);
    }

    //monster boss
    for(int i = 8; i != 13; ++i)
    {
        char filename[33];
        sprintf(filename, "bullet/bullet%d.png", i + MONSTERDIFF);
        gInGameImageFiles.push_back(filename);
    }
}

void StartGameLoading::loadImage()
{
    for(int i = 0; i != gInGameImageFiles.size(); ++i)
    {
        CCTextureCache::sharedTextureCache()->addImageAsync(gInGameImageFiles[i].c_str(), this, callfuncO_selector(StartGameLoading::loadImageCallBack));
    }
}
void StartGameLoading::addLoadPlist()
{
    //加载boss动画
    for(int i = 0; i != 13; i++)
    {
        char filename[33];
        sprintf(filename, "monster/monster%d.plist", i);
        gLoadPlists.push_back(filename);
    }

	gLoadPlists.push_back("monster/monster1007.plist");
	gLoadPlists.push_back("monster/smoke.plist");
	gLoadPlists.push_back("monster/attach.plist");
	gLoadPlists.push_back("monster/blast.plist");
	gLoadPlists.push_back("monster/blood.plist");
	gLoadPlists.push_back("item/item_4.plist");
	gLoadPlists.push_back("hero/mHeroEffect0.plist");
	gLoadPlists.push_back("hero/mHeroEffect1.plist");
	gLoadPlists.push_back("hero/mHeroEffect2.plist");
	gLoadPlists.push_back("hero/mHeroEffect3.plist");
	gLoadPlists.push_back("hero/mHeroEffect4.plist");


	int heroId = GameConfigData::initialGameData().getCurHeroId();
	char filename[64];
	sprintf(filename, "hero/hero_%d.plist", heroId);
	gLoadPlists.push_back(filename);

    for(int i = 0; i != gLoadPlists.size(); ++i)
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(gLoadPlists[i].c_str());
    }
}

void StartGameLoading::loadingFiles()
{
    addLoadPlist();
    /*所以图片加载完成，需要生成部分动画*/
    /*生成角色动画*/
    float aniTime = 0.19f;
    int heroId = GameConfigData::initialGameData().getCurHeroId();

    char filename[64];
    sprintf(filename, "hero%d_%%d.png", heroId);
    SpriteAniManager::getAniManager()->addSpriteEffectAniBypList(filename, 4, aniTime, true);
    /*生成怪物动画*/
 
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster1007_%d.png", 2, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster0_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster1_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster2_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster3_%d.png", 2, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster4_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster5_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster6_%d.png", 4, aniTime, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("monster7_%d.png", 4, aniTime, true);

	map<int, int> monsterBossFrameCount;
	map <int, int>::iterator m2_Iter;
	monsterBossFrameCount.insert(pair <int, int> (8, 2));
    monsterBossFrameCount.insert(pair <int, int> (9, 1));
    monsterBossFrameCount.insert(pair <int, int> (10, 1));
    monsterBossFrameCount.insert(pair <int, int> (11, 1));
    monsterBossFrameCount.insert(pair <int, int> (12, 1));

    for(m2_Iter = monsterBossFrameCount.begin(); m2_Iter != monsterBossFrameCount.end(); m2_Iter++)
    {
        int count = m2_Iter->second;

        for(int i = 0; i < count; i++)
        {
            char filename[64];
            sprintf(filename, "monster%d_%d%%d.png", m2_Iter->first, i);
            SpriteAniManager::getAniManager()->addSpriteEffectAniBypList(filename, 2, 0.5f, true);
        }
    }

	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("item4_%d.png", 6, 0.12f);

    /*生成怪物死亡动画blood*/
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("blood0_%d.png", 4, 0.1f, true);

    /*生成怪物死亡动画smoke*/
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("smoke_%d.png", 6, 0.1f, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("blast_%d.png", 6, 0.1f, true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("attack_%d.png", 6, 0.1f, true);

	/*生成无敌动画*/
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("mHeroEffect0_%d.png",6,0.1f,true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("mHeroEffect1_%d.png",8,0.1f,true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("mHeroEffect2_%d.png",4,0.15f,true);
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("mHeroEffect4_%d.png",5,0.13f,true);

	/*生成保护罩动画*/
	SpriteAniManager::getAniManager()->addSpriteEffectAniBypList("mHeroEffect3_%d.png",10,0.1f,true);
    loadingEnd(0);
}

void StartGameLoading::loadingEnd(float t)
{
    CCScene *scene = CCScene::create();
    GameLayer *gLayer = GameLayer::create();
    scene->addChild(gLayer);
    CCDirector::sharedDirector()->replaceScene(scene);
    gLayer->scheduleStart(); 
}

void StartGameLoading::loadImageCallBack(CCObject *pSender)
{
    gLoadedCount++;
    //float percent = 90 * (float)gLoadedCount / gInGameImageFiles.size();
	float percent = 100 * (float)gLoadedCount / gInGameImageFiles.size();
    m_loadProgress->setPercentage(percent);

    if(gLoadedCount == gInGameImageFiles.size())
    {
        loadingFiles();
    }
}

void StartGameLoading::releaseImageInGame()
{
    for(int i = 0; i != gInGameImageFiles.size(); ++i)
    {
        CCTextureCache::sharedTextureCache()->removeTextureForKey(gInGameImageFiles[i].c_str());
    }

    SpriteAniManager::getAniManager()->releaseAllByInstande(true);
}
