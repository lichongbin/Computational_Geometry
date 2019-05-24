#include "StdAfx.h"
#include "Const.h"
#include "GraphElement.h"
#include "Algorithm.h"
#include "Utility.h"

// Function：
//          compute relative orientation
// Inputs:  
//          p1 -- first point
//          p2 -- second point
//          p3 -- third point
// Outputs: 
//          
// Return: 
//          cross product
//          if the p1-p3 segment is counterclockwise by p1-p2, 
//          a positive value is returned, otherwise a negative
//          value is returned
FLOAT_T Direction(RealPoint &p1, RealPoint &p2, RealPoint &p3)
{
	FLOAT_T x1 = p2.x - p1.x;
	FLOAT_T y1 = p2.y - p1.y;
	FLOAT_T x2 = p3.x - p1.x;
	FLOAT_T y2 = p3.y - p1.y;
	return (x1 * y2 - x2 * y1);
}

// Function：
//          check p locate between the 
//          two end points of a segment
// Inputs:  
//          s -- segment
//          p -- point of tested
//
// Outputs: 
//          
// Return: 
//        true -- p locate between the 
//                two end points of a segment
//        false -- otherwise
bool IsOnSegment(RealSegment &s, RealPoint &p)
{
	return p.x >= MIN(s.m_s.x, s.m_e.x)
		&& p.x <= MAX(s.m_s.x, s.m_e.x) 
		&& p.y >= MIN(s.m_s.y, s.m_e.y)
		&& p.y <= MAX(s.m_s.y, s.m_e.y);
}

// Function：
//          check whether two points are the
//          same side of a line without care
//          about that the endpoints of one segment
//          lies on the other one.
// Inputs:  
//          s -- line segment
//          p1 -- first point
//          p2 -- second point
// Outputs: 
// 
// Return: true or false
bool IsSameSide(RealSegment s, RealPoint p1, RealPoint p2)
{
    FLOAT_T dx = s.m_e.x - s.m_s.x;
    FLOAT_T dy = s.m_e.y - s.m_s.y;

    FLOAT_T dx1 = p1.x - s.m_s.x;
    FLOAT_T dy1 = p1.y - s.m_s.y;

    FLOAT_T dx2 = p2.x - s.m_e.x;
    FLOAT_T dy2 = p2.y - s.m_e.y;

    FLOAT_T v = (dx * dy1 - dy * dx1) * (dx * dy2 - dy * dx2);
    return v > EPS;
}

