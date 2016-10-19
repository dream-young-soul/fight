#include "..\include\c3_phy.h"
#include "..\include\c3_main.h"
#include "..\include\c3_datafile.h"
#include "..\include\nvtristrip.h"

C3_CORE_DLL_API
void Motion_Clear ( C3Motion *lpMotion )
{
	lpMotion->dwBoneCount = 0;
	lpMotion->dwFrames = 0;
	lpMotion->nFrame = 0;
	//lpMotion->lpFrames = 0;

	lpMotion->dwKeyFrames = 0;
	lpMotion->lpKeyFrame = 0;

	lpMotion->matrix = 0;
	lpMotion->nFrame = 0;
	lpMotion->dwMorphCount = 0;
	lpMotion->lpMorph = 0;
}

C3_CORE_DLL_API
BOOL Motion_LoadPack ( C3Motion **lpMotion, HANDLE f )
{
	*lpMotion = new C3Motion;
	Motion_Clear ( *lpMotion );
	DWORD bytes ;

	// bone
	ReadFile ( f, &( *lpMotion )->dwBoneCount, sizeof ( DWORD ), &bytes, 0 ) ;

	// matrix
	ReadFile ( f, &( *lpMotion )->dwFrames, sizeof ( DWORD ), &bytes, 0 ) ;

	( *lpMotion )->matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
	for ( DWORD n = 0; n < ( *lpMotion )->dwBoneCount; n++ )
		D3DXMatrixIdentity ( &( *lpMotion )->matrix[n] );

/*	ReadFile (	f,
				( *lpMotion )->lpFrames,
				sizeof ( D3DXMATRIX ) * ( *lpMotion )->dwBoneCount * ( *lpMotion )->dwFrames,
				&bytes,
				0 ) ;*/

	// 是否有关键帧
	char kf[4];
	ReadFile ( f, kf, sizeof ( char ) * 4, &bytes, 0 ) ;
	if ( kf[0] == 'K' &&
		 kf[1] == 'K' &&
		 kf[2] == 'E' &&
		 kf[3] == 'Y' )
	{
		ReadFile ( f, &( *lpMotion )->dwKeyFrames, sizeof ( DWORD ), &bytes, 0 ) ;
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwKeyFrames];

		for ( DWORD kk = 0; kk < ( *lpMotion )->dwKeyFrames; kk++ )
		{
			ReadFile ( f,
					   &( *lpMotion )->lpKeyFrame[kk].pos,
					   sizeof ( DWORD ),
					   &bytes,
					   0 ) ;

			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
			ReadFile ( f,
					   ( *lpMotion )->lpKeyFrame[kk].matrix,
					   sizeof ( D3DXMATRIX ),	
					   &bytes,
					   0 ) ;
		}
	}
	else if (	kf[0] == 'X' &&
		kf[1] == 'K' &&
		kf[2] == 'E' &&
		kf[3] == 'Y' )
	{	// 减小C3大小, 去掉运动矩阵中 14, 24, 34, 44的格式
		ReadFile ( f, &( *lpMotion )->dwKeyFrames, sizeof ( DWORD ), &bytes, 0 ) ;
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwKeyFrames];
		
		for ( DWORD kk = 0; kk < ( *lpMotion )->dwKeyFrames; kk++ )
		{
			WORD wPos = 0;
			ReadFile ( f,
				&wPos,
				sizeof ( WORD ),	// 注意这儿也减小了节省 2 个字节. :)
				&bytes,
				0 );
			
			( *lpMotion )->lpKeyFrame[kk].pos = ( DWORD )wPos;
			
			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
			
			struct TIDY_MATRIX {
				float        _11, _12, _13;
				float        _21, _22, _23;
				float        _31, _32, _33;
				float        _41, _42, _43;				
			};
			
			TIDY_MATRIX mx[256];	// 骨骼数不可能超过256
			ReadFile ( f,
				&mx, 
				sizeof ( TIDY_MATRIX ) * ( *lpMotion )->dwBoneCount,
				&bytes,
				0 );
			
			for ( DWORD bb = 0; bb < ( *lpMotion )->dwBoneCount; bb++ )
			{
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._11 = mx[bb]._11;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._12 = mx[bb]._12;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._13 = mx[bb]._13;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._14 = 0.0f;
				
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._21 = mx[bb]._21;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._22 = mx[bb]._22;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._23 = mx[bb]._23;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._24 = 0.0f;
				
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._31 = mx[bb]._31;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._32 = mx[bb]._32;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._33 = mx[bb]._33;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._34 = 0.0f;
				
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._41 = mx[bb]._41;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._42 = mx[bb]._42;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._43 = mx[bb]._43;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._44 = 1.0f;
			}			
		}
	}
	else
	{
		SetFilePointer ( f, -4, 0, FILE_CURRENT ) ;
		( *lpMotion )->dwKeyFrames = ( *lpMotion )->dwFrames;
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwFrames];

		for ( DWORD kk = 0; kk < ( *lpMotion )->dwFrames; kk++ )
		{
			( *lpMotion )->lpKeyFrame[kk].pos = kk;
			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
		}
		for ( DWORD bb = 0; bb < ( *lpMotion )->dwBoneCount; bb++ )
		{
			for ( DWORD kk = 0; kk < ( *lpMotion )->dwFrames; kk++ )
			{
				ReadFile ( f,
						   ( *lpMotion )->lpKeyFrame[kk].matrix[bb],
						   sizeof ( D3DXMATRIX ),	
						   &bytes,
						   0 ) ;
			}
		}
	}


	// morph
	ReadFile ( f, &( *lpMotion )->dwMorphCount, sizeof ( DWORD ), &bytes, 0 ) ;

	( *lpMotion )->lpMorph = new float[( *lpMotion )->dwMorphCount * ( *lpMotion )->dwFrames];
	ReadFile (	f,
				( *lpMotion )->lpMorph,
				sizeof ( float ) * ( *lpMotion )->dwMorphCount * ( *lpMotion )->dwFrames,
				&bytes,
				0 ) ;
	return true ;
}

