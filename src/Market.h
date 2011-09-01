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
