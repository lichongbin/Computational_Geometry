#pragma once
#include "graphelement.h"
#include "Const.h"

class SwpSegment : public RealSegment
{
public:
	SwpSegment();
	SwpSegment(RealPoint s, RealPoint e);
	~SwpSegment();

public:
	bool IsLeftEnd(const RealPoint &p) const;
	bool IsRightEnd(const RealPoint &p) const;
	bool IsOnSegment(const RealPoint &p) const;

	void Init(RealPoint s, RealPoint e);
	bool Less(const SwpSegment& s, const RealPoint& p) const;
	FLOAT_T Height(const RealPoint &p) const;

	bool Intersect(const SwpSegment &segment, RealPoint &result) const;

public:
	Slope     m_k;
};

// Event type
enum EventType{
	ET_Start = 0,
	ET_End   = 1,
	ET_Inter = 2
};

// 
class Event
{
public:
	Event()
		: m_evt(ET_Start)
	{}
	Event(EventType t, RealPoint p)
		: m_evt(t)
		, m_pt(p)
	{}
	Event(const Event &other)
		: m_evt(other.m_evt)
		, m_pt(other.m_pt)
	{}

	bool operator < (const Event& b)const
	{
		if( EQUAL(m_pt.x, b.m_pt.x) )
			return LESS(m_pt.y, b.m_pt.y);
		return LESS(m_pt.x, b.m_pt.x);
	}

	EventType    m_evt;
	RealPoint    m_pt;
};

struct sCompare
{
	RealPoint* mComp;
	sCompare(RealPoint* p):mComp(p)
	{}
	bool operator()(SwpSegment*a, SwpSegment*b) const
	{
		bool result = a->Less(*b, *mComp);
		return a->Less(*b,*mComp);
	}
};

typedef set<SwpSegment*, sCompare>		    SegBST;
typedef SegBST::iterator			        SegBSTIterator;

typedef vector<SwpSegment*>			        SegVec;
typedef SegVec::iterator			        SegVecIterator;

typedef map<Event, vector<SwpSegment*>>	    PriorityQueue;
typedef PriorityQueue::iterator			    PriorityQueueIterator;

class EventQueue : public PriorityQueue
{
	typedef pair<typename PriorityQueue::iterator, bool> InsertStatus;
	typedef pair<Event, SegVec> EventType;
public:
	void Push(const Event& key);
	void Push(const Event& key, SwpSegment* value);

	Event TopEvent();
	SegVec& TopSegVec();
	void Pop();
	bool IsExist(const Event& key) const;
};