
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMPCGOODS_H__93A5C16D_1A12_482A_8C65_E2BB4658DAEE__INCLUDED_)
#define AFX_DLGMPCGOODS_H__93A5C16D_1A12_482A_8C65_E2BB4658DAEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMpcGoods.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMpcGoods dialog
#include "MyGrid.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyImage.h"
#include "MyCheck.h"

class CDlgMpcGoods : public CDialog
{
    // Construction
public:
    void Show();
    CDlgMpcGoods(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bShow ;
private:
    CPoint		m_Pnt ;
    int 		m_nPntX ;
    BYTE		m_btCurPage ;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;

    // for pickup item
    CPoint				m_PickUpMousePnt ;
    int					m_nPickUpItemPosition ;
    int					m_nPickUpItemID ;

    // for mouse move item
    int					m_nMouseMovePosition ;
public:
    void SetStoneGrid();
    void SetGridContain ();
    void HeroEquipItem( int nPos );
    int GetMouseMovePosition();
    int GetPickUpPosition();
    CPoint GetPickUpMousePnt();
    void SetPickUpItemID( int nID );
    // Dialog Data
    //{{AFX_DATA(CDlgMpcGoods)
    enum { IDD = IDD_DIALOG_MPCGOODS };
    CMyGrid	m_MpcStonesGri;
    CMyCheck	m_ChkStones;
    CMyCheck	m_ChkGoods;
    CMyGrid		m_MpcGoodsGri;
    CMyStatic	m_MpcGoodsMoneySta ;
    CMyButton	m_MpcGoodsCloseBtn ;
    CMyButton	m_MpcGoodsMoneyBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMpcGoods)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgMpcGoods)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMpcgoodsBtnClose();
    afx_msg void OnMpcgoodsBtnMoney();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnChkGoods();
    afx_msg void OnChkStones();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMPCGOODS_H__93A5C16D_1A12_482A_8C65_E2BB4658DAEE__INCLUDED_)
