#ifndef Comparators_h
#define Comparators_h

#include "Order.h"

struct MinCmp {
	inline bool operator()(const Order* order1, const Order* order2) {
		if (order1->price1 < order2->price1)
			return true; //TODO: Add comparizon with timestap (older goes to top!)
		else
			return false;
	}
};

struct MaxCmp {
	inline bool operator()(const Order* order1, const Order* order2) {
		if (order1->price1 > order2->price1)
			return true;
		else
			return false;
	}
};

#endif // Comparators_h
