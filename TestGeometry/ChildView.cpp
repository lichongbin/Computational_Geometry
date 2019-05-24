
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "TestGeometry.h"
#include "Const.h"
#include "GraphElement.h"
#include "ChildView.h"
#include "VirtualDC.h"
#include "Algorithm.h"
#include "GraphSet.h"
#include "Utility.h"
#include "GenerateSegment.h"
#include "UserMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
CChildView::CChildView()
{
    m_pDC = new VirtualDC();
    
    m_hDrag = NULL;
    m_hCross = NULL;
    m_hCurrent = NULL;

    m_dm = DM_NONE;
    m_bStartLine = false;

	m_graph = new GraphSet();
}

CChildView::~CChildView()
{
	if (m_graph != NULL)
	{
		delete m_graph;
		m_graph = NULL;
	}

    if (m_pDC != NULL)
    {
        m_pDC->Release();
        delete m_pDC;
        m_pDC = NULL;
    }
}

bool CChildView::GetCurrentCoordinate(FLOAT_T &x, FLOAT_T &y)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	RealPoint tp(pt);
	m_pDC->DPtoLP(&tp);
	x = tp.x;
	y = tp.y;
	return true;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_MBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_SETCURSOR()
    ON_WM_CONTEXTMENU()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_UPDATE_COMMAND_UI(ID_DRAW_DOT, &CChildView::OnUpdateDrawDot)
    ON_COMMAND(ID_DRAW_DOT, &CChildView::OnDrawDot)
    ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CChildView::OnUpdateDrawLine)
    ON_COMMAND(ID_DRAW_LINE, &CChildView::OnDrawLine)
    ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CChildView::OnUpdateDrawRect)
    ON_COMMAND(ID_DRAW_RECT, &CChildView::OnDrawRect)
    ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CChildView::OnUpdateDrawCircle)
    ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
    ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, &CChildView::OnUpdateDrawPolygon)
    ON_COMMAND(ID_DRAW_POLYGON, &CChildView::OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_TEST_SEG_INTERSECT, &CChildView::OnUpdateTestSegIntersect)
	ON_COMMAND(ID_TEST_SEG_INTERSECT, &CChildView::OnTestSegIntersect)
    ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CChildView::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE, &CChildView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CChildView::OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_TEST_SLOW_CONVEX_HULL, &CChildView::OnUpdateTestSlowConvexHull)
	ON_COMMAND(ID_TEST_SLOW_CONVEX_HULL, &CChildView::OnTestSlowConvexHull)
	ON_UPDATE_COMMAND_UI(ID_RANDOM_LINE, &CChildView::OnUpdateRandomLine)
	ON_COMMAND(ID_RANDOM_LINE, &CChildView::OnRandomLine)
	ON_UPDATE_COMMAND_UI(ID_TEST_MULTI_SEG, &CChildView::OnUpdateTestMultiSeg)
	ON_COMMAND(ID_TEST_MULTI_SEG, &CChildView::OnTestMultiSeg)
END_MESSAGE_MAP()


// CChildView 消息处理程序
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	
    CRect rc;
    GetClientRect(&rc);

