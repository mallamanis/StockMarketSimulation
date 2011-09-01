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
#include "Log.h"

/**
 *  Logs the various instances
 */

/** 
 *  opens the log and initializes the mutex
 */
void Log::openLog()
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F7F begin
{
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);
	filePt.open("transactions.log");
	if (!filePt) {
		std::cout << "Error creating log";
	}
	recordsWritter = 0;
}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F7F end


/** 
 *  Adds a record to the file pointer and periodically flushes the file
 */
void Log::logTransaction(long timestamp, int price, int volume, long orderid1,
		long orderid2)
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F81 begin
{
	pthread_mutex_lock(mutex);
	filePt << timestamp << " " << price << " " << volume << " " << orderid1
			<< " " << orderid2 << "\n";
	recordsWritter++;
	if (recordsWritter > 0) {
		filePt.flush();
		recordsWritter = 0;
	}
	pthread_mutex_unlock(mutex);

}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000F81 end

Log::~Log() {
	filePt.close();
	pthread_mutex_destroy(mutex);
}

