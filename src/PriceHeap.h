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