// 	Pen p(Color(128, 0, 255, 255));
// 
// 	Graphics g(dc.GetSafeHdc());
// 	g.DrawLine(&p, 458.0000f ,472.5000f, 1458.0000f, 472.5000f);
// 	g.DrawLine(&p, 958.0000f ,972.5000f, 958.0000f, -27.5000f);
// 
// 	SolidBrush b(Color(128, 255, 0, 0));
// 	g.FillEllipse(&b, 956.0f, 470.5f, 4.0f, 4.0f);

    m_pDC->Begin(&dc, rc.Width(), rc.Height());

    m_pDC->DrawBackground();
	m_graph->Draw(m_pDC);

    switch (m_dm)
    {
    case DM_LINE:
        {
            if (m_bStartLine)
            {
                Pen p1(Color(255, 10, 10, 255));
                m_pDC->SetCurrentPen(&p1);
                m_pDC->DrawLine(m_sg.m_s, m_sg.m_e);
                m_pDC->SetCurrentPen(NULL);
            }
            break;
        }
    default:
        {
            break;
        }
    }

	if (m_hull.size() >= 3)
	{
		Pen p(Color(255, 10, 255, 10));
		m_pDC->SetCurrentPen(&p);
		m_pDC->DrawPolygon(m_hull);
// 		for (UINT i = 1; i < m_hull.size(); i++)
// 		{
// 			m_pDC->DrawLine(m_hull[i-1], m_hull[i]);
// 		}
// 		m_pDC->DrawLine(m_hull[m_hull.size()-1], m_hull[0]);
		m_pDC->SetCurrentPen(NULL);
	}

    m_pDC->End();
    m_pDC->Release();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_ptLButtonDown.x = point.x;
    m_ptLButtonDown.y = point.y;
    m_pDC->DPtoLP(&m_ptLButtonDown);

    SetCapture();
    CRect rc;
    GetClientRect(&rc);
    ClientToScreen(&rc);
    ClipCursor(rc);

    switch (m_dm)
    {
	case DM_DOT:
		{
			break;
		}
    case DM_LINE:
        {
            if (m_bStartLine)
            {
                RealPoint tp(point);
                m_pDC->DPtoLP(&tp);
                m_sg.m_e = tp;
                m_bStartLine = false;

                m_graph->Add(m_sg);
            }
            else
            {
                m_sg.m_s = m_sg.m_e = m_ptLButtonDown;
                m_bStartLine = true;
            }
            InvalidateRect(NULL);
            UpdateWindow();
            break;
        }
    default:
        {
            break;
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	RealPoint p(point);
	m_pDC->DPtoLP(&p);

	switch (m_dm)
	{
	case DM_DOT:
		{
			FLOAT_T v = Distance(p, m_ptLButtonDown, DT_MH);
			if (v < EPS)
			{
				m_graph->Add(p);
				InvalidateRect(NULL);
			}
			break;
		}
	}

    ClipCursor(NULL);
    ReleaseCapture();

    CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnMButtonDown(UINT nFlags, CPoint point)
{
    SetCapture();
    CRect rc;
    GetClientRect(&rc);
    ClientToScreen(&rc);
    ClipCursor(rc);

    m_ptMButtonDown.x = point.x;
    m_ptMButtonDown.y = point.y;
    m_pDC->DPtoLP(&m_ptMButtonDown);

    m_hCurrent = m_hDrag;
    SetCursor(m_hCurrent);

    CWnd::OnMButtonDown(nFlags, point);
}

void CChildView::OnMButtonUp(UINT nFlags, CPoint point)
{
    m_hCurrent = NULL;

    ClipCursor(NULL);
    ReleaseCapture();
    CWnd::OnMButtonUp(nFlags, point);
}

void CChildView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnMButtonDblClk(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnRButtonDblClk(nFlags, point);
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hCurrent != NULL)
    {
        SetCursor(m_hCurrent);
        return TRUE;
    }
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CChildView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
    // TODO: Add your message handler code here
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar)
    {
    case VK_ESCAPE:
        {
            switch (m_dm)
            {
            case DM_LINE:
                {
                    if (m_bStartLine)
                    {
                        m_bStartLine = false;
                        InvalidateRect(NULL);
                        UpdateWindow();
                    }
                    else
                    {
                        m_dm = DM_NONE;
                        m_hCurrent = NULL;
                    }
                    break;
                }
			case DM_DOT:
				{
					m_dm = DM_NONE;
					m_hCurrent = NULL;
					break;
				}
            }

            break;
        }
	case VK_ADD:
		{
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			m_pDC->ZoomIn(RealPoint(pt));
			InvalidateRect(NULL);
			break;
		}
	case VK_SUBTRACT:
		{
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			m_pDC->ZoomOut(RealPoint(pt));
			InvalidateRect(NULL);
			break;
		}
    default:
        {
            break;
        }
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	RealPoint tp(point);
	m_pDC->DPtoLP(&tp);
	CWnd *p = GetParent();
	if (p != NULL)
	{
		p->PostMessage(WM_UPDATE_COORDINATE);
	}

    // Middle button down
    if (nFlags & MK_MBUTTON)
    {
        RealPoint pe(point);
        m_pDC->DPtoLP(&pe);

        RealPoint ptOffset = pe - m_ptMButtonDown;
        RealPoint rp = m_pDC->GetWindowOrg();
        rp.x -= ptOffset.x;
        rp.y -= ptOffset.y;
        m_pDC->SetWindowOrg(rp);

        InvalidateRect(NULL);
        UpdateWindow();
        return;
    }
 
    switch (m_dm)
    {
    case DM_NONE:
        {
           break;
        }
    case DM_LINE:
        {
            if (m_bStartLine)
            {
                m_sg.m_e = tp;
                InvalidateRect(NULL);
                UpdateWindow();
            }

            break;
        }
    default:
        break;
    }

    CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    ScreenToClient(&pt);
    RealPoint rp(pt.x, pt.y);

    while (zDelta > 0)
    {
        m_pDC->ZoomIn(rp);

        InvalidateRect(NULL);
        UpdateWindow();

        zDelta -= WHEEL_DELTA;
    }

    while (zDelta < 0)
    {
        m_pDC->ZoomOut(rp);

        InvalidateRect(NULL);
        UpdateWindow();

        zDelta += WHEEL_DELTA;
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    m_pDC->SetDeviceExtent(RealSize(cx, cy));
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    RealSize szWin(PLATFORM_SIZE, PLATFORM_SIZE),
             szView(PLATFORM_SIZE, PLATFORM_SIZE);
    m_pDC->SetMapParam(szWin, szView,
        RealPoint(0 - szWin.cx / 2, szWin.cy / 2),
        RealPoint(0, 0));

    CWinApp *p = AfxGetApp();
    if (p != NULL)
    {
        m_hDrag = p->LoadCursor(IDC_CURSOR_DRAG);
        m_hCross = p->LoadStandardCursor(IDC_CROSS);
    }

// #ifdef _DEBUG
// 	m_lines.push_back(RealSegment(RealPoint(-500, 100), RealPoint(500, 100)));
// 	m_lines.push_back(RealSegment(RealPoint(-200, -300), RealPoint(-200, 300)));
// #endif

    return 0;
}

void CChildView::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnFileOpen()
{
	CFileDialog f(TRUE, _T(".mgf"), _T("sample"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("My graph file (*.mgf) | *.mgf||"),
		this);
	if (f.DoModal() == IDOK)
	{
		CString s = f.GetPathName();
		if (m_graph->Load(s))
		{
			InvalidateRect(NULL, FALSE);
		}		
	}
}

void CChildView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnFileSave()
{
	CFileDialog f(FALSE,
		_T(".mgf"), _T("sample"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("My graph file (*.mgf) | *.mgf||"),
		this);
	if (f.DoModal() == IDOK)
	{
		CString s = f.GetPathName();
		if (m_graph->Save(s))
		{
			AfxMessageBox(_T("OK!"));
		}
	}
}

void CChildView::OnUpdateDrawDot(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnDrawDot()
{
	switch (m_dm)
	{
	case DM_DOT:
		{
			m_hCurrent = NULL;
			m_dm = DM_NONE;
			return;
		}
	case DM_LINE:
		{
			m_bStartLine = false;
			break;
		}
	default:
		{
			m_hCurrent = m_hCross;
			m_dm = DM_DOT;
			break;
		}
	}
}

void CChildView::OnUpdateDrawLine(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

void CChildView::OnDrawLine()
{
    if (m_dm == DM_LINE)
    {
        m_dm = DM_NONE;
        m_bStartLine = false;
        m_hCurrent = NULL;
    }
    else
    {
        m_dm = DM_LINE;
        m_bStartLine = false;
        m_hCurrent = m_hCross;
    }
}


void CChildView::OnUpdateDrawRect(CCmdUI *pCmdUI)
{

}

void CChildView::OnDrawRect()
{

}

void CChildView::OnUpdateDrawCircle(CCmdUI *pCmdUI)
{

}

void CChildView::OnDrawCircle()
{

}

void CChildView::OnUpdateDrawPolygon(CCmdUI *pCmdUI)
{

}

void CChildView::OnDrawPolygon()
{

}

void CChildView::OnDestroy()
{
    CWnd::OnDestroy();

    DestroyCursor(m_hCross);
    DestroyCursor(m_hDrag);
}

void CChildView::OnUpdateTestSegIntersect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnTestSegIntersect()
{
	if (m_graph->GetCount(GT_Segment) >= 2)
	{
		RealSegment s1, s2;
		m_graph->GetSegment(0, s1);
		m_graph->GetSegment(1, s2);

		RealPoint p;
		if (GetIntersection(s1, s2, p))
		{
			m_graph->Add(p);
			InvalidateRect(NULL);
			UpdateWindow();
		}
	}
}

void CChildView::OnUpdateTestMultiSeg(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnTestMultiSeg()
{
	vector<RealPoint> pts;
	if (GetIntersection(m_graph->m_sgs, pts))
	{
		m_graph->Add(pts);
		InvalidateRect(NULL);
	}
}

void CChildView::OnUpdateTestSlowConvexHull(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnTestSlowConvexHull()
{
	if (m_graph->GetCount(GT_Dot) >= 3)
	{
		m_graph->GetConvexHull(GT_Dot, m_hull);
		InvalidateRect(NULL);
	}
}

void CChildView::OnUpdateRandomLine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnRandomLine()
{
	GenerateSegment dlg;
	if (dlg.DoModal() == IDOK)
	{
		float left = dlg.m_fLeft;
		float right= dlg.m_fRight;
		float top  = dlg.m_fTop;
		float bottom = dlg.m_fBottom;

		int nXRange = int(right - left);
		int nYRange = int(top - bottom);

		srand(GetTickCount());
		int nCount = dlg.m_nCount;
		for (int i = 0; i < nCount; i++)
		{
			RealSegment s;
			s.m_s.x = rand() % nXRange - nXRange / 2;
			s.m_s.y = rand() % nXRange - nXRange / 2;

			s.m_e.x = rand() % nYRange - nYRange / 2;
			s.m_e.y = rand() % nYRange - nYRange / 2;
			m_graph->Add(s);
		}

		InvalidateRect(NULL);
	}
}
