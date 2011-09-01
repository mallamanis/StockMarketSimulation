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
