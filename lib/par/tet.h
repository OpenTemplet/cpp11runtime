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

#ifndef _TEMPLET_SEQ
#define _TEMPLET_SEQ

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace TEMPLET_PAR {
////////////////////C++11 run-time///////////////////////
struct engine;
struct proc;
struct chan;

struct engine{
	volatile int active;
	std::mutex mtx;
	std::condition_variable cv;
	std::queue<chan*> ready;
};

struct proc{
	std::mutex mtx;
	void(*recv)(chan*, proc*);
};

struct chan{
	proc*p;
	bool sending;
};

inline void duration(engine*e, double t){}

inline void send(engine*e, chan*c, proc*p)
{
	if (c->sending) return;
	c->sending = true;	c->p = p;
	std::unique_lock<std::mutex> lck(e->mtx);
	e->ready.push(c);	e->cv.notify_one();
}

inline bool access(chan*c, proc*p)
{
	return c->p == p && !c->sending;
}

inline void tfunc(engine*e)
{
	chan*c; proc*p;

	for (;;){
		{
			std::unique_lock<std::mutex> lck(e->mtx);
			while (e->ready.empty()){
				e->active--;
				if (!e->active){ e->cv.notify_one(); return; }
				e->cv.wait(lck);
				e->active++;
			}
			c = e->ready.front();
			e->ready.pop();
		}
		p = c->p;
		{
			std::unique_lock<std::mutex> lck(p->mtx);
			c->sending = false;
			p->recv(c, p);
		}
	}
}

inline void run(engine*e, int n = 1)
{
	std::vector<std::thread> threads(n);
	e->active = n;
	for (int i = 0; i<n; i++) threads[i] = std::thread(tfunc, e);
	for (auto& th : threads) th.join();
}

inline void stat(engine*e, double&T1, double&Tp, int&Pmax, double&Smax, int P, double&Sp){}
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
	Process(Assemble*a) :_assemble(a){ recv = TEMPLET_PAR::recv; }
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