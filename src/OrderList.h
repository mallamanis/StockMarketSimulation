#ifndef OrderList_h
#define OrderList_h

#include "Order.h"
#define LISTSIZE 10000
#include <pthread.h>

class Market;

class OrderList {
public:
	OrderList() {
	}
	;
	void initialize(Market *currentMarket, char cPopStatus, char cDeleteStatus);
	void lockList();
	void unlockList();
	virtual bool cancelOrder(int id)=0;
	void waitNotEmpty();
	virtual int getSize()=0;
	virtual Order* peek(int position, bool forcePop)=0;
	virtual void addOrder(Order *oOrder)=0;

protected:
	pthread_mutex_t *mutex;
	pthread_cond_t *notFull;
	pthread_cond_t *notEmpty;

	Order *list[LISTSIZE];
	char popOrderStatus, deleteOrderStatus;
	void handlePoppedOrder(Order *oOrder);

	Market *myMarket;

};

#include "Market.h"

#endif // OrderList_h
