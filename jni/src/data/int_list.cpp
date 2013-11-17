#include "int_list.h"
#include <stdio.h>
#include <stdlib.h>
IntList::IntList()
{
	//哑节点
	pHead = (ListNode *)malloc(sizeof(ListNode *));
	pHead->mData = 0;
	pHead->pNext = NULL;

	pTail = pHead;
	len = 0;
}
IntList::~IntList()
{
	int i = 0;
	while(pHead != NULL)
	{
		ListNode *tmp = pHead->pNext;
		free(pHead);
		pHead = tmp;
		i++;
	}
	len = 0;
}
void IntList::insert(int data)
{
	ListNode *pNew = (ListNode *)malloc(sizeof(ListNode*));
	pNew->mData = data;
	pNew->pNext = NULL;
	pTail->pNext = pNew;
	pTail = pNew;
	len++;
}

const ListNode *IntList::getHead()
{
	return pHead->pNext;
}

const ListNode *IntList::getTail()
{
	return pTail;
}

int IntList::getLength()
{
	return len;
}