C3_CORE_DLL_API
BOOL Motion_Load ( C3Motion **lpMotion, FILE *file )
{
	*lpMotion = new C3Motion;
	Motion_Clear ( *lpMotion );
	
	// bone
	fread ( &( *lpMotion )->dwBoneCount, sizeof ( DWORD ), 1, file );
	
	// matrix
	fread ( &( *lpMotion )->dwFrames, sizeof ( DWORD ), 1, file );
	
	( *lpMotion )->matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
	for ( DWORD n = 0; n < ( *lpMotion )->dwBoneCount; n++ )
		D3DXMatrixIdentity ( &( *lpMotion )->matrix[n] );
	
	// 是否有关键帧
	char kf[4];
	fread ( kf, sizeof ( char ), 4, file );
	if ( kf[0] == 'K' &&
		kf[1] == 'K' &&
		kf[2] == 'E' &&
		kf[3] == 'Y' )
	{
		fread ( &( *lpMotion )->dwKeyFrames, sizeof ( DWORD ), 1, file );
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwKeyFrames];
		
		for ( DWORD kk = 0; kk < ( *lpMotion )->dwKeyFrames; kk++ )
		{
			fread ( &( *lpMotion )->lpKeyFrame[kk].pos,
				sizeof ( DWORD ),
				1,
				file );
			
			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
			fread ( ( *lpMotion )->lpKeyFrame[kk].matrix,
				sizeof ( D3DXMATRIX ),
				( *lpMotion )->dwBoneCount,
				file );
		}
	}
	else if (	kf[0] == 'Z' &&
		kf[1] == 'K' &&
		kf[2] == 'E' &&
		kf[3] == 'Y' )
	{	// 用运动分量购置运动矩阵, 减小数据冗余信息
		fread ( &( *lpMotion )->dwKeyFrames, sizeof ( DWORD ), 1, file );
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwKeyFrames];
		
		for ( DWORD kk = 0; kk < ( *lpMotion )->dwKeyFrames; kk++ )
		{
			WORD wPos = 0;
			fread ( &wPos,
				sizeof ( WORD ),	// 注意这儿也减小了节省 2 个字节. :)
				1,
				file );
			
			( *lpMotion )->lpKeyFrame[kk].pos = ( DWORD )wPos;
			
			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
			
			
			struct DIV_INFO {
				D3DXQUATERNION	quaternion;	// 旋转数据
				float x, y, z;				// 位移数据
			};
			
			DIV_INFO mx[256];	// 骨骼数不可能超过256
			fread ( &mx, sizeof ( DIV_INFO ), ( *lpMotion )->dwBoneCount, file );
			
			for ( DWORD bb = 0; bb < ( *lpMotion )->dwBoneCount; bb++ )
			{
				::D3DXMatrixIdentity( &( *lpMotion )->lpKeyFrame[kk].matrix[bb] );
				
				::D3DXMatrixRotationQuaternion( &( *lpMotion )->lpKeyFrame[kk].matrix[bb], &mx[bb].quaternion );
				
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._41 = mx[bb].x;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._42 = mx[bb].y;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._43 = mx[bb].z;
				( *lpMotion )->lpKeyFrame[kk].matrix[bb]._44 = 1.0f;
			}			
		}
	}
	else
	{
		fseek ( file, -4, SEEK_CUR );

		( *lpMotion )->dwKeyFrames = ( *lpMotion )->dwFrames;
		( *lpMotion )->lpKeyFrame = new C3KeyFrame[( *lpMotion )->dwFrames];

		for ( DWORD kk = 0; kk < ( *lpMotion )->dwFrames; kk++ )
		{
			( *lpMotion )->lpKeyFrame[kk].pos = kk;
			( *lpMotion )->lpKeyFrame[kk].matrix = new D3DXMATRIXA16[( *lpMotion )->dwBoneCount];
		}
		for ( DWORD bb = 0; bb < ( *lpMotion )->dwBoneCount; bb++ )
		{
			for ( DWORD kk = 0; kk < ( *lpMotion )->dwFrames; kk++ )
			{
				fread ( ( *lpMotion )->lpKeyFrame[kk].matrix[bb],
					sizeof ( D3DXMATRIX ),
					1,
					file );
			}
		}
	}

	
	// morph
	fread ( &( *lpMotion )->dwMorphCount, sizeof ( DWORD ), 1, file );
	
	( *lpMotion )->lpMorph = new float[( *lpMotion )->dwMorphCount * ( *lpMotion )->dwFrames];
	fread ( ( *lpMotion )->lpMorph,
			sizeof ( float ),
			( *lpMotion )->dwMorphCount * ( *lpMotion )->dwFrames,
			file );
	return true;
}

