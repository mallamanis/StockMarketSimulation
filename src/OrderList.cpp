#include "OrderList.h"

void OrderList::initialize(Market *currentMarket, char cPopStatus,
		char cDeleteStatus) {
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);

	notFull = new pthread_cond_t;
	notEmpty = new pthread_cond_t;
	pthread_cond_init(notFull, NULL);
	pthread_cond_init(notEmpty, NULL);

	popOrderStatus = cPopStatus;
	deleteOrderStatus = cDeleteStatus;

	myMarket = currentMarket;
}

void OrderList::waitNotEmpty() {
	pthread_mutex_lock(mutex);
	if (getSize() == 0) {
		pthread_cond_wait(notEmpty, mutex);
	}
	pthread_mutex_unlock(mutex);
}

void OrderList::lockList() {
	pthread_mutex_lock(mutex);
}

void OrderList::unlockList() {
	pthread_mutex_unlock(mutex);
}

void OrderList::handlePoppedOrder(Order *oOrder) {
	if (oOrder->status == deleteOrderStatus) {
		myMarket->deleteOrder(oOrder);
	} else {
		oOrder->status = popOrderStatus;
	}

}
