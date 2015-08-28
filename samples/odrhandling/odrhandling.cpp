/*$TET$odrhandling$!cpp-copyright!*/
/*--------------------------------------------------------------------------*/
/*  Copyright 2015 Sergey Vostokin                                          */
/*                                                                          */
/*  Licensed under the Apache License, Version 2.0 (the "License");         */
/*  you may not use this file except in compliance with the License.        */
/*  You may obtain a copy of the License at                                 */
/*                                                                          */
/*  http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                          */
/*  Unless required by applicable law or agreed to in writing, software     */
/*  distributed under the License is distributed on an "AS IS" BASIS,       */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*  See the License for the specific language governing permissions and     */
/*  limitations under the License.                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*	Business process model : Customer Order procedure                       */
/*                                                                          */
/*	A Customer prepares an Order.                                           */
/*	He / she sends the order to Order Handling.                             */
/*	Order Handling checks the order.                                        */
/*	It sends an order confirmation to the Customer.                         */
/*	The Customer receives the order confirmation.                           */
/*	The Customer waits for the product.                                     */
/*	Order Handling sends a delivery request to Shipment. Shipment receives  */
/*	the request and sends the product delivery to the Customer.             */
/*	The Customer receives the product delivery and checks the delivery.     */
/*                                                                          */
/*	Source: S-BPM in the Wild.Practical Value Creation                      */
/*			http://www.springer.com/de/book/9783319175416 (page 280)        */
/*--------------------------------------------------------------------------*/
/*$TET$*/

/*$TET$odrhandling$!templet!*/
/*
~Customer_Order_handling=
+	PREPARES? order -> CHECKS;
	CHECKS! order_conformation -> RECEIVES; RECEIVES?.

~Order_handling_Shipment=
+	SENDS? delivery_request -> RECIEVES; RECIEVES!.

~Shipment_Customer=
+	SEND? product_delivery -> RECIEVES_AND_CHECKS; RECIEVES_AND_CHECKS!.

*Customer=
	order_handling:Customer_Order_handling ! order_conformation->receive;
	shipment:Shipment_Customer ? product_delivery->recieves_and_checks;

+	prepare()->send; send(order_handling!order);
	receive(order_handling?order_conformation);
	recieves_and_checks(shipment?product_delivery).

*Order_handling=
	customer:Customer_Order_handling ? order->check;
	shipment:Order_handling_Shipment !;

	check(customer?order, customer!order_conformation)->send;
	send(shipment!delivery_request).

*Shipment=
	order_handling:Order_handling_Shipment ?
	delivery_request -> receive;
	customer:Shipment_Customer !;

	receive(order_handling?delivery_request) -> deliver;
	deliver(customer!product_delivery).
*/
/*$TET$*/

#include "odrhandling.h"

