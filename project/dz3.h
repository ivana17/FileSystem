#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <iomanip>
typedef char byte;
typedef std::string Text;
using namespace std;
#define CAPACITY 8000

class FSObject;
class Folder;
class FilesystemVisitor;

//AccessDescriptor

class AccessDescriptor{
private:
	FSObject& protectedObject;
	struct listElem{
		Text operName;
		listElem* next;
		listElem(Text name, listElem* p = nullptr) :operName(name), next(p){}
		~listElem(){ next = nullptr; }
	};
	listElem* allowedOperations;
public:
	AccessDescriptor(FSObject& fso) :
		protectedObject(fso), allowedOperations(nullptr){}
	void add(Text operationName);
	void remove(Text operationName);
	listElem* getAllowedOperations();
	bool checkAccess(Text operationName);
	~AccessDescriptor(){}
};

class FSObject{
private:
	Text name;
	Folder* parent;
	AccessDescriptor accessDescriptor;
public:
	inline virtual bool isFolder() = 0;
	Text getName(){ return this->name; }
	void setName(Text){ this->name = name; }
	FSObject(Text name, Folder* p = nullptr) :
		accessDescriptor(*this), name(name), parent(p){}
	AccessDescriptor& getAccessDescriptor(){
		return accessDescriptor;
	}
	virtual void deleteObject() = 0;
	virtual void accept(FilesystemVisitor&) = 0;
	virtual long size() = 0;
	Folder* getParent(){ return parent; }
	void setParent(Folder* p){ this->parent = p; }
	virtual FSObject& copy() = 0;
	virtual ~FSObject() { parent = nullptr; }
};

class Folder : public FSObject{
private:
	long occupiedMemory;
	vector<FSObject*> containedObjects;
public:
	void setOccMemory(long);
	void deleteObject();
	inline bool isFolder(){ return true; }
	Folder(Text name, vector<FSObject*> contObj) :
		FSObject(name), occupiedMemory(0), containedObjects(contObj){}
	Folder(Text name) :
		FSObject(name), occupiedMemory(0){}
	void accept(FilesystemVisitor& v);
	long size();
	void addContObject(FSObject&);
	/* Folder(Folder& folder) :FSObject(folder.getName(), folder.getParent()){
		Folder* newF = &folder.copy();
		*this = *newF;
	} */ //ne moze ovako konstr kopije
	~Folder(){ deleteObject(); }
	FSObject& Folder::copy();
	void add(FSObject& o);
	vector<FSObject*>& getObjects();
	void remove(FSObject& obj);
};

class File : public FSObject{
private:
	byte* content;
	long contentSize;
	void deleteObject();
public:
	inline bool isFolder(){ return false; }
	File(Text name, byte* c) :
		FSObject(name), contentSize(0), content(c){
		if (content) contentSize = strlen(content);
	}
	File(Text name) :
		FSObject(name), contentSize(0), content(nullptr){}
	void accept(FilesystemVisitor&);
	void write(byte*);
	FSObject& copy();
	~File(){ deleteObject(); }
	byte* read();
	long size();
};

//FilesystemVisitor

class FilesystemVisitor{
public:
	FilesystemVisitor(){}
	virtual void visitFile(File f) = 0;
	virtual void visitFolder(Folder f) = 0;
	~FilesystemVisitor(){}
};

class SearchVisitor :public FilesystemVisitor{
private:
	Text filename; 
	vector<FSObject*> foundObjects;
public:
	SearchVisitor(Text s) :filename(s){}
	void visitFile(File f);
	void search(Folder f);
	void search(File f);
	void visitFolder(Folder f);
	vector<FSObject*>& getFoundObjects();
	~SearchVisitor(){ foundObjects.clear(); }
};

//FSOperation


class FSOperation{
private:
	Text opName;
public:
	FSOperation(Text n) :opName(n){}
	virtual void execute() = 0;// raises OperationFailedException, AccessException
	virtual Text getName() = 0;
	virtual FSObject& getObject() = 0;
	~FSOperation(){}
};

class ReadFile :public FSOperation{
private:
	byte* contentOfFile;
	File file;
public:
	ReadFile(File f, Text name = "ReadFile") :
		FSOperation(name), file(f){}
	void execute(){
		contentOfFile = new byte(file.size());
		for (int i = 0; i < file.size(); ++i)
			contentOfFile[i] = file.read()[i];
	}
	FSObject& getObject() {
		return file;
	}
	byte* getContent(){ return contentOfFile; }
	Text getName(){ return "ReadFile"; }
};

class CreateFile :public FSOperation{
private:
	File file;
	Folder* parent;
public:
	CreateFile(Folder* p, Text fileName, Text name = "CreateFile") :
		FSOperation(name), file(fileName), parent(p){}
	Text getName(){ return "CreateFile"; }
	File& getFile(){
		return file;
	}
	void execute(){
		file.setParent(parent);
		parent->add(file);
	}
	FSObject& getObject() {
		return file;
	}
	~CreateFile(){
		parent = nullptr;
	}
};

