#ifndef Price_h
#define Price_h

#include <pthread.h>
#include <time.h>
#include <sys/time.h>

class Price {

public:

	int getPrice();

	void setPrice(int newPrice);

	/**
	 *  Constructor. Initializes mutexes
	 */
	Price();

	/**
	 *  A blocking function to wait for price change. We lock the price mutex read it, if it has changed we return, else we wait for the change condition
	 */
	int waitPriceChange(int priceFrom);

private:

	/**
	 *  Condition about changing price
	 */
	pthread_cond_t *priceChanged;

	/**
	 *  mutex allowing to change the price
	 */
	pthread_mutex_t *mutex; //TODO: Change
	int price;

};

#endif // Price_h