/*$TET$odrhandling$!cpp-prologue!*/
#include <iostream>
using namespace std;
#include <math.h>
#include <time.h>
/*$TET$*/
//////////////////////class Customer_Order_handling////////////////////
Customer_Order_handling::Customer_Order_handling(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Channel(a)
{
/*$TET$Customer_Order_handling$!constructor!*/
/*$TET$*/
}

Customer_Order_handling::~Customer_Order_handling()
{
/*$TET$Customer_Order_handling$!destructor!*/
/*$TET$*/
}

//////////////////////class Order_handling_Shipment////////////////////
Order_handling_Shipment::Order_handling_Shipment(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Channel(a)
{
/*$TET$Order_handling_Shipment$!constructor!*/
/*$TET$*/
}

Order_handling_Shipment::~Order_handling_Shipment()
{
/*$TET$Order_handling_Shipment$!destructor!*/
/*$TET$*/
}

//////////////////////class Shipment_Customer////////////////////
Shipment_Customer::Shipment_Customer(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Channel(a)
{
/*$TET$Shipment_Customer$!constructor!*/
/*$TET$*/
}

Shipment_Customer::~Shipment_Customer()
{
/*$TET$Shipment_Customer$!destructor!*/
/*$TET$*/
}

//////////////////////class Customer////////////////////
Customer::Customer(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$Customer$!constructor!*/
/*$TET$*/
}

Customer::~Customer()
{
/*$TET$Customer$!destructor!*/
/*$TET$*/
}

/*$TET$Customer$!usercode!*/
/*$TET$*/

bool Customer::prepare()
{
/*$TET$Customer$prepare*/
		cout << "\tA Customer prepares an Order." << endl;
		return true;
/*$TET$*/
}

bool Customer::send(/*out*/Customer_Order_handling::order*p1)
{
/*$TET$Customer$send*/
		cout << "\tHe/she sends the order to Order Handling." << endl;
		return true;
/*$TET$*/
}

bool Customer::receive(/*in*/Customer_Order_handling::order_conformation*p1)
{
/*$TET$Customer$receive*/
		cout << "\tThe Customer receives the order confirmation." << endl;
		return true;
/*$TET$*/
}

bool Customer::recieves_and_checks(/*in*/Shipment_Customer::product_delivery*p1)
{
/*$TET$Customer$recieves_and_checks*/
		cout << "\tThe Customer receives the product delivery and checks the delivery." << endl;
		return true;
/*$TET$*/
}

void Customer::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$Customer$!run!*/
		cout << "Customer:" << endl;
/*$TET$*/

}

//////////////////////class Order_handling////////////////////
Order_handling::Order_handling(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$Order_handling$!constructor!*/
/*$TET$*/
}

Order_handling::~Order_handling()
{
/*$TET$Order_handling$!destructor!*/
/*$TET$*/
}

/*$TET$Order_handling$!usercode!*/
/*$TET$*/

bool Order_handling::check(/*in*/Customer_Order_handling::order*p1,/*out*/Customer_Order_handling::order_conformation*p2)
{
/*$TET$Order_handling$check*/
		cout << "\tOrder Handling checks the order. It sends an order confirmation to the Customer." << endl;
		return true;
/*$TET$*/
}

bool Order_handling::send(/*out*/Order_handling_Shipment::delivery_request*p1)
{
/*$TET$Order_handling$send*/
		cout << "\tOrder Handling sends a delivery request to Shipment." << endl;
		return true;
/*$TET$*/
}

void Order_handling::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$Order_handling$!run!*/
		cout << "Order handling:" << endl;
/*$TET$*/

}

//////////////////////class Shipment////////////////////
Shipment::Shipment(TEMPLET_DBG::Assemble*a):TEMPLET_DBG::Process(a)
{
/*$TET$Shipment$!constructor!*/
/*$TET$*/
}

Shipment::~Shipment()
{
/*$TET$Shipment$!destructor!*/
/*$TET$*/
}

/*$TET$Shipment$!usercode!*/
/*$TET$*/

bool Shipment::receive(/*in*/Order_handling_Shipment::delivery_request*p1)
{
/*$TET$Shipment$receive*/
		cout << "\tShipment receives the request" << endl;
		return true;
/*$TET$*/
}

bool Shipment::deliver(/*out*/Shipment_Customer::product_delivery*p1)
{
/*$TET$Shipment$deliver*/
		cout << "\t  and sends the product delivery to the Customer." << endl;
		return true;
/*$TET$*/
}

void Shipment::_run(int _selector,TEMPLET_DBG::Channel*_channel)
{
	bool res;
/*$TET$Shipment$!run!*/
		cout << "Shipment:" << endl;
/*$TET$*/

}

//////////////////////class odrhandling////////////////////
odrhandling::odrhandling(int NT): TEMPLET_DBG::Assemble(NT)
{
/*$TET$odrhandling$!constructor!*/
/*$TET$*/
}

odrhandling::~odrhandling()
{
/*$TET$odrhandling$!destructor!*/
/*$TET$*/
}

/*$TET$odrhandling$!cpp-epilogue!*/
int main()
{
	odrhandling runtime(1);

	Customer* customer = runtime.new_Customer();
	Order_handling* order_handling = runtime.new_Order_handling();
	Shipment* shipment = runtime.new_Shipment();

	order_handling->p_customer(customer->p_order_handling());
	shipment->p_order_handling(order_handling->p_shipment());
	customer->p_shipment(shipment->p_customer());


	cout << "--Customer Order procedure--" << endl;
	srand(time(NULL));// to show different system behaviors (because of different times of message processing)
	runtime.run();

	return 0;
}
/*$TET$*/

