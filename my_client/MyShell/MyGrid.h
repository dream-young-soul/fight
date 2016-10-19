
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYGRID_H__891D9907_6DA2_4F0D_A12A_4C9620126950__INCLUDED_)
#define AFX_MYGRID_H__891D9907_6DA2_4F0D_A12A_4C9620126950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGrid.h : header file
//
#include "Item.h"
/////////////////////////////////////////////////////////////////////////////
// CMyGrid window

enum { ICON_TYPE_ITEM = 0,
       ICON_TYPE_ACTION,
       ICON_TYPE_SKILL,
       ICON_TYPE_XP,
       ICON_TYPE_SHOP,
       ICON_TYPE_DEPOT,
       ICON_TYPE_EQUIP,
       ICON_TYPE_QUSE,
       ICON_TYPE_TRADE,
       ICON_TYPE_TASKITEM
     };

class CMyGrid : public CButton
{
    // Construction
public:
    CMyGrid();

    // Attributes
private:
    struct MyGridParameter
    {
        CPoint		m_IntPoint ;			// the grid comparative to the parent dialog point
        CPoint		m_Point ;				// the grid show point
        BYTE		m_btGridLine ;			// the grid line
        BYTE		m_btGridRow ;			// the gridRow
        int			m_iGridSpacex;			// the  line space between
        int			m_iGridSpacey;			// the row space between
        BYTE		m_btGridHeight ;		// the grid's height
        BYTE		m_btGridWidth ;			// the grid's width
        POINT		m_PickUpMousePoint ;	// the mouse point
        CPoint		m_PickUpIconLandR ;		// the grid pick up icon's former point
        BOOL		m_bMouseMove ;			// the mouse move
        CPoint		m_MouseMoveIconLandR ;  // the mouse move icon
        BYTE		m_btOwnerID ;			// the Grid's owner id
        CPoint		m_InsetLandR ;			// the insert Icon LandR
        OBJID		m_IconTypeID[256] ;		// the Icon's type id
        OBJID		m_IconRealID[256] ;		// the Icon's real id
        BOOL		m_bGridAble[256] ;		// the grid's able
        DWORD		m_IconIndex[256] ;		// the grid's index
        OBJID		m_PickUpIconTypeID ;	// the pickup icon type id
        BYTE		m_btPickUpIconType ;	// the pickup icon type
        DWORD		m_dwPickUpIconIndex ;	// the pickup icon's index
        OBJID		m_PickUpIconRealID ;	// the pickup icon's realindex
        BYTE		m_btPickUpIconActType ; // the pick up icon's act type
        DWORD		m_dwMouseMoveIconIndex ;// the mouse move's icon's index
        DWORD		m_dwMouseMoveIconTypeID;// the mouse move's icons typeid
        int			m_nGridAbleNum ;		// the grid's able num
        BOOL		m_bEnable ;				// the grid enable
        BOOL		m_bPickEnable ;			// the grid pick enable
        CRect		m_ClientRect ;			// the grid client rect
        int			m_IconNum[256] ;		// the grid icon num
        BYTE		m_btIconType[256] ;		// the grid icon type
        CItem		m_QUseItem[10] ;			// the quse item
        BOOL		m_bScale;
        CGame3DEffectEx	m_objEffect[256];
    } m_GriPar ;

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyGrid)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void ShowTypeItem(OBJID idType, int nX, int nY);
    BOOL GetGridAbleByIndex(int nIndex);
    void ReSetGridSpace(int x, int y);
    int GetPickUpRealID();
    int GetIconRealID ( int nIndex );
    void SetIconRealID( int nIndex, OBJID rID );
    BYTE GetPickUpActType( );
    BYTE GetIconType( int nIndex );
    BYTE GetPickUpType();
    void SetIconNum ( int nIndex, int nNum);
    DWORD GetMouseMoveTypeID();
    CPoint GetGridPnt(int Line, int Row);
    int GetIconTypeID ( int nIndex );
    DWORD GetMouseMoveIndex ();
    void SetPickEnable ( BOOL bEnable );
    void SetEnable ( BOOL bEnable );
    void ReSumePickUp ();
    int GetPickUpIndex ();
    void RemoveIcon ( int nLine, int nRow, BOOL bAll = false );
    int ChangeIcon ( CPoint IconLandR, OBJID ItemTypeID, OBJID ItemRealID, DWORD dwItemIndex, BYTE btItemType = ICON_TYPE_ITEM );
    CPoint GetInsertLandR();
    CPoint GetPickUpMousePnt ();
    void MouseMoveTest();
    int GetGridIndex ( CPoint lrPoint );
    BOOL LoadIcon ( CPoint lrPoint, OBJID ItemTypeID, DWORD dwItemIndex, BYTE btItemType );
    void SetGridAble ( CPoint lrPoint, BOOL bAble );
    int InsertIcon ( int nLine, int nRow, OBJID ItemTypeID, DWORD dwItemIndex, BYTE btItemType = ICON_TYPE_ITEM, BOOL bAuto = true);
    BOOL GetGridAble ( CPoint lrPoint );
    CPoint GetLineAndRow ( CPoint MousePoint );
    void Show ( int x, int y );
    BOOL Init ( int xPos, int yPos, BYTE btLine, BYTE btRow, BYTE btOwnerID );
    virtual ~CMyGrid();
    OBJID GetPickUpTypeID ( ) ;
    void  SetShowScale(BOOL bScale) {m_GriPar.m_bScale = bScale;}
    // Generated message map functions
protected:
    //{{AFX_MSG(CMyGrid)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGRID_H__891D9907_6DA2_4F0D_A12A_4C9620126950__INCLUDED_)
