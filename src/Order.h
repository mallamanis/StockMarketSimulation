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
