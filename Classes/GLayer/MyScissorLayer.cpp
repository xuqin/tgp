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

	CCPoint pos = m_rect.origin + this->getPosition(); //**世界坐标的左下角**//
	CCSize size = m_rect.size;                                           //**剪切区域的大小**//
	CCEGLView::sharedOpenGLView()->setScissorInPoints(pos.x, pos.y, size.width, size.height);

	glDisable(GL_SCISSOR_TEST);
}

void MyScissorLayer::setScissorRect(const CCRect& rect)
{
	m_rect = rect;
}