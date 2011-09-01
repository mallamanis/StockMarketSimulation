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
#ifndef TransactionPerformer_h
#define TransactionPerformer_h

#include "Order.h"
#include "Log.h"
#include "Price.h"
#include <iostream>

class OrderList;
class Market;

class TransactionPerformer {

public:

	/**
	 *  the transaction performer, performs transactions by taking the first of the activeQueue checking the Queues (first the heap and then the market list). Then moves the orders to the deletedQueue.
	 *  During all checks at a top of a heap if an order is already deleted and deletes it...
	 */
	void transactionThread(Market *oMarket);

	/**
	 *  checks if the order is valid (i.e. there are stocks to exchange and not cancelled)
	 */
	bool isValidOrder(Order *oOrder);

	bool transactMarket(OrderList *oMainList, OrderList *oMarketList,
			int active, int activeSize);

	bool transactLimit(OrderList *oMainList, OrderList *oMarketList,
			int active, int activeSize);

private:

	OrderList *activeQueue;
	OrderList *marketBuyList;
	OrderList *marketSellList;
	OrderList *buyLimitList;
	OrderList *sellLimitList;
	Log *marketLog;
	Price *marketPrice;
	Market *currentMarket;

};

#include "OrderList.h"
#include "Market.h"

#endif // TransactionPerformer_h
