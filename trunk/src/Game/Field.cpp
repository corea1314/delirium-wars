#include "Field.h"
#include "Engine.h"

CField::CField() : vb_grid_size(0)
{
	vb_grid = new Vertex[FIELD_SIZE_X*FIELD_SIZE_Y];
	m_pCell = new Cell[FIELD_SIZE_X*FIELD_SIZE_Y];
}


void CField::Update( float in_fDeltaTime )
{
// todo
}

void CField::Render()
{
	if( vb_grid_size )
		gl_RenderPoints( vb_grid, 0, vb_grid_size, 2 );
}

void CField::LoadFile( const char* in_szFilename )
{
	FILE* pFile = 0;
	fopen_s( &pFile, in_szFilename, "rb" );

	unsigned int* pBuffer = new unsigned int[FIELD_SIZE_X*FIELD_SIZE_Y];

	if( pFile )
	{
		assert( fread( pBuffer, sizeof(unsigned int), FIELD_SIZE_X*FIELD_SIZE_Y, pFile ) == FIELD_SIZE_X*FIELD_SIZE_Y );
		fclose(pFile);

		unsigned char* pCell = (unsigned char*)pBuffer;

//		for ( int y=0;y<FIELD_SIZE_Y; y++ )
		for ( int y=FIELD_SIZE_Y-1;y>=0; y-- )
		{
			for ( int x=0;x<FIELD_SIZE_X; x++ )
			{
				m_pCell[y*FIELD_SIZE_X+x].nDensity = *(unsigned int*)pCell;
//				m_pCell[y*FIELD_SIZE_X+x].nDensity |= 0xff;
				pCell += 4;
			}
		}

		RebuildGrid();
	}

	delete [] pBuffer;
}

void CField::RebuildGrid()
{
	Vertex* p = vb_grid;
	
	int x,y;
	for( y=0; y<FIELD_SIZE_Y; y++ )
	{
		for( x=0; x<FIELD_SIZE_X; x++ )
		{
			p->x = 2.0f * (x - FIELD_SIZE_X/2);
			p->y = 2.0f * (y - FIELD_SIZE_Y/2);
			p->z = 0.0f;
			p->color = (m_pCell[y*FIELD_SIZE_X+x].nDensity | 0xff000000);
			++p;
		}
	}

	vb_grid_size = FIELD_SIZE_X * FIELD_SIZE_Y;

}

void CField::Connect( CEngine* in_pEngine )
{
	in_pEngine->Connect_Update( this, &CField::Update );
	in_pEngine->Connect_Render( this, &CField::Render );
	in_pEngine->Connect_LoadFile( this, &CField::LoadFile );
}

void CField::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_Update( this );
	in_pEngine->Disconnect_Render( this );
	in_pEngine->Disconnect_LoadFile( this );
}