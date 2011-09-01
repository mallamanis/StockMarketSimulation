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
#include "TransactionPerformer.h"

/** 
 *  the transaction performer, performs transactions by taking the first of the activeQueue checking the Queues (first the heap and then the market list). Then moves the orders to the deletedQueue.
 *  During all checks at a top of a heap if an order is already deleted and deletes it...
 */
void TransactionPerformer::transactionThread(Market *oMarket)
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E45 begin
{
	int currentActive = 0;
	int oldActiveSize = 0, newActiveSize = 0;
	activeQueue = (OrderList *) oMarket->activeQueue;
	marketBuyList = (OrderList *) oMarket->marketBuyList;
	marketSellList = (OrderList *) oMarket->marketSellList;
	//std::cout<<"Buy Market: "<<marketBuyList<<" Sell Market: "<<marketSellList<<"\n";
	buyLimitList = (OrderList *) oMarket->buyLimitList;
	sellLimitList = (OrderList *) oMarket->sellLimitList;
	marketLog = &oMarket->myLog;
	marketPrice = &oMarket->currentPrice;
	currentMarket = oMarket;
	while (true) {
		//Pick an order from the active queue
		activeQueue->waitNotEmpty();
		newActiveSize = activeQueue->getSize();
		if (oldActiveSize != newActiveSize || activeQueue->getSize() - 1) { //if new orders have come, or we have reached the end of the queue...
			currentActive = 0;
		}
		Order *topPriorityOrder = 0;
		topPriorityOrder = activeQueue->peek(currentActive, false);
		if (topPriorityOrder == 0)
			continue;
		if (topPriorityOrder->status == STATUS_CANCELED) {
			topPriorityOrder->vol = 0;
			continue;
		}

		assert(activeQueue->getSize()>0);
		assert(currentActive==0?topPriorityOrder->vol>0:true);

		if (topPriorityOrder->vol == 0) {
			currentActive++;
			continue;
		}
		bool transacted = false;
		if (topPriorityOrder->isBuy()) {
			//We want to buy at the lowest price...

			if (topPriorityOrder->type == 'L') {
				if (sellLimitList->peek(0, false) != 0 && sellLimitList->peek(
						0, false)->price1 < topPriorityOrder->price1) {
					transacted = transactLimit(activeQueue, sellLimitList,
							currentActive, oldActiveSize);
				} else {
					transacted = transactMarket(activeQueue, marketSellList,
							currentActive, oldActiveSize);
				}
			} else {
				assert(topPriorityOrder->type=='M');
				if (sellLimitList->peek(0, false) != 0 && sellLimitList->peek(
						0, false)->price1 < marketPrice->getPrice()) {
					transacted = transactLimit(activeQueue, sellLimitList,
							currentActive, oldActiveSize);
					if (!transacted)
						transacted = transactMarket(activeQueue,
								marketSellList, currentActive, oldActiveSize);
				} else {
					transacted = transactMarket(activeQueue, marketSellList,
							currentActive, oldActiveSize);
					if (!transacted)
						transacted = transactLimit(activeQueue, sellLimitList,
								currentActive, oldActiveSize);
				}
			}
			//then (if not good), look at the market list			
			if (!transacted) {
				currentActive++;
			} else {
				currentActive = 0;
			}

		} else {
			//We want to sell at the lowest price
			if (topPriorityOrder->type == 'L') {
				if (buyLimitList->peek(0, false) != 0 && buyLimitList->peek(0,
						false)->price1 > topPriorityOrder->price1) {
					transacted = transactLimit(activeQueue, buyLimitList,
							currentActive, oldActiveSize);
				} else {
					transacted = transactMarket(activeQueue, marketBuyList,
							currentActive, oldActiveSize);
				}
			} else {
				assert(topPriorityOrder->type=='M');
				if (buyLimitList->peek(0, false) != 0 && buyLimitList->peek(0,
						false)->price1 > marketPrice->getPrice()) {
					transacted = transactLimit(activeQueue, buyLimitList,
							currentActive, oldActiveSize);
					if (!transacted)
						transacted = transactMarket(activeQueue, marketBuyList,
								currentActive, oldActiveSize);
				} else {
					transacted = transactMarket(activeQueue, marketBuyList,
							currentActive, oldActiveSize);
					if (!transacted)
						transacted = transactLimit(activeQueue, buyLimitList,
								currentActive, oldActiveSize);
				}
			}
			//then (if no good), look at the market list
			if (!transacted) {
				currentActive++;
				//if(activeQueue->peekTop(currentActive)!=0) std::cout<<"Could not transact S->B"<<activeQueue->peekTop(currentActive)->id<<" with "<<marketBuyList->peekTop(0)->id<<"\n";
			} else {
				currentActive = 0;
			}
		}

	}

}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E45 end


