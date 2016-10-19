
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
    friend class CMyConstRecord;
    friend class CMyField;
    // Constructor
public:
    CMyRecordset(CMyDatabase*   pMyDatabase);
    virtual ~CMyRecordset();

public: // recordset
    virtual UINT		RecordCount();								// return -1: error
    virtual bool		MoveNext();

protected: // record
    virtual IRecord*	CreateNewRecord(DWORD idNewKey = NEWKEY_NONE);
    virtual IRecord*	CreateNewConstRecord(DWORD idNewKey = NEWKEY_NONE);
    virtual	void	Release() { delete this; }

    virtual DWORD	InsertRecord();
    virtual bool	UpdateRecord();
    virtual bool	DeleteRecord();

    virtual int		LoadInt		(LPCTSTR szField);
    virtual void	SetInt		(LPCTSTR szField, int nData);
    virtual UINT	LoadUInt	(LPCTSTR szField)					{ return (UINT)LoadInt(szField); }
    virtual void	SetUInt		(LPCTSTR szField, UINT nData)		{ SetInt(szField, nData); }
    virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize);
    virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize);

    // 新模式的接口，用于代替应用程序的DATA层。
    virtual int		GetInt		(int idx);
    virtual LPCTSTR	GetStr		(int idx);
    virtual void	SetInt		(int idx, int nData);
    virtual void	SetStr		(int idx, LPCTSTR szData, int nSize);
    virtual void	ClearUpdateFlags	()							{ ClsEditFlag(); }

    // 立即写库。如无共享冲突，可不提供该接口
    virtual bool	AddInt		(LPCTSTR szField, int nData)		{ return AddField(szField, nData); }

public:
#ifdef	MULTITHREAD_SAFE
    static CCriticalSection	m_xCtrl;
#endif
#ifdef	USE_NEW
    static CMyHeap	s_heapString;
#endif
    static CMyRecordset* CreateNew(CMyDatabase*  pMyDatabase);

public:
    MYHEAP_DECLARATION(s_heap)

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
    int UpdateFieldsCount();

    void			BuildUpdateSQL();
    void			BuildUpdateCondition();
    void			BuildUpdateOpration();

    CMyField* 		GetFieldRef(UINT uiIndex);

    bool			Query(char* szSQL, MYSQL_RES * * res);

    void			GetFieldValue(UINT uiIndex);
    void			GetAllField();
public:
    bool			GetDefauldField();

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

    bool			m_bDefaultValue;				// paled add, have a default field value.
};

#endif // MYRECORDSET_H
