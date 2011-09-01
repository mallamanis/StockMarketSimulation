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
#ifndef FIFOQueue_h
#define FIFOQueue_h
#include <pthread.h>
#include <iostream>
#include <assert.h>
#include "Order.h"
#include "OrderList.h"
class Market;

using namespace std;

/**
 *  Implements a simple queue where the the elements come in a timely order and get out at the same order
 */
class FIFOQueue: public OrderList {

public:

	FIFOQueue() :
		OrderList() {
	}
	;
	void initialize(Market *currentMarket, char cPopStatus, char cDeleteStatus);

	//void add(Order *oOrder);
	bool cancelOrder(int id);

	int getSize();
	Order* peek(int position, bool forcePop);
	void addOrder(Order *oOrder);

private:

	int head, tail, full, empty;
	void _removeTop();
	void _cleanTop();

	void _compactQueue();

};

#include "Market.h"

#endif // FIFOQueue_h
