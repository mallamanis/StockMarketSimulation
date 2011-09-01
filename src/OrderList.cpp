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
#include "OrderList.h"

void OrderList::initialize(Market *currentMarket, char cPopStatus,
		char cDeleteStatus) {
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);

	notFull = new pthread_cond_t;
	notEmpty = new pthread_cond_t;
	pthread_cond_init(notFull, NULL);
	pthread_cond_init(notEmpty, NULL);

	popOrderStatus = cPopStatus;
	deleteOrderStatus = cDeleteStatus;

	myMarket = currentMarket;
}

void OrderList::waitNotEmpty() {
	pthread_mutex_lock(mutex);
	if (getSize() == 0) {
		pthread_cond_wait(notEmpty, mutex);
	}
	pthread_mutex_unlock(mutex);
}

void OrderList::lockList() {
	pthread_mutex_lock(mutex);
}

void OrderList::unlockList() {
	pthread_mutex_unlock(mutex);
}

void OrderList::handlePoppedOrder(Order *oOrder) {
	if (oOrder->status == deleteOrderStatus) {
		myMarket->deleteOrder(oOrder);
	} else {
		oOrder->status = popOrderStatus;
	}

}
