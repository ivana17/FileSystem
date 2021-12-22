#include "dz3.h"

int main() {

	Folder folder1("folder1");
	Folder folder2("folder2");
	Folder folder3("folder3");

	byte* mem1 = "1234567890";
	byte* mem2 = "12345678901234567890";
	byte* mem3 = "123456789012345";


	File file1("file1", mem1);
	File file2("file2", mem2);
	File file3("file3", mem3);
	Folder* istiFolder3 = new Folder(folder3);


	//ADD
	folder3.add(file3);
	folder2.add(file2);
	folder1.add(file1);

	//REMOVE
	folder1.remove(file1);

	folder2.add(folder3);

	folder1.add(folder2);


	folder1.add(file1);
	folder2.add(file2);
	folder3.add(*istiFolder3);


	//	cout << folder1.size() << endl;
	//	cout << folder2.size() << endl;
	//	cout << folder3.size() << endl;
	//	delete file1;
	//	delete file2;
	//	delete file3;

	/*

	*/



	//Folder*pok = folder1.copy();
	//cout << pok->size();
	//cout << pok->numberOFFilesAndFoldersInThisFolder();
	//Folder folder1Copy(folder1);
	//Folder* folderr = new Folder(folder1);



	//folder1.ispisi();
	//cout << endl;
	
	
	
	//File* f = &file1;

	/*
	for (int i = 0; i < f->size(); i++)
	{
	cout << *(*f->getMemory())[i];
	}
	*/
	//	delete folder3->getObjects();
	//	delete folder2->getObjects();
	//	delete folder1->getObjects();
	//	delete folder2;
	//	delete folder3;
	//	delete folder1;


	//folder2 = nullptr;
	//		delete folder3;
	//		delete folder1;

	//folder1.~Folder();
	//folder2.~Folder();

	/*
	for (int i = 0; i < 15; i++)
	{

	delete mem2[i];
	mem2[i] = nullptr;
	}
	for (int i = 0; i < 20; i++)
	{

	delete mem3[i];
	mem3[i] = nullptr;
	}
	for (int i = 0; i < 10; i++)
	{
	delete mem1[i];
	mem1[i] = nullptr;
	}
	*/
	//m1->clear();
	//m2->clear();
	//m3->clear();

	//	delete istiFolder3;
}
/*
//file


long File::size(){
	if (content)
		return contentSize;
	else return 0;
}

void File::write(byte* newContent){
	//contentSize = sizeof(newContent) / sizeof(byte);
	this->contentSize = strlen(newContent);
	//if (freeSpace() >= sizeOfFile){ //vrati, kad napravis filesystem
	if (1){
		if (!this->content) delete this->content;
		this->content = new byte[contentSize];
		strcpy(content, newContent);
		
		//int i = 0;
		//while (i < contentSize)
		//	this->content[i] = newContent[i++];
		
	}
	else
		cout << "There is not enough disk space" << endl;
} 

byte* File::read(){
	return content;
}

void File::deleteObject(){
	if (content){
		//delete content;
		content = nullptr;
		contentSize = 0;
	}
}

void File::accept(FilesystemVisitor& fsVisitor){
	fsVisitor.visitFile(*this);
}

FSObject& File::copy(){

	File newFile(this->getName());
	newFile.content = new byte[contentSize];
	strcpy(newFile.content, content);
	
	//int i = 0;
	//while (i < sizeOfFile)
	//	this->content[i] = this->content[i++];
	
	return newFile;
}

//folder

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

//visitor

void SearchVisitor::visitFile(File f){
if (strstr(f.getName().c_str(), this->filename.c_str()))
foundObjects.push_back(&f);
}

vector<FSObject*>& SearchVisitor::getFoundObjects(){
return foundObjects;
}

void SearchVisitor::visitFolder(Folder f){
for (int it = 0; it != f.getObjects().size(); ++it){
if (f.getObjects().at(it)->getAccessDescriptor().checkAccess("ReadFile"))
if (strstr(f.getObjects().at(it)->getName().c_str(), this->filename.c_str()))
foundObjects.push_back(f.getObjects().at(it));
f.getObjects().at(it)->accept(*this);
}
}

//acc

bool AccessDescriptor::checkAccess(Text opName){

listElem* currElem = allowedOperations;
while (currElem && currElem->operName != opName)
currElem = currElem->next;

if (currElem && currElem->operName == opName)
return true;
else return false;
}

void AccessDescriptor::remove(Text opName){

if (!allowedOperations){
cout << "There is no allowed operations." << endl;
return;
}

listElem* currElem = allowedOperations;
listElem* prevElem = nullptr;
while (currElem && currElem->operName != opName){
prevElem = currElem;
currElem = currElem->next;
}

if (!currElem)
cout << "The operation " << opName << " is not allowed." << endl;
if (currElem->operName == opName){

if (!prevElem)
allowedOperations = currElem->next;
else
prevElem->next = currElem->next;

delete currElem;
}
prevElem = nullptr;
}

void AccessDescriptor::add(Text opName){

if (!allowedOperations){
allowedOperations = new listElem(opName);
return;
}

listElem* currElem = allowedOperations;
while (currElem && currElem->operName != opName)
currElem = currElem->next;

if (!currElem){
currElem = new listElem(opName, allowedOperations);
allowedOperations = currElem;
}
else if (currElem->operName == opName)
cout << "The operation was already allowed";

currElem = nullptr;
}

*/