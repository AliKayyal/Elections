#include "uniqueOrderedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** This struct is a node, in this node we have the fields:
 1-data which is a pointer to the data in each node
 2-nextindex which is a pointer to the next node in the UniqueOrderedList
 3- previous which is a pointer to the previous node in the UniqueOrderedList
 The UniqueOrderedList itself is built of nodes which contain all of the data
 regarding it*/
typedef struct node_t {
	Element data;
	struct node_t* nextindex;
	struct node_t* previous;
}*Node;

/** This struct is our main struct, in the UniqueOrderedList we have the fields:
 1-Head- which "bounds" the UniqueOrderedList from the left
 2-Tail- which "bounds" the UniqueOrderedList from the left
 Head and tail have a typedef struct Node
 3-copyelement,freeelement,elementequal,elementgreater are all pointers
 to functions which we are going
 to use to compare,add,free or clear elements to or from the UniqueOrderedList
 4-Iterator is a Node that marks our exact location in the UniqueOrderedList at
 a given time.
 The iterator is going to be modified after certain functions
 5-Size- This element defines the size of the UniqueOrderedList at a given time.
 Certain elements are going to change it.*/
struct uniqueOrderedList_t {
	Node head;
	Node tail;
	Element (*copyElements)( Element);
	void (*freeElements)( Element);
	bool (*elementsEquals)( Element, Element);
	bool (*elementGreaterThan)( Element, Element);
	Node iterator;
	int size;
};

/** The function uniqueOrderedListCreate creates the UniqueOrderedList,
 with which we are going to work.
 It recieves copyFunction,freeFunction,equalFunction,greaterFunction and
 creates a UniqueOrderedList
 which is capable of using these functions. Certain fields in the
 UniqueOrderedList get their starting
 values such as head,tail,dummy_head,dummy_tail.If everything works as it should
 and no mallocs fails
 then we are going to get a UniqueOrderedList with the size that is equal to
 zero
 */
UniqueOrderedList uniqueOrderedListCreate(Element (*copyElements)( Element),
		void (*freeElements)( Element),
		bool (*elementsEquals)( Element, Element),
		bool (*elementGreaterThan)( Element, Element)) {
	if (!copyElements || !freeElements || !elementsEquals
			|| !elementGreaterThan) {
		return NULL;
	}

	UniqueOrderedList uniqueorderedlist = malloc(sizeof(*uniqueorderedlist));
	if (!uniqueorderedlist) {
		return NULL;
	}
	Node dummy_head = malloc(sizeof(*dummy_head));
	if (!dummy_head) {
		free(uniqueorderedlist);
		return NULL;
	}

	Node dummy_tail = malloc(sizeof(*dummy_tail));
	if (!dummy_tail) {
		free(dummy_head);
		free(uniqueorderedlist);
		return NULL;
	}

	uniqueorderedlist->head = dummy_head;
	uniqueorderedlist->tail = dummy_tail;
	dummy_head->nextindex = dummy_tail;
	dummy_head->previous = uniqueorderedlist->head;
	dummy_tail->nextindex = NULL;
	dummy_tail->previous = dummy_head;
	uniqueorderedlist->copyElements = copyElements;
	uniqueorderedlist->freeElements = freeElements;
	uniqueorderedlist->elementsEquals = elementsEquals;
	uniqueorderedlist->elementGreaterThan = elementGreaterThan;
	uniqueorderedlist->iterator = uniqueorderedlist->head->nextindex;
	uniqueorderedlist->size = 0;
	return uniqueorderedlist;
}

/**This function recieves a certain data. If everything works according to
 * how it should and no mallocs
 fail or no NULL arguments appear, the the function is going to determine
 whether the Element will be
 inserted and where it should be inserted*/
UniqueOrderedListResult uniqueOrderedListInsert(
		UniqueOrderedList uniqueorderedlist, Element datatoinsert) {
	if (datatoinsert == NULL || uniqueorderedlist == NULL)
		return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
	if (uniqueOrderedListContains(uniqueorderedlist, datatoinsert) == true) {
		return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
	}
	Node inserted = malloc(sizeof(*inserted));
	if (!inserted) {
		return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
	}
	inserted->data = uniqueorderedlist->copyElements(datatoinsert);
	if (uniqueorderedlist->size == 0) {
		inserted->nextindex = uniqueorderedlist->tail;
		inserted->previous = uniqueorderedlist->head;
		uniqueorderedlist->head->nextindex = inserted;
		uniqueorderedlist->tail->previous = inserted;
		uniqueorderedlist->size++;
		return UNIQUE_ORDERED_LIST_SUCCESS;
	}
    Node ptr;
	for (ptr = uniqueorderedlist->head->nextindex;
			ptr != uniqueorderedlist->tail; ptr = ptr->nextindex) {
		if (uniqueorderedlist->elementGreaterThan(ptr->data, datatoinsert)) {
			inserted->nextindex = ptr;
			inserted->previous = ptr->previous;
			ptr->previous->nextindex = inserted;
			ptr->previous = inserted;
			uniqueorderedlist->size++;
			return UNIQUE_ORDERED_LIST_SUCCESS;

		}

	}

	inserted->nextindex = uniqueorderedlist->tail;
	inserted->previous = uniqueorderedlist->tail->previous;
	uniqueorderedlist->tail->previous->nextindex = inserted;
	uniqueorderedlist->tail->previous = inserted;
	uniqueorderedlist->size++;
	return UNIQUE_ORDERED_LIST_SUCCESS;
}

/**This function recieves certain data which we want to remove from and checks
 (If everything works as it should and no NULL arguments appear) whether
 it exists.
 If so then it removes it and orders itself so that it stays uniquely ordered
 (No repeats and in a certain order) */
