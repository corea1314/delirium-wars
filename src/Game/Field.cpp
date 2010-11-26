#include "Field.h"
#include "Engine.h"


CField::CField()
:vb_grid_size(0),
m_v2CellWorldSize(Vector2(0.0f,0.0f)),
m_v2Position(Vector2(0.0f,0.0f)),
m_v2InverseCellWorldSize(Vector2(0.0f,0.0f)),
m_v2GridWorldSize(Vector2(0.0f,0.0f))
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
	in_pEngine->Connect_OnUpdate( this, &CField::Update );
	in_pEngine->Connect_OnRenderBackLayer( this, &CField::Render );
	in_pEngine->Connect_OnLoadFile( this, &CField::LoadFile );
}

void CField::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );
	in_pEngine->Disconnect_OnRenderBackLayer( this );
	in_pEngine->Disconnect_OnLoadFile( this );
}

bool CField::WorldPositionToIndex( const Vector2 & in_v2WorldPosition, unsigned int & out_IndexX, unsigned int & out_IndexY) const
{
    bool bIsInBound(true);
    if( in_v2WorldPosition.x >= m_v2Position.x && in_v2WorldPosition.x < ( m_v2Position.x + m_v2CellWorldSize.x * FIELD_SIZE_X) && in_v2WorldPosition.y >= m_v2Position.y && in_v2WorldPosition.y < (m_v2Position.y + m_v2CellWorldSize.y * FIELD_SIZE_Y) )
    {
        out_IndexX = static_cast<unsigned int>(in_v2WorldPosition.x - m_v2Position.x * m_v2InverseCellWorldSize.x);
        out_IndexY = static_cast<unsigned int>(in_v2WorldPosition.y - m_v2Position.y * m_v2InverseCellWorldSize.y);
    }else
    {
        bIsInBound = false;
        out_IndexX = 0;
        out_IndexY = 0;
    }
    return bIsInBound;
}


bool CField::IndexToWorldPosition( unsigned int in_IndexX, unsigned int in_IndexY, Vector2& out_v2WorldPosition) const
{
    bool bIsInBound(true);
    if( in_IndexX >= 0 && in_IndexX < FIELD_SIZE_X && in_IndexY >= 0 && in_IndexY < FIELD_SIZE_Y)
    {
        out_v2WorldPosition.x = m_v2Position.x + m_v2CellWorldSize.x * in_IndexX;
        out_v2WorldPosition.y = m_v2Position.y + m_v2CellWorldSize.y * in_IndexY;

    }else
    {
        bIsInBound = false;
        out_v2WorldPosition.x = 0.0f;
        out_v2WorldPosition.y = 0.0f;
    }
    return bIsInBound;
}

void CField::SetCellWorldSize( const Vector2 & val )
{
    m_v2CellWorldSize = val;
    m_v2InverseCellWorldSize.x = 1.0f/val.x;
    m_v2InverseCellWorldSize.y = 1.0f/val.y;
    m_v2GridWorldSize.x = m_v2CellWorldSize.x * FIELD_SIZE_X;
    m_v2GridWorldSize.y = m_v2CellWorldSize.y * FIELD_SIZE_Y;
}

bool CField::WorldPositionToIndexHorizontal( float in_fXWorldPosition, unsigned int & out_IndexX ) const
{
    bool bIsInBound(true);
    if( in_fXWorldPosition >= m_v2Position.x && in_fXWorldPosition < ( m_v2Position.x + m_v2CellWorldSize.x * FIELD_SIZE_X) )
    {
        out_IndexX = static_cast<unsigned int>(in_fXWorldPosition - m_v2Position.x * m_v2InverseCellWorldSize.x);
    }else
    {
        bIsInBound = false;
        out_IndexX = 0;
    }
    return bIsInBound;
}

bool CField::WorldPositionToIndexVertical( float in_fYWorldPosition, unsigned int & out_IndexY ) const
{
    bool bIsInBound(true);
    if( in_fYWorldPosition >= m_v2Position.y && in_fYWorldPosition < ( m_v2Position.y + m_v2CellWorldSize.y * FIELD_SIZE_Y) )
    {
        out_IndexY = static_cast<unsigned int>(in_fYWorldPosition - m_v2Position.y * m_v2InverseCellWorldSize.y);
    }else
    {
        bIsInBound = false;
        out_IndexY = 0;
    }
    return bIsInBound;
}

bool CField::IndexToWorldPositionHorizontal( unsigned int in_IndexX, float& out_fXWorldPosition ) const
{
    bool bIsInBound(true);
    if( in_IndexX >= 0 && in_IndexX < FIELD_SIZE_X)
    {
        out_fXWorldPosition = m_v2Position.x + m_v2CellWorldSize.x * in_IndexX;

    }else
    {
        bIsInBound = false;
        out_fXWorldPosition = 0.0f;
    }
    return bIsInBound;
}

bool CField::IndexToWorldPositionVertical( unsigned int in_IndexY, float& out_fYWorldPosition ) const
{
    bool bIsInBound(true);
    if( in_IndexY >= 0 && in_IndexY < FIELD_SIZE_Y)
    {
        out_fYWorldPosition = m_v2Position.x + m_v2CellWorldSize.x * in_IndexY;

    }else
    {
        bIsInBound = false;
        out_fYWorldPosition = 0.0f;
    }
    return bIsInBound;
}