/** 
 *  checks if the order is valid (i.e. there are stocks to exchange and not cancelled)
 */
bool TransactionPerformer::isValidOrder(Order *oOrder)
// section -64--88-13-1-4137f8f2:129306551b9:-8000:000000000000127F begin
{
	return false;
}
// section -64--88-13-1-4137f8f2:129306551b9:-8000:000000000000127F end


bool TransactionPerformer::transactMarket(OrderList *oMainList,
		OrderList *oMarketList, int active, int activeSize)
// section -64--88-13-1-4137f8f2:129306551b9:-8000:0000000000001283 begin
{
	//std::cout<<active<<"h\n";
	Order *topPriorityOrder = oMainList->peek(active, false);
	if (topPriorityOrder == 0)
		return false;
	Order *transactionPair = 0;

	transactionPair = oMarketList->peek(0, false);
	if (transactionPair == 0)
		return false;
	while (transactionPair->status == STATUS_CANCELED) {
		transactionPair->vol = 0;
		transactionPair = oMarketList->peek(0, false);
		if (transactionPair == 0)
			return false;
	}
	if (oMainList->getSize() != activeSize && active > 0)
		return false;

	assert(transactionPair->id!=topPriorityOrder->id);

	//Set new price
	if (topPriorityOrder->type == 'L') {
		marketPrice->setPrice(topPriorityOrder->price1);
	}

	if (transactionPair->vol > topPriorityOrder->vol) {
		transactionPair->vol -= topPriorityOrder->vol;
		marketLog->logTransaction(currentMarket->getTimestamp(),
				marketPrice->getPrice(), topPriorityOrder->vol,
				topPriorityOrder->id, transactionPair->id);
		topPriorityOrder->vol = 0;

	} else {
		topPriorityOrder->vol -= transactionPair->vol;
		marketLog->logTransaction(currentMarket->getTimestamp(),
				marketPrice->getPrice(), transactionPair->vol,
				topPriorityOrder->id, transactionPair->id);
		transactionPair->vol = 0;

	}
	//std::cout<<"OK!"<<topPriorityOrder->id<<"\n";
	return true;

}
// section -64--88-13-1-4137f8f2:129306551b9:-8000:0000000000001283 end

bool TransactionPerformer::transactLimit(OrderList *oMainList,
		OrderList *oLimitHeap, int active, int activeSize) {

	Order *topPriorityOrder = oMainList->peek(active, false);
	if (topPriorityOrder == 0)
		return false;

	Order *transactionPair = 0;
	transactionPair = oLimitHeap->peek(0, false);

	if (transactionPair == 0)
		return false;
	while (transactionPair->status == STATUS_CANCELED) {
		transactionPair->vol = 0;
		transactionPair = oLimitHeap->peek(0, false);
		if (transactionPair == 0)
			return false;
	}
	if (oMainList->getSize() != activeSize && active > 0)
		return false;
	//TODO:Check canceled...
	assert(transactionPair->type=='L');
	assert((transactionPair->action=='B' && topPriorityOrder->action=='S')||(transactionPair->action=='S' && topPriorityOrder->action=='B') );
	//Perform Transaction

	//Set new price
	if (topPriorityOrder->type == 'M') {
		marketPrice->setPrice(transactionPair->price1);
	} else {
		assert(topPriorityOrder->type=='L');
		marketPrice->setPrice(
				(transactionPair->price1 + topPriorityOrder->price1) / 2);
	}

	if (transactionPair->vol > topPriorityOrder->vol) {
		transactionPair->vol -= topPriorityOrder->vol;

		marketLog->logTransaction(currentMarket->getTimestamp(),
				marketPrice->getPrice(), topPriorityOrder->vol,
				topPriorityOrder->id, transactionPair->id);
		topPriorityOrder->vol = 0;
	} else {
		topPriorityOrder->vol -= transactionPair->vol;

		marketLog->logTransaction(currentMarket->getTimestamp(),
				marketPrice->getPrice(), transactionPair->vol,
				topPriorityOrder->id, transactionPair->id);
		transactionPair->vol = 0;
	}
	//std::cout<<"OK!"<<topPriorityOrder->id<<"\n";
	return true;

}
