#if !defined(AFX_GRAPHWND_H__6556B6B8_DE08_11D3_B4B4_00C04F89477F__INCLUDED_)
#define AFX_GRAPHWND_H__6556B6B8_DE08_11D3_B4B4_00C04F89477F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphWnd.h : header file
//

#include "GraphFrame.h"
#include "graph_props.h"
#include "graph_general.h"

enum OPT
{
    OPT_POINT=0,
    OPT_DRAG ,
    OPT_ZOOM ,
};

class CAxisPropertyPage;
class CGraphicsPropertyPage;
/////////////////////////////////////////////////////////////////////////////
// CGraphWnd window

class CGraphWnd : public CWnd, public virtual CGraphBaseClass
{
// Construction
public:
	CGraphWnd(long maximum_graphs = 32);

// Attributes
public:
    int m_opt; //Operation OPT_POINT, OPT_DRAG, OPT_ZOOM

protected:
    CGraphFrame* graph_frame;
    CGraphProps** m_graphs;
    long m_graphCount;
    long m_graph_maximum;
    //axis
    CAxisProps y_axis;
    CAxisProps x_axis;

    //property pages
    CAxisPropertyPage* axis_prop_page;
    CGraphicsPropertyPage* graph_prop_page;

    BOOL m_bAutoUpdateTimer;

    //current select graph 
    int  m_nGraphSelect;
    //show mode
    //BOOL m_nShowMode;

// Operations
public:

    BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID, 
	DWORD dwStyle = WS_CHILD | WS_VISIBLE, BOOL bAutoUpdateTimer = FALSE);

    //operations with graphs
    virtual int AddGraph(COLORREF newColor = 0, const TCHAR* title = _T(""), BOOL bRedraw = TRUE,
	                                    BOOL b_sort_x = FALSE, BOOL b_keep_same_x = TRUE);
    virtual int RemoveGraph(int index, BOOL bRedraw = TRUE);
   
    virtual CGraphProps* GetGraph(int index){ return m_graphs[index]; };
    
    virtual int GetGraphCount(){return m_graphCount;};

    //set current select graph
    virtual void SetGraphSelect(int graph_index, int item_select =0)
    {
        m_nGraphSelect = graph_index; 
        graph_frame->pview_bar.SetCurrentSelect(graph_index, item_select);
    };
    virtual int  GetGraphSelect(){ return m_nGraphSelect; };

    //enumeration functions
    virtual CGraphProps* GetFirstGraph(int* index);
    virtual CGraphProps* GetNextGraph(int* index);

    //points operations
    virtual int  AddPoint(int graphnum, double x, double y, int index);
    virtual int  InsertPoint(int graphnum, double x, double y, int insertPos = -1);
    virtual int  EditPoint(int graphnum, int index, double x, double y, BOOL bRedraw = TRUE);
    virtual int  RemovePoint(int graphnum, int index, BOOL bRedraw = TRUE);
    virtual void ClearGraph(int graphnum, BOOL bRedraw = TRUE);

    //graph properties
    virtual void  SetGraphFlags(DWORD new_flags, BOOL bRedraw = TRUE);
    virtual DWORD GetGraphFlags();
	    
    //coordinates functions
    virtual void SetGraphWorldCoords(double x1, double x2, double y1, double y2, BOOL bRedraw = TRUE);
    virtual void GetGraphWorldCoords(double* x1, double* x2, double* y1, double* y2);
    virtual BOOL GetBoundRect(double* minx, double* maxx, double* miny, double* maxy);

    virtual CAxisProps* GetAxisProps(unsigned long axis_enum);
    virtual void SetAxisProps(const TCHAR* _title, const TCHAR* _UOM, int _precision, BOOL bXAxis, BOOL bRedraw = TRUE);
    virtual void FormatAxisOutput(double value, BOOL bXAxis, int format_level, CString& res_str);

    virtual void AppendMenuItems(CMenu* menu);
    virtual void AppendPropertyPage(CPropertySheet* prop_sheet);
    virtual void ReleasePropertyPage(UINT dialog_status);

    virtual void UpdateWindows(unsigned long what_to_update);
    virtual void OperateWithPointView(unsigned long pview_operations);

    void DrawGraphToDC(CDC* dest_dc, CRect& rect_to_draw);
    
    //extend graph method
    //void SetShowMode(BOOL bTime);
    void GotoChartPoint(ULONG index);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphWnd)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHWND_H__6556B6B8_DE08_11D3_B4B4_00C04F89477F__INCLUDED_)
