//////////////////////////////////////////////////////////////////////////
//
// ClassType.h
//
//////////////////////////////////////////////////////////////////////////

#ifndef _CLASS_TYPE_H
#define _CLASS_TYPE_H


typedef void* TYPE_CLASS_TYPE;

// Use this macro in the root class definition (.h).
#define DECLARE_CLASS_TYPE_ROOT(_class_name_)                    \
	public:                                                      \
	static TYPE_CLASS_TYPE GetClassType();                       \
	static _class_name_* SafeCast(_class_name_* in_pElement)     \
	{                                                            \
		if(in_pElement->IsA(_class_name_::GetClassType()))       \
		{                                                        \
			return reinterpret_cast<_class_name_*>(in_pElement); \
		}                                                        \
		return 0;                                                \
	}                                                            \
	virtual bool Is(TYPE_CLASS_TYPE in_nType)                    \
	{                                                            \
		return in_nType == _class_name_::GetClassType();         \
	}                                                            \
	virtual bool IsA(TYPE_CLASS_TYPE in_nType)                   \
	{                                                            \
		return in_nType == _class_name_::GetClassType();         \
	}                                                            \
	private:

// Use this macro in the derived class definition (.h).
// Ex:
// Here is a derivation chain from class A to class E
// A <- B <- C <- D <- E
//
// A: _root_class_name_
// D: _base_class_name_
// E: _class_name_
// 
// Note: _root_class_name_ and _base_class_name_ are the same if the
// immediate base class of the current class is the root.
#define DECLARE_CLASS_TYPE(_class_name_, _base_class_name_, _root_class_name_) \
	public:                                                                    \
	static TYPE_CLASS_TYPE GetClassType();                                     \
	static _class_name_* SafeCast(_root_class_name_* in_pElement)              \
	{                                                                          \
		if(in_pElement->IsA(_class_name_::GetClassType()))                     \
		{                                                                      \
			return reinterpret_cast<_class_name_*>(in_pElement);               \
		}                                                                      \
		return 0;                                                              \
	}                                                                          \
	virtual bool Is(TYPE_CLASS_TYPE in_nType)                                  \
	{                                                                          \
		return in_nType == _class_name_::GetClassType();                       \
	}                                                                          \
	virtual bool IsA(TYPE_CLASS_TYPE in_nType)                                 \
	{                                                                          \
		if(in_nType == _class_name_::GetClassType())                           \
		{                                                                      \
			return true;                                                       \
		}                                                                      \
		return _base_class_name_::IsA(in_nType);                               \
	}                                                                          \
	private:

//////////////////////////////////////////////////////////////////////////
//**********************************************************************//
// IMPORTANT: You MUST use this macro in a .cpp!!!                      //
//**********************************************************************//
//////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_CLASS_TYPE(_class_name_) \
	void* _class_name_::GetClassType()     \
	{                                      \
		static int s_nClassType = 0;       \
		return &s_nClassType;              \
	}


#endif // _CLASS_TYPE_H
