#ifndef Log_h
#define Log_h

#include <iostream>
#include <fstream>

/**
 *  Logs the various instances
 */
class Log {

private:

	/**
	 *  the file pointer of the actual file
	 */
	std::ofstream filePt;

	/**
	 *  is the number of records written. This is used to know when we should flush the file
	 */
	long recordsWritter;

	/** 
	 *  internal object mutex to allow simulataneous writing to the file
	 */
	pthread_mutex_t *mutex;

public:

	/**
	 *  opens the log and initializes the mutex
	 */
	void openLog();

	/**
	 *  destructor
	 */
	~Log();

	/**
	 *  Adds a record to the file pointer and periodically flushes the file
	 */
	void logTransaction(long timestamp, int price, int volume, long orderid1,
			long orderid2);

};

#endif // Log_h
