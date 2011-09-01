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
#ifndef TransactionRouter_h
#define TransactionRouter_h

#include <iostream>
#include "Order.h"

#include "Comparators.cpp"

class Market;
class OrderList;

/**
 *  This object contains the threads that are responsible for routing incoming threads to the corresponding queues and heaps
 */
class TransactionRouter {

public:

	void routerThread(Market *oMarket);

	void cancelThread(int orderId);

	/**
	 *  adds a stop order to the corresponding heaps
	 */
	void addStopOrder(Order *oOrder);

	/**
	 *  converts a stop order (given as an arg) to a corresponding (market or limit) order. It then pushes the order at the end of the active linked list and to the corresponding heaps
	 */
	void convertStopOrder(Order *oOrder);

	/**
	 *  adds an active Market order to the corresponding queue
	 */
	void addMarketOrder(Order *oOrder);

	/**
	 *  add a limit order at the corresponding heap
	 */
	void addLimitOrder(Order *oOrder);

public:

	OrderList *incomingQueue;
	OrderList *activeQueue;
	OrderList *marketBuyList;
	OrderList *marketSellList;
	OrderList *buyLimitList;
	OrderList *sellLimitList;
	OrderList *buyStopList;
	OrderList *sellStopList;

};

#include "Market.h"
#include "OrderList.h"

#endif // TransactionRouter_h
