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
#ifndef Market_h
#define Market_h
#include <time.h>	
#include <assert.h>
#include <sys/time.h>
#include "Log.h"
#include "Order.h"
#include "Price.h"
#include "Comparators.cpp"
class OrderList;

class FIFOQueue;
template<typename cmp> class PriceHeap;

#include "TransactionPerformer.h"
#include "StopOrderMonitor.h"
#include "TransactionRouter.h"
#include "OrderProducer.h"

struct cancelArg {
	Market *oMarket;
	int id;
};

class Market {

public:

	/**
	 *  Starts the simulation and contains all the objects and lists (or references to them)
	 */
	void start();

	long getTimestamp();

	static void *startOrderProducer(void *oMarket);
	static void *startTransactionRouter(void *oMarket);
	static void *startStopOrderMonitor(void *oMarket);
	static void *startTransactionPerformer(void *oMarket);
	static void *startCancelThread(void *cancelArguments);

public:

	TransactionPerformer myTransactionPerformer;

	StopOrderMonitor myStopOrderMonitor;

	FIFOQueue *incomingQueue;

	FIFOQueue *activeQueue;

	TransactionRouter myTransactionRouter;

	PriceHeap<MaxCmp> *buyLimitList;

	PriceHeap<MinCmp> *sellLimitList;

	PriceHeap<MaxCmp> *buyStopList;

	PriceHeap<MinCmp> *sellStopList;

	FIFOQueue *marketBuyList;

	FIFOQueue *marketSellList;

	Price currentPrice;

	OrderProducer myOrderProducer;

	Log myLog;

	struct timeval startwtime, endwtime;

	pthread_mutex_t *memMutex;

	Order *allocOrder();

	void deleteOrder(Order *oOrder);

};

#include "PriceHeap.h"
#include "FIFOQueue.h"

#endif // Market_h
