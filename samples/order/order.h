/*$TET$order$!h-copyright!*/
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
/*$TET$*/

#ifndef _TEMPLET_MODULE_order
#define _TEMPLET_MODULE_order

#include <string.h>
#include <assert.h>

#include "dbg/tet.h"

/*$TET$order$!h-prologue!*/
/*$TET$*/

class Customer_Order_handling:public TEMPLET_DBG::Channel{
public:
	Customer_Order_handling(TEMPLET_DBG::Assemble*a);
	~Customer_Order_handling();
public:
	struct order{//<PREPARES> 
/*$TET$Customer_Order_handling$order*/
/*$TET$*/
	};
	struct order_conformation{//<CHECKS> 
/*$TET$Customer_Order_handling$order_conformation*/
/*$TET$*/
	};
};

class Order_handling_Shipment:public TEMPLET_DBG::Channel{
public:
	Order_handling_Shipment(TEMPLET_DBG::Assemble*a);
	~Order_handling_Shipment();
public:
	struct delivery_request{//<SENDS> 
/*$TET$Order_handling_Shipment$delivery_request*/
/*$TET$*/
	};
};

class Shipment_Customer:public TEMPLET_DBG::Channel{
public:
	Shipment_Customer(TEMPLET_DBG::Assemble*a);
	~Shipment_Customer();
public:
	struct product_delivery{//<SEND> 
/*$TET$Shipment_Customer$product_delivery*/
/*$TET$*/
	};
};

class Customer:public TEMPLET_DBG::Process{
public:
	Customer(TEMPLET_DBG::Assemble*a);
	~Customer();
private:
	//methods
	bool prepare();
	bool send(/*out*/Customer_Order_handling::order*p1);
	bool receive(/*in*/Customer_Order_handling::order_conformation*p1);
	bool recieves_and_checks(/*in*/Shipment_Customer::product_delivery*p1);

/*$TET$Customer$!userdata!*/
/*$TET$*/

public:
	Customer_Order_handling* p_order_handling(){return 0; }
	void p_shipment(Shipment_Customer*p){}
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class Order_handling:public TEMPLET_DBG::Process{
public:
	Order_handling(TEMPLET_DBG::Assemble*a);
	~Order_handling();
private:
	//methods
	bool check(/*in*/Customer_Order_handling::order*p1,/*out*/Customer_Order_handling::order_conformation*p2);
	bool send(/*out*/Order_handling_Shipment::delivery_request*p1);

/*$TET$Order_handling$!userdata!*/
/*$TET$*/

public:
	void p_customer(Customer_Order_handling*p){}
	Order_handling_Shipment* p_shipment(){return 0; }
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class Shipment:public TEMPLET_DBG::Process{
public:
	Shipment(TEMPLET_DBG::Assemble*a);
	~Shipment();
private:
	//methods
	bool receive(/*in*/Order_handling_Shipment::delivery_request*p1);
	bool deliver(/*out*/Shipment_Customer::product_delivery*p1);

/*$TET$Shipment$!userdata!*/
/*$TET$*/

public:
	void p_order_handling(Order_handling_Shipment*p){}
	Shipment_Customer* p_customer(){return 0; }
protected:
	virtual void _run(int _selector,TEMPLET_DBG::Channel*_channel);
};

class order:public TEMPLET_DBG::Assemble{
public:
	order(int NT);
	~order();

/*$TET$order$!userdata!*/
/*$TET$*/
public:
	Customer*new_Customer(){return 0;}
	Order_handling*new_Order_handling(){return 0;}
	Shipment*new_Shipment(){return 0;}
};

/*$TET$order$!h-epilogue!*/
/*$TET$*/
#endif
