#include "Database.h"

Data* Data::Create( int i ) { return new DataInt(i); }
Data* Data::Create( float f ) { return new DataFloat(f); }
Data* Data::Create( std::string& s ) { return new DataString(s); }
Data* Data::Create( Object* o ) { return new DataObject(o); }
Data* Data::Create( ObjectMethod m ) { return new DataObjectMethod(m); }
