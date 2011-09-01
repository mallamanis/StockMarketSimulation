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
#include "StopOrderMonitor.h"

void StopOrderMonitor::stopMonitorThread(Market *oMarket)
// don't delete the following line as it's needed to preserve source code of this autogenerated element
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E4E begin
{
	OrderList *buyStopList = (OrderList *) oMarket->buyStopList;
	OrderList *sellStopList = (OrderList *) oMarket->sellStopList;
	OrderList *marketBuyList = (OrderList *) oMarket->marketBuyList;
	OrderList *marketSellList = (OrderList *) oMarket->marketSellList;
	OrderList *buyLimitList = (OrderList *) oMarket->buyLimitList;
	OrderList *sellLimitList = (OrderList *) oMarket->sellLimitList;
	OrderList *activeQueue = (OrderList *) oMarket->activeQueue;

	Price *currentPrice = &oMarket->currentPrice;
	int marketPrice = -1;
	Order *temp;
	while (true) {
		marketPrice = currentPrice->waitPriceChange(marketPrice);
		//std::cout<<"Changed...";
		temp = buyStopList->peek(0, false);
		while (temp != 0 && temp->price1 < marketPrice) {
			Order *buyOrder = buyStopList->peek(0, true);
			//Cancelled
			if (buyOrder->status == STATUS_CANCELED) {
				oMarket->deleteOrder(buyOrder);
				continue;
			}
			//Convert & reroute
			buyOrder->price1 = buyOrder->price2;
			assert(buyOrder->action=='B');
			if (buyOrder->type == 'S') {
				buyOrder->type = 'M';
				marketBuyList->addOrder(buyOrder);
			} else {
				assert(buyOrder->type=='T');
				buyOrder->type = 'L';
				buyLimitList->addOrder(buyOrder);
			}

			activeQueue->addOrder(buyOrder);
			temp = buyStopList->peek(0, false);
		}

		temp = sellStopList->peek(0, false);
		while (temp != 0 && temp->price1 > marketPrice) {
			Order *sellOrder = sellStopList->peek(0, true);

			if (sellOrder->status == STATUS_CANCELED) {
				oMarket->deleteOrder(sellOrder);
				continue;
			}

			//Convert & reroute
			sellOrder->price1 = sellOrder->price2;
			assert(sellOrder->action=='S');
			if (sellOrder->type == 'S') {
				sellOrder->type = 'M';
				marketSellList->addOrder(sellOrder);
			} else {
				assert(sellOrder->type=='T');
				sellOrder->type = 'L';
				sellLimitList->addOrder(sellOrder);
			}

			activeQueue->addOrder(sellOrder);
			temp = sellStopList->peek(0, false);
		}

	}

}
// section -64--88-13-1--72469725:1292cfe5a46:-8000:0000000000000E4E end
// don't delete the previous line as it's needed to preserve source code of this autogenerated element
