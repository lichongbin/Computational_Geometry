#include "StdAfx.h"
#include "GraphElement.h"
#include "VirtualDC.h"
#include "Const.h"

VirtualDC::VirtualDC(void)
: m_dc(NULL)
, m_mdc(NULL)
, m_bmp(NULL)
, m_pOldBmp(NULL)
, m_szBmp(0, 0)
, m_pGraphics(NULL)
, m_pAxis(NULL)
, m_pCurrentPen(NULL)
, m_pCurrentBrush(NULL)
, m_pCurrentFont(NULL)
, m_clrBgd(0, 0, 0)
, m_clrAxis(60, 20, 220, 120)
, m_clrRuler(255, 255, 255, 255)
, m_clrScale(200, 20, 20)
, m_fZoomScale(1.0f)
{
    Reset();
}

VirtualDC::~VirtualDC(void)
{
}

void VirtualDC::Begin(CDC *dc, int cx, int cy)
{
    TRACE(_T("VirtualDC::Begin...\n"));
#ifdef _DEBUG
    m_dwTick = GetTickCount();
#endif

    m_dc = dc;
    if (cx <= 0 || cy <= 0)
    {
        return;
    }

    if (m_dc != NULL)
    {
        m_mdc = new CDC();
        m_mdc->CreateCompatibleDC(m_dc);

        m_szBmp.cx = cx;
        m_szBmp.cy = cy;
        m_bmp = new CBitmap();
        m_bmp->CreateCompatibleBitmap(m_dc, m_szBmp.cx, m_szBmp.cy);
        m_pOldBmp = m_mdc->SelectObject(m_bmp);

        m_pGraphics = new Graphics(m_mdc->GetSafeHdc());
        m_pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

        m_pAxis = new Pen(m_clrAxis);
		m_pRuler = new Pen(m_clrRuler);
    }
    Clear();
}

void VirtualDC::End()
{
    if (m_szBmp.cx > 0 && m_szBmp.cy > 0)
    {
        m_dc->BitBlt(0, 0, m_szBmp.cx, m_szBmp.cy, m_mdc, 0, 0, SRCCOPY);
    }
#ifdef _DEBUG
    DWORD dw = GetTickCount();
    TRACE(_T("%dms \n"), dw - m_dwTick);
#endif
    TRACE(_T("VirtualDC::End\n"));
}

void VirtualDC::Release()
{
    m_dc = NULL;

    if (m_mdc != NULL)
    {
        m_mdc->SelectObject(m_pOldBmp);

        delete m_mdc;
        m_mdc = NULL;
    }

    if (m_bmp != NULL)
    {
        delete m_bmp;
        m_bmp = NULL;
    }

    m_pOldBmp = NULL;

    m_szBmp.cx = m_szBmp.cy = 0;

	if (m_pRuler != NULL)
	{
		delete m_pRuler;
		m_pRuler = NULL;
	}

    if (m_pAxis != NULL)
    {
        delete m_pAxis;
        m_pAxis = NULL;
    }

    if (m_pGraphics != NULL)
    {
        delete m_pGraphics;
        m_pGraphics = NULL;
    }
}

void VirtualDC::DrawBackground()
{
    if (m_pGraphics != NULL)
    {
        SolidBrush solidBrush(m_clrBgd);
        m_pGraphics->FillRectangle(&solidBrush, 0, 0, m_szBmp.cx, m_szBmp.cy);

        m_pCurrentPen = m_pAxis;
        RealPoint xs(-m_szWin.cx/2, 0), 
                  xe(m_szWin.cx/2, 0),
                  ys(0, -m_szWin.cy/2),
                  ye(0, m_szWin.cy/2);
        DrawLine(xs, xe);
        DrawLine(ys, ye);
        DrawArrow(xe, 0, 20, PI/10);
        DrawArrow(ye, PI/2.0f, 20, PI/10);
        m_pCurrentPen = NULL;

		DrawRuler();
    }
}

COLORREF VirtualDC::GetBackgroundColor() const
{
    return m_clrBgd.ToCOLORREF();
}

