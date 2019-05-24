
// ChildView.h : CChildView ��Ľӿ�
//

#pragma once

class VirtualDC;
class GraphSet;

// CChildView ����
class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	bool GetCurrentCoordinate(FLOAT_T &x, FLOAT_T &y);

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();

	afx_msg void OnUpdateDrawDot(CCmdUI *pCmdUI);
    afx_msg void OnDrawDot();
    afx_msg void OnUpdateDrawLine(CCmdUI *pCmdUI);
    afx_msg void OnDrawLine();
    afx_msg void OnUpdateDrawRect(CCmdUI *pCmdUI);
    afx_msg void OnDrawRect();
    afx_msg void OnUpdateDrawCircle(CCmdUI *pCmdUI);
    afx_msg void OnDrawCircle();
    afx_msg void OnUpdateDrawPolygon(CCmdUI *pCmdUI);
    afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateTestSegIntersect(CCmdUI *pCmdUI);
	afx_msg void OnTestSegIntersect();
	DECLARE_MESSAGE_MAP()

protected:
    VirtualDC   *m_pDC;
    RealPoint    m_ptMButtonDown;
    HCURSOR      m_hDrag, m_hCross, m_hCurrent;

    enum DrawMode
    {
        DM_NONE = 0,
		DM_DOT  = 1,
        DM_LINE = 2,
        DM_RECT = 3,
        DM_CIRCLE = 4
    };
    DrawMode     m_dm;
    RealPoint    m_ptLButtonDown;
    RealSegment  m_sg;
    bool         m_bStartLine;

	GraphSet    *m_graph;
	vector<RealPoint> m_hull;

public:
	afx_msg void OnUpdateTestSlowConvexHull(CCmdUI *pCmdUI);
	afx_msg void OnTestSlowConvexHull();
	afx_msg void OnUpdateRandomLine(CCmdUI *pCmdUI);
	afx_msg void OnRandomLine();
	afx_msg void OnUpdateTestMultiSeg(CCmdUI *pCmdUI);
	afx_msg void OnTestMultiSeg();
};
