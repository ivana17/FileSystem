#include "dz3.h"
//#include "Filesystem.h"

vector<FSObject*>& Folder::getObjects(){
	return containedObjects;
}

void Folder::addContObject(FSObject& fso){
	if (1){ //if (freeSpace() >= sizeOfFile){
		containedObjects.push_back(&fso);
		this->setOccMemory(fso.size());
	}
}

void Folder::accept(FilesystemVisitor& fsVisitor){
	fsVisitor.visitFolder(*this); 
}

long Folder::size(){
	return this->occupiedMemory;
}

FSObject& Folder::copy(){
 
	vector<FSObject*> newContainedObjects(containedObjects.size());

	for (int it = 0; it != containedObjects.size(); it++)
		newContainedObjects.push_back(&containedObjects.at(it)->copy());

	Folder newFolder(this->getName());

	for (int it = 0; it != containedObjects.size(); it++)
		newContainedObjects.push_back(containedObjects.at(it));
	
	return newFolder;
}

void Folder::deleteObject(){
	//proveri
	for (int it = 0; it < containedObjects.size(); ++it){
		containedObjects.at(it)->deleteObject();
		this->setParent(nullptr);
		this->setOccMemory(-size());
	}
}

void Folder::remove(FSObject& object){

	int it = 0;
	for (; it != containedObjects.size(); it++){
		if (object.getName() == containedObjects.at(it)->getName())
			break;
	}

	if (it != containedObjects.size()){
		containedObjects.at(it)->deleteObject();
		containedObjects.erase(containedObjects.begin()+it);

		Folder* hObj = this;
		while (hObj){
			hObj->setOccMemory(-object.size());
			hObj = hObj->getParent();
		}
		hObj = nullptr;
	}

}

void Folder::setOccMemory(long deltaSize){
	this->occupiedMemory = this->occupiedMemory + deltaSize;
}

void Folder::add(FSObject& object){ 

	bool exist = false;
	auto it = containedObjects.begin();
	while (it != containedObjects.end()){
		if ((*it)->getName() == object.getName()){
			exist = true;
			break;
		}
		else ++it;
	}

	if (exist == true) return;

	//if (object.size() <= freeSpace()){ //vrati
	if (1){
		object.setParent(this);
		this->containedObjects.push_back(&object);
		this->setOccMemory(object.size());
		Folder* hObj = this->getParent();

		while (hObj){
			hObj->setOccMemory(object.size());
			hObj = hObj->getParent();
		}
		hObj = nullptr;
	}
	else
		cout << "There is no free space on disk." << endl;
}
