
// TestGeometry.h : TestGeometry Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTestGeometryApp:
// �йش����ʵ�֣������ TestGeometry.cpp
//

class CTestGeometryApp : public CWinAppEx
{
public:
	CTestGeometryApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
    ULONG_PTR      m_gdiplusToken;
public:
    virtual int ExitInstance();
};

extern CTestGeometryApp theApp;
