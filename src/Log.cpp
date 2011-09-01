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

