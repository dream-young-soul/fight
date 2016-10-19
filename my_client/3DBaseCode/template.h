
//**********************************************************
// 代码编辑器
//**********************************************************


//		模板

#ifndef TEMPLATE_LIB
#define TEMPLATE_LIB


#define NULL				0

//typedef unsigned char		BYTE;
//typedef unsigned short		WORD;
//typedef unsigned int		DWORD;

//-----------------------------------------------------------------------------

template < class T, int nSize = 0 >
class STACK 										//栈
{

private:
    T*					 pStack;					//数据指针数组
    int					nPoint;						//栈指针
    int					nCount;						//栈空间

protected:
    void Init( int nC )
    {
        if ( nC && ( pStack = new T[ nC ] ) != NULL )
        {
            SetMaxCount( nC );
        }
        else
        {
            SetMaxCount( 0 );
        }
    }
    void Release( void )
    {
        if ( GetMaxCount() )
        {
            delete []pStack, SetMaxCount( 0 ),
                   Clear();
        }
    };
    void SetMaxCount( int nC ) { nCount = nC; };

public:
    STACK( void ) : nPoint( 0 ), nCount( 0 ) { Init( nSize ); };
    ~STACK( void ) { Release(); };

    bool IsEmpty( void )							//栈空？
    { return ( GetCount() <= 0 ); };
    bool IsFull( void )								//栈满？
    { return ( GetCount() >= GetMaxCount() ); };
    int GetCount( void )							//返回栈中数据数量
    { return nPoint; };
    void Push( T t )								//进栈
    { if ( !IsFull() ) { pStack[ nPoint++ ] = t; } }
    T Pop( void )									//出栈
    { return ( ( !IsEmpty() ) ? pStack[ --nPoint ] : NULL ); }
    void UnsafePush( T t )							//进栈(非安全检查)
    { pStack[ nPoint++ ] = t; };
    T UnsafePop( void )								//出栈(非安全检查)
    { return pStack[ --nPoint ]; };
    void Clear( void )								//清空栈
    { nPoint = 0; };
    void Resize( int nC )							//重设栈尺寸
    { Release(); Init( nC ); };
    int GetMaxCount( void )							//得到栈尺寸
    { return nCount; };
};

//-----------------------------------------------------------------------------

template < class T, int nSize = 0 >
class FILOMEMORY 									//FILO(栈)内存分配/释放
{

private:
    T*				 pMemory;						//内存堆
    int				nCount;							//数量
    int				nPoint;							//已分配数量

protected:
    void Init( int nC )
    {
        if ( nC && ( pMemory = new T[ nC ] ) != NULL )
        {
            SetMaxCount( nC );
        }
        else { SetMaxCount( 0 ); }
    }
    void Release( void )		{ if ( GetMaxCount() ) { delete []pMemory, SetMaxCount( 0 ), Clear(); } };
    void SetMaxCount( int nC )	{ nCount = nC; };

public:
    FILOMEMORY( void ) : nPoint( 0 ), nCount( 0 ) { Init( nSize ); };
    ~FILOMEMORY( void ) { Release(); };

    bool IsFull( void )								//堆满？
    { return ( GetCount() >= GetMaxCount() ); }
    int GetCount( void )							//返回堆中数据数量
    { return nPoint; }
    T* GetOne( void )								//分配一块
    { return ( IsFull() ) ? NULL : &pMemory[ nPoint++ ]; };
    T* CGetOne( void )								//分配一块(清零)
    {
        T*			 pTemp;
        if ( ( pTemp = GetOne() ) == NULL ) { return NULL; }
        memset( pTemp, 0, sizeof( T ) );
        return pTemp;
    }
    T* UnsafeGetOne( void )						//分配一块(非安全检查)
    { return &pMemory[ nPoint++ ]; };
    T* CUnsafeGetOne( void )						//分配一块(非安全检查，清零)
    {
        T*			 pTemp;
        if ( ( pTemp = UnsafeGetOne() ) == NULL ) { return NULL; }
        memset( pTemp, 0, sizeof( T ) );
        return pTemp;
    }
    void Clear( void )								//清空堆
    { nPoint = 0; };
    void Free( T* pT )								//释放
    { if ( nPoint && pT == ( pMemory + nPoint - 1 ) ) { nPoint--; } };
    void Resize( int nC )							//重设堆尺寸
    { Release(); Init( nC ); };
    int GetMaxCount( void )		{ return nCount; };
};

