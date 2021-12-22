#include "dz3.h"

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