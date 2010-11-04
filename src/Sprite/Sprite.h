#ifndef _SPRITE_H
#define _SPRITE_H

class Sprite
{
private:
	class Transform
	{
	public:
		// perform in those order
		Vector2		vScale;	
		float		fRotate;
		Vector2		vTranslate;		
	};

	class Frame
	{
	public:
		void Cache( std::string in_szFilename );
		void Uncache();
		
		// all of those are in pixel space
		Vector2 GetSize() const		{ return m_vSize;	}
		Vector2 GetOffset() const	{ return m_vOffset;	}
		float	GetAngle() const	{ return m_fAngle;	}

		void SetSize( Vector2& in_vSize ) const { return m_vSize; }
		void SetOffset( Vector2&) const { return m_vOffset;	}
		void SetAngle( float in_fAngle ) const { return m_fAngle; }

		unsigned long GetDuration() const {}	// returns duration of frame
		
	private:
		void RebuildVB();	//if internal params of a frame change, we need to rebuild the reference VB

	private:
		Texture*	m_pTexture;
		
		//todo: add reference VB
	};

	class Sequence
	{
	public:
		void Cache( std::string in_szFilename );
		void Uncache();
		
		Frame* GetFrame();

	private:
		std::vector< Frame* >	m_vecFrame;
	};

	std::map< std::string, Sequence* >	m_mapSequence;		//todo: move to SequenceMan and have sequence shared among everybody

	Sequence* m_pCurrSequence;
	bool	m_bIsPlaying;

public:
	void Play( std::string in_szSequenceName, bool in_bLoop=false );
	void Update();

	bool IsPlaying() const;

	//todo: connect to rendering signal
};


/*
Sequence File Spec

<string:image> <int:duration(milliseconds)> <number:scale_X> <number:scale_y> <number:angle(degrees)> <number:offset_x(pixel)> <number:offset_y(pixel)>
...


*/


#endif//_SPRITE_H