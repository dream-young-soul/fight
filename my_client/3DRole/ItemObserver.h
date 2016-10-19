
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemObserver.h: interface for the ItemObserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMOBSERVER_H__4380DF0B_8835_434A_A3E9_B74C4F09CF2E__INCLUDED_)
#define AFX_ITEMOBSERVER_H__4380DF0B_8835_434A_A3E9_B74C4F09CF2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DSimpleObj.h"
#include "Item.h"

class CItemObserver
{
public:
    CItemObserver();
    virtual ~CItemObserver();
public:
    void Show();
    void ShowBg();
    void Switch() {m_bShowItem = !m_bShowItem;}
    void SwitchAction() {m_bShowAction = !m_bShowAction;}
    void ShowAction();
private:
    BOOL			m_bShowItem;
    BOOL			m_bShowAction;
    int				m_nIndex;
    C3DSimpleObj	m_obj;
};
extern CItemObserver g_objObserver;
#endif // !defined(AFX_ITEMOBSERVER_H__4380DF0B_8835_434A_A3E9_B74C4F09CF2E__INCLUDED_)
