#include "MyScissorLayer.h"

MyScissorLayer::MyScissorLayer(): m_touchOnLayer(false)
{
	this->setTouchEnabled(true);
}

MyScissorLayer::~MyScissorLayer()
{

}

bool MyScissorLayer::init()
{

	return true;
}

void MyScissorLayer::visit()
{
	glEnable(GL_SCISSOR_TEST);
	CCLayer::visit();

	CCPoint pos = m_rect.origin + this->getPosition(); //**������������½�**//
	CCSize size = m_rect.size;                                           //**��������Ĵ�С**//
	CCEGLView::sharedOpenGLView()->setScissorInPoints(pos.x, pos.y, size.width, size.height);

	glDisable(GL_SCISSOR_TEST);
}

void MyScissorLayer::setScissorRect(const CCRect& rect)
{
	m_rect = rect;
}