bool VirtualDC::SetMapParam(RealSize szWin, RealSize szView, RealPoint ptWinOrg, RealPoint ptViewOrg)
{
    if (0 == szWin.cx || 0 == szWin.cy)
    {
        return false;
    }

    m_szWin = szWin;
    m_szView = szView;

    UpdateRatio();

    m_ptWinOrg = ptWinOrg;
    m_ptViewOrg = ptViewOrg;

    return true;
}

RealPoint VirtualDC::GetViewOrg() const
{
    return m_ptViewOrg;
}

void VirtualDC::SetViewOrg(RealPoint pt)
{
    m_ptViewOrg = pt;
}

RealPoint VirtualDC::GetWindowOrg() const
{
    return m_ptWinOrg;
}

void VirtualDC::SetWindowOrg(RealPoint pt)
{
    m_ptWinOrg = pt;
}

void VirtualDC::SetWindowExtent(RealSize szWin)
{
    m_szWin = szWin;
    UpdateRatio();
}

RealSize VirtualDC::GetWindowExtent() const
{
    return m_szWin;
}

void VirtualDC::SetViewExtent(RealSize szView)
{
    m_szView = szView;
    UpdateRatio();
}

RealSize VirtualDC::GetViewExtent() const
{
    return m_szView;
}

void VirtualDC::SetDeviceExtent(RealSize szDev)
{
    RealPoint ptOrg(m_szDev.cx/2, m_szDev.cy/2);
    if (m_szDev.cx > EPS && m_szDev.cy > EPS)
    {
        DPtoLP(&ptOrg);
    }

    m_szDev = szDev;

    m_ptViewOrg.x = m_szDev.cx/2. - m_fXViewPerWin * (ptOrg.x - m_ptWinOrg.x + m_ptGraph.x);
    m_ptViewOrg.y = m_szDev.cy/2. + m_fYViewPerWin * (ptOrg.y - m_ptWinOrg.y + m_ptGraph.y);
}

RealSize VirtualDC::GetDeviceExtent() const
{
	return m_szDev;
}

void VirtualDC::SetGraphOffset(int x, int y)
{
	m_ptGraph.x = x;
	m_ptGraph.y = y;
}

void VirtualDC::SetGraphSize(int cx, int cy)
{
	m_szGraph.cx = cx;
	m_szGraph.cy = cy;
}

RealRect VirtualDC::GetLogicalVisibleRect() const
{
	RealRect rc(0, 0, m_szDev.cx, m_szDev.cy);
	DPtoLP(rc);
	return rc;
}

void VirtualDC::DrawText(LPCTSTR lpStr, RealPoint p)
{
	LPtoDP(&p);

	PointF tp;
	tp.X = p.x;
	tp.Y = p.y;

	StringFormat f;
	f.SetAlignment(StringAlignmentCenter);

	int len = _tcslen(lpStr);
	m_pGraphics->DrawString(
		lpStr, 
		len,
		m_pCurrentFont, 
		tp, 
		&f,
		m_pCurrentBrush);
}

void VirtualDC::DrawLine(RealPoint s, RealPoint e)
{
    DrawLine(s.x, s.y, e.x, e.y);
}

void VirtualDC::DrawLine(FLOAT_T sx, FLOAT_T sy, FLOAT_T ex, FLOAT_T ey)
{
    FLOAT_T dsx, dsy,
        dex, dey;

    LPtoDP(sx, sy, dsx, dsy);
    LPtoDP(ex, ey, dex, dey);

    if (m_pGraphics != NULL && m_pCurrentPen != NULL)
    {
        m_pGraphics->DrawLine(m_pCurrentPen, int(dsx), int(dsy), int(dex), int(dey));
// 		TRACE(_T("DrawLine:%.4ff ,%.4ff, %.4ff, %.4ff\n"), dsx, dsy, dex, dey);
    } 

//     m_mdc->MoveTo(dsx, dsy);
//     m_mdc->LineTo(dex, dey);
}