UniqueOrderedListResult uniqueOrderedListRemove(
		UniqueOrderedList uniqueorderedlist, Element to_remove) {
	if (to_remove == NULL || uniqueorderedlist == NULL)
		return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
	if (!uniqueOrderedListContains(uniqueorderedlist, to_remove))
		return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
	Node temporary = uniqueorderedlist->head->nextindex;
	while (temporary != uniqueorderedlist->tail) {
		if ((uniqueorderedlist->elementsEquals)(to_remove, temporary->data)) {
			temporary->previous->nextindex = temporary->nextindex;
			temporary->nextindex->previous = temporary->previous;
			uniqueorderedlist->size--;
			(uniqueorderedlist->freeElements)(temporary->data);
			free(temporary);
			return UNIQUE_ORDERED_LIST_SUCCESS;
		}
		temporary = temporary->nextindex;
	}
	return UNIQUE_ORDERED_LIST_SUCCESS;
}

/**This function recieves a UniqueOrderedList and returns the data with the
 * lowest value in it
 (before that it checks if any null arguments appear and acts accordingly)*/
Element uniqueOrderedListGetLowest(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL
			|| uniqueorderedlist->head->nextindex == uniqueorderedlist->tail)
		return NULL;
	uniqueorderedlist->iterator = uniqueorderedlist->head->nextindex;
	return uniqueorderedlist->iterator->data;
}

/**This function recieves a UniqueOrderedList and returns the data with the
 *  highest value in it
 (before that it checks if any null arguments appear and acts accordingly)*/
Element uniqueOrderedListGetGreatest(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL
			|| uniqueorderedlist->head->nextindex == uniqueorderedlist->tail)
		return NULL;
	uniqueorderedlist->iterator = uniqueorderedlist->tail->previous;
	return uniqueorderedlist->iterator->data;
}

/**This function recieves a UniqueOrderedList and returns the data which
 are the next index to the
 iterator at the time when it's being called.
 (before that it checks if any null arguments appear and acts accordingly)*/
Element uniqueOrderedListGetNext(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL
			|| uniqueorderedlist->head->nextindex == uniqueorderedlist->tail)
		return NULL;
	if (uniqueorderedlist->iterator->nextindex == uniqueorderedlist->tail)
		return NULL;
	uniqueorderedlist->iterator = uniqueorderedlist->iterator->nextindex;
	return uniqueorderedlist->iterator->data;
}

/**This function recieves a UniqueOrderedList and returns it's size at the
 * very time it's being called.
 (before that it checks if any null arguments appear and acts accordingly)*/
int uniqueOrderedListSize(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL) {
		return -1;
	}
	return uniqueorderedlist->size;
}

/**This function recieves a UniqueOrderedList frees up all of its contents
 *  except for the head and tail
 and updates its size to zero.
 It checks before everything if null arguments are passed to it or if its
 size is zero and acts accordingly*/
void uniqueOrderedListClear(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL
			|| uniqueorderedlist->head->nextindex == uniqueorderedlist->tail)
		return;
	int size = uniqueOrderedListSize(uniqueorderedlist);
	Node temporary = uniqueorderedlist->head->nextindex;
	while (size > 0) {
		Node toclear = temporary;
		temporary = temporary->nextindex;
		uniqueOrderedListRemove(uniqueorderedlist, toclear->data);
		size--;
	}
	uniqueorderedlist->size = 0;
}

/**This function recieves a UniqueOrderedList frees up all of its
 * contents using the clear function
 It frees its head,tail and lastly itself.
 It checks before everything if null arguments are passed to it or
 if its size is zero and acts accordingly*/
void uniqueOrderedListDestroy(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL) {
		return;
	}
	uniqueOrderedListClear(uniqueorderedlist);
	free(uniqueorderedlist->head);
	free(uniqueorderedlist->tail);
	free(uniqueorderedlist);
}

/**This function recieves a UniqueOrderedList and copies all of
 * its contents to another UniqueOrderedList
 It checks in the beginning if any null arguments appear.
 If everything works it starts gliding over the nodes using the internal
 iterator.
 It inserts the data in destination using the insert function.
 If no problems pop up then the iterator to its place at the time of the
 call of the function
 and the copy of the list is returned*/
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList uniqueorderedlist) {
	if (uniqueorderedlist == NULL)
		return NULL;
	UniqueOrderedList unique_copy = uniqueOrderedListCreate(
			uniqueorderedlist->copyElements, uniqueorderedlist->freeElements,
			uniqueorderedlist->elementsEquals,
			uniqueorderedlist->elementGreaterThan);
	if (unique_copy == NULL) {
		return NULL;
	}
	int size = uniqueOrderedListSize(uniqueorderedlist);
	uniqueOrderedListGetLowest(uniqueorderedlist);
	while (size > 0) {
		uniqueOrderedListInsert(unique_copy, uniqueorderedlist->iterator->data);
		uniqueOrderedListGetNext(uniqueorderedlist);
		size--;
	}

	return unique_copy;

}

/**this function gets a UniqueOrderedList and a cetain element and checks if
 the list contains this element if yes returns true otherwise false*/
bool uniqueOrderedListContains(UniqueOrderedList uniqueorderedlist,
		Element contained) {
	if (uniqueorderedlist == NULL || contained == NULL) {
		return false;
	}
	Element to_compare = uniqueOrderedListGetLowest(uniqueorderedlist);
	int size = uniqueorderedlist->size;
	while (size > 0) {
		if (uniqueorderedlist->elementsEquals(contained, to_compare)) {
			return true;
		}
		uniqueOrderedListGetNext(uniqueorderedlist);
		to_compare = uniqueorderedlist->iterator->data;
		size--;
	}
	return false;
}
