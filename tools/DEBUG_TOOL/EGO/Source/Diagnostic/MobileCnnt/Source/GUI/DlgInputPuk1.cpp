// DlgInputPuk1.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgInputPuk1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputPuk1 dialog


CDlgInputPuk1::CDlgInputPuk1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputPuk1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputPuk1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_uRemainNum = 10;
	m_strPin = _T("");
	m_strPuk = _T("");
}


void CDlgInputPuk1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputPuk1)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDT_PUK_PIN1, m_strPin);
	DDV_MaxChars(pDX, m_strPin, 8);
	DDX_Text(pDX, IDC_EDT_PUK1, m_strPuk);
	DDV_MaxChars(pDX, m_strPuk, 8);
	DDX_Text(pDX, IDC_EDT_PUK_PIN1_CONFIRM, m_strPinCfm);
	DDV_MaxChars(pDX, m_strPinCfm, 8);
	DDX_Text(pDX, IDC_STC_REMAIN_PUK1, m_uRemainNum);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CDlgInputPuk1, CDialog)
BEGIN_MESSAGE_MAP(CDlgInputPuk1, CDialog)
	//{{AFX_MSG_MAP(CDlgInputPuk1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputPuk1 message handlers

BOOL CDlgInputPuk1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PUK_PIN1);
	pEdit->SetFocus();

	SendCheckRequest();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputPuk1::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(PUK1_REMAIN_NUM, 0, NULL, 0);
	}
}

BOOL CDlgInputPuk1::SendSetRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		TCHAR szPara[MAX_PATH] = {0};

#ifdef _UNICODE
		swprintf(szPara, _T("%s,%s"), m_strPuk, m_strPin);
#else
		sprintf(szPara, _T("%s,%s"), m_strPuk, m_strPin);
#endif
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(PUK_INPUT, 0, (LPVOID)szPara, _tcslen(szPara));
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		CString strtemp = szRet;
		
		if(strstr(szRet, "OK") != NULL)
		{
			AfxMessageBox(IDS_SET_SUCCESS, MB_OK);
			return TRUE;
		}
		if(strstr(szRet, "ERROR") != NULL)
		{
			if(strstr(szRet, "SIM PIN required") != NULL || strstr(szRet, "16") != NULL 
				|| strstr(szRet, "SIM PUK required") != NULL || strstr(szRet, "16") != NULL
				|| strstr(szRet, "SIM PIN2 required") != NULL || strstr(szRet, "16") != NULL
				|| strstr(szRet, "SIM PUK2 required") != NULL || strstr(szRet, "16") != NULL)
			{
				AfxMessageBox(IDS_PWD_ERROR, MB_ICONERROR);
				m_strPin.Empty();
				m_strPuk.Empty();
				m_strPinCfm.Empty();
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PUK_PIN1);
				pEdit->SetFocus();
				UpdateData(FALSE);
			}
			else
			{
				AfxMessageBox(IDS_SET_FAILED, MB_ICONERROR);
			}
			SendCheckRequest();
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CDlgInputPuk1::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("+XX: "));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex + 5);

#ifdef _UNICODE
		swscanf(strText, _T("%d"), &m_uRemainNum);
#else
		sscanf(strText, _T("%d"), &m_uRemainNum);
#endif
		if(m_uRemainNum < 1)
		{
			AfxMessageBox(IDS_PUK_INVALID, MB_ICONERROR);
		}
	}
	
	UpdateData(FALSE);
}

void CDlgInputPuk1::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData();
//	BOOL bPromtPin = FALSE;

	if(m_strPin.GetLength() < 4)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PUK_PIN1);
		pEdit->SetSel(0,-1);
		pEdit->SetFocus();
		AfxMessageBox(IDS_PIN_PROMT, MB_ICONERROR);
		return;
	}

	if(m_strPuk.GetLength() < 8)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PUK1);
		pEdit->SetSel(0,-1);
		pEdit->SetFocus();
		AfxMessageBox(IDS_PUK_PROMT, MB_ICONERROR);
		return;
	}

	if(m_strPinCfm != m_strPin)
	{
		AfxMessageBox(IDS_PIN_ERROR);
		m_strPin.Empty();
		m_strPinCfm.Empty();
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PUK_PIN1_CONFIRM);
		pEdit->SetFocus();
		UpdateData(FALSE);
		return;
	}

	if(SendSetRequest())
	{
		CDialog::OnOK();
	}
}
