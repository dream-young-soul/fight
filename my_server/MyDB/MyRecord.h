
//**********************************************************
// 代码编辑器
//**********************************************************

// myrecords.h 用于分解出myrecordset.h中的单个记录。
// 仙剑修，2002.9.12
//////////////////////////////////////////////////////////////////////

#ifndef	MYRECORD_H
#define	MYRECORD_H

class CCriticalSection;
class CMyDatabase;

class CMyRecord : public IRecord
{
protected: // interface
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

public: // Constructor
    CMyRecord(CMyDatabase*   pMyDatabase);
    virtual ~CMyRecord();
//	IRecord*	GetInterface() { return (IRecord*)this; }

public:
#ifdef	MULTITHREAD_SAFE
    static CCriticalSection	m_xCtrl;
#endif
    static HANDLE	s_hHeap;
    static UINT		s_uNumAllocsInHeap;

    static BOOL		IsValidPt(void* p);
    static CMyRecord* CreateNew(CMyDatabase*  pMyDatabase);

public:
    void*	operator new		(size_t size);
    void	operator delete		(void* p);

public:
    enum { modeEdit, modeAddNew, modeDelete };

    virtual bool Open( CMyRecordset* pRes, DWORD idNewKey = 0);
    virtual bool Spawn(CMyRecord*  pMyRecord, DWORD idNewKey = 0);
    virtual void Close();

    int FieldsCount();

    void AddNew();
    void Edit();
    bool Update();
    void Delete();

    bool IsOpen();

    bool CanUpdate() const;     // Can Edit/AddNew/Delete be called?
    void ClsEditFlag();

    bool AddField(LPCTSTR szField, int nData);

    void SetDatabase(CMyDatabase* pDb) { m_pMyDatabase = pDb; }
    bool SetTableName(LPCTSTR szName, DWORD idNewKey = 0);

    CMyField		Fields(char* szField);
    CMyField 		Fields(UINT	uiIndex );

    CMyDatabase*		m_pMyDatabase;

protected: // inside function
    int				UpdateFieldsCount();

    void			BuildUpdateSQL();
    void			BuildUpdateCondition();
    void			BuildUpdateOpration();

    CMyField* 		GetFieldRef(UINT uiIndex);

    bool			Query(char* szSQL, MYSQL_RES * * res);

    void			GetFieldValue(UINT uiIndex);
    bool			FormatField(char* szFormat, const CMyField& objField);

protected:
    MYSQL_FIELD*	m_Fields;				//? COPY的结构，table、def未复制，不可用

    unsigned int 	m_uiFieldsCount;

    char	  		m_szKey[16];
    unsigned int 	m_uiKeyIndex;
    char		 	m_szKeyData[256];		//paled add 可能有点大

    bool*			m_pbDirty;

    bool 			m_bEdit;
    bool 			m_bIsOpen;

    int				m_nEditMode;

    CMyField*		m_objFields;

    char 			m_szTableName[32];
    char 			m_szUpdateSQL[1024];
    char			m_szUpdateOpration[1024];
    char			m_szUpdateCondition[128];

    bool			m_bCanUpdate;
};

#endif // MYRECORD_H
