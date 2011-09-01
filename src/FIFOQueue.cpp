#include "FIFOQueue.h"

/**
 *  Implements a simple queue where the the elements come in a timely order and get out at the same order
 */

void FIFOQueue::initialize(Market *currentMarket, char cPopStatus,
		char cDeleteStatus) {
	empty = 1;
	full = 0;
	head = 0;
	tail = 0;
	OrderList::initialize(currentMarket, cPopStatus, cDeleteStatus);

}

Order *FIFOQueue::peek(int position, bool forcePop)
// section -64--88-1-100-188704ec:129357a59f8:-8000:0000000000001297 begin
{
	Order* front;
	pthread_mutex_lock(mutex);
	if (position == 0) { //if we peek at the top, then start popping...

		_cleanTop();
	}

	assert(position<getSize()|| empty);

	position += head; //Find position
	if (position >= LISTSIZE)
		position %= LISTSIZE;

	if (!empty) {
		front = list[position];
		pthread_cond_broadcast(notEmpty);
	} else {
		front = 0;
	}
	if (!full)
		pthread_cond_broadcast(notFull);

	if (forcePop) {
		_removeTop();
	}

	pthread_mutex_unlock(mutex);
	return front;
}
// section -64--88-1-100-188704ec:129357a59f8:-8000:0000000000001297 end

void FIFOQueue::_cleanTop() {
	while (!empty) {
		if (list[head]->vol > 0)
			return;
		assert(list[head]->vol==0);
		_removeTop();
	}
}

void FIFOQueue::_removeTop() {
	if (empty)
		return;
	Order *poppedOrder;
	poppedOrder = list[head];

	list[head] = 0;
	head++;
	if (head == LISTSIZE)
		head = 0;
	if (head == tail)
		empty = 1;

	full = 0;
	pthread_cond_broadcast(notFull);

	handlePoppedOrder(poppedOrder);

}

void FIFOQueue::addOrder(Order *oOrder)
// section -64--88-1-100-188704ec:129357a59f8:-8000:00000000000012A3 begin
{
	pthread_mutex_lock(mutex);

	if (full) {
		_cleanTop();
		_compactQueue(); //try to compact first..

	}
	while (full) {
		pthread_cond_wait(notFull, mutex);
	}
	assert(!full);

	list[tail] = oOrder;
	tail++;
	empty = 0;
	pthread_cond_broadcast(notEmpty);
	if (tail == LISTSIZE)
		tail = 0;
	if (tail == head)
		full = 1;

	pthread_mutex_unlock(mutex);
}
// section -64--88-1-100-188704ec:129357a59f8:-8000:00000000000012A3 end

int FIFOQueue::getSize() {

	//std::cout<<"head"<<head<<"tail"<<tail<<"empty"<<empty;
	//assert((empty==1 && ((tail-head==0)||(LISTSIZE-(head-tail)==0)))||(empty==0 && head!=tail));
	assert(empty==1?((tail-head==0)||(LISTSIZE-(head-tail)==0)):true);
	if (head <= tail && !full)
		return tail - head;
	else
		return LISTSIZE - (head - tail);

}

bool FIFOQueue::cancelOrder(int id) {
	pthread_mutex_lock(mutex);
	if (getSize() > 0) {
		int lastPosition = tail;
		if (tail <= head)
			lastPosition += LISTSIZE;

		for (int i = head; i < lastPosition; i++) { //Delete any transacted...
			if (list[i % LISTSIZE]->id == id) {
				list[i % LISTSIZE]->status = STATUS_CANCELED;
				pthread_mutex_unlock(mutex);
				return true;
			}
		}
	}
	pthread_mutex_unlock(mutex);
	return false;
}

void FIFOQueue::_compactQueue() {
	int lastPosition = tail;
	if (tail <= head)
		lastPosition += LISTSIZE;

	for (int i = head; i < lastPosition; i++) { //Delete any transacted...
		if (list[i % LISTSIZE]->vol == 0) {
			Order *o = list[i % LISTSIZE];
			list[i % LISTSIZE] = 0;
			handlePoppedOrder(o);
		}
	}

	int zeros = 0;

	for (int i = head; i < lastPosition; i++) { //Move to cover "blanks"...
		if (list[i % LISTSIZE] == 0) {
			zeros++;
		} else {
			list[(i % LISTSIZE) - zeros] = list[i % LISTSIZE];
		}
	}
	tail = (lastPosition - zeros) % LISTSIZE;
	if (zeros > 0)
		full = 0;
	std::cout << "zeros:" << zeros;
	pthread_cond_broadcast(notFull);
}

