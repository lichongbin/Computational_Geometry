#pragma once

enum DistanceType{
	DT_EU = 0, // Euclidean
	DT_MH = 1, // Manhattan
};

FLOAT_T Distance(RealPoint p1, RealPoint p2, DistanceType dt);