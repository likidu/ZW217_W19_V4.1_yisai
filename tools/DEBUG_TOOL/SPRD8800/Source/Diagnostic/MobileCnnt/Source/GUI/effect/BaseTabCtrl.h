#if !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)
#define AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl window

enum
{
	BTC_NONE = 0,
	BTC_TABS = 1,
	BTC_ALL = 2,
};

class CBaseTabCtrl : public CTabCtrl
{
// Construction
public:
	CBaseTabCtrl(int nType = BTC_NONE);
	BOOL EnableDraw(int nType = BTC_ALL);
	void SetBkgndColor(COLORREF color);

protected:
	COLORREF m_crBack;
	int m_nDrawType;

	CBrush  m_brshDlgBckGrnd;
    static const COLORREF CLR_DLGBCKGRND;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseTabCtrl)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);
	virtual void PreSubclassWindow();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)
