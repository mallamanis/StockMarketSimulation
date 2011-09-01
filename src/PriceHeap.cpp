/*
 *	Copyright (C) 2009 by Miltiadis Allamanis
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */
#include "PriceHeap.h"

/**
 *  Acts as a wrapper around STL Heap
 */
template<typename cmp>
void PriceHeap<cmp>::initialize(Market *currentMarket, char cPopStatus,
		char cDeleteStatus) {
	numberOfElements = -1;
	OrderList::initialize(currentMarket, cPopStatus, cDeleteStatus);
}

template<typename cmp>
Order *PriceHeap<cmp>::peek(int position, bool forcePop)
// section -64--88-1-100-188704ec:129357a59f8:-8000:000000000000129B begin
{
	Order *ord = 0;
	pthread_mutex_lock(mutex);
	_cleanTop();
	ord = list[0];
	assert(numberOfElements<0?ord==0:true);
	if (forcePop) {
		_removeTop();
	}
	pthread_mutex_unlock(mutex);
	return ord;
}
// section -64--88-1-100-188704ec:129357a59f8:-8000:000000000000129B end

template<typename cmp>
void PriceHeap<cmp>::_cleanTop() {
	while (numberOfElements > -1) {
		if (list[0]->vol > 0)
			return;
		assert(list[0]->vol==0);
		_removeTop();
	}

}

template<typename cmp>
void PriceHeap<cmp>::_removeTop() {
	Order *poppedOrder;

	std::pop_heap(list, list + numberOfElements + 1, myComparator);//TODO Add compare
	numberOfElements--;
	pthread_cond_broadcast(notFull);
	poppedOrder = list[numberOfElements + 1];
	list[numberOfElements + 1] = 0;
	handlePoppedOrder(poppedOrder);
}

template<typename cmp>
void PriceHeap<cmp>::addOrder(Order *oOrder)
// section -64--88-1-100-188704ec:129357a59f8:-8000:00000000000012A9 begin
{
	assert(oOrder!=0);
	pthread_mutex_lock(mutex);
	if (numberOfElements == LISTSIZE - 1) {
		_cleanTop();
		_compactQueue();
	}
	if (numberOfElements == LISTSIZE - 1) {
		pthread_cond_wait(notFull, mutex);
	}
	numberOfElements++;
	list[numberOfElements] = oOrder;

	std::push_heap(list, list + numberOfElements + 1, myComparator);
	pthread_mutex_unlock(mutex);
}
// section -64--88-1-100-188704ec:129357a59f8:-8000:00000000000012A9 end

template<typename cmp>
int PriceHeap<cmp>::getSize() {
	return numberOfElements + 1;
}

template<typename cmp>
bool PriceHeap<cmp>::cancelOrder(int id) {
	pthread_mutex_lock(mutex);
	int lastPosition = numberOfElements;

	for (int i = 0; i <= lastPosition; i++) { //Delete any transacted...
		if (list[i]->id == id) {
			list[i]->status = STATUS_CANCELED;
			pthread_mutex_unlock(mutex);
			return true;
		}
	}
	pthread_mutex_unlock(mutex);
	return false;
}
template<typename cmp>
void PriceHeap<cmp>::_compactQueue() {
	int lastPosition = numberOfElements;

	for (int i = 0; i <= lastPosition; i++) { //Delete any transacted...
		if (list[i]->vol == 0) {
			Order *o = list[i];
			list[i] = 0;
			handlePoppedOrder(o);
		}
	}

	int zeros = 0;

	for (int i = 0; i <= lastPosition; i++) { //Move to cover "blanks"...
		if (list[i] == 0) {
			zeros++;
		} else {
			list[i - zeros] = list[i];
		}
	}
	numberOfElements = lastPosition - zeros;
	if (zeros > 0) {
		make_heap(list, list + numberOfElements + 1, myComparator);
		pthread_cond_broadcast(notFull);
	}
}
