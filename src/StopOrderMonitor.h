#ifndef StopOrderMonitor_h
#define StopOrderMonitor_h

class Market;
class OrderList;

class StopOrderMonitor {

public:

	void stopMonitorThread(Market *oMarket);

public:

	OrderList *SellStopList;

	OrderList *BuyStopList;

};

#include "Market.h"
#include "OrderList.h"

#endif // StopOrderMonitor_h
