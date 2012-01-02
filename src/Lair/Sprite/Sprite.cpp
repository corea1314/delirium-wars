#include "Sprite.h"

#include "Lair/Lair.h"

void SpriteMan::Init( unsigned long in_nReservedSpriteCount )
{
	m_nMaxSpriteCount = in_nReservedSpriteCount;
	m_pCurrSpriteData = m_pSpriteDataBuffer = new SpriteData[m_nMaxSpriteCount];
	memset( m_pSpriteDataBuffer, 0, m_nMaxSpriteCount*sizeof(SpriteData) );
	for( unsigned int k=0; k<m_nMaxSpriteCount; k++ )
		m_pSpriteDataBuffer[k].color = 0xFFFFFFFF;

	m_pShader = new ShaderGLSL;
	m_pShader->Create( "shaders/spriter", GL_POINTS );
	
	glGenBuffers( 1, &m_nVBO );

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBufferData(GL_ARRAY_BUFFER, m_nMaxSpriteCount*sizeof(SpriteData), &m_pSpriteDataBuffer[0].pos.x, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

//	Test();
}

void SpriteMan::Exit()
{
	glDeleteBuffers( 1, &m_nVBO );

	SAFE_DELETE(m_pShader);
}

Sprite* SpriteMan::GetSprite()
{
	Sprite* pSprite = new Sprite(m_pCurrSpriteData);
	
	++m_pCurrSpriteData;

	return pSprite;
}

void SpriteMan::Render()
{
	// bind shader
	m_pShader->Bind();

	// bind texture (from to be atlas)
	Lair::GetAtlasMan()->Bind();
		
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

	/*
	Vector2 pos;		// 0
	float	depth;		// 8
	Vector2 size;		// 12
	Vector2 offset;		// 20
	float	angle;		// 28
	Vector2 uv_min;		// 32
	Vector2 uv_max;		// 40
	unsigned int color;	// 48
	*/

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

	// draw
	glDrawArrays( GL_POINTS, 0, GetCurrSpriteCount() );

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
	glVertexPointer(3, GL_FLOAT, sizeof(SpriteData), &m_pSpriteDataBuffer[0].pos.x );			// pos, depth, angle
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteData), &m_pSpriteDataBuffer[0].color );	// color

	glClientActiveTexture( GL_TEXTURE0 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_pSpriteDataBuffer[0].size.x );	// size

	glClientActiveTexture( GL_TEXTURE1 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(3, GL_FLOAT, sizeof(SpriteData), &m_pSpriteDataBuffer[0].offset.x );	// offset, angle

	glClientActiveTexture( GL_TEXTURE2 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_pSpriteDataBuffer[0].uv_min.x );	// texture coordinates min

	glClientActiveTexture( GL_TEXTURE3 );
	//	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_pSpriteDataBuffer[0].uv_max.x );	// texture coordinates max

	//	glPointSize( 32.0f );
	// draw
	glDrawArrays( GL_POINTS, 0, GetCurrSpriteCount() );

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


// ============================================================================

Sprite::Sprite(SpriteMan::SpriteData* in_pSD) : m_pSD(in_pSD), m_pCurrSequence(0), m_pCurrFrame(0), m_bIsPlaying(false), m_bIsLooping(false), m_fAnimTime(0)
{
	Set( 0.0f, 0.0f );

	m_pColor = (unsigned char*)&(m_pSD->color);
}

Sprite::~Sprite()
{

}

void Sprite::Set( float x, float y, float a, float sx, float sy )
{
	m_vScale.x = sx;
	m_vScale.y = sy;
	m_fAngle = a;
	m_vPos.x = x;
	m_vPos.y = y;

	if( m_pCurrFrame )
	{
		UpdateFromFrame();
	}
}

void Sprite::SetAlpha( float a )
{
	m_pColor[3] = (unsigned char)(a * 255);
}

void Sprite::SetColor( float r, float g, float b )
{
	m_pColor[0] = (unsigned char)(r * 255);
	m_pColor[1] = (unsigned char)(g * 255);
	m_pColor[2] = (unsigned char)(b * 255);
}

void Sprite::Play( const std::string& in_szSequenceName, bool in_bLoop )
{
	m_pCurrSequence = Lair::GetSequenceMan()->Get( in_szSequenceName );

	if( m_pCurrSequence )
	{
		m_fAnimTime = 0.0f;
		m_bIsPlaying = true;
		m_bIsLooping = in_bLoop;
	}
}

void Sprite::Update( float in_fDeltaTime )
{
	if( m_pCurrSequence && m_bIsPlaying )
	{
		m_fAnimTime += in_fDeltaTime;

		float fDuration = m_pCurrSequence->GetDuration()/1000.0f;	// duration in secs

		if( m_fAnimTime > fDuration )
		{
			if( m_bIsLooping )
				m_fAnimTime = fmodf( m_fAnimTime, fDuration );
			else
			{
				m_fAnimTime = fDuration;
				m_bIsPlaying = false;
			}
		}
		m_pCurrFrame = m_pCurrSequence->GetFrame( (unsigned long)(m_fAnimTime*1000) );
		UpdateFromFrame();
	}
}

bool Sprite::IsPlaying() const
{
	return m_bIsPlaying;
}

void Sprite::SetFrame( AtlasFrame* in_pFrame )
{
	m_pSD->pos = m_vPos;
	m_pSD->offset.x = in_pFrame->GetOffset().x;
	m_pSD->offset.y = in_pFrame->GetOffset().y;
	m_pSD->angle = m_fAngle;
	m_pSD->size.x = in_pFrame->GetSize().x/2;
	m_pSD->size.y = in_pFrame->GetSize().y/2;
	m_pSD->uv_min = in_pFrame->GetMinUV();
	m_pSD->uv_max = in_pFrame->GetMaxUV();
}

void Sprite::UpdateFromFrame()
{
	float fGlobalScaleX = m_vScale.x * m_pCurrFrame->scale.x;
	float fGlobalScaleY = m_vScale.y * m_pCurrFrame->scale.y;

	m_pSD->pos = m_vPos;
	m_pSD->offset.x = m_pCurrFrame->offset.x * fGlobalScaleX;
	m_pSD->offset.y = m_pCurrFrame->offset.y * fGlobalScaleY;
	m_pSD->angle = m_pCurrFrame->angle + m_fAngle;
	m_pSD->size.x = m_pCurrFrame->frame->GetSize().x/2 * fGlobalScaleX;
	m_pSD->size.y = m_pCurrFrame->frame->GetSize().y/2 * fGlobalScaleY;
	m_pSD->uv_min = m_pCurrFrame->frame->GetMinUV();
	m_pSD->uv_max = m_pCurrFrame->frame->GetMaxUV();
}



void SpriteMan::Test()
{
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
 	for( int i=-8;i<8; i++ )
 	for( int j=-8;j<8; j++ )
 	{
 		SpriteData spr[] = { { Vector2( i*64.0f, j*64.0f), 0.0f, Vector2(32.0f,32.0f), Vector2(0.0f,0.0f),  0.0f, Vector2(0.0f,0.0f), Vector2(1.0f,1.0f), 0xFFFFFFFF }};
 		*m_pCurrSpriteData = spr[0];
 		++m_pCurrSpriteData;
 	}

	// ~4 million
//	for( int i=-1024;i<1024; i++ )
//	for( int j=-1024;j<1024; j++ )
//	{
//		SpriteData spr[] = { { Vector2( i*64, j*64), 0, Vector2(32,32), Vector2(0,0),  rand()%6, Vector2(0,0), Vector2(1,1), 0xFFFFFFFF }};
//		m_vecSpriteDataBuffer.push_back( spr[0] );
//	}
	
	// Atlas
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Climb.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Climb.1-3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Climb.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/ClimbFire.pistol.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/ClimbFire.pistol.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Crouch.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Crouch.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Crouch.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Crouch.3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Death.knockback.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Death.lying.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Idle.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Idle.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/IdleFire.pistol.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/IdleFire.pistol.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/IdleFire.rifle.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/IdleFire.sub.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Jetpack.Ascend.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Jetpack.Fall 0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Jetpack.Fall 1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackFire.pistol.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackFire.pistol.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackThrow.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackThrow.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackThrow.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/JetpackThrow.3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.4.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.5.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Run.6.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Throw.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Throw.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Throw.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Throw.3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.0.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.1.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.2.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.3.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.4.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.5.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.6.png") );
	m_vecAtlasFrame.push_back( Lair::GetAtlasMan()->Get( "atlas/Walk.7.png") );

	// Apply index to sprite data
	for(unsigned int i=0;i<GetCurrSpriteCount(); i++)
	{
		AtlasFrame* pIndex = m_vecAtlasFrame[i%m_vecAtlasFrame.size()];
		
		m_pSpriteDataBuffer[i].size = pIndex->GetSize();

		m_pSpriteDataBuffer[i].uv_min = pIndex->GetMinUV();
		m_pSpriteDataBuffer[i].uv_max = pIndex->GetMaxUV();		
	}

}