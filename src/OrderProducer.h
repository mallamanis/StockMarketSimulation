#ifndef OrderProducer_h
#define OrderProducer_h

#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include "Price.h"
#include "Order.h"

class FIFOQueue;
class Market;

class OrderProducer {

public:

	void producerThread(Market *oMarket);

private:

	Market *myMarket;
	FIFOQueue *incomingQueue;
};

#include "FIFOQueue.h"
#include "Market.h"

#endif // OrderProducer_h
