#include "Sprite2.h"

#include "../Lair.h"

void SpriteMan::Init( unsigned long in_nReservedSpriteCount )
{
	m_vecSpriteDataBuffer.reserve( in_nReservedSpriteCount );

	m_pShader = new ShaderGLSL;
	m_pShader->Create( "shaders/spriter", GL_POINTS );

	// test
	SpriteData spr[] = 
	{
		// Vector2 pos, float depth, Vector2 size, Vector2 offset, float angle, Vector2 uv_min, Vector2 uv_max
		{ Vector2( 0, 0), 0, Vector2(32,32), Vector2(0,0), (float) PI/2, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF },
		{ Vector2(64, 0), 0, Vector2(32,32), Vector2(0,0), (float)-PI/3, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF },
		{ Vector2( 0,64), 0, Vector2(32,32), Vector2(0,0), (float) PI/8, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF },
		{ Vector2(64,64), 0, Vector2(32,32), Vector2(0,0), (float)-PI/7, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF },
	};

//	m_vecSpriteDataBuffer.push_back( spr[0] );

	// 16k
	for( int i=-64;i<64; i++ )
	for( int j=-64;j<64; j++ )
	{
		SpriteData spr[] = { { Vector2( i*64, j*64), 0, Vector2(32,32), Vector2(0,0),  0.0f/*rand()%6*/, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF }};
		m_vecSpriteDataBuffer.push_back( spr[0] );
	}

	// ~4 million
	/*
	for( int i=-1024;i<1024; i++ )
	for( int j=-1024;j<1024; j++ )
	{
		SpriteData spr[] = { { Vector2( i*64, j*64), 0, Vector2(32,32), Vector2(0,0),  rand()%6, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF }};
		m_vecSpriteDataBuffer.push_back( spr[0] );
	}
	*/
	
	glGenBuffers( 1, &m_nVBO );

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vecSpriteDataBuffer.size()*sizeof(SpriteData), &m_vecSpriteDataBuffer[0].pos.x, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	// Atlas
	m_pAtlas = new Atlas(1024,1024);

	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Climb.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Climb.1-3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Climb.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/ClimbFire.pistol.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/ClimbFire.pistol.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Crouch.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Crouch.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Crouch.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Crouch.3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Death.knockback.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Death.lying.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Idle.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Idle.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/IdleFire.pistol.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/IdleFire.pistol.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/IdleFire.rifle.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/IdleFire.sub.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Jetpack.Ascend.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Jetpack.Fall 0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Jetpack.Fall 1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackFire.pistol.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackFire.pistol.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackThrow.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackThrow.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackThrow.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/JetpackThrow.3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.4.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.5.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Run.6.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Throw.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Throw.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Throw.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Throw.3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.0.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.1.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.2.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.3.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.4.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.5.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.6.png") );
	m_vecAtlasIndex.push_back( m_pAtlas->Get( "atlas/Walk.7.png") );

	// Apply index to sprite data
	for(unsigned int i=0;i<m_vecSpriteDataBuffer.size(); i++)
	{
		Atlas::Index* pIndex = m_vecAtlasIndex[i%m_vecAtlasIndex.size()];
		m_vecSpriteDataBuffer[i].size = pIndex->GetSize();
		m_vecSpriteDataBuffer[i].uv_min = pIndex->GetMinUV();
		m_vecSpriteDataBuffer[i].uv_max = pIndex->GetMaxUV();		
	}

	m_pAtlas->ReloadTexture();
}

void SpriteMan::Exit()
{
	glDeleteBuffers( 1, &m_nVBO );

	SAFE_DELETE(m_pShader);
	SAFE_DELETE(m_pAtlas);
}

void SpriteMan::Render()
{
	// bind shader
	m_pShader->Bind();
	
	// bind texture (from to be atlas)
	m_pAtlas->BindTexture(0);
		
	// bind attributes

	// render
//	RenderWithVBO();
	RenderWithVA();

	m_pShader->Unbind();

}

void SpriteMan::RenderWithVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);

	#define BUFFER_OFFSET(i) ((char*)NULL + (i))

	Vector2 pos;		// 0
	float	depth;		// 8
	Vector2 size;		// 12
	Vector2 offset;		// 20
	float	angle;		// 28
	Vector2 uv_min;		// 32
	Vector2 uv_max;		// 40
	unsigned int color;	// 48

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(3, GL_FLOAT, sizeof(SpriteData), BUFFER_OFFSET(0) );			// pos, depth
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteData), BUFFER_OFFSET(48) );	// color

	glClientActiveTexture( GL_TEXTURE0 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), BUFFER_OFFSET(12) );		// size

	glClientActiveTexture( GL_TEXTURE1 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(3, GL_FLOAT, sizeof(SpriteData), BUFFER_OFFSET(20) );	// offset, angle

	glClientActiveTexture( GL_TEXTURE2 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), BUFFER_OFFSET(32) );	// texture coordinates min

	glClientActiveTexture( GL_TEXTURE3 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), BUFFER_OFFSET(40) );	// texture coordinates max

	//	glPointSize( 32.0f );
	// draw
	glDrawArrays( GL_POINTS, 0, m_vecSpriteDataBuffer.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	//	glActiveTexture( GL_TEXTURE3 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE2 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE1 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteMan::RenderWithVA()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer(3, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].pos.x );			// pos, depth, angle
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].color );	// color

	glClientActiveTexture( GL_TEXTURE0 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].size.x );		// size

	glClientActiveTexture( GL_TEXTURE1 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(3, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].offset.x );	// offset, angle

	glClientActiveTexture( GL_TEXTURE2 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].uv_min.x );	// texture coordinates min

	glClientActiveTexture( GL_TEXTURE3 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].uv_max.x );	// texture coordinates max

	//	glPointSize( 32.0f );
	// draw
	glDrawArrays( GL_POINTS, 0, m_vecSpriteDataBuffer.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	//	glActiveTexture( GL_TEXTURE3 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE2 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE1 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}