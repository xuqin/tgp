#ifndef SYSTEMTIPNODE_H
#define  SYSTEMTIPNODE_H
#include <iostream>
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;



class SystemTipNode : public CCNode
{
public:
	SystemTipNode();
	~SystemTipNode();
	virtual bool init();
	CREATE_FUNC(SystemTipNode);
	void updateAddtTime(float t);
	void showTip();
	static void setTimeInterval(float t);
	void touch(CCObject *pSender);
	void setParentType(int type);
	void removeMenu(CCObject *pSender);
public:

private:
	static float m_timeInterval;
	int m_parentNodeType;
};
#endif