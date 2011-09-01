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
