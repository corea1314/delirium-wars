#ifndef _ASM_H
#define _ASM_H

namespace ASM
{
	class CParameter
	{
	public:
		CParameter( int inValue ) : mType(Integer)  { mValue.v_int = inValue; }
		CParameter( bool inValue ) : mType(Bool)	{ mValue.v_bool = inValue; }
		CParameter( float inValue ) : mType(Float)  { mValue.v_float = inValue; }

		inline bool GetValue(int& outValue) const 
		{ 
			if(mType==Integer) 
			{
				outValue = mValue.v_int; 
				return true;
			}
			return false;
		}
		inline bool GetValue(bool& outValue) const
		{ 
			if(mType==Bool) 
			{
				outValue = mValue.v_bool; 
				return true;
			}
			return false;
		}
		inline bool GetValue(float& outValue) const
		{ 
			if(mType==Float) 
			{
				outValue = mValue.v_float; 
				return true;
			}
			return false;
		}

		inline bool SetValue(int inValue) 
		{ 
			if(mType==Integer) 
			{
				mValue.v_int = inValue; 
				return true;
			}
			return false;
		}
		inline bool SetValue(bool inValue)
		{ 
			if(mType==Bool) 
			{
				mValue.v_bool = inValue; 
				return true;
			}
			return false;
		}
		inline bool SetValue(float inValue)
		{ 
			if(mType==Float) 
			{
				mValue.v_float = inValue; 
				return true;
			}
			return false;
		}
		
		typedef enum DataType { Integer, Float, Bool } DataType;

		inline void* GetValue() const { return (void*)&mValue; }
		inline DataType GetType() const { return mType; }

	private:
		typedef union DataValue
		{
			int v_int;
			bool v_bool;
			float v_float;		

		} DataValue;
		
		const DataType	mType;
		DataValue	mValue;
	};
	typedef std::shared_ptr<CParameter> parameter_ptr;
	
	class CCondition
	{
	public:
		virtual bool Evaluate() = 0;

		enum Operator { NoOp, Equal, NotEqual, Less, Greater };

		static Operator StringToOperator( const char* OpName )
		{
				 if( _stricmp( OpName, "Equal" ) == 0 )		return Equal;
			else if( _stricmp( OpName, "NotEqual" ) == 0 )	return NotEqual;
			else if( _stricmp( OpName, "Less" ) == 0 )		return Less;
			else if( _stricmp( OpName, "Greater" ) == 0 )	return Greater;
			
			return NoOp;
		}
	};
	typedef std::shared_ptr<CCondition> condition_ptr;

	template <class T>
	class CConditionParameter : public CCondition
	{
	public:
		CConditionParameter( T* inpTarget, Operator inOperator, T inValue ) : mpParameter(inpTarget), mValue(inValue), mOperator(inOperator) {}
		virtual bool Evaluate() 
		{ 
			switch(mOperator)
			{
			case CCondition::Equal:		return *mpParameter == mValue; 
			case CCondition::NotEqual:	return *mpParameter != mValue; 
			case CCondition::Less:		return *mpParameter < mValue; 
			case CCondition::Greater:	return *mpParameter > mValue; 
			}		
			return false;
		}

	private:
		T *	mpParameter;
		T	mValue;
		Operator mOperator;
	};

	class CConditionSequenceEnd : public CCondition
	{
	public:
		CConditionSequenceEnd( Sprite* inSprite ) : mpSprite(inSprite) {}

		virtual bool Evaluate() 
		{
			return !mpSprite->IsPlaying();
		}
	private:
		Sprite* mpSprite;
	};
	
	class CTransition
	{
	public:
		std::vector< condition_ptr >	mConditions;	// conditions to check for transit to occur
		std::string mTransitStateName;					// state name where the transition leads to

		const std::string& GetTransitStateName() const { return mTransitStateName; }

		bool Evaluate()
		{
			bool ShouldTransit = true;
			std::for_each( mConditions.begin(), mConditions.end(), [&ShouldTransit]( condition_ptr Condition ) { ShouldTransit &= Condition->Evaluate(); } );
			return ShouldTransit;
		}
	};
	typedef std::shared_ptr<CTransition> transition_ptr;

	class CState
	{
	public:
		std::string mName;
		std::string mSequenceName;
		bool mLoop;
		std::vector<transition_ptr>	mTransitions;

		std::string EvaluateTransitStateName()
		{
			std::vector<transition_ptr>::iterator Transit = std::find_if( mTransitions.begin(), mTransitions.end(), []( transition_ptr Transition ){ return Transition->Evaluate(); } );
			if( Transit != mTransitions.end() )
				return (*Transit)->GetTransitStateName();			
			return "";
		}
	};
	typedef std::shared_ptr<CState> state_ptr;

	class CLayer
	{
	public:
		CLayer();
		virtual ~CLayer();
		
		void Reset();
		void Transit( const std::string& inTransitStateName );
		void Update( float in_fDeltaTime );

		std::map<std::string, state_ptr>	mStates;

		state_ptr mpCurrentState;

		std::string mInitialStateName;
		std::string mCurrentStateName;
		std::string mTransitStateName;

		std::string mName;

		Sprite* mpSprite;
	};
	typedef std::shared_ptr<CLayer> layer_ptr;

	class CMachine
	{
	public:
		void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );

		void Reset();
		void Update( float in_fDeltaTime );

		void Load( const char* inFilename );

		template <class T>
		bool SetParameter( const std::string& inName, T inValue )
		{
			parameter_ptr Parameter;
			if( FindParameter( inName, Parameter ) )
			{
				Parameter->SetValue( inValue );
				return true;
			}
			return false;
		}

	private:
		bool FindParameter( const std::string& inName, parameter_ptr& outParameter );

		void ParseParameters( TiXmlElement* pxmlParameters );
		void ParseConditions( TiXmlElement* pxmlConditions, layer_ptr& Layer, state_ptr& State, transition_ptr& Transition );
		void ParseTransitions( TiXmlElement* pxmlTransitions, layer_ptr& Layer, state_ptr& State );
		void ParseStates( TiXmlElement* pxmlStates, layer_ptr& Layer );
		void ParseLayers( TiXmlElement* pxmlLayers );
		
	private:
		std::map<std::string, parameter_ptr>	mParameters;
		std::vector<layer_ptr>	mLayers;
	};

	inline void Test()
	{
		int IntParam = 6;
		float FloatParam = 3.0;
		bool BoolParam = true;

		condition_ptr TestInt(new CConditionParameter<int>( &IntParam, CCondition::Equal, 6));
		condition_ptr TestFloat(new CConditionParameter<float>( &FloatParam, CCondition::Less, 8.0));
		condition_ptr TestBool(new CConditionParameter<bool>( &BoolParam, CCondition::NotEqual, false));

		assert( TestInt->Evaluate() );
		assert( TestFloat->Evaluate() );
		assert( TestBool->Evaluate() );	
				
		CMachine Machine;
		Machine.Load("pilot.asm");
	}
}

#endif//_ASM_H