void VirtualDC::DrawArc(FLOAT_T cx, FLOAT_T cy, FLOAT_T radius, FLOAT_T startAngle, FLOAT_T endAngle)
{

}

void VirtualDC::DrawPolygon(RealPoint *lpPoints, int count)
{
    if (count < 3)
    {
        return;
    }

    PointF *pts = new PointF[count];
    for (int i = 0; i < count; i++)
    {
		FLOAT_T x = 0., y = 0.;
		LPtoDP(lpPoints[i].x, lpPoints[i].y, x, y);
        pts[i].X = REAL(x);
        pts[i].Y = REAL(y);
    }
	m_pGraphics->DrawPolygon(m_pCurrentPen, pts, count);
    delete []pts;
}

void VirtualDC::DrawPolygon(vector<RealPoint> &pts)
{
	int count = pts.size();
	if (count >= 3)
	{
		PointF *p = new PointF[count];
		for (int i = 0; i < count; i++)
		{
			FLOAT_T x = 0., y = 0.;
			LPtoDP(pts[i].x, pts[i].y, x, y);
			p[i].X = REAL(x);
			p[i].Y = REAL(y);
		}
		m_pGraphics->DrawPolygon(m_pCurrentPen, p, count);
		delete []p;
	}
}

void VirtualDC::FillEllipse(RealRect rc)
{
	LPtoDP(rc);
	if (m_pGraphics != NULL && m_pCurrentBrush != NULL)
	{
		m_pGraphics->FillEllipse(m_pCurrentBrush, int(rc.left), int(rc.top),
			 int(rc.right-rc.left), int(rc.bottom-rc.top));
	}
}

void VirtualDC::DrawEllipse(RealRect rc)
{
	LPtoDP(rc);
	if (m_pGraphics != NULL && m_pCurrentPen != NULL)
	{
		m_pGraphics->DrawEllipse(m_pCurrentPen, int(rc.left), int(rc.top),
			int(rc.right-rc.left), int(rc.bottom-rc.top));
	}
}

void VirtualDC::DrawArrow(RealPoint pos, FLOAT_T dir, FLOAT_T len, FLOAT_T angle)
{
    FLOAT_T cx = pos.x - len * cos(dir);
    FLOAT_T cy = pos.y - len * sin(dir);

    FLOAT_T fDis = len * tan(angle);
    FLOAT_T fTransX = fDis * cos(PI/2.0f - dir);
    FLOAT_T fTransY = fDis * sin(PI/2.0f - dir);

    RealPoint pl(cx - fTransX, cy + fTransY);
    RealPoint pr(cx + fTransX, cy - fTransY);
    DrawLine(pos, pl);
    DrawLine(pos, pr);
}

void VirtualDC::LPtoDP(FLOAT_T in, FLOAT_T &out, bool bHor)
{
	if (bHor)
	{
		out = (m_fXViewPerWin * (in - m_ptWinOrg.x + m_ptGraph.x)) + m_ptViewOrg.x;
	}
	else
	{
		out = (0 - m_fYViewPerWin * (in - m_ptWinOrg.y + m_ptGraph.y)) + m_ptViewOrg.y;
	}
}

void VirtualDC::LPtoDP(FLOAT_T ix, FLOAT_T iy, FLOAT_T &ox, FLOAT_T &oy)
{
    ox = (m_fXViewPerWin * (ix - m_ptWinOrg.x + m_ptGraph.x)) + m_ptViewOrg.x;
    oy = (0 - m_fYViewPerWin * (iy - m_ptWinOrg.y + m_ptGraph.y)) + m_ptViewOrg.y;
}

void VirtualDC::LPtoDP(RealPoint *lpPoints, int nCount /*= 1*/) const
{
    for (int i = 0; i < nCount; i++)
    {
        lpPoints[i].x = m_fXViewPerWin * (lpPoints[i].x - m_ptWinOrg.x + m_ptGraph.x) + m_ptViewOrg.x;
        lpPoints[i].y = 0 - m_fYViewPerWin * (lpPoints[i].y - m_ptWinOrg.y + m_ptGraph.y) + m_ptViewOrg.y;
    }
}