//-----------------------------------------------------------------------------

template < class T, int nSize = 0 >
class FIXMEMORY 								//数组内存堆
{
private:
    T*			pMemory;						//内存堆
    char*		pUse;							//使用标记
    int			nCount;							//数量
    int			nPoint;							//当前数组编号

protected:
    void Init( int nC )
    {
        if ( nC && ( pMemory = new T[ nC ] ) != NULL )
        {
            if ( ( pUse = new char[ nC ] ) != NULL )
            {
                SetMaxCount( nC );
                Clear();
            }
            else
            {
                delete []pMemory;
                SetMaxCount( 0 );
            }
        }
        else
        {
            SetMaxCount( 0 );
        }
    }
    void Release( void )
    {
        if ( GetMaxCount() )
        {
            delete []pUse, delete []pMemory, SetMaxCount( 0 ), SetPoint( 0 );
        }
    }
    void SetMaxCount( int nC )		{ nCount = nC; };
    void SetPoint( int nC )			{ nPoint = nC; }
    int GetPoint( void )			{ return nPoint; }

public:
    FIXMEMORY( void ) : nCount( 0 ), nPoint( 0 ) { Init( nSize ); };
    ~FIXMEMORY( void ) { Release(); };

    bool IsFull( void )								//堆满？
    {
        for( int i = 0; i < GetMaxCount(); i++ )
            if ( !pUse[ i ] )
            {
                return false;
            }
        return true;
    }
    int GetCount( void )							//返回堆中数据数量
    {
        int i, j;
        for( i = j = 0; i < GetMaxCount(); i++ )
            if ( pUse[ i ] ) { j++; }
        return j;
    }
    T* GetOne( void )								//分配一块
    {
        int i;
        for( i = GetPoint(); i < GetMaxCount(); i++ )
            if ( !pUse[ i ] )
            {
                pUse[ i ] = 1;
                SetPoint( i++ );
                return ( t + i );
            }
        for( i = 0; i < GetPoint(); i++ )
            if ( !pUse[ i ] )
            {
                pUse[ i ] = 1;
                SetPoint( i++ );
                return ( t + i );
            }
        return NULL;
    }
    T* CGetOne( void )								//分配一块(清零)
    {
        T*			 pTemp;
        if ( ( pTemp = GetOne() ) == NULL ) { return NULL; }
        memset( pTemp, 0, sizeof( T ) );
        return pTemp;
    };
    void Clear( void )								//清空堆
    { 
		memset( pUse, 0, GetMaxCount() ), SetPoint( 0 ); 
	};
    void Free( T* )								//释放
    {
        int i;
        for( i = GetPoint() - 1; i >= 0; i-- )
            if ( pUse[ i ] && ( pMemory + i ) == pT )
            {
                pUse[ i ] = 0;
                SetPoint( i );
                return;
            }
        for( i = GetMaxCount() - 1; i >= GetPoint(); i-- )
            if ( pUse[ i ] && ( pMemory + i ) == pT )
            {
                pUse[ i ] = 0;
                SetPoint( i );
                return;
            }
        return;
    }
    void Resize( int nC )							//重设堆尺寸
    { Release(); Init( nC ); };
    int GetMaxCount( void )
    { return nCount; };
};

//-----------------------------------------------------------------------------

template< class T >
class CHAIN_S;

template< class T >
class CHAINNODE_S : public T 						//结构连结点
{

    friend CHAIN_S< T >;

private:
    CHAINNODE_S< T >	* pNext;

public:
    CHAINNODE_S( void ) : pNext( NULL ) {};
};

template< class T >
class CHAIN_S 										//结构链表
{

private:
    CHAINNODE_S< T >	* pFirst;

public:
    CHAIN_S( void ) : pFirst( NULL ) {};
    ~CHAIN_S( void ) { Reset(); };

