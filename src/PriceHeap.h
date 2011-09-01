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
#ifndef PriceHeap_h
#define PriceHeap_h

#include <algorithm>
#include <pthread.h>
#include "OrderList.h"
#include "Comparators.cpp"
/**
 *  Acts as a wrapper around STL Heap
 */

template<typename cmp>
class PriceHeap: public OrderList {

public:
	PriceHeap() {
	}
	;
	void initialize(Market *currentMarket, char cPopStatus, char cDeleteStatus);

	bool cancelOrder(int id);

	int getSize();
	Order* peek(int position, bool forcePop);
	void addOrder(Order *oOrder);
	void _compactQueue();

private:
	int numberOfElements;

	void _removeTop();
	void _cleanTop();
	cmp myComparator;

};

#include "Market.h"

#endif // PriceHeap_h
