#ifndef MYSCISSORLAYER_H
#define MYSCISSORLAYER_H

#include "cocos2d.h"

using namespace cocos2d;
class MyScissorLayer : public CCLayer
{
public:
	MyScissorLayer();
	~MyScissorLayer();
	virtual bool init();
	virtual void visit();
	CREATE_FUNC(MyScissorLayer);
	void setScissorRect(const CCRect& rect);

protected:
	bool m_touchOnLayer;
	CCNode *m_scrowRootNode;
	CCRect m_rect;
};
#endif