void VirtualDC::LPtoDP(vector<RealPoint> &pts)
{
	for (vector<RealPoint>::iterator iter = pts.begin();
		iter != pts.end(); iter++)
	{
		iter->x = m_fXViewPerWin * (iter->x - m_ptWinOrg.x + m_ptGraph.x) + m_ptViewOrg.x;
		iter->y = 0 - m_fYViewPerWin * (iter->y - m_ptWinOrg.y + m_ptGraph.y) + m_ptViewOrg.y;
	}
}

void VirtualDC::LPtoDP(RealRect &rc) const
{
    rc.left = (m_fXViewPerWin * (rc.left - m_ptWinOrg.x + m_ptGraph.x)) + m_ptViewOrg.x;
    rc.right= (m_fXViewPerWin * (rc.right - m_ptWinOrg.x + m_ptGraph.x)) + m_ptViewOrg.x;
    rc.top  = (0 - m_fYViewPerWin * (rc.top - m_ptWinOrg.y + m_ptGraph.y)) + m_ptViewOrg.y;
    rc.bottom = (0 - m_fYViewPerWin * (rc.bottom - m_ptWinOrg.y + m_ptGraph.y)) + m_ptViewOrg.y;
}

void VirtualDC::LPtoDP(LPSIZE lpSize) const
{
}

void VirtualDC::DPtoLP(RealPoint *lpPoints, int nCount /*= 1*/) const
{
    for (int i = 0; i < nCount; i++)
    {
        lpPoints[i].x = (lpPoints[i].x - m_ptViewOrg.x) / m_fXViewPerWin + m_ptWinOrg.x - m_ptGraph.x;
        lpPoints[i].y = 0 - (lpPoints[i].y - m_ptViewOrg.y) / m_fYViewPerWin + m_ptWinOrg.y - m_ptGraph.y;
    }
}

void VirtualDC::DPtoLP(RealRect &rc) const
{
	rc.left = (rc.left - m_ptViewOrg.x) / m_fXViewPerWin + m_ptWinOrg.x - m_ptGraph.x;
	rc.right= (rc.right - m_ptViewOrg.x) / m_fXViewPerWin + m_ptWinOrg.x - m_ptGraph.x;
	rc.top = 0 - (rc.top - m_ptViewOrg.y) / m_fYViewPerWin + m_ptWinOrg.y - m_ptGraph.y;
	rc.bottom = 0 - (rc.bottom - m_ptViewOrg.y) / m_fYViewPerWin + m_ptWinOrg.y - m_ptGraph.y;
}

void VirtualDC::DPtoLP(LPSIZE lpSize) const
{

}

Pen* VirtualDC::SetCurrentPen(Pen *p)
{
	Pen *t = m_pCurrentPen;
    m_pCurrentPen = p;
	return t;
}

Brush* VirtualDC::SetCurrentBrush(Brush *p)
{
	Brush *t = m_pCurrentBrush;
	m_pCurrentBrush = p;
	return t;
}

Font* VirtualDC::SetCurrentFont(Font *p)
{
	Font *t = m_pCurrentFont;
	m_pCurrentFont = p;
	return t;
}

bool VirtualDC::ZoomIn(RealPoint zp)
{
    if (m_fZoomScale > ZOOM_MAX)
    {
        return false;
    }
    DoZoom(ZOOM_STEP, zp);
    return true;
}

bool VirtualDC::ZoomOut(RealPoint zp)
{
    if (m_fZoomScale < ZOOM_MIN)
    {
        return false;
    }
    DoZoom(-ZOOM_STEP, zp);
    return true;
}

void VirtualDC::Clear()
{
    if (m_mdc != NULL)
    {
        m_mdc->FillSolidRect(0, 0, m_szBmp.cx, m_szBmp.cy, m_clrBgd.ToCOLORREF());
    }
}