C3_CORE_DLL_API
BOOL Motion_Save ( char *lpName, C3Motion *lpMotion, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// phy
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'M';
	chunk.byChunkID[1] = 'O';
	chunk.byChunkID[2] = 'T';
	chunk.byChunkID[3] = 'I';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// bone
	fwrite ( &lpMotion->dwBoneCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// matrix
	fwrite ( &lpMotion->dwFrames, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// key 标志
	char kf[4];
	kf[0] = 'K';
	kf[1] = 'K';
	kf[2] = 'E';
	kf[3] = 'Y';

	fwrite ( kf, sizeof ( char ), 4, file );
	chunk.dwChunkSize += sizeof ( char ) * 4;

	fwrite ( &lpMotion->dwKeyFrames,
			 sizeof ( DWORD ),
			 1,
			 file );
	chunk.dwChunkSize += sizeof ( DWORD );

	for ( DWORD kk = 0; kk < lpMotion->dwKeyFrames; kk++ )
	{
		fwrite ( &lpMotion->lpKeyFrame[kk].pos,
				 sizeof ( DWORD ),
				 1,
				 file );
		chunk.dwChunkSize += sizeof ( DWORD );

		fwrite ( lpMotion->lpKeyFrame[kk].matrix,
				 sizeof ( D3DXMATRIXA16 ),
				 lpMotion->dwBoneCount,
				 file );
		chunk.dwChunkSize += sizeof ( D3DXMATRIX ) * lpMotion->dwBoneCount;
	}

	// morph
	fwrite ( &lpMotion->dwMorphCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpMotion->lpMorph,
			 sizeof ( float ),
			 lpMotion->dwMorphCount * lpMotion->dwFrames,
			 file );
	chunk.dwChunkSize += sizeof ( float ) * lpMotion->dwMorphCount * lpMotion->dwFrames;

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}

C3_CORE_DLL_API
void Motion_Unload ( C3Motion **lpMotion )
{
	for ( DWORD kk = 0; kk < ( *lpMotion )->dwKeyFrames; kk++ )
	{
		SafeDeleteEx ( ( *lpMotion )->lpKeyFrame[kk].matrix );
	}
	SafeDeleteEx ( ( *lpMotion )->lpKeyFrame );

	SafeDeleteEx ( ( *lpMotion )->matrix );
	SafeDeleteEx ( ( *lpMotion )->lpMorph );

	SafeDelete ( *lpMotion );
}

C3_CORE_DLL_API
void Motion_GetMatrix ( C3Motion *lpMotion, DWORD dwBone, D3DXMATRIXA16 
*lpMatrix )
{
	// alpha
	int sindex = -1;
	int eindex = -1;
	for ( int n = 0; n < ( int )lpMotion->dwKeyFrames; n++ )
	{
		if ( lpMotion->lpKeyFrame[n].pos <= ( DWORD )lpMotion->nFrame )
		{
			if ( sindex == -1 || n > sindex)
				sindex = n;
		}
		if ( lpMotion->lpKeyFrame[n].pos > ( DWORD )lpMotion->nFrame )
		{
			if ( eindex == -1 || n < eindex )
				eindex = n;
		}
	}
	if ( sindex == -1 && eindex > -1 )
	{
		*lpMatrix = lpMotion->lpKeyFrame[eindex].matrix[dwBone];
	}
	else
	if ( sindex > -1 && eindex == -1 )
	{
		*lpMatrix = lpMotion->lpKeyFrame[sindex].matrix[dwBone];
	}
	else
	if ( sindex > -1 && eindex > -1 )
	{
		*lpMatrix = lpMotion->lpKeyFrame[sindex].matrix[dwBone] + 
				   ( ( float )( lpMotion->nFrame - lpMotion->lpKeyFrame[sindex].pos ) / ( float )( lpMotion->lpKeyFrame[eindex].pos - lpMotion->lpKeyFrame[sindex].pos ) ) * 
				   ( lpMotion->lpKeyFrame[eindex].matrix[dwBone] - lpMotion->lpKeyFrame[sindex].matrix[dwBone] );
	}
}

C3_CORE_DLL_API
void Phy_Clear ( C3Phy *lpPhy )
{
	lpPhy->lpName = 0;

	lpPhy->dwBlendCount = 0;

	lpPhy->dwNVecCount = 0;		// 顶点数(普通顶点)
	lpPhy->dwAVecCount = 0;		// 顶点数(透明顶点)
	lpPhy->lpVB = 0;			// 顶点池(普通顶点/透明顶点)
	lpPhy->vb = 0;

	lpPhy->dwNTriCount = 0;		// 多边形数(普通多边形)
	lpPhy->dwATriCount = 0;		// 多边形数(透明多边形)
	lpPhy->lpIB = 0;			// 索引池(普通多边形/透明多边形)
	lpPhy->ib = 0;

	lpPhy->lpTexName = 0;
	lpPhy->nTex = -1;

	lpPhy->bboxMin.x = lpPhy->bboxMin.y = lpPhy->bboxMin.z = 0;
	lpPhy->bboxMax.x = lpPhy->bboxMax.y = lpPhy->bboxMax.z = 0;

	lpPhy->lpMotion = 0;

	lpPhy->fA = lpPhy->fR = lpPhy->fG = lpPhy->fB = 1.0f;
	lpPhy->bDraw = true;
	lpPhy->dwTexRow = 1;
	lpPhy->uvstep.x = 0;
	lpPhy->uvstep.y = 0;

	D3DXMatrixIdentity ( &lpPhy->InitMatrix );
}

C3_CORE_DLL_API
BOOL Phy_Load ( C3Phy **lpPhy, FILE *file, BOOL bTex )
{
	*lpPhy = new C3Phy;
	Phy_Clear ( *lpPhy );

	// 读入名称
	DWORD temp;
	fread ( &temp, sizeof ( unsigned long ), 1, file );
	( *lpPhy )->lpName = new char[temp + 1];
	fread ( ( *lpPhy )->lpName, 1, temp, file );
	( *lpPhy )->lpName[temp] = '\0';

	// blend
	fread ( &( *lpPhy )->dwBlendCount, sizeof ( DWORD ), 1, file );

	// vec count
	fread ( &( *lpPhy )->dwNVecCount, sizeof ( DWORD ), 1, file );
	fread ( &( *lpPhy )->dwAVecCount, sizeof ( DWORD ), 1, file );

  

	// 创建输出 vb
	if ( FAILED ( g_D3DDevice->CreateVertexBuffer ( ( ( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount ) * sizeof ( PhyOutVertex ),
													D3DUSAGE_WRITEONLY,
													0,
													D3DPOOL_MANAGED,
													&( *lpPhy )->vb,0 ) ) )
		return false;

	// 读取顶点
	( *lpPhy )->lpVB = new PhyVertex[( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount];
	fread ( ( *lpPhy )->lpVB,
			sizeof ( PhyVertex ),
			( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount,
			file );

	PhyOutVertex *vertex;
	if ( FAILED ( ( *lpPhy )->vb->Lock ( 0,
										 0,
										 ( void** )&vertex,
										 D3DLOCK_NOSYSLOCK ) ) )
		return false;

	for ( DWORD n = 0; n < ( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount; n++ )
	{
		vertex[n].color = D3DCOLOR_XRGB ( 255, 255, 255 );
		vertex[n].u = ( *lpPhy )->lpVB[n].u;
		vertex[n].v = ( *lpPhy )->lpVB[n].v;
	}

	if ( FAILED ( ( *lpPhy )->vb->Unlock () ) )
		return false;


	// tri count
	fread ( &( *lpPhy )->dwNTriCount, sizeof ( DWORD ), 1, file );
	fread ( &( *lpPhy )->dwATriCount, sizeof ( DWORD ), 1, file );
	// tri
	if ( FAILED ( g_D3DDevice->CreateIndexBuffer ( ( ( *lpPhy )->dwNTriCount + ( *lpPhy )->dwATriCount ) * sizeof ( WORD ) * 3,
												   D3DUSAGE_WRITEONLY,
												   D3DFMT_INDEX16,
												   D3DPOOL_MANAGED,
												   &( *lpPhy )->ib,0 ) ) )
		return false;

	WORD *tri;
	if ( FAILED ( ( *lpPhy )->ib->Lock ( 0,
									0,
									( void** )&tri,
									D3DLOCK_NOSYSLOCK ) ) )
		return false;
	fread ( tri, sizeof ( WORD ), ( ( *lpPhy )->dwNTriCount + ( *lpPhy )->dwATriCount ) * 3, file );

	/*
	// 优化
	PrimitiveGroup *nl, *al;
	unsigned short ns, as;

	SetListsOnly ( true );

	if ( ( *lpPhy )->dwNTriCount > 0 )
	{
		if ( !( tri[0] == tri[1] ||
			 tri[1] == tri[2] ||
			 tri[2] == tri[0] ) )
		{
			GenerateStrips ( ( const unsigned short* )tri,
							 ( *lpPhy )->dwNTriCount * 3,
							 &nl,
							 &ns );
			CopyMemory ( tri,
						 nl->indices,
						 nl->numIndices * sizeof ( WORD ) );
			SafeDeleteEx ( nl );
		}
		else
		{
			//MessageBox ( g_hWnd, lpName, "file erro", MB_OK );
		}
	}
	if ( ( *lpPhy )->dwATriCount > 0 )
	{
		if ( !( tri[0] == tri[1] ||
			 tri[1] == tri[2] ||
			 tri[2] == tri[0] ) )
		{
			GenerateStrips ( ( const unsigned short* )( tri + ( *lpPhy )->dwNTriCount * 3 ),
							 ( *lpPhy )->dwATriCount * 3,
							 &al,
							 &as );

			CopyMemory ( tri + ( *lpPhy )->dwNTriCount * 3,
						 al->indices,
						 al->numIndices * sizeof ( WORD ) );
			SafeDeleteEx ( al );
		}
		else
		{
			//MessageBox ( g_hWnd, lpName, "file erro", MB_OK );
		}
	}
	*/

	if ( FAILED ( ( *lpPhy )->ib->Unlock () ) )
		return false;

	// 读入贴图
	fread ( &temp, sizeof ( DWORD ), 1, file );
	( *lpPhy )->lpTexName = new char[temp + 1];
	fread ( ( *lpPhy )->lpTexName, 1, temp, file );
	( *lpPhy )->lpTexName[temp] = '\0';
	// 创建贴图
	if ( bTex )
	{
		C3Texture *tex;
		( *lpPhy )->nTex = Texture_Load ( &tex, ( *lpPhy )->lpTexName );
		if ( ( *lpPhy )->nTex == -1 )
			return false;
	}

	// bounding box
	fread ( &( *lpPhy )->bboxMin, sizeof ( D3DXVECTOR3 ), 1, file );
	fread ( &( *lpPhy )->bboxMax, sizeof ( D3DXVECTOR3 ), 1, file );

	fread ( &( *lpPhy )->InitMatrix, sizeof ( D3DXMATRIXA16 ), 1, file );

	// 用户自定义消息
	fread ( &( *lpPhy )->dwTexRow, sizeof ( DWORD ), 1, file );

	fread ( &( *lpPhy )->Key.dwAlphas, sizeof ( DWORD ), 1, file );
	( *lpPhy )->Key.lpAlphas = new C3Frame[( *lpPhy )->Key.dwAlphas];
	fread ( ( *lpPhy )->Key.lpAlphas, sizeof ( C3Frame ), ( *lpPhy )->Key.dwAlphas, file );

	fread ( &( *lpPhy )->Key.dwDraws, sizeof ( DWORD ), 1, file );
	( *lpPhy )->Key.lpDraws = new C3Frame[( *lpPhy )->Key.dwDraws];
	fread ( ( *lpPhy )->Key.lpDraws, sizeof ( C3Frame ), ( *lpPhy )->Key.dwDraws, file );

	fread ( &( *lpPhy )->Key.dwChangeTexs, sizeof ( DWORD ), 1, file );
	( *lpPhy )->Key.lpChangeTexs = new C3Frame[( *lpPhy )->Key.dwChangeTexs];
	fread ( ( *lpPhy )->Key.lpChangeTexs, sizeof ( C3Frame ), ( *lpPhy )->Key.dwChangeTexs, file );

	char flag[4];
	// 12-20-2002 : STEP
	fread ( flag, sizeof ( char ), 4, file );

	if ( flag[0] == 'S' &&
		 flag[1] == 'T' &&
		 flag[2] == 'E' &&
		 flag[3] == 'P' )
	{
		fread ( &( *lpPhy )->uvstep.x, sizeof ( float ), 1, file );
		fread ( &( *lpPhy )->uvstep.y, sizeof ( float ), 1, file );
	}
	else
		fseek ( file, -4, SEEK_CUR ) ;

	Phy_SetColor ( *lpPhy, 1, 1, 1, 1 );
	return true;
}

C3_CORE_DLL_API
BOOL Phy_LoadPack ( C3Phy **lpPhy, HANDLE f, BOOL bTex )
{
	*lpPhy = new C3Phy;
	Phy_Clear ( *lpPhy );
	DWORD bytes ;

	// Load name
	DWORD temp;
	ReadFile ( f, &temp, sizeof ( unsigned long ), &bytes, 0 ) ;
	( *lpPhy )->lpName = new char[temp + 1];
	ReadFile ( f, ( *lpPhy )->lpName, temp, &bytes, 0 ) ;
	( *lpPhy )->lpName[temp] = '\0';

	// Blend
	ReadFile ( f, &( *lpPhy )->dwBlendCount, sizeof ( DWORD ), &bytes, 0 ) ;

	// Vec count
	ReadFile ( f, &( *lpPhy )->dwNVecCount , sizeof ( DWORD ), &bytes, 0 ) ;
	ReadFile ( f, &( *lpPhy )->dwAVecCount , sizeof ( DWORD ), &bytes, 0 ) ;

	// 创建输出 vb
	if ( FAILED ( g_D3DDevice->CreateVertexBuffer ( ( ( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount ) * sizeof ( PhyOutVertex ),
													D3DUSAGE_WRITEONLY,
													0,
													D3DPOOL_MANAGED,
													&( *lpPhy )->vb,0 ) ) )
		return false;

	// 读取顶点
	( *lpPhy )->lpVB = new PhyVertex[( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount];
	ReadFile (	f,
				( *lpPhy )->lpVB,
				sizeof ( PhyVertex ) * ( ( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount ),
				&bytes,
				0 ) ;

	PhyOutVertex *vertex;
	if ( FAILED ( ( *lpPhy )->vb->Lock ( 0,
										 0,
										 ( void** )&vertex,
										 D3DLOCK_NOSYSLOCK ) ) )
		return false;

	for ( DWORD n = 0; n < ( *lpPhy )->dwNVecCount + ( *lpPhy )->dwAVecCount; n++ )
	{
		vertex[n].color = D3DCOLOR_XRGB ( 255, 255, 255 );
		vertex[n].u = ( *lpPhy )->lpVB[n].u;
		vertex[n].v = ( *lpPhy )->lpVB[n].v;
	}

	if ( FAILED ( ( *lpPhy )->vb->Unlock () ) )
		return false;

	// Tri count
	ReadFile ( f, &( *lpPhy )->dwNTriCount, sizeof ( DWORD ), &bytes, 0 ) ;
	ReadFile ( f, &( *lpPhy )->dwATriCount, sizeof ( DWORD ), &bytes, 0 ) ;

	// tri
	if ( FAILED ( g_D3DDevice->CreateIndexBuffer ( ( ( *lpPhy )->dwNTriCount + ( *lpPhy )->dwATriCount ) * sizeof ( WORD ) * 3,
												   D3DUSAGE_WRITEONLY,
												   D3DFMT_INDEX16,
												   D3DPOOL_MANAGED,
												   &( *lpPhy )->ib,0 ) ) )
		return false;

	WORD *tri;
	if ( FAILED ( ( *lpPhy )->ib->Lock ( 0,
										 0,
										 ( void** )&tri,
										 D3DLOCK_NOSYSLOCK ) ) )
		return false;

	ReadFile (	f,
				tri,
				sizeof ( WORD ) * ( ( ( *lpPhy )->dwNTriCount + ( *lpPhy )->dwATriCount ) * 3 ),
				&bytes,
				0 ) ;
	
	/*
	// 优化
	PrimitiveGroup *nl, *al;
	unsigned short ns, as;

	SetListsOnly ( true );






	if ( ( *lpPhy )->dwNTriCount > 0 )
	{
		if ( !( tri[0] == tri[1] ||
			 tri[1] == tri[2] ||
			 tri[2] == tri[0] ) )
		{
			GenerateStrips ( ( const unsigned short* )tri,
							 ( *lpPhy )->dwNTriCount * 3,
							 &nl,
							 &ns );
			CopyMemory ( tri,
						 nl->indices,
						 nl->numIndices * sizeof ( WORD ) );
			SafeDeleteEx ( nl );
		}
		else
		{
			//MessageBox ( g_hWnd, lpName, "file erro", MB_OK );
		}
	}
	if ( ( *lpPhy )->dwATriCount > 0 )
	{
		if ( !( tri[0] == tri[1] ||
			 tri[1] == tri[2] ||
			 tri[2] == tri[0] ) )
		{
			GenerateStrips ( ( const unsigned short* )( tri + ( *lpPhy )->dwNTriCount * 3 ),
							 ( *lpPhy )->dwATriCount * 3,
							 &al,
							 &as );

			CopyMemory ( tri + ( *lpPhy )->dwNTriCount * 3,
						 al->indices,
						 al->numIndices * sizeof ( WORD ) );
			SafeDeleteEx ( al );
		}
		else
		{
			//MessageBox ( g_hWnd, lpName, "file erro", MB_OK );
		}
	}

	*/
	if ( FAILED ( ( *lpPhy )->ib->Unlock () ) )
		return false;

	// Load texture
	ReadFile ( f, &temp, sizeof (DWORD ), &bytes, 0 ) ;
	( *lpPhy )->lpTexName = new char[temp + 1];
	ReadFile ( f, ( *lpPhy )->lpTexName, temp, &bytes, 0 ) ;
	( *lpPhy )->lpTexName[temp] = '\0';

	// Create the texture
	if ( bTex)
	{
		C3Texture *tex;
		( *lpPhy )->nTex = Texture_Load ( &tex, ( *lpPhy )->lpTexName );
		if ( ( *lpPhy )->nTex == -1 )
			return false;
	}



	// bounding box
	ReadFile ( f, &( *lpPhy )->bboxMin, sizeof ( D3DXVECTOR3 ), &bytes, 0 ) ;
	ReadFile ( f, &( *lpPhy )->bboxMax, sizeof ( D3DXVECTOR3 ), &bytes, 0 ) ;
	ReadFile ( f, &( *lpPhy )->InitMatrix, sizeof ( D3DXMATRIXA16), &bytes, 0 ) ;
	
	// 用户自定义消息
	ReadFile ( f, &( *lpPhy )->dwTexRow, sizeof ( DWORD ), &bytes, 0 ) ;
	ReadFile ( f, &( *lpPhy )->Key.dwAlphas, sizeof ( DWORD ), &bytes, 0 );
	( *lpPhy )->Key.lpAlphas = new C3Frame[( *lpPhy )->Key.dwAlphas];
	ReadFile ( f,
			   ( *lpPhy )->Key.lpAlphas, 
			   sizeof ( C3Frame ) * ( *lpPhy )->Key.dwAlphas,
			   &bytes, 
			   0 );
	ReadFile ( f, &( *lpPhy )->Key.dwDraws, sizeof ( DWORD ), &bytes, 0 );
	( *lpPhy )->Key.lpDraws = new C3Frame[( *lpPhy )->Key.dwDraws];
	ReadFile ( f,
			   ( *lpPhy )->Key.lpDraws,
			   sizeof ( C3Frame ) * ( *lpPhy )->Key.dwDraws,
			   &bytes,
			   0 );
	ReadFile ( f, &( *lpPhy )->Key.dwChangeTexs, sizeof ( DWORD ), &bytes, 0 );
	( *lpPhy )->Key.lpChangeTexs = new C3Frame[( *lpPhy )->Key.dwChangeTexs];
	ReadFile ( f,
			   ( *lpPhy )->Key.lpChangeTexs,
			   sizeof ( C3Frame ) * ( *lpPhy )->Key.dwChangeTexs,
			   &bytes,
			   0 );
	
	char flag[4];
	// 12-20-2002 : STEP
	ReadFile ( f, flag, sizeof ( char ) * 4, &bytes, 0 );
	if ( flag[0] == 'S' &&
		 flag[1] == 'T' &&
		 flag[2] == 'E' &&
		 flag[3] == 'P' )
	{
		ReadFile ( f, &( *lpPhy )->uvstep.x, sizeof ( float ), &bytes, 0 );
		ReadFile ( f, &( *lpPhy )->uvstep.y, sizeof ( float ), &bytes, 0 );
	}
	else
		SetFilePointer ( f, -4, 0, FILE_CURRENT ) ;
	Phy_SetColor ( *lpPhy, 1, 1, 1, 1 ) ;
	return true ;
}

C3_CORE_DLL_API
BOOL Phy_Save ( char *lpName, C3Phy *lpPhy, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// phy
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'P';
	chunk.byChunkID[1] = 'H';
	chunk.byChunkID[2] = 'Y';
	chunk.byChunkID[3] = ' ';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// name
	DWORD length = strlen ( lpPhy->lpName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPhy->lpName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	// blend
	fwrite ( &lpPhy->dwBlendCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// vec count
	fwrite ( &lpPhy->dwNVecCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );
	fwrite ( &lpPhy->dwAVecCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// vec
	/*if ( lpPhy->dwBlendCount == 0 )
	{
		if ( lpPhy->lpVB )
		{
			fwrite ( lpPhy->lpVB,
					 sizeof ( PhyVertex0 ),
					 lpPhy->dwVecCount,
					 file );
		}
		else
		if ( lpPhy->vb )
		{
			PhyVertex0 *vertex;
			if ( FAILED ( lpPhy->vb->Lock ( 0,
											0,
											( BYTE** )&vertex,
											D3DLOCK_NOSYSLOCK ) ) )
				return false;

			fwrite ( ( PhyVertex0* )vertex,
					  sizeof ( PhyVertex0 ),
					  lpPhy->dwVecCount,
					  file );

			if ( FAILED ( lpPhy->vb->Unlock () ) )
				return false;
		}
		chunk.dwChunkSize += sizeof ( PhyVertex0 ) * lpPhy->dwVecCount;
	}
	else
	if ( lpPhy->dwBlendCount == 2 )*/
	fwrite ( lpPhy->lpVB,
			 sizeof ( PhyVertex ),
			 lpPhy->dwNVecCount + lpPhy->dwAVecCount,
			 file );
	chunk.dwChunkSize += sizeof ( PhyVertex ) * ( lpPhy->dwNVecCount + 
lpPhy->dwAVecCount );

	// tri count
	fwrite ( &lpPhy->dwNTriCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );
	fwrite ( &lpPhy->dwATriCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );
	// tri
	if ( lpPhy->lpIB )
		fwrite ( lpPhy->lpIB, sizeof ( WORD ), ( lpPhy->dwNTriCount + 
lpPhy->dwATriCount ) * 3, file );
	else
	if ( lpPhy->ib )
	{
		WORD *tri;
		if ( FAILED ( lpPhy->ib->Lock ( 0,
										0,
										( void** )&tri,
										D3DLOCK_NOSYSLOCK ) ) )
			return false;
		fwrite ( tri, sizeof ( WORD ), ( lpPhy->dwNTriCount + lpPhy->dwATriCount ) 
* 3, file );
		if ( FAILED ( lpPhy->ib->Unlock () ) )
			return false;
	}
	chunk.dwChunkSize += sizeof ( WORD ) * ( lpPhy->dwNTriCount + 
lpPhy->dwATriCount ) * 3;

	// texture
	length = strlen ( lpPhy->lpTexName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPhy->lpTexName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	// bounding box
	fwrite ( &lpPhy->bboxMin, sizeof ( D3DXVECTOR3 ), 1, file );
	chunk.dwChunkSize += sizeof ( D3DXVECTOR3 );

	fwrite ( &lpPhy->bboxMax, sizeof ( D3DXVECTOR3 ), 1, file );
	chunk.dwChunkSize += sizeof ( D3DXVECTOR3 );

	fwrite ( &lpPhy->InitMatrix, sizeof ( D3DXMATRIXA16 ), 1, file );
	chunk.dwChunkSize += sizeof ( D3DXMATRIXA16 );

	// 用户自定义消息
	fwrite ( &lpPhy->dwTexRow, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( &lpPhy->Key.dwAlphas, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPhy->Key.lpAlphas, sizeof ( C3Frame ), lpPhy->Key.dwAlphas, file 
);
	chunk.dwChunkSize += sizeof ( C3Frame ) * lpPhy->Key.dwAlphas;

	fwrite ( &lpPhy->Key.dwDraws, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPhy->Key.lpDraws, sizeof ( C3Frame ), lpPhy->Key.dwDraws, file 
);
	chunk.dwChunkSize += sizeof ( C3Frame ) * lpPhy->Key.dwDraws;

	fwrite ( &lpPhy->Key.dwChangeTexs, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPhy->Key.lpChangeTexs, sizeof ( C3Frame ), 
lpPhy->Key.dwChangeTexs, file );
	chunk.dwChunkSize += sizeof ( C3Frame ) * lpPhy->Key.dwChangeTexs;


	char flag[4];
	// 12-20-2002 : "STEP"
	flag[0] = 'S';
	flag[1] = 'T';
	flag[2] = 'E';
	flag[3] = 'P';

	fwrite ( flag, sizeof ( char ), 4, file );
	chunk.dwChunkSize += sizeof ( char ) * 4;

	fwrite ( &lpPhy->uvstep.x, sizeof ( float ), 1, file );
	chunk.dwChunkSize += sizeof ( float );

	fwrite ( &lpPhy->uvstep.y, sizeof ( float ), 1, file );
	chunk.dwChunkSize += sizeof ( float );

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void Phy_Unload ( C3Phy **lpPhy )
{
	SafeDeleteEx ( ( *lpPhy )->lpName );
	SafeDeleteEx ( ( *lpPhy )->lpVB );
	SafeRelease ( ( *lpPhy )->vb );
	SafeDeleteEx ( ( *lpPhy )->lpIB );
	SafeRelease ( ( *lpPhy )->ib );
	SafeDeleteEx ( ( *lpPhy )->lpTexName );


	SafeDeleteEx ( ( *lpPhy )->Key.lpAlphas );
	SafeDeleteEx ( ( *lpPhy )->Key.lpDraws );
	SafeDeleteEx ( ( *lpPhy )->Key.lpChangeTexs );

	SafeDelete ( *lpPhy );
}
C3_CORE_DLL_API
void Phy_Prepare ( void )
{
	SetRenderState ( D3DRS_ZENABLE, true );
	SetRenderState ( D3DRS_ZWRITEENABLE, true );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	SetRenderState ( D3DRS_DITHERENABLE, true );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_CW );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR2 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR2 );

	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	//SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	SetTextureStageState ( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	//SetTextureStageState ( 1, D3DTSS_MINFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MAGFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE );

	g_D3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}
C3_CORE_DLL_API
BOOL Phy_Calculate ( C3Phy *lpPhy )
{
	float alpha;
	if ( Key_ProcessAlpha ( &lpPhy->Key, lpPhy->lpMotion->nFrame, 
lpPhy->lpMotion->dwFrames, &alpha ) )
		lpPhy->fA = alpha;

	BOOL draw;
	if ( Key_ProcessDraw ( &lpPhy->Key, lpPhy->lpMotion->nFrame, &draw ) )
		lpPhy->bDraw = draw;

	int tex = -1;
	Key_ProcessChangeTex ( &lpPhy->Key, lpPhy->lpMotion->nFrame, &tex );

	if ( !lpPhy->bDraw )
		return true;


	PhyOutVertex *vertex;
	if ( FAILED ( lpPhy->vb->Lock ( 0,
									0,
									( void** )&vertex,
									D3DLOCK_NOSYSLOCK ) ) )
		return false;

	D3DXMATRIX *bone = new D3DXMATRIX[lpPhy->lpMotion->dwBoneCount];
	for ( DWORD b = 0; b < lpPhy->lpMotion->dwBoneCount; b++ )
	{
		D3DXMATRIXA16 mm;
		Motion_GetMatrix ( lpPhy->lpMotion, b, &mm );
/*
		D3DXMatrixMultiply ( &bone[b],
							 &lpPhy->InitMatrix, 
							 &lpPhy->lpMotion->lpFrames[b * lpPhy->lpMotion->dwFrames + 
							 lpPhy->lpMotion->nFrame] );
*/
		D3DXMatrixMultiply ( &bone[b],
							 &lpPhy->InitMatrix, 
							 &mm );

		//bone[b] = lpPhy->InitMatrix;

		D3DXMatrixMultiply ( &bone[b],
							 &bone[b],
							 &lpPhy->lpMotion->matrix[b] );
	}

	// 计算皮肤
	for ( DWORD v = 0; v < lpPhy->dwNVecCount + lpPhy->dwAVecCount; v++ )
	{
		D3DXVECTOR3 mix = lpPhy->lpVB[v].pos[0];
		// 计算 MORPH
/*		BOOL zero[3] = { true, true, true };
		for ( DWORD m = 0; m < lpPhy->lpMotion->dwMorphCount; m++ )
		{
			// x
			if ( FloatCmp ( lpPhy->lpVB[v].pos[0].x, lpPhy->lpVB[v].pos[m + 1].x ) != 
0 )
			{
				mix.x += ( lpPhy->lpVB[v].pos[m + 1].x - lpPhy->lpVB[v].pos[0].x ) * 
lpPhy->lpMotion->lpMorph[lpPhy->lpMotion->dwMorphCount * 
lpPhy->lpMotion->nFrame + m];
			}
			// y
			if ( FloatCmp ( lpPhy->lpVB[v].pos[0].y, lpPhy->lpVB[v].pos[m + 1].y ) != 
0 )
			{
				mix.y += ( lpPhy->lpVB[v].pos[m + 1].y - lpPhy->lpVB[v].pos[0].y ) * 
lpPhy->lpMotion->lpMorph[lpPhy->lpMotion->dwMorphCount * 
lpPhy->lpMotion->nFrame + m];
			}
			// z
			if ( FloatCmp ( lpPhy->lpVB[v].pos[0].z, lpPhy->lpVB[v].pos[m + 1].z ) != 
0 )
			{
				mix.z += ( lpPhy->lpVB[v].pos[m + 1].z - lpPhy->lpVB[v].pos[0].z ) * 
lpPhy->lpMotion->lpMorph[lpPhy->lpMotion->dwMorphCount * 
lpPhy->lpMotion->nFrame + m];
			}
		}

*/


		// 计算骨骼
		D3DXVECTOR3 vec;

		vertex[v].x = 0;
		vertex[v].y = 0;
		vertex[v].z = 0;

		for ( int l = 0; l < 2; l++ )
		{
			DWORD index = lpPhy->lpVB[v].index[l];
			float weight = lpPhy->lpVB[v].weight[l];

			if ( lpPhy->lpVB[v].weight[l] > 0 )
			{

/*				D3DXVec3TransformCoord ( &vec,
										 &mix,
										 &lpPhy->InitMatrix );
				D3DXVec3TransformCoord ( &vec,
										 &vec,
										 &lpPhy->lpMotion->lpFrames[index * lpPhy->lpMotion->dwFrames + 
										 lpPhy->lpMotion->nFrame] );
				D3DXVec3TransformCoord ( &vec,
										 &vec,
										 &lpPhy->lpMotion->matrix[index] );*/

				D3DXVec3TransformCoord ( &vec,
										 &mix,
										 &bone[index] );

				//D3DXVec3Scale ( &vec, &vec, weight );
				D3DXVec3Add ( ( D3DXVECTOR3* )&vertex[v],
							  ( D3DXVECTOR3* )&vertex[v],
							  &vec );
				break;
			}
		}

		//接口内部只改变改变x, y
//		D3DXVec2Add ( ( D3DXVECTOR2* )&vertex[v],
//					  ( D3DXVECTOR2* )&vertex[v],
//					  &lpPhy->uvstep );
		//要改变u, v
		vertex[v].u = vertex[v].u + lpPhy->uvstep.x;
		vertex[v].v = vertex[v].v + lpPhy->uvstep.y;

		if ( tex > -1 )
		{
			float segsize = 1.0f / lpPhy->dwTexRow;
			vertex[v].u = lpPhy->lpVB[v].u + tex % lpPhy->dwTexRow * segsize;
			vertex[v].v = lpPhy->lpVB[v].v + tex / lpPhy->dwTexRow * segsize;
		}
	}
	if ( FAILED ( lpPhy->vb->Unlock () ) )
		return false;

	delete [] bone;

	return true;
}
C3_CORE_DLL_API
BOOL Phy_DrawNormal ( C3Phy *lpPhy )
{
	if ( !lpPhy->bDraw )
		return true;

	if ( lpPhy->dwNTriCount > 0 && lpPhy->fA == 1.0f )
	{
		SetRenderState ( D3DRS_ALPHABLENDENABLE, false );
		SetRenderState ( D3DRS_ALPHATESTENABLE, false );
		SetRenderState ( D3DRS_ZWRITEENABLE, true );

		// material
		D3DMATERIAL9 material;
		ZeroMemory ( &material, sizeof ( material ) );

		material.Diffuse.a = lpPhy->fA;
		material.Diffuse.r = lpPhy->fR;
		material.Diffuse.g = lpPhy->fG;
		material.Diffuse.b = lpPhy->fB;

		material.Ambient.a = lpPhy->fA;
		material.Ambient.r = lpPhy->fR;
		material.Ambient.g = lpPhy->fG;
		material.Ambient.b = lpPhy->fB;

		if ( FAILED ( g_D3DDevice->SetMaterial ( &material ) ) )
			return false;


		// texture
		if ( lpPhy->nTex == -1 )
		{
			if ( FAILED ( g_D3DDevice->SetTexture ( 0, 0 ) ) )
				return false;
		}
		else
		{
			if ( !g_lpTex[lpPhy->nTex] )
				return false;

			if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[lpPhy->nTex]->lpTex ) ) )
				return false;
		}

		D3DXMATRIXA16 matrix;
		D3DXMatrixIdentity ( &matrix );
		g_D3DDevice->SetTransform ( D3DTS_WORLD, &matrix );
		//if ( FAILED ( g_D3DDevice->SetVertexShader ( PHY_OUT_VERTEX ) ) )
		if ( FAILED (	g_D3DDevice->SetFVF(PHY_OUT_VERTEX)))
			return false;

		if ( FAILED ( g_D3DDevice->SetStreamSource ( 0, lpPhy->vb, 0,sizeof ( 
PhyOutVertex ) ) ) )
			return false;
		if ( FAILED ( g_D3DDevice->SetIndices ( lpPhy->ib) ) )
			return false;

		// draw normal
		if ( FAILED ( g_D3DDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST,
														  0,//dx9
														  0,
														  lpPhy->dwNVecCount,
														  0,
														  lpPhy->dwNTriCount ) ) )
			return false;
	}
	return true;
}
C3_CORE_DLL_API
BOOL Phy_DrawAlpha ( C3Phy *lpPhy, BOOL bZ, int nAsb, int nAdb )
{
	if ( !lpPhy->bDraw )
		return true;

	if ( ( lpPhy->dwNTriCount > 0 && lpPhy->fA != 1.0f ) ||
		 ( lpPhy->dwATriCount > 0 ) )
	{
		SetRenderState ( D3DRS_ZWRITEENABLE, bZ );

		SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
		SetRenderState ( D3DRS_SRCBLEND, nAsb );
		SetRenderState ( D3DRS_DESTBLEND, nAdb );

		SetRenderState ( D3DRS_ALPHATESTENABLE, true );
		SetRenderState ( D3DRS_ALPHAREF, 0x08 );
		SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

		// material
		D3DMATERIAL9 material;
		ZeroMemory ( &material, sizeof ( material ) );

		material.Diffuse.a = lpPhy->fA;
		material.Diffuse.r = lpPhy->fR;
		material.Diffuse.g = lpPhy->fG;
		material.Diffuse.b = lpPhy->fB;

		material.Ambient.a = lpPhy->fA;
		material.Ambient.r = lpPhy->fR;
		material.Ambient.g = lpPhy->fG;
		material.Ambient.b = lpPhy->fB;

		if ( FAILED ( g_D3DDevice->SetMaterial ( &material ) ) )
			return false;

		// texture
		if ( lpPhy->nTex == -1 )
		{
			if ( FAILED ( g_D3DDevice->SetTexture ( 0, 0 ) ) )
				return false;
		}
		else
		{
			if ( !g_lpTex[lpPhy->nTex])
				return false;

			if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[lpPhy->nTex]->lpTex ) 
) )
				return false;
		}

		D3DXMATRIXA16 matrix;
		D3DXMatrixIdentity ( &matrix );
		g_D3DDevice->SetTransform ( D3DTS_WORLD, &matrix );
//		if ( FAILED ( g_D3DDevice->SetVertexShader ( PHY_OUT_VERTEX ) ) )
		if ( FAILED (	g_D3DDevice->SetFVF(PHY_OUT_VERTEX)))
			return false;

		if ( FAILED ( g_D3DDevice->SetStreamSource ( 0, lpPhy->vb,0, sizeof ( 
PhyOutVertex ) ) ) )
			return false;
		if ( FAILED ( g_D3DDevice->SetIndices ( lpPhy->ib ) ) )
			return false;
	}
	if ( lpPhy->dwNTriCount > 0 && lpPhy->fA != 1.0f )
	{
		// draw normal
		if ( FAILED ( g_D3DDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST,
														 0,//dx9
														  0,
														  lpPhy->dwNVecCount,
														  0,
														  lpPhy->dwNTriCount ) ) )
			return false;
	}
	if ( lpPhy->dwATriCount > 0 )
	{
		// draw alpha
		if ( FAILED ( g_D3DDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST,
														 0, //dx9
														  lpPhy->dwNVecCount,
														  lpPhy->dwAVecCount,
														  lpPhy->dwNTriCount * 3,
														  lpPhy->dwATriCount ) ) )
			return false;
	}
	return true;
}
C3_CORE_DLL_API
void Phy_NextFrame ( C3Phy *lpPhy, int nStep )
{
	lpPhy->lpMotion->nFrame = ( lpPhy->lpMotion->nFrame + nStep ) % 
lpPhy->lpMotion->dwFrames;
}

C3_CORE_DLL_API
void Phy_SetFrame ( C3Phy *lpPhy, DWORD dwFrame )
{
	if(lpPhy->lpMotion->dwFrames == 0)
		lpPhy->lpMotion->nFrame = 0;
	else
		lpPhy->lpMotion->nFrame = dwFrame % lpPhy->lpMotion->dwFrames;
}

C3_CORE_DLL_API
void Phy_Muliply ( C3Phy *lpPhy, int nBoneIndex, D3DXMATRIXA16 *matrix )
{
	int start, end;
	if ( nBoneIndex == -1 )
	{
		start = 0;
		end = lpPhy->lpMotion->dwBoneCount;
	}
	else
	{
		start = nBoneIndex;
		end = start + 1;
	}
	for ( int n = start; n < end; n++ )
	{
		D3DXMatrixMultiply ( &lpPhy->lpMotion->matrix[n],
							 &lpPhy->lpMotion->matrix[n],
							 matrix );
	}
}
C3_CORE_DLL_API
void Phy_SetColor ( C3Phy *lpPhy,
					float alpha,
					float red,
					float green,
					float blue )
{
	lpPhy->fA = alpha;
	lpPhy->fR = red;
	lpPhy->fG = green;
	lpPhy->fB = blue;
}

C3_CORE_DLL_API
void Phy_ClearMatrix ( C3Phy *lpPhy )
{
	// 清除运动矩阵
	for ( DWORD n = 0; n < lpPhy->lpMotion->dwBoneCount; n ++ )
		D3DXMatrixIdentity ( &lpPhy->lpMotion->matrix[n] );
}

C3_CORE_DLL_API
void Phy_ChangeTexture ( C3Phy *lpPhy, int nTexID, int nTexID2)
{
	lpPhy->nTex = nTexID;
	lpPhy->nTex2 = nTexID2;
}

/*C3_CORE_DLL_API
void Phy_LookAt ( C3Phy *lpPhy,
				  int nAxis,
				  D3DXMATRIX *lpRet )
{
	D3DXVECTOR3 camera = g_From - g_To;
	D3DXVECTOR3 axis;

	switch ( nAxis )
	{
	case 0: // x
		camera.x = 0;

		axis.x = 0;
		axis.y = 0;
		axis.z = -1;
		break;
	case 1: // y
		camera.y = 0;

		axis.x = -1;
		axis.y = 0;
		axis.z = 0;
		break;
	case 2: // z
		camera.z = 0;

		axis.x = 0;
		axis.y = -1;
		axis.z = 0;
		break;
	};

	D3DXVec3Normalize ( &camera, &camera );
	D3DXVec3Normalize ( &axis, &axis );

	float radian = ( float )acosf ( D3DXVec3Dot ( &axis, &camera ) );

	switch ( nAxis )
	{
	case 0: // x
		if ( camera.y < 0 )
			radian = D3DX_PI * 2 - radian;
		D3DXMatrixRotationX ( lpRet, radian );
		break;
	case 1: // y
		if ( camera.z < 0 )
			radian = D3DX_PI * 2 - radian;
		D3DXMatrixRotationY ( lpRet, radian );
		break;
	case 2: // z
		if ( camera.x < 0 )
			radian = D3DX_PI * 2 - radian;
		D3DXMatrixRotationZ ( lpRet, radian );
		break;
	};
}*/


