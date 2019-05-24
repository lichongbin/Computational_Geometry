// GenerateSegment.cpp : implementation file
//

#include "stdafx.h"
#include "TestGeometry.h"
#include "GenerateSegment.h"
#include "Const.h"

// GenerateSegment dialog

IMPLEMENT_DYNAMIC(GenerateSegment, CDialog)

GenerateSegment::GenerateSegment(CWnd* pParent /*=NULL*/)
	: CDialog(GenerateSegment::IDD, pParent)
	, m_fLeft(-PLATFORM_SIZE)
	, m_fRight(PLATFORM_SIZE)
	, m_fTop(PLATFORM_SIZE)
	, m_fBottom(-PLATFORM_SIZE)
	, m_nCount(10)
{
}

GenerateSegment::~GenerateSegment()
{
}

void GenerateSegment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LEFT, m_fLeft);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_fRight);
	DDX_Text(pDX, IDC_EDIT_TOP, m_fTop);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_fBottom);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
}


BEGIN_MESSAGE_MAP(GenerateSegment, CDialog)
	ON_BN_CLICKED(IDOK, &GenerateSegment::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GenerateSegment::OnBnClickedCancel)
END_MESSAGE_MAP()


// GenerateSegment message handlers
void GenerateSegment::OnBnClickedOk()
{
	if (UpdateData(TRUE))
	{
		OnOK();
	}
}

void GenerateSegment::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL GenerateSegment::OnInitDialog()
{
	CDialog::OnInitDialog();



	return TRUE;
}
