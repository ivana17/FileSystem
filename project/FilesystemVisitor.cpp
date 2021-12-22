#include "dz3.h"

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

