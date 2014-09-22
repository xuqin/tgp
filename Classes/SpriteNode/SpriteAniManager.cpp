#include "SpriteAniManager.h"

SpriteAniManager* SpriteAniManager::m_aniManager = NULL;
SpriteAniManager::SpriteAniManager()
{
}

SpriteAniManager::~SpriteAniManager()
{
	releaseAllAni();
}

SpriteAniManager* SpriteAniManager::getAniManager()
{
	if(!m_aniManager)
	{
		m_aniManager = new SpriteAniManager();
	}

	return m_aniManager;
}

CCAnimation* SpriteAniManager::addSpriteEffectAniBypList(string spriteFrameName, int frameCount, float t, bool isInside)
{
	CCAnimation *aniHad = getSpriteAnimationByName(spriteFrameName);

	if(aniHad)
	{
		return aniHad;
	}

	CCArray *frameAyyay = CCArray::createWithCapacity(frameCount);

	for(int i = 0; i != frameCount; ++i)
	{
		char filename[33];
		sprintf(filename, spriteFrameName.c_str(), i);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename);
		frameAyyay->addObject(frame);
	}

	MyAnimation *myAnimation = new MyAnimation();
	myAnimation->isRelease = isInside;
	myAnimation->animation = CCAnimation::createWithSpriteFrames(frameAyyay, t);
	myAnimation->animation->retain();

	m_effectAniMap.insert(map<string, MyAnimation* >::value_type(spriteFrameName, myAnimation));

	return myAnimation->animation;
}

CCAnimation* SpriteAniManager::addSpriteEffectAniByFiles(string spriteFilename, int frameCount, float t, bool isInside)
{
	CCAnimation *aniHad = getSpriteAnimationByName(spriteFilename);

	if(aniHad)
	{
		return aniHad;
	}

	CCArray *frameAyyay = CCArray::createWithCapacity(frameCount);

	for(int i = 0; i != frameCount; ++i)
	{
		char filename[33];
		sprintf(filename, spriteFilename.c_str(), i);
		CCTexture2D *fileTexture = CCTextureCache::sharedTextureCache()->addImage(filename);
		CCSpriteFrame *frame =   CCSpriteFrame::createWithTexture(fileTexture, CCRect(0, 0, fileTexture->getContentSize().width, fileTexture->getContentSize().height));
		frameAyyay->addObject(frame);
	}

	MyAnimation *myAnimation = new MyAnimation();
	myAnimation->isRelease = isInside;
	myAnimation->animation = CCAnimation::createWithSpriteFrames(frameAyyay, t);
	myAnimation->animation->retain();

	m_effectAniMap.insert(map<string, MyAnimation* >::value_type(spriteFilename, myAnimation));

	return myAnimation->animation;
}

CCAnimation *SpriteAniManager::getSpriteAnimationByName(string firstFrameName)
{
	map<string, MyAnimation*>::iterator findIter = m_effectAniMap.find(firstFrameName);

	if(findIter != m_effectAniMap.end())
	{
		return findIter->second->animation;
	}
	else
	{
		return NULL;
	}
}

void SpriteAniManager::releaseAniManager()
{
	if(m_aniManager)
	{
		delete m_aniManager;
	}
}

void SpriteAniManager::releaseAniByName(string firstFrameName)
{
	map<string, MyAnimation*>::iterator findIter = m_effectAniMap.find(firstFrameName);

	if(findIter != m_effectAniMap.end())
	{
		//findIter->second.animation->release();
		delete findIter->second;
	}
}

void SpriteAniManager::releaseAllAni()
{
	for(map<string, MyAnimation*>::iterator iter = m_effectAniMap.begin(); iter != m_effectAniMap.end(); ++iter)
	{
		//iter->second.animation->release();
		delete iter->second;
	}

	m_effectAniMap.clear();
}

void SpriteAniManager::releaseAllByInstande(bool insande)
{
	CCArray *array = CCArray::create();

	for(map<string, MyAnimation*>::iterator iter = m_effectAniMap.begin(); iter != m_effectAniMap.end(); ++iter)
	{
		if(insande == iter->second->isRelease)
		{
			delete iter->second;
			array->addObject(CCString::create(iter->first.c_str()));
		}
	}

	std::map<string, MyAnimation*>::iterator it;
	CCObject *obj;
	CCARRAY_FOREACH(array, obj)
	{
		CCString *str = (CCString *)obj;

		it = m_effectAniMap.find(str->getCString());
		m_effectAniMap.erase(it);
	}
}
