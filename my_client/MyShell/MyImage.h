
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYIMAGE_H__A0276D1F_52DC_4D7C_83DB_FB3D265B03F9__INCLUDED_)
#define AFX_MYIMAGE_H__A0276D1F_52DC_4D7C_83DB_FB3D265B03F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyImage.h : header file
//
#include "Game3DEffectEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMyImage window
const int NORMAL_IMAGE  = 0 ;
const int ITEM_IMAGE	= 1 ;
const int BG_IMAGE		= 2 ;
const int FACE_IMAGE	= 3 ;
const int MEDAL_IMAGE	= 4 ;
const int SKILL_IMAGE	= 5 ;
const int NPCITEM_IMAGE = 6 ;
const int EMBLEM_IMAGE	= 7 ;
const int BOOTHITEM_IMAGE = 8 ;
const int PETFACE_IMAGE = 9;
const int SMALLPETFACE_IMAGE = 10;

class CMyImage : public CButton
{
    // Construction
public:
    CMyImage();
    BOOL		m_bMouseMove ;			// the mouse move
    // Attributes
private:
    CPoint		m_IntPoint ;			// the image comparative to the parent dialog point
    CPoint		m_Point ;				// the show point
    BOOL		m_bImageLoad ;			// the image box's image is load
    UINT		m_uFrame ;				// the show frame
    char		m_strImageID[64] ;		// the image id
    DWORD		m_dwImageID ;			// the dword image id
    int			m_nWidth ;				// the image width
    int			m_nHeight ;				// the image height
    CRect		m_ShowRect ;			// the show rect
    BOOL		m_btItemType ;			// the type
    char		m_strAni[63] ;			// the ani name
    int			m_nItemPos ;			// the itempos
    int			m_nImageTypeID ;		// the image type id
    BOOL		m_bImgZoom ;			// the image is disable
    int			m_nCurMoney ;			// the current money
    BYTE		m_btMilNum ;			// the million number
    BYTE		m_btThoNum ;			// the thousand number
    BYTE		m_btHunNum ;			// the hundred number
    BYTE		m_btGoldStar ;			// the gold star number
    BYTE		m_btSillerStar ;		// the siller star number
    BYTE		m_btCopperStar ;		// the copper star number
    // for equip item
    int			m_nPosition ;			// the item's position
    CGame3DEffectEx	m_objEffect;		// the item's effect
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyImage)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void ShowItemEffect();
    void ChangeImageType(int nType);
    void SetItemPos( int nPos );
    void ShowHp( int x, int y, int nCurHp, int nMaxHp );
    void ShowMoney ( int x, int y );
    void SetMoney ( int nMoney );
    void SetCurFrame(UINT uFrame);
    void ShowBk( const char* strBkID );
    void ShowFocus();
    BOOL Init ( int xPos, int yPos, char* ImageID = NULL, BYTE btItemType = 0, int nItemPos = -1 );
    void MouseMoveText();
    char* GetImageID ();
    OBJID GetImageRealID() {return m_dwImageID;}
    BOOL GetImageLoadState ();
    void Show ( int x, int y );
    void RemoveImage ();
    BOOL InsertImage ( char* ImageID, DWORD dwImageID = 0, int nImageTypeID = 0, BOOL bImgZoom = false );
    virtual ~CMyImage();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyImage)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYIMAGE_H__A0276D1F_52DC_4D7C_83DB_FB3D265B03F9__INCLUDED_)
