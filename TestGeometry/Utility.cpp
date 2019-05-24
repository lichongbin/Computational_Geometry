#include "StdAfx.h"
#include "GraphElement.h"
#include "Utility.h"

FLOAT_T Distance(RealPoint p1, RealPoint p2, DistanceType dt)
{
	FLOAT_T dx = p2.x - p1.x;
	FLOAT_T dy = p2.y - p1.y;

	switch (dt)
	{
	case DT_EU:
		return sqrt(pow(dx, 2) + pow(dy, 2));
	case DT_MH:
		return abs(dx) + abs(dy);
	default:
		return 0;
	}
}