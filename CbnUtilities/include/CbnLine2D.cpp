#include "CbnUtilitiesPCH.h"

float rczEngine::Line2D::Slope() const
{
	return 
		(m_PointTwo.m_y - m_PointOne.m_y) 
		/ 
		(m_PointTwo.m_x - m_PointOne.m_x);
}

float rczEngine::Line2D::XIntercept() const
{
	return -YIntercept() / Slope();
}

float rczEngine::Line2D::YIntercept() const
{
	return m_PointOne.m_y - Slope() * m_PointOne.m_x;
}

bool rczEngine::Line2D::TestPoint(const Vector2 & point)
{
	return Math::Equals(Slope() * point.m_x + YIntercept() - point.m_y, 0.0f, std::numeric_limits<float>::epsilon());
}
