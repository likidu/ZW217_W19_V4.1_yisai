/*
 *	list.h
 *	Release $Name: MATRIXSSL-3-4-2-COMM $
 */
/*
 *	Copyright (c) 2013 INSIDE Secure Corporation
 *	All Rights Reserved
 *
 *	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF INSIDE.
 *
 *	Please do not edit this file without first consulting INSIDE support.
 *	Unauthorized changes to this file are not supported by INSIDE.
 *
 *	The copyright notice above does not evidence any actual or intended
 *	publication of such source code.
 *
 *	This Module contains Proprietary Information of INSIDE and should be
 *	treated as Confidential.
 *
 *	The information in this file is provided for the exclusive use of the
 *	licensees of INSIDE. Such users have the right to use, modify,
 *	and incorporate this code into products for purposes authorized by the
 *	license agreement provided they include this notice and the associated
 *	copyright notice with any such product.
 *
 *	The information in this file is provided "AS IS" without warranty.
 */
/******************************************************************************/

#ifndef _h_PS_COREUTIL
#define _h_PS_COREUTIL
#include "sci_types.h"
/********************************** Defines ***********************************/
/*
 *	Fast circular doubly-linked list and branchless macro-functions.
 *	Provides a struct-independent way to have a small head and hang a list
 *	of structs off of it. Examples of use:
 *
 *	typedef struct {
 *		int bar;
 *		DLListEntry List;
 *	} foo;
 *
 *  Define foo list head:
 *      DLListEntry FooListHead;
 *
 *  Init for list head:
 *      DLListInit(&FooListHead);
 *
 *	Define and init list head:
 *		DEFINE_DLLIST(FooListHead);
 *			or
 *		static DEFINE_DLLIST(FooListHead);
 *
 *  Insert *pMyFoo after the head:
 *      foo *pMyFoo;
 *      pMyFoo = malloc(sizeof(foo));
 *      DLListInsertHead(&FooListHead, &pMyFoo->List);
 *
 *  Do the same but place at tail:
 *      DLListInsertTail(&FooListHead, &pMyFoo->List);
 *
 *  Given a foo that you know is in a list, detach it from the list:
 *      DLListRemove(&pMyFoo->List);
 *
 *  Detach and return the list entry for the head item:
 *      DLListEntry *pList;
 *      pList = DLListGetHead(&FooListHead);
 *      pMyFoo = DLListGetContainer(pList, foo, List);
 *
 *  Iterate the entire list:
 *      DLListEntry *pList;
 *      pList = FooListHead.pNext;
 *      while (pList != &FooListHead) {
 *          pCurFoo = DLListGetContainer(pList, foo, List);
 *          ... (do something with the current foo)
 *          pList = pList->pNext;
 *      }
 *
 *  Iterate the entire list, removing each item (e.g. on all-list destruction):
 *      DLListEntry *pList;
 *      while (!DLListIsEmpty(&FooListHead)) {
 *          pList = DLListGetHead(&FooListHead);
 *          pMyFoo = DLListGetContainer(pList, foo, List);
 *          DestroyFoo(pMyFoo);
 *          free(pMyFoo);
 *      }
 */
typedef struct _DLListEntry {
    struct _DLListEntry *pNext, *pPrev;
} DLListEntry;

#define DEFINE_DLLIST(x) DLListEntry x = { .pNext = &x, .pPrev = &x };

#define DLListInit(__pList)  \
(__pList)->pNext = (__pList)->pPrev = (__pList)

/* Inserts an item as the first item of the list */
#define DLListInsertHead(__pHead, __pNode) {  \
psAssert((__pHead) != (__pNode));  \
(__pNode)->pNext = (__pHead)->pNext;  \
(__pNode)->pPrev = (__pHead);  \
(__pHead)->pNext->pPrev = (__pNode);  \
(__pHead)->pNext = (__pNode);  \
}

/* Inserts an item as the last in the list */
#define DLListInsertTail(__pHead, __pNode) {  \
psAssert((__pHead) != (__pNode));  \
(__pNode)->pNext = (__pHead);  \
(__pNode)->pPrev = (__pHead)->pPrev;  \
(__pHead)->pPrev->pNext = (__pNode);  \
(__pHead)->pPrev = (__pNode);  \
}

/* Detaches node from current position in the list. */
#define DLListRemove(__pNode) {  \
(__pNode)->pNext->pPrev = (__pNode)->pPrev;  \
(__pNode)->pPrev->pNext = (__pNode)->pNext;  \
}

#define DLListIsEmpty(__pHead) ((__pHead)->pNext == (__pHead))

/*
	Gets the pointer to the containing object given the DLList pointer,
	the type name of the struct, and the name of the DLList field within
	the struct.
 */
#define DLListGetContainer(__pDLList, __ContainerType, __DLListFieldName)  \
((__ContainerType *)((char *)(__pDLList) -  \
(long)(&((__ContainerType *)0)->__DLListFieldName)))

/*
	Detaches first list item after the head and returns a pointer to it.
	List must not be empty.
 */
#define DLListGetHead(__pHead)  \
(__pHead)->pNext;  \
(__pHead)->pNext->pNext->pPrev = (__pHead);  \
(__pHead)->pNext = (__pHead)->pNext->pNext;

/*
	Detaches list list item before the head and returns a pointer to it.
	List must not be empty.
 */
#define DLListGetTail(__pHead)  \
(__pHead)->pPrev;  \
(__pHead)->pPrev->pPrev->pNext = (__pHead);  \
(__pHead)->pPrev = (__pHead)->pPrev->pPrev;

/******************************************************************************/
/*
	Simpler, single linked list
*/
typedef struct psList {
	unsigned char	*item;
	uint32			len;
	struct psList	*next;
} psList_t;

#endif /* _h_PS_COREUTIL */
/******************************************************************************/

