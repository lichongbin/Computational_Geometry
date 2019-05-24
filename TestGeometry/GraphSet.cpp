#include "StdAfx.h"
#include "GraphElement.h"
#include "GraphSet.h"
#include "VirtualDC.h"
#include "Const.h"
#include "Utility.h"
#include "Algorithm.h"

GraphSet::GraphSet(void)
{
}

GraphSet::~GraphSet(void)
{
}

bool GraphSet::Add(RealPoint p)
{
	for (vector<RealPoint>::iterator iter = m_dots.begin();
		iter != m_dots.end(); iter++)
	{
		if (Distance(p, *iter, DT_MH) < EPS)
		{
			return false;
		}
	}

	m_dots.push_back(p);
	return true;
}

bool GraphSet::Add(vector<RealPoint> &ps)
{
	m_dots.insert(m_dots.end(), ps.begin(), ps.end());
	return true;
}

bool GraphSet::Add(RealSegment s)
{
	for (vector<RealSegment>::iterator iter = m_sgs.begin();
		iter != m_sgs.end(); iter++)
	{
		if (Distance(s.m_s, iter->m_s, DT_MH) < EPS &&
			Distance(s.m_e, iter->m_e, DT_MH) < EPS)
		{
			return false;
		}
	}

	m_sgs.push_back(s);
	return true;
}

void GraphSet::Draw(VirtualDC *pDC)
{
	RealRect rcVisible = pDC->GetLogicalVisibleRect();

	// dots
	SolidBrush brush(Color(255, 0, 0, 255));
	Pen p1(Color(255, 255, 0, 0));
	Brush *pob = pDC->SetCurrentBrush(&brush);
	Pen *pop = pDC->SetCurrentPen(&p1);
	for (vector<RealPoint>::iterator iter = m_dots.begin();
		iter != m_dots.end(); iter++)
	{
		if (!PointInRectangle(*iter, rcVisible))
		{
			continue;
		}

		RealRect rc(iter->x - DOT_SIZE, iter->y + DOT_SIZE, 
			iter->x + DOT_SIZE, iter->y - DOT_SIZE);
		pDC->FillEllipse(rc);
		pDC->DrawEllipse(rc);

// #ifdef _DEBUG
// 		Font myFont(_T("Arial"), 8);
// 		Font *pof = pDC->SetCurrentFont(&myFont);
// 		TCHAR sz[32] = {0};
// 		_stprintf(sz, _T("%.2f, %.2f"),
// 			iter->x, iter->y);
// 		pDC->DrawText(sz, *iter);
// 		pDC->SetCurrentFont(pof);
// #endif // _DEBUG
	}
	pDC->SetCurrentPen(pop);
	pDC->SetCurrentBrush(pob);

	// segments
	Pen p2(Color(255, 0, 255, 255));
	pop = pDC->SetCurrentPen(&p2);
	for (vector<RealSegment>::iterator iter = m_sgs.begin();
		iter != m_sgs.end(); iter++)
	{
		RealSegment si;
		if (GetIntersection(*iter, rcVisible, si))
		{
			pDC->DrawLine(si.m_s, si.m_e);
			FLOAT_T theta = atan2(si.m_e.y - si.m_s.y,
				                  si.m_e.x - si.m_s.x);
			pDC->DrawArrow(si.m_e, theta, 10, PI/10.);
		}
	}
	pDC->SetCurrentPen(pop);
}

UINT GraphSet::GetCount(GraphType t)
{
	switch (t)
	{
	case GT_Dot:
		return m_dots.size();
	case GT_Segment:
		return m_sgs.size();
	default:
		return 0;
	}
}

bool GraphSet::GetSegment(UINT n, RealSegment &s)
{
	if (n >= m_sgs.size())
	{
		return false;
	}

	s = m_sgs[n];
	return true;
}

bool GraphSet::GetConvexHull(GraphType t, vector<RealPoint> &hull)
{
	switch (t)
	{
	case GT_Dot:
		return SlowConvexHull(m_dots, hull);
	case GT_Segment:
		{
			break;
		}
	default:
		break;
	}
	return false;
}

bool GraphSet::Save(LPCTSTR lpFile)
{
	UINT nDots = m_dots.size(),
		nSegments = m_sgs.size();
	if (0 == nDots && 0 == nSegments)
	{
		return true;
	}

	CFile f;
	if (!f.Open(lpFile, CFile::modeCreate | CFile::modeWrite))
	{
		return false;
	}

	// dots
	if (!WriteFile(nDots, f))
	{
		f.Close();
		return false;
	}
	for (UINT i = 0; i < nDots; i++)
	{
		if (!WriteFile(m_dots[i], f))
		{
			f.Close();
			return false;
		}
	}

	// segments
	if (!WriteFile(nSegments, f))
	{
		f.Close();
		return false;
	}
	for (UINT i = 0; i < nSegments; i++)
	{
		if (!WriteFile(m_sgs[i], f))
		{
			f.Close();
			return false;
		}
	}

	f.Close();
	return true;
}

bool GraphSet::Load(LPCTSTR lpFile)
{
	CFile f;
	if (!f.Open(lpFile, CFile::modeRead))
	{
		return false;
	}

	// dots
	m_dots.clear();
	UINT nDots = 0;
	if (!ReadFile(nDots, f))
	{
		f.Close();
		return false;
	}
	for (UINT i = 0; i < nDots; i++)
	{
		RealPoint p;
		if (!ReadFile(p, f))
		{
			break;
		}
		m_dots.push_back(p);
	}

	if (nDots != m_dots.size())
	{
		f.Close();
		return false;
	}

	// segments
	m_sgs.clear();
	UINT nSegments = 0;
	if (!ReadFile(nSegments, f))
	{
		f.Close();
		return false;
	}
	for (UINT i = 0; i < nSegments; i++)
	{
		RealSegment s;
		if (!ReadFile(s, f))
		{
			break;
		}
		TRACE(_T("(%8.2f, %8.2f) -> (%8.2f, %8.2f)\n"),
			s.m_s.x, s.m_s.y,
			s.m_e.x, s.m_e.y);
		m_sgs.push_back(s);
	}
	if (nSegments != m_sgs.size())
	{
		f.Close();
		return false;
	}

	f.Close();
	return true;
}

bool GraphSet::ReadFile(UINT &v, CFile &f)
{
	UINT nRead = f.Read(&v, sizeof(UINT));
	return (nRead == sizeof(UINT));
}

bool GraphSet::ReadFile(FLOAT_T &v, CFile &f)
{
	UINT nRead = f.Read(&v, sizeof(FLOAT_T));
	return (nRead == sizeof(FLOAT_T));
}

bool GraphSet::ReadFile(RealPoint &p, CFile &f)
{
	return ReadFile(p.x, f) && ReadFile(p.y, f);
}

bool GraphSet::ReadFile(RealSegment &s, CFile &f)
{
	return ReadFile(s.m_s, f) && ReadFile(s.m_e, f);
}

bool GraphSet::WriteFile(UINT &v, CFile &f)
{
	f.Write(&v, sizeof(UINT));
	return true;
}

bool GraphSet::WriteFile(FLOAT_T &v, CFile &f)
{
	f.Write(&v, sizeof(FLOAT_T));
	return true;
}

bool GraphSet::WriteFile(RealPoint &p, CFile &f)
{
	return WriteFile(p.x, f) && WriteFile(p.y, f);
}

bool GraphSet::WriteFile(RealSegment &s, CFile &f)
{
	return WriteFile(s.m_s, f) && WriteFile(s.m_e, f);
}