void VirtualDC::Reset()
{
    m_ptGraph.x = m_ptGraph.y = 0;
    m_szGraph.cx = m_szGraph.cy = 0;

    m_szWin.cx = m_szWin.cy = 0;
    m_szView.cx = m_szView.cy = 0;

    m_fXViewPerWin = 1.0;
    m_fYViewPerWin = 1.0;

    m_ptWinOrg.x = m_ptWinOrg.y = 0;
    m_ptViewOrg.x = m_ptViewOrg.y = 0;
}

void VirtualDC::UpdateRatio()
{
    FLOAT_T fScale = pow(m_fZoomScale, 3);
    m_fXViewPerWin = fScale * m_szView.cx / (1.0f * m_szWin.cx);
    m_fYViewPerWin = fScale * m_szView.cy / (1.0f * m_szWin.cy);
}

void VirtualDC::DoZoom(FLOAT_T offset, RealPoint zp)
{
    RealPoint lzp(zp);
    DPtoLP(&lzp);

    m_fZoomScale += offset;
    UpdateRatio();

    m_ptViewOrg.x = zp.x - m_fXViewPerWin * (lzp.x - m_ptWinOrg.x + m_ptGraph.x);
    m_ptViewOrg.y = zp.y + m_fYViewPerWin * (lzp.y - m_ptWinOrg.y + m_ptGraph.y);
}

FLOAT_T VirtualDC::GetRulerStep()
{
	FLOAT_T x0 = 0;
	LPtoDP(0, x0, true);

	FLOAT_T arr[] = {
		0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0,
		2.0, 5.0, 10., 20., 50., 100., 200., 500.,
	};

	for (size_t n = 0; n < sizeof(arr)/sizeof(arr[0]); n++)
	{		
		FLOAT_T x = 0;
		LPtoDP(arr[n], x, true);
		FLOAT_T diff = x - x0;
		TRACE(_T("%8.4f, %8.4f\n"), arr[n], diff);
		if (diff > 4 && diff < 10)
		{
			return arr[n];
		}
	}
	return 1.;
}

void VirtualDC::DrawRuler()
{
	FLOAT_T step = GetRulerStep();

	m_pCurrentPen = m_pRuler;

	const int RULER_SHORT = 8;
	const int RULER_LONG = 16;

	RealRect rcVisible = GetLogicalVisibleRect();
	int count = 0;
	while (true)
	{
		FLOAT_T x = 0.;
		LPtoDP(count * step, x, true);
		if (x >= m_szDev.cx)
		{
			break;
		}
		m_pGraphics->DrawLine(m_pCurrentPen, x, 0,
			x, count % 10 == 0 ? RULER_LONG : RULER_SHORT);
		count++;
	}

	count = 0;
	while (true)
	{
		FLOAT_T y = 0.;
		LPtoDP(count * step, y, false);
		if (y < 16)
		{
			break;
		}
		m_pGraphics->DrawLine(m_pCurrentPen, 0, y,
			count % 10 == 0 ? RULER_LONG : RULER_SHORT, y);
		count++;
	}

	count = -1;
	while (true)
	{
		FLOAT_T x = 0.;
		LPtoDP(count * step, x, true);
		if (x < 0)
		{
			break;
		}
		m_pGraphics->DrawLine(m_pCurrentPen, x, 0,
			x, count % 10 == 0 ? RULER_LONG : RULER_SHORT);
		count--;
	}

	count = -1;
	while (true)
	{
		FLOAT_T y = 0.;
		LPtoDP(count * step, y, false);
		if (y >= m_szDev.cy)
		{
			break;
		}
		m_pGraphics->DrawLine(m_pCurrentPen, 0, y,
			count % 10 == 0 ? RULER_LONG : RULER_SHORT, y);
		count--;
	}

	m_pCurrentPen = NULL;
}

Color VirtualDC::COLORREF2Color(COLORREF clr)
{
    Color t;
    t.SetFromCOLORREF(clr);
    return t;
}

COLORREF VirtualDC::Color2COLORREF(Color clr)
{
    return clr.ToCOLORREF();
}