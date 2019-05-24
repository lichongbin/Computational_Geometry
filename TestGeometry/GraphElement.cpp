#include "StdAfx.h"
#include "Const.h"
#include "GraphElement.h"

// Real Point
RealPoint::RealPoint(void)
: x(0)
, y(0)
{
}

RealPoint::RealPoint(FLOAT_T ix, FLOAT_T iy)
: x(ix)
, y(iy)
{
}

RealPoint::RealPoint(const RealPoint& other)
: x(other.x)
, y(other.y)
{
}

RealPoint::RealPoint(CPoint pt)
: x(pt.x)
, y(pt.y)
{
}

RealPoint::~RealPoint(void)
{
}

void RealPoint::SetValue(FLOAT_T ix, FLOAT_T iy)
{
	x = ix;
	y = iy;
}

RealPoint& RealPoint::operator =(const RealPoint& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

RealPoint RealPoint::operator-(RealPoint &other)
{
    return RealPoint(x - other.x, y - other.y);
}

// Real size
RealSize::RealSize(void)
: cx(0)
, cy(0)
{
}

RealSize::RealSize(FLOAT_T ix, FLOAT_T iy)
: cx(ix)
, cy(iy)
{
}

RealSize::RealSize(const RealSize& other)
: cx(other.cx)
, cy(other.cy)
{
}

RealSize::~RealSize(void)
{
}

RealSize& RealSize::operator =(const RealSize& other)
{
    cx = other.cx;
    cy = other.cy;
    return *this;
}

// Real rectangle
RealRect::RealRect(void)
: left(0)
, top(0)
, right(0)
, bottom(0)
{
}

RealRect::RealRect(const RealRect& other)
{
	left = other.left;
	top  = other.top;
	right= other.right;
	bottom = other.bottom;
}

RealRect::RealRect(FLOAT_T l, FLOAT_T t, FLOAT_T r, FLOAT_T b)
: left(l)
, top(t)
, right(r)
, bottom(b)
{
}

RealRect::RealRect(RealPoint lb, RealPoint tr)
: left(lb.x)
, top(tr.y)
, right(tr.x)
, bottom(lb.y)
{
}

RealRect::~RealRect()
{
}

RealRect& RealRect::operator=(const RealRect &other)
{
	left = other.left;
	top  = other.top;
	right= other.right;
	bottom = other.bottom;
	return *this;
}

FLOAT_T RealRect::Width() const
{
	return right-left;
}

FLOAT_T RealRect::Height() const
{
	return top-bottom;
}

RealSize RealRect::Size() const
{
	return RealSize(right-left, top - bottom);
}


// slope
Slope::Slope()
: m_t(ST_Invalid)
, m_v(0)
{
}

Slope::~Slope()
{}

void Slope::Init(const RealPoint& p0, const RealPoint& p1)
{
	if( EQUAL(p0.x, p1.x) )
	{
		m_t = ST_Infinity;
	}
	else
	{
		m_t = ST_Normal;
		m_v = (p1.y - p0.y) / (p1.x - p0.x);
	}
}

int Slope::Type() const
{
	return m_t;
}


bool Slope::operator < (const Slope& s) const
{
	if( m_t == ST_Infinity) 
		return false;
	return (s.m_t == ST_Infinity || LESS(m_v, s.m_v));
}


// line segment
RealSegment::RealSegment(void)
: m_s(0, 0)
, m_e(0, 0)
{
}

RealSegment::RealSegment(RealPoint s, RealPoint e)
: m_s(s)
, m_e(e)
{
}

RealSegment::~RealSegment(void)
{
}

// intersection of two segments
SegmentIntersection::SegmentIntersection(void)
: m_pSeg1(NULL)
, m_pSeg2(NULL)
{
}

SegmentIntersection::SegmentIntersection(RealPoint p, RealSegment *s1, RealSegment *s2)
: RealPoint(p)
, m_pSeg1(s1)
, m_pSeg2(s2)
{
}

SegmentIntersection::~SegmentIntersection(void)
{
}

SegmentIntersection& SegmentIntersection::operator=(const SegmentIntersection &other)
{
	x = other.x;
	y = other.y;
	m_pSeg1 = other.m_pSeg1;
	m_pSeg2 = other.m_pSeg2;
	return *this;
}