#include "dz3.h"
//#include "Filesystem.h"

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
		/*
		int i = 0;
		while (i < contentSize)
			this->content[i] = newContent[i++];
		*/
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
	/*
	int i = 0;
	while (i < sizeOfFile)
		this->content[i] = this->content[i++];
	*/
	return newFile;
}