/*
 *	Copyright (C) 2009 by Miltiadis Allamanis
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */
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