    void Reset( void )								//链表清零
    {
        CHAINNODE_S< T >	* pTemp;
        while( pFirst )
        {
            pTemp = pFirst->pNext;
            pFirst->pNext = NULL;
            pFirst = pTemp;
        }
    }
    bool InstertF( CHAINNODE_S< T > * pNode )		//插入结点(头)
    {
        if ( pNode->pNext != NULL ) { return false; }
        if ( pFirst == NULL )
        {
            pFirst = pNode;
            return true;
        }
        pNode->pNext = pFirst;
        pFirst = pNode;
        return true;
    };
    bool Delete( CHAINNODE_S< T > * pNode )			//删除指定结点
    {
        CHAINNODE_S< T >		* pTemp = pFirst, * pT1 = NULL;
        if ( pTemp == NULL ) { return false; }
        while( pTemp != pNode )
        {
            pT1 = pTemp;
            if ( ( pTemp = pTemp->pNext ) == NULL )
            {
                return false;
            }
        }
        if ( pTemp == pFirst )
        {
            pFirst = pFirst->pNext;
        }
        else
        {
            pT1->pNext = pTemp->pNext;
        }
        pNode->pNext = NULL;
        return true;
    };
    int GetAllNode( CHAINNODE_S< T > * pNode[], int nCount )	//得到所有节点指针和数量
    {
        int					i;
        CHAINNODE< T >		* pTemp = pFirst;
        for( i = 0; pTemp != NULL && i < nCount; pTemp = pTemp->pNext )
        {
            pNode[ i++ ] = pTemp;
        }
        return i;
    };
    int GetCount( void )							//取得链表中节点数量
    {
        int					i;
        CHAINNODE_S< T >	* pTemp = pFirst;
        for( i = 0; pTemp != NULL; i++, pTemp = pTemp->pNext );
        return i;
    };
};

//-----------------------------------------------------------------------------

template< class T >
class CHAIN;

template< class T >
class CHAINNODE
{

    friend CHAIN< T >;

private:
    T*			 pNode;
    CHAINNODE*	 pNext;
};

template< class T >
class CHAIN
{

private:
    CHAINNODE< T >	* pFirst;

public:
    CHAIN( void ) : pFirst( NULL ) {};

    void Reset( void )								//链表清零
    {
        CHAINNODE< T > * pT1 = pFirst, * pT2;
        while( pT1 ) { pT2 = pT1->pNext; delete pT1; pT1 = pT2; };
        pFirst = NULL;
    };
    bool InstertF( T* pNode )						//插入结点(头)
    {
        if ( pFirst == NULL )
        {
            if ( ( pFirst = new CHAINNODE< T > ) == NULL )
            {
                return false;
            }
            pFirst->pNext = NULL;
            pFirst->pNode = pNode;
            return true;
        }
        CHAINNODE< T > pTemp;
        if ( ( pTemp = new CHAINNODE< T > ) == NULL )
        {
            return false;
        }
        pTemp->pNext = pFirst;
        pTemp->pNode = pNode;
        pFirst = pTemp;
        return true;
    };
    bool Delete( T* pNode )						//删除指定结点
    {
        CHAINNODE< T >		* pTemp = pFirst, * pT1 = NULL;
        if ( pTemp == NULL ) { return false; }
        while( pTemp->pNode != pNode )
        {
            pT1 = pTemp;
            if ( ( pTemp = pTemp->pNext ) == NULL )
            {
                return false;
            }
        }
        if ( pTemp == pFirst )
        {
            pTemp = pTemp->pNext;
            delete pFirst;
            pFirst = pTemp;
        }
        else
        {
            pT1->pNext = pTemp->pNext;
            delete pTemp;
        }
        return true;
    };
    int GetAllNode( T pNode[], int nCount )			//得到所有节点指针和数量
    {
        int					i;
        CHAINNODE< T >		* pTemp = pFirst;
        for( i = 0; pTemp != NULL && i < nCount; pTemp = pTemp->pNext )
        {
            pNode[ i++ ] = pTemp->pNode;
        }
        return i;
    };
    int GetCount( void )							//取得链表中节点数量
    {
        int					i;
        CHAINNODE< T >		* pTemp = pFirst;
        for( i = 0; pTemp != NULL; i++, pTemp = pTemp->pNext );
        return i;
    };
};

//-----------------------------------------------------------------------------

#endif
