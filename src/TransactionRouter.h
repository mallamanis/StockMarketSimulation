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