// Function：
//          check whether two lines are intersecting
// Inputs:  
//          s1 -- first line
//          s2 -- second line
// Outputs: 
// 
// Return: 
//         true -- intersecting
//         or false
bool IsIntersecting(RealSegment s1, RealSegment s2)
{
//     return !IsSameSide(s1, s2.m_s, s2.m_e) && 
//            !IsSameSide(s2, s1.m_s, s1.m_e);

	FLOAT_T d1 = Direction(s1.m_s, s1.m_e, s2.m_s);
	FLOAT_T d2 = Direction(s1.m_s, s1.m_e, s2.m_e);
	FLOAT_T d3 = Direction(s2.m_s, s2.m_e, s1.m_s);
	FLOAT_T d4 = Direction(s2.m_s, s2.m_e, s1.m_e);

	if ( ((d1 > EPS && d2 < -EPS) || (d1 < -EPS && d2 > EPS)) &&
		 ((d3 > EPS && d4 < -EPS) || (d3 < -EPS && d4 > EPS)) )
	{
		return true;
	}
	else if (d1 >= -EPS && d1 < EPS && IsOnSegment(s1, s2.m_s))
	{
		return true;
	}
	else if (d2 >= -EPS && d2 < EPS && IsOnSegment(s1, s2.m_e))
	{
		return true;
	}
	else if (d3 >= -EPS && d3 < EPS && IsOnSegment(s2, s1.m_s))
	{
		return true;
	}
	else if (d4 >= -EPS && d4 < EPS && IsOnSegment(s2, s1.m_e))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Cohen–Sutherland
const int INSIDE = 0; // 0000
const int LEFT   = 1; // 0001
const int RIGHT  = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP    = 8; // 1000

int ComputeOutCode(FLOAT_T x, FLOAT_T y, RealRect &rc)
{
	int code = INSIDE;          // initialized as being inside of clip window
	if (x < rc.left)          // to the left of clip window
		code |= LEFT;
	else if (x > rc.right)    // to the right of clip window
		code |= RIGHT;
	if (y < rc.bottom)        // below the clip window
		code |= BOTTOM;
	else if (y > rc.top)      // above the clip window
		code |= TOP;

	return code;
}

// Function：
//          check whether a segment and a rectangle are 
//          intersecting ,
//          calculate the intersection
// Inputs:  
//          si -- a segment
//          rc -- a rectangle
// Outputs: 
//          so -- intersection segment
// Return: 
//         true -- intersecting
//         or false
bool GetIntersection(RealSegment &si, RealRect &rc, RealSegment &so)
{
	FLOAT_T	x0 = si.m_s.x,
		    y0 = si.m_s.y,
		    x1 = si.m_e.x,
		    y1 = si.m_e.y;
	int code0 = ComputeOutCode(x0, y0, rc);
	int code1 = ComputeOutCode(x1, y1, rc);

	bool accept = false;

	while (true)
	{
		if (!(code0 | code1)) // 相或为0，接受并且退出循环
		{ 
			so.m_s.x = x0;
			so.m_s.y = y0;
			so.m_e.x = x1;
			so.m_e.y = y1;
			accept = true;
			break;
		} 
		else if (code0 & code1) // 相与为1，拒绝且退出循环
		{ 
			break;
		} 
		else
		{
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			int codeOut = code0 != 0 ? code0 : code1;

			// y = y0 + slope * (x - x0)
			// x = x0 + (1 / slope) * (y - y0)
			if (codeOut & TOP) // point is above the clip rectangle
			{           
				x = x0 + (x1 - x0) * (rc.top - y0) / (y1 - y0);
				y = rc.top;
			}
			else if (codeOut & BOTTOM) // point is below the clip rectangle
			{ 
				x = x0 + (x1 - x0) * (rc.bottom - y0) / (y1 - y0);
				y = rc.bottom;
			} 
			else if (codeOut & RIGHT) // point is to the right of clip rectangle
			{  
				y = y0 + (y1 - y0) * (rc.right - x0) / (x1 - x0);
				x = rc.right;
			}
			else if (codeOut & LEFT) // point is to the left of clip rectangle
			{   
				y = y0 + (y1 - y0) * (rc.left - x0) / (x1 - x0);
				x = rc.left;
			}

			// Now we move outside point to intersection point to clip
			if (codeOut == code0)
			{
				x0 = x;
				y0 = y;
				code0 = ComputeOutCode(x0, y0, rc);
			} 
			else
			{
				x1 = x;
				y1 = y;
				code1 = ComputeOutCode(x1, y1, rc);
			}
		}
	}
	return accept;
}

// Function:
//          calculate the intersecting point of two lines
// Inputs:  
//          s1 -- first line
//          s2 -- second line
// Outputs: 
//          p -- intersecting point
// Return: 
//          true -- these two lines are intersecting and 
//                 their intersecting point is returned
//          false -- failed
bool GetIntersection(RealSegment s1, RealSegment s2, RealPoint &p)
{
    if (!IsIntersecting(s1, s2))
    {
        return false;
    }

	const FLOAT_T a0 = s1.m_s.x;
	const FLOAT_T a1 = s1.m_s.y;
	const FLOAT_T b0 = s1.m_e.x;
	const FLOAT_T b1 = s1.m_e.y;
	
	const FLOAT_T c0 = s2.m_s.x;
	const FLOAT_T c1 = s2.m_s.y;
	const FLOAT_T d0 = s2.m_e.x;
	const FLOAT_T d1 = s2.m_e.y;

	FLOAT_T d = a0 * (d1 - c1) + b0 * (c1 - d1) + d0 * (b1 - a1) + c0 * (a1 - b1);
	if (d > -EPS && d < EPS )
	{
		return false;
	}

	FLOAT_T t1 = (a0 * (d1 - c1) + c0 * (a1 - d1) + d0 * (c1 - a1)) / d;
	FLOAT_T t2 = -(a0 * (c1 - b1) + b0 * (a1 - c1) + c0 * (b1 - a1)) / d;
	if (t1 < -EPS || t1 > 1.f || t2 < -EPS || t2 > 1.f)
	{
		return false;
	}

	FLOAT_T vs1x = s1.m_e.x - s1.m_s.x;
	FLOAT_T vs1y = s1.m_e.y - s1.m_s.y;
	FLOAT_T vs2x = s2.m_e.x - s2.m_s.x;
	FLOAT_T vs2y = s2.m_e.y - s2.m_s.y;

	p.x = abs(vs1x) < abs(vs2x) ? (s1.m_s.x + t1 * vs1x) : (s2.m_s.x + t2 * vs2x);
	p.y = abs(vs1y) < abs(vs2y) ? (s1.m_s.y + t1 * vs1y) : (s2.m_s.y + t2 * vs2y);

	return true;
}

bool PointInRectangle(RealPoint p, RealRect rc)
{
	return (p.x > rc.left 
		 && p.x < rc.right
		 && p.y < rc.top
		 && p.y > rc.bottom);
}

// Function:
//          add a new segment into polygon
// Inputs:  
//          sgs -- polygon
//          s -- start point
//          e -- end point
// Outputs: 
//          
// Return: 
//        
void EnlargePolygon(vector<RealSegment> &sgs, RealPoint &s, RealPoint &e)
{
	bool bFound = false;
	for (vector<RealSegment>::iterator iter = sgs.begin();
		iter != sgs.end(); iter++)
	{
		FLOAT_T d1 = Distance(iter->m_e, s, DT_MH);
		if (d1 < EPS)
		{
			sgs.insert(++iter, RealSegment(s, e));
			bFound = true;
			break;
		}
		FLOAT_T d2 = Distance(iter->m_s, e, DT_MH);
		if (d2 < EPS)
		{
			sgs.insert(iter, RealSegment(s, e));
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		sgs.push_back(RealSegment(s, e));
	}
}

// Function:
//          a brute-force algorithm about convex hull problem
// Inputs:  
//          pts -- convex
//          
// Outputs: 
//          hull -- convex hull
//          
// Return: 
//          true or false
// 
bool SlowConvexHull(vector<RealPoint> &pts, vector<RealPoint> &hull)
{
	hull.clear();

	vector<RealSegment> polygon;
	for (UINT i = 0; i < pts.size(); i++)
	{
		for (UINT j = 0; j < pts.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			bool valid = true;
			for (UINT k = 0; k < pts.size(); k++)
			{
				if (k == i || k == j)
				{
					continue;
				}
				if (Direction(pts[i], pts[j], pts[k]) >= 0)
				{
					valid = false;
					break;
				}
			}

			if (valid)
			{
				EnlargePolygon(polygon, pts[i], pts[j]);
			}
		}
	}

	if (polygon.size() == 0)
	{
		return false;
	}

	for (UINT i = 0; i < polygon.size(); i++)
	{
		hull.push_back(polygon[i].m_s);
	}
	return true;
}