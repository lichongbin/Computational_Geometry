#pragma once


// GenerateSegment dialog

class GenerateSegment : public CDialog
{
	DECLARE_DYNAMIC(GenerateSegment)

public:
	GenerateSegment(CWnd* pParent = NULL);   // standard constructor
	virtual ~GenerateSegment();

// Dialog Data
	enum { IDD = IDD_RANDOM_LINE };

	float m_fLeft,
		m_fRight,
		m_fTop,
		m_fBottom;
	UINT m_nCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
