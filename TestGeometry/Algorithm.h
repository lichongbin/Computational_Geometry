#pragma once

bool GetIntersection(RealSegment s1, RealSegment s2, RealPoint &p);
bool GetIntersection(vector<RealSegment> &sgs, vector<RealPoint> &pts);
bool GetIntersection(RealSegment &si, RealRect &rc, RealSegment &so);
bool PointInRectangle(RealPoint p, RealRect rc);

bool SlowConvexHull(vector<RealPoint> &pts, vector<RealPoint> &hull);