#pragma once

class RealPoint;

class VirtualDC
{
public:
	VirtualDC(void);
	~VirtualDC(void);

public:
	void Begin(CDC *dc, int cx, int cy);
	void End();
    void Release();

    void DrawBackground();
    COLORREF GetBackgroundColor() const;

    bool SetMapParam(RealSize szWin, RealSize szView, RealPoint ptWinOrg, RealPoint ptViewOrg);
    RealPoint GetViewOrg() const;
    void SetViewOrg(RealPoint pt);
    RealPoint GetWindowOrg() const;
    void SetWindowOrg(RealPoint pt);

    void SetWindowExtent(RealSize szWin);
    RealSize GetWindowExtent() const;

    void SetViewExtent(RealSize szView);
    RealSize GetViewExtent() const;

    void SetDeviceExtent(RealSize szDev);
	RealSize GetDeviceExtent() const;

	void SetGraphOffset(int x, int y);
	void SetGraphSize(int cx, int cy);

	RealRect GetLogicalVisibleRect() const;

	void DrawText(LPCTSTR lpStr, RealPoint p);

    void DrawLine(RealPoint s, RealPoint e);
    void DrawLine(FLOAT_T sx, FLOAT_T sy, FLOAT_T ex, FLOAT_T ey);
    void DrawArc(FLOAT_T cx, FLOAT_T cy, FLOAT_T radius, FLOAT_T startAngle, FLOAT_T endAngle);
    void DrawPolygon(RealPoint *lpPoints, int count);
	void DrawPolygon(vector<RealPoint> &pts);
	void FillEllipse(RealRect rc);
	void DrawEllipse(RealRect rc);

    // 画箭头
    // pos -- 终点位置
    // dir -- 所指方向，单位：弧度
    // len -- 长度
    // angle -- 箭头偏转角度，单位：弧度
    void DrawArrow(RealPoint pos, FLOAT_T dir, FLOAT_T len, FLOAT_T angle);

	void LPtoDP(FLOAT_T in, FLOAT_T &out, bool bHor);
    void LPtoDP(FLOAT_T ix, FLOAT_T iy, FLOAT_T &ox, FLOAT_T &oy);
    void LPtoDP(RealPoint *lpPoints, int nCount = 1) const;
	void LPtoDP(vector<RealPoint> &pts);
    void LPtoDP(RealRect &rc) const;
    void LPtoDP(LPSIZE lpSize) const;

    void DPtoLP(RealPoint *lpPoints, int nCount = 1) const;
    void DPtoLP(RealRect &rc) const;
    void DPtoLP(LPSIZE lpSize) const;

    Pen* SetCurrentPen(Pen *p);
	Brush* SetCurrentBrush(Brush *p);
	Font* SetCurrentFont(Font *p);

    bool ZoomIn(RealPoint zp);
    bool ZoomOut(RealPoint zp);

protected:
    // clear the information about the device context
    void Clear();
    // reset all map parameters
	void Reset();
    void UpdateRatio();
    void DoZoom(FLOAT_T offset, RealPoint zp);
	FLOAT_T GetRulerStep();
	void DrawRuler();

public:
    static Color COLORREF2Color(COLORREF clr);
    static COLORREF Color2COLORREF(Color clr);

protected:
	CDC    *m_dc;
	CDC    *m_mdc;
	CBitmap*m_bmp;
	CBitmap*m_pOldBmp;
	CSize   m_szBmp;

    Graphics *m_pGraphics;
    Pen      *m_pAxis; 
	Pen      *m_pRuler;

    Pen      *m_pCurrentPen;
	Brush    *m_pCurrentBrush;
	Font     *m_pCurrentFont;

    Color m_clrBgd, 
          m_clrAxis,
          m_clrScale,
		  m_clrRuler;

	RealPoint  m_ptGraph; // Graph offset
	RealSize   m_szGraph; // Graph size

    RealSize   m_szWin,   // Window extent
               m_szView,  // View extent
               m_szDev;   // Device extent
    FLOAT_T      m_fXViewPerWin, // unit in view per unit in window
               m_fYViewPerWin;
	RealPoint  m_ptWinOrg,  // window origin
               m_ptViewOrg; // view origin

    FLOAT_T      m_fZoomScale;

#ifdef _DEBUG
    DWORD   m_dwTick;
#endif
};