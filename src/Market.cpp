#include "Market.h"
#include "PriceHeap.cpp"

long Market::getTimestamp() {

	gettimeofday(&endwtime, NULL);

	return ((double) ((endwtime.tv_usec - startwtime.tv_usec) / 1.0e6
			+ endwtime.tv_sec - startwtime.tv_sec) * 1000);
}

/** 
 *  Starts the simulation and contains all the objects and lists (or references to them)
 */
void Market::start()
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000EA9 begin
{
	myLog.openLog();
	currentPrice.setPrice(1000);
	gettimeofday(&startwtime, NULL);
	pthread_t orderProducerThread, transactionRouterThread,
			stopOrderMonitorThread, transactionPerformer;
	memMutex = new pthread_mutex_t;
	pthread_mutex_init(memMutex, NULL);
	incomingQueue = new FIFOQueue();
	incomingQueue->FIFOQueue::initialize(this, STATUS_ACTIVE, STATUS_INVALID);
	activeQueue = new FIFOQueue();
	activeQueue->FIFOQueue::initialize(this, STATUS_TRANSACTED_FROM_TIME_ORDER,
			STATUS_TRANSACTED_FROM_LIST);
	marketBuyList = new FIFOQueue();
	marketBuyList->FIFOQueue::initialize(this, STATUS_TRANSACTED_FROM_LIST,
			STATUS_TRANSACTED_FROM_TIME_ORDER);
	marketSellList = new FIFOQueue();
	marketSellList->FIFOQueue::initialize(this, STATUS_TRANSACTED_FROM_LIST,
			STATUS_TRANSACTED_FROM_TIME_ORDER);

	sellLimitList = new PriceHeap<MinCmp> ();
	sellLimitList->PriceHeap::initialize(this, STATUS_TRANSACTED_FROM_LIST,
			STATUS_TRANSACTED_FROM_TIME_ORDER);

	buyLimitList = new PriceHeap<MaxCmp> ();
	buyLimitList->PriceHeap::initialize(this, STATUS_TRANSACTED_FROM_LIST,
			STATUS_TRANSACTED_FROM_TIME_ORDER);

	sellStopList = new PriceHeap<MinCmp> ();
	sellStopList->PriceHeap::initialize(this, STATUS_ACTIVE, STATUS_INVALID);

	buyStopList = new PriceHeap<MaxCmp> ();
	buyStopList->PriceHeap::initialize(this, STATUS_ACTIVE, STATUS_INVALID);

	pthread_create(&transactionRouterThread, NULL,
			Market::startTransactionRouter, this);
	pthread_create(&orderProducerThread, NULL, Market::startOrderProducer, this);
	pthread_create(&stopOrderMonitorThread, NULL,
			Market::startStopOrderMonitor, this);
	pthread_create(&transactionPerformer, NULL,
			Market::startTransactionPerformer, this);

	//Infinetly wait to join
	pthread_join(orderProducerThread, NULL);
	pthread_join(transactionRouterThread, NULL);
	pthread_join(stopOrderMonitorThread, NULL);
	pthread_join(transactionPerformer, NULL);
	assert(false); //We should never reach this point!

}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000EA9 end


void *Market::startOrderProducer(void *oMarket)
// section 127-0-1-1--cdf7e0f:12935813eb8:-8000:000000000000129E begin
{
	((Market*) oMarket)->myOrderProducer.producerThread((Market*) oMarket);
	assert(false); //We should never reach this point!
	pthread_exit(NULL);
	return 0;
}
// section 127-0-1-1--cdf7e0f:12935813eb8:-8000:000000000000129E end

void *Market::startTransactionRouter(void *oMarket) {
	((Market*) oMarket)->myTransactionRouter.routerThread((Market*) oMarket);
	assert(false); //We should never reach this point!
	pthread_exit(NULL);
	return 0;
}

void *Market::startStopOrderMonitor(void *oMarket) {
	((Market*) oMarket)->myStopOrderMonitor.stopMonitorThread((Market*) oMarket);
	assert(true); //We should never reach this point!
	pthread_exit(NULL);
	return 0;
}

void *Market::startTransactionPerformer(void *oMarket) {
	((Market*) oMarket)->myTransactionPerformer.transactionThread(
			(Market*) oMarket);
	assert(false); //We should never reach this point!
	pthread_exit(NULL);
	return 0;
}

void *Market::startCancelThread(void *cancelArguments) {
	cancelArg *arguments = (cancelArg*) cancelArguments;
	((Market*) arguments->oMarket)->myTransactionRouter.cancelThread(
			arguments->id);
	pthread_exit(NULL);
	return 0;
}

Order *Market::allocOrder() {
	Order *newOrder;
	pthread_mutex_lock(memMutex);
	newOrder = new Order;
	assert(newOrder!=0);
	newOrder->status = STATUS_ACTIVE;
	pthread_mutex_unlock(memMutex);
	return newOrder;
}

void Market::deleteOrder(Order *oOrder) {
	pthread_mutex_lock(memMutex);
	oOrder->status = STATUS_DELETED;
	delete oOrder;
	pthread_mutex_unlock(memMutex);
}
