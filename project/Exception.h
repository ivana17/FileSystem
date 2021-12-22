#include "dz3.h"

class FSException :public exception{
public:
	FSException* causedBy;
	FSException(Text* msg) :exception() {}
	FSException(Text* msg, FSException& cause) :exception(cause){}
};

class AccessException :public FSException {
public: //zasto text(zvezdica)
	AccessException(Text* msg) :FSException(msg) { cout << msg; }
	AccessException(Text* msg, FSException& cause) :FSException(msg, cause) {}
};

class OperationFailedException :public FSException {
public:
	OperationFailedException(Text* msg) :FSException(msg) { cout << msg; }
	OperationFailedException(Text* msg, FSException& cause) :FSException(msg, cause) {}
};

class NameCollisionException :public OperationFailedException {
public:
	NameCollisionException(Text* msg) :OperationFailedException(msg) { cout << msg; }
	NameCollisionException(Text* msg, FSException& cause) :OperationFailedException(msg, cause) {}
};

class WriteFailedException :public OperationFailedException {
public:
	WriteFailedException(Text* msg) :OperationFailedException(msg){ cout << msg; }
};