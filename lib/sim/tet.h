/*--------------------------------------------------------------------------*/
/*  Copyright 2010-2015 Sergey Vostokin                                     */
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

#ifndef _TEMPLET_SIM
#define _TEMPLET_SIM

#include <vector>
#include <queue>

namespace TEMPLET_SIM {
////////////////////C++11 run-time///////////////////////
struct engine;
struct proc;
struct chan;

struct event{
	double time;
	enum{ CHAN, PROC0, PROC1 } type;
	chan*c;
	proc*p;
};

class cmp{ public: bool operator()(const event&t1, const event&t2){ return t1.time > t2.time; } };

struct engine{
	std::priority_queue<event, std::vector<event>, cmp> calendar;
	double Tp;
	double T1;
	int Pmax;
};

struct proc{
	bool lck;
	std::queue<chan*> ready;
	void(*recv)(chan*, proc*);
};

struct chan{
	proc*p;
	bool sending;
};

inline void duration(engine*e, double t)
{
	e->T1 += t;
	e->Tp += t;
}

inline void send(engine*e, chan*c, proc*p)
{
	if (c->sending) return;
	c->sending = true;	c->p = p;

	event ev;
	ev.time = e->Tp; ev.type = event::CHAN; ev.c = c;
	e->calendar.push(ev);
}

inline bool access(chan*c, proc*p)
{
	return c->p == p && !c->sending;
}

inline void run(engine*e, int n = 1)
{
	proc*p = 0; chan*c = 0;
	double Tcur = 0.0, Tprev = 0.0;
	int Pcur = 0, Pmax = 0;

	while (!e->calendar.empty()){
		event ev;
		ev = e->calendar.top();	e->calendar.pop();

		Tcur = ev.time;
		if (Tcur - Tprev > 0 && Pcur > Pmax) Pmax = Pcur;
		Tprev = Tcur;

		switch (ev.type){
		case event::CHAN:
		{
			p = ev.c->p;
			if (!p->lck && p->ready.empty()){
				ev.p = p; ev.type = event::PROC0;
				e->calendar.push(ev);
			}
			p->ready.push(ev.c);
		}
		break;

		case event::PROC0:
		{
			p = ev.p;
			c = p->ready.front(); p->ready.pop();
			c->sending = false;

			p->lck = true; Pcur++;

			e->Tp = Tcur; p->recv(c, p); Tcur = e->Tp;

			ev.time = Tcur; ev.type = event::PROC1;
			e->calendar.push(ev);
		}
		break;

		case event::PROC1:
		{
			p = ev.p;
			if (!p->ready.empty()){
				ev.type = event::PROC0;
				e->calendar.push(ev);
			}
			p->lck = false; Pcur--;
		}
		}
	}
	e->Tp = Tcur; e->Pmax = Pmax;
}

inline void stat(engine*e, double&T1, double&Tp, int&Pmax, double&Smax, int P, double&Sp)
{
	T1 = e->T1; Tp = e->Tp; Pmax = e->Pmax;
	Smax = T1 / Tp;
	double alfa = (1 - Smax / Pmax) / (Smax - Smax / Pmax);
	Sp = (P > Pmax) ? Smax : 1 / (alfa + (1 - alfa) / P);
}
/////////////////////////////////////////////////////////

class Assemble;
class Channel;
class Process;
class Activator;

class Assemble: public engine{
	friend class Channel; 
	friend class Activator;
protected:
	virtual ~Assemble();
	Assemble(int NT=0){}
public:	
	bool run();
public:
	void _regChan(Channel*chan){channels.push_back(chan);}
	void _regProc(Process*prc){processes.push_back(prc);}
private:
	std::vector<Channel*> channels;
	std::vector<Process*> processes;
	std::vector<Channel*> ready;
};

class Channel: public chan{
	friend class Assemble;
	friend class Process;
	friend class Activator;
	friend void recv(chan*c, proc*p);
protected:
	Channel(Assemble*a):_assemble(a),_active(CLI),
		_cliPort(0), _srvPort(0), _cli_selector(0), _srv_selector(0){
		p = 0; sending = false;
	}
	virtual ~Channel(){}
public:
	enum ChanActivity{CLI,SRV,RTL_CLI,RTL_SRV};
protected:	
	void _send();
	ChanActivity _active;
public:
	void resend();
public:
	Process* _cliPort;
	Process* _srvPort;
	int _cli_selector;
	int _srv_selector;
private:
	Assemble* _assemble;
};

class Activator:public Channel{
public:
	Activator(Assemble*a,Process*p):Channel(a){_cliPort=p;}
	void _send(int port);
};

class Process: public proc{
	friend class Channel;
	friend class Assemble;
	friend void recv(chan*c, proc*p);
protected:
	void lock(){}
	void unlock(){}
protected:
	Process(Assemble*a) :_assemble(a){ recv = TEMPLET_SIM::recv; lck = false; }
	virtual ~Process(){}
protected:
	Activator* _createActivator(){return new Activator(_assemble,this);}
private:
	virtual void _run(int _selector,Channel*)=0;
public:
	Assemble* _assemble;
};

}
#endif