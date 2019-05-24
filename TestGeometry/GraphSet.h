#pragma once

class VirtualDC;


enum GraphType{
	GT_Dot      = 0,
	GT_Segment  = 1
};

class GraphSet
{
public:
	GraphSet(void);
	~GraphSet(void);

public:
	bool Add(RealPoint p);
	bool Add(vector<RealPoint> &ps);
	bool Add(RealSegment s);
	void Draw(VirtualDC *pDC);

	UINT GetCount(GraphType t);
	bool GetSegment(UINT n, RealSegment &s);

	bool GetConvexHull(GraphType t, vector<RealPoint> &hull);

	bool Save(LPCTSTR lpFile);
	bool Load(LPCTSTR lpFile);

public:
	static bool ReadFile(UINT &v, CFile &f);
	static bool ReadFile(FLOAT_T &v, CFile &f);
	static bool ReadFile(RealPoint &p, CFile &f);
	static bool ReadFile(RealSegment &s, CFile &f);
	static bool WriteFile(UINT &v, CFile &f);
	static bool WriteFile(FLOAT_T &v, CFile &f);
	static bool WriteFile(RealPoint &p, CFile &f);
	static bool WriteFile(RealSegment &s, CFile &f);

public:
	vector<RealPoint>   m_dots;
	vector<RealSegment> m_sgs;
};