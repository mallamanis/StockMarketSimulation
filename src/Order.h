#ifndef Order_h
#define Order_h

#include <stdio.h>
#define STATUS_INCOMING 0
#define STATUS_ACTIVE 1
#define STATUS_TRANSACTED_FROM_LIST 2
#define STATUS_TRANSACTED_FROM_TIME_ORDER 3
#define STATUS_CANCELED 4
#define STATUS_DELETED 5
#define STATUS_INVALID 100

/**
 *  Represents a single order. This is the order's only instance in the program. The objects are referenced from here.
 */
class Order {

public:

	/**
	 *  returns true if the order is a Buy order, false otherwise
	 */
	bool isBuy();

	void display();

public:

	long id;
	long timestamp;
	int vol;
	int price1, price2;
	char action, type;
	char status;

};

#endif // Order_h
