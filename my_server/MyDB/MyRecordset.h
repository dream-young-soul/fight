
//**********************************************************
// 代码编辑器
//**********************************************************

// myrecordset.h

#ifndef	MYRECORDSET_H
#define	MYRECORDSET_H

class CCriticalSection;
class CMyDatabase;
class IRecordset;
class IRecord;

class CMyRecordset : public IRecordset
{
    friend class CMyRecord;
    friend class CMyField;
// Constructor
public:
    CMyRecordset(CMyDatabase*   pMyDatabase);
    virtual ~CMyRecordset();
//	IRecordset*		GetInterface() { return (IRecordset*)this; }

public: // recordset
//	virtual IRecord*	GetRecordInterface() { return (IRecord*)this; }		// 取IRecord
    virtual UINT		RecordCount();								// return -1: error
    virtual bool		MoveNext();

protected: // record
    virtual IRecord*	CreateNewRecord(DWORD idNewKey = 0);
    virtual	void	Release() { delete this; }

    virtual bool	InsertRecord();
    virtual bool	UpdateRecord();
    virtual bool	DeleteRecord();

    virtual int		LoadInt		(LPCTSTR szField)					{ return (int)Fields((char*)szField); }
    virtual void	SetInt		(LPCTSTR szField, int nData)		{ m_bEdit = true; Fields((char*)szField) = nData; }
    virtual UINT	LoadUInt	(LPCTSTR szField)					{ return (UINT)Fields((char*)szField); }
    virtual void	SetUInt		(LPCTSTR szField, UINT nData)		{ m_bEdit = true; Fields((char*)szField) = (DWORD)nData; }
    virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		{ Fields((char*)szField).CopyTo(szData, nSize); }
    virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	{ m_bEdit = true; Fields((char*)szField).CopyFrom(szData, nSize); }
    // 注：暂时还需要遵守“先取后写”的规则，待具体实现改变后可不按此规则。

    // 立即写库。如无共享冲突，可不提供该接口
    virtual bool	AddInt		(LPCTSTR szField, int nData)		{ return AddField(szField, nData); }

public:
#ifdef	MULTITHREAD_SAFE
    static CCriticalSection	m_xCtrl;
#endif
    static HANDLE	s_hHeap;
    static UINT		s_uNumAllocsInHeap;

    static BOOL		IsValidPt(void* p);
    static CMyRecordset* CreateNew(CMyDatabase*  pMyDatabase);

public:
    void*	operator new		(size_t size);
    void	operator delete		(void* p);

public:
    enum { modeEdit, modeAddNew, modeDelete };


    virtual bool Open( const char* lpszSQL);
    virtual void Close();

public: // no used
    int FieldsCount();

    void AddNew();
    void Edit();
    bool Update();
    void Delete();

    void MoveFirst();
    void MoveLast();
    void MovePrev();
//	void MoveNext();
    void Move(ULONG ulIndex);

    bool IsBOF();
    bool IsEOF();
    bool IsOpen();

    bool CanUpdate() const;     // Can Edit/AddNew/Delete be called?
    void ClsEditFlag();

    bool AddField(LPCTSTR szField, int nData);

//	_int64 RecordCount() const;

    CMyField		Fields(char* szField);
    CMyField 		Fields(UINT	uiIndex );

protected:
    int				UpdateFieldsCount();

    void			BuildUpdateSQL();
    void			BuildUpdateCondition();
    void			BuildUpdateOpration();

    CMyField* 		GetFieldRef(UINT uiIndex);

    bool			Query(char* szSQL, MYSQL_RES * * res);

    void			GetFieldValue(UINT uiIndex);

protected:
    CMyDatabase*	m_pMyDatabase;

    MYSQL_RES*		m_Recordset;
    MYSQL_FIELD* 	m_Fields;
    MYSQL_ROW		m_Row;

    _int64			m_ulRowsCount;
    _int64			m_ulCursor;			//★ 当INSERT和DELETE后, 要及时更新
    unsigned int 	m_uiFieldsCount;

    char	  		m_szKey[16];
    unsigned int 	m_uiKeyIndex;

    bool*			m_pbDirty;

    bool 			m_bIsOpen;
    bool 			m_bBOF;			//★ 当INSERT和DELETE后, 要及时更新
    bool 			m_bEOF;			//★ 当INSERT和DELETE后, 要及时更新
    bool 			m_bEdit;

    int				m_nEditMode;

    CMyField*		m_objFields;

    char  			m_szSQL[1024];
    char 			m_szTableName[32];
    char 			m_szUpdateSQL[1024];
    char			m_szUpdateOpration[1024];
    char			m_szUpdateCondition[128];

    bool			m_bCanUpdate;
};

#endif // MYRECORDSET_H
