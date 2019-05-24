#pragma once

typedef double          FLOAT_T;

//////////////////////////////////////////////////////////////////////////
// 
class RealPoint
{
public:
    RealPoint(void);
    RealPoint(const RealPoint& other);
    RealPoint(FLOAT_T ix, FLOAT_T iy);
    RealPoint(CPoint pt);
    ~RealPoint(void);

	void SetValue(FLOAT_T ix, FLOAT_T iy);

    RealPoint& operator=(const RealPoint& other);
    RealPoint operator-(RealPoint &other);

    FLOAT_T x;
    FLOAT_T y;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 
class RealSize
{
public:
    RealSize(void);
    RealSize(const RealSize& other);
    RealSize(FLOAT_T ix, FLOAT_T iy);
    ~RealSize(void);

    RealSize& operator=(const RealSize& other);

    FLOAT_T cx;
    FLOAT_T cy;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
class RealRect
{
public:
	RealRect(void);
	RealRect(const RealRect& other);
	RealRect(FLOAT_T l, FLOAT_T t, FLOAT_T r, FLOAT_T b);
	RealRect(RealPoint lb, RealPoint tr);
	~RealRect();

	RealRect& operator=(const RealRect &other);

	FLOAT_T Width() const;
	FLOAT_T Height() const;
	RealSize Size() const;

	FLOAT_T left, right,
		  top, bottom;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Slope of a segment
enum SlopType{
	ST_Invalid  = 0,
	ST_Infinity = 1,
	ST_Normal   = 2
};

class Slope
{
public:
	Slope();
	~Slope();

public:
	void Init(const RealPoint& p0, const RealPoint& p1);

	int Type() const;
	FLOAT_T Value() const;
	bool operator < (const Slope& s) const;

protected:
	SlopType m_t;
	FLOAT_T  m_v;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
class RealSegment
{
public:
    RealSegment(void);
    RealSegment(RealPoint s, RealPoint e);
    ~RealSegment(void);

    RealPoint m_s, m_e;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 
class SegmentIntersection : public RealPoint
{
public:
	SegmentIntersection(void);
	SegmentIntersection(RealPoint p, RealSegment *s1, RealSegment *s2);
	~SegmentIntersection(void);

	SegmentIntersection& operator=(const SegmentIntersection &other);

	RealSegment *m_pSeg1,
		        *m_pSeg2;
};
//////////////////////////////////////////////////////////////////////////