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
#ifndef OrderList_h
#define OrderList_h

#include "Order.h"
#define LISTSIZE 10000
#include <pthread.h>

class Market;

class OrderList {
public:
	OrderList() {
	}
	;
	void initialize(Market *currentMarket, char cPopStatus, char cDeleteStatus);
	void lockList();
	void unlockList();
	virtual bool cancelOrder(int id)=0;
	void waitNotEmpty();
	virtual int getSize()=0;
	virtual Order* peek(int position, bool forcePop)=0;
	virtual void addOrder(Order *oOrder)=0;

protected:
	pthread_mutex_t *mutex;
	pthread_cond_t *notFull;
	pthread_cond_t *notEmpty;

	Order *list[LISTSIZE];
	char popOrderStatus, deleteOrderStatus;
	void handlePoppedOrder(Order *oOrder);

	Market *myMarket;

};

#include "Market.h"

#endif // OrderList_h
