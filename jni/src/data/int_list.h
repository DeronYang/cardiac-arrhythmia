#ifndef INT_LIST_H
#define INT_LIST_H

struct ListNode
{
	int mData;
	ListNode *pNext;
};
class IntList
{
	private:
		ListNode *pHead;
		ListNode *pTail;
		int len;

	public:
		IntList();
		~IntList();
		void insert(int data);
		const ListNode *getHead();
		int getLength();
};


#endif
