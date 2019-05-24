#include "StdAfx.h"
#include "AlgoLineSweep.h"
#include "Const.h"
#include "Algorithm.h"

SwpSegment::SwpSegment()
{
}

SwpSegment::SwpSegment(RealPoint s, RealPoint e)
{
	Init(s, e);
}

SwpSegment::~SwpSegment()
{}

bool SwpSegment::IsLeftEnd(const RealPoint &p) const
{
	return (EQUAL(m_s.x, p.x) && EQUAL(m_s.y, p.y));
}

bool SwpSegment::IsRightEnd(const RealPoint &p) const
{
	return (EQUAL(m_e.x, p.x) && EQUAL(m_e.y, p.y));
}

bool SwpSegment::IsOnSegment(const RealPoint &p) const
{
	return (!IsLeftEnd(p)) && (!IsRightEnd(p));
}

void SwpSegment::Init(RealPoint s, RealPoint e)
{
	if( LESS(s.x , e.x) || (EQUAL(s.x, e.x) && LESS(s.y, e.y)))
	{
		m_s = s;
		m_e = e;
	}
	else
	{
		m_s = e;
		m_e = s;
	}
	m_k.Init(m_s, m_e);
}

bool SwpSegment::Less(const SwpSegment& s, const RealPoint& p) const
{
	FLOAT_T h1 = Height(p), h2 = s.Height(p);
	return ( LESS(h1, h2) || ((EQUAL(h1, h2) && m_k < s.m_k)) );
}

FLOAT_T SwpSegment::Height(const RealPoint &p) const
{
	if( m_k.Type() == ST_Infinity )
	{
		FLOAT_T py = p.y;
		FLOAT_T ly = m_s.y;
		FLOAT_T ry = m_e.y;
		if(LESS(py, ly))
			return ly;
		else if(LESS(ry , py))
			return ry;
		return py;
	}
	else
	{
		return (m_e.y - m_s.y)*(p.x - m_s.x)/(m_e.x - m_s.x) + m_s.y;
	}
}

bool SwpSegment::Intersect(const SwpSegment &segment, RealPoint &result) const
{
	return GetIntersection(*this, segment, result);
}

void EventQueue::Push(const Event& key)
{
	SegVec v;
	insert(EventType(key, v));
}

void EventQueue::Push(const Event& key, SwpSegment* value)
{
	SegVec v;
	v.push_back(value);
	InsertStatus status = insert(EventType(key, v));
	if( !status.second )
		status.first->second.push_back(value);
}

Event EventQueue::TopEvent()
{
	return begin()->first;
}

SegVec& EventQueue::TopSegVec()
{
	return begin()->second;
}

void EventQueue::Pop()
{
	erase(begin());
}

bool EventQueue::IsExist(const Event& key)const
{
	return find(key) != end();
}