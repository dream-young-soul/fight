#include "..\include\c3_omni.h"
#include "..\include\c3_main.h"

C3_CORE_DLL_API
void Omni_Clear ( C3Omni *lpOmni )
{
	lpOmni->lpName = 0;
	ZeroMemory ( &lpOmni->pos, sizeof ( D3DXVECTOR3	) );
	ZeroMemory ( &lpOmni->color, sizeof ( D3DXVECTOR3 ) );
	lpOmni->fRadius = 10.0f;
	lpOmni->fAttenuation = 1.0f;
}
C3_CORE_DLL_API
BOOL Omni_Load ( C3Omni **lpOmni,
				 char *lpName,
				 DWORD dwIndex )
{
	FILE *file = fopen ( lpName, "rb" );
	if ( !file )
		return false;

	char version[64];
	fread ( version, sizeof ( char ), 16, file );
	version[16] = '\0';
	if ( strcmp ( version, C3_VERSION ) != 0 )
	{
		MessageBox ( g_hWnd, "phy version error", "error", MB_OK );
		return false;
	}

	*lpOmni = new C3Omni;
	Omni_Clear ( *lpOmni );

	ChunkHeader chunk;
	DWORD add = 0;

	while ( 1 )
	{
		fread ( &chunk, sizeof ( ChunkHeader ), 1, file );
		if ( feof ( file ) )
		{
			fclose ( file );
			return false;
		}
		if ( chunk.byChunkID[0] == 'O' &&
			 chunk.byChunkID[1] == 'M' &&
			 chunk.byChunkID[2] == 'N' &&
			 chunk.byChunkID[3] == 'I' )
		{
			if ( add < dwIndex )
			{
				add++;
				fseek ( file, chunk.dwChunkSize, SEEK_CUR );
				continue;
			}

			// name
			DWORD temp;
			fread ( &temp, sizeof ( DWORD ), 1, file );
			( *lpOmni )->lpName = new char[temp + 1];
			fread ( ( *lpOmni )->lpName, 1, temp, file );
			( *lpOmni )->lpName[temp] = '\0';
			// pos
			fread ( &( *lpOmni )->pos, sizeof ( D3DXVECTOR3 ), 1, file );
			// color
			fread ( &( *lpOmni )->color, sizeof ( D3DXVECTOR3 ), 1, file );
			break;
		}
		else
			fseek ( file, chunk.dwChunkSize, SEEK_CUR );
	};
	fclose ( file );
	return true;
}

C3_CORE_DLL_API
BOOL Omni_Save ( char *lpName, C3Omni *lpOmni, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// omni
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'O';
	chunk.byChunkID[1] = 'M';
	chunk.byChunkID[2] = 'N';
	chunk.byChunkID[3] = 'I';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// name
	DWORD length = strlen ( lpOmni->lpName );
	fwrite ( &length, sizeof ( DWORD), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpOmni->lpName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	// pos
	fwrite ( &lpOmni->pos, sizeof ( D3DXVECTOR3 ), 1, file );
	chunk.dwChunkSize += sizeof ( D3DXVECTOR3 );

	// color
	fwrite ( &lpOmni->color, sizeof ( D3DXCOLOR ), 1, file );
	chunk.dwChunkSize += sizeof ( D3DXCOLOR );

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void Omni_Unload ( C3Omni **lpOmni )
{
	SafeDeleteEx ( ( *lpOmni )->lpName );
	SafeDelete ( *lpOmni );
}
