#include "dz3.h"
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