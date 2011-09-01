#include "Price.h"

int Price::getPrice()
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E22 begin
{
	return price;
}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E22 end


void Price::setPrice(int newPrice)
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E24 begin
{
	pthread_mutex_lock(mutex);
	if (newPrice < 0) {
		price = 0;
	} else {
		price = newPrice;
	}
	pthread_cond_broadcast(priceChanged);
	pthread_mutex_unlock(mutex);
}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E24 end


/** 
 *  Constructor. Initializes mutexes
 */
Price::Price()
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F74 begin
{
	//**Initialize Mutex
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);

	//**Initialize Condition
	priceChanged = new pthread_cond_t;
	pthread_cond_init(priceChanged, NULL);
}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F74 end


/** 
 *  A blocking function to wait for price change. We lock the price mutex read it, if it has changed we return, else we wait for the change condition
 */
int Price::waitPriceChange(int priceFrom)
// section -64--88-13-1-4137f8f2:129306551b9:-8000:0000000000001231 begin
{
	int currentPrice;
	pthread_mutex_lock(mutex);
	currentPrice = price;
	if (currentPrice == priceFrom) {//if it has not changed since the call
		struct timeval now;
		struct timespec timeout;
		gettimeofday(&now, NULL);
		timeout.tv_sec = now.tv_sec + 1;
		timeout.tv_nsec = now.tv_usec * 1000;
		pthread_cond_timedwait(priceChanged, mutex, &timeout);
		currentPrice = price;
	}
	pthread_mutex_unlock(mutex);
	return currentPrice;
}
// section -64--88-13-1-4137f8f2:129306551b9:-8000:0000000000001231 end