class CreateDirectory :public FSOperation{
private:
	Folder newFolder;
	Folder* parentFolder;
public:
	CreateDirectory(Folder* pF, Text nameOfFolder, Text name = "CreateDirectory") :
		FSOperation(name), parentFolder(pF), newFolder(nameOfFolder){}
	Text getName(){ return "CreateDirectory"; }
	Folder& getFile(){
		return newFolder;
	}
	FSObject& getObject() {
		return newFolder;
	}
	void execute(){
		newFolder.setParent(parentFolder);
		parentFolder->add(newFolder);
	}
	~CreateDirectory(){
		parentFolder = nullptr;
	}
};

class WriteFile :public FSOperation{
private:
	byte* contentOfFile;
	File* file;
public:
	WriteFile(File f, byte* cF, Text name = "WriteFile") :
		FSOperation(name), contentOfFile(cF), file(&f){}
	void execute(){
		file->write(contentOfFile);
	}
	FSObject& getObject() {
		return *file;
	}
	File getFile(){ return *file; }
	Text getName(){ return "WriteFile"; }
	~WriteFile(){
		delete contentOfFile;
		contentOfFile = nullptr;
		file = nullptr;
	}
};

class DeleteObject :public FSOperation{
private:
	FSObject* object;
public:
	DeleteObject(FSObject& f, Text name = "DeleteObject") :
		FSOperation(name), object(&f){}
	void execute(){	object->deleteObject(); }
	FSObject& getObject() {
		return *object;
	}
	Text getName(){ return "DeleteObject"; }
	~DeleteObject(){
		object = nullptr;
	}
};

class CopyPaste :public FSOperation{
private:
	FSObject* originalObject;
	FSObject* copyOfObject;
	Folder* folder;
public:
	CopyPaste(FSObject& f, Folder fold, Text name = "CopyPaste") :
		FSOperation(name), originalObject(&f), folder(&fold), copyOfObject(nullptr){}
	void execute(){ 
		copyOfObject = &originalObject->copy();
		copyOfObject->setParent(folder);
		folder->addContObject(*copyOfObject);
	}
	FSObject& getObject() {
		return *originalObject;
	}
	Text getName(){ return "CopyPaste"; }
	~CopyPaste(){
		copyOfObject = nullptr;
		originalObject = nullptr;
		folder = nullptr;
	}
};

class Search :public FSOperation{
private:
	Folder* sObject;
	SearchVisitor sv;
	Text searchText;
public:
	Search(Folder& f, Text t, Text name = "Search") :
		FSOperation(name), searchText(t), sObject(&f), sv(t){}
	void execute(){
		sv.visitFolder(*sObject);
	}
	vector<FSObject*> getFoundObjects(){
		 return sv.getFoundObjects();
	}
	FSObject& getObject() {
		return *sObject;
	}
	Text getName(){ return "Search"; }
	~Search(){
		sObject = nullptr;
	}
};

class ListDirectory :public FSOperation{
private:
	Folder* directory;
public:
	ListDirectory(Folder& f, Text t, Text name = "ListDirectory") :
		FSOperation(name), directory(&f){}
	void execute(){}
	vector<FSObject*> getFoundObjects(){
		return directory->getObjects();
	}
	FSObject& getObject() {
		return *directory;
	}
	Text getName(){ return "ListDirectory"; }
	~ListDirectory(){
		directory = nullptr;
	}
};

class ProtectedOperation :public FSOperation{
private:
FSOperation* wrappedOperation;
protected:
	bool checkPrecondition() {
		return (wrappedOperation->getObject().getAccessDescriptor().checkAccess(wrappedOperation->getName()));
	}
public:
ProtectedOperation(Text t) : FSOperation(t){}
virtual void execute(){
	if (checkPrecondition())
		wrappedOperation->execute();
}
~ProtectedOperation(){
	wrappedOperation = nullptr;
}
};

//EX

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


/*
class Filesystem{
private:
Folder* rootFolder;
const long memoryCapacity = CAPACITY;
Filesystem() :rootFolder(nullptr){}
~Filesystem(){ }
public:
friend long freeSpace(){
if (rootFolder)
return memoryCapacity - rootFolder->Folder::size();
else
return memoryCapacity;
}
void createFile(Text fName, Folder destFolder); // File raises NameCollisionException, AccessException
void createFolder(Text fName, Folder parentFolder); // Folder raises NameCollisionException, AccessException
FSObject* listFolder(Folder folder); // raises AccessException
byte* readFile(File file); // raises AccessException, ObjectNotFoundException
void writeFile(byte* content); //raises AccessException, WriteFailedException
void grantAccess(FSObject& fObj, Text opName = nullptr);
void revokeAccess(FSObject& fObj, Text opName = nullptr);
void openFolder(Text folderPath); // Folder raises AccessException, ObjectNotFoundException
FSObject* search(Text objName);
void copyPaste(FSObject& objToCopy, Folder destFolder, string newName); // raises AccessException, WriteFailedException, NameCollisionException
void move(FSObject& objToMove, Folder destFolder); // raises AccessException, NameCollisionException
//bool delete(FSObject objToDelete); // bool raises AccessException
};

*/