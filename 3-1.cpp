#include <cstdio>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

enum names { D, B, P, N };
class Context;
class State;


class Context {
	State actState;
	State states[];
public:
	Context() {
		
	}

	void changeState(State& newState) {
      
	}
};

class State {
	bool isActive;
	names stateName;
public:
	void stateService() {

	}
};

class ConcreteState1 : public State {
	State toState2;
	State toState3;
};

class ConcreteState2 : public State {
	ConcreteState3	toState3;
};

class ConcreteState3 : public State {
	 ConcreteState1 toState1;
};

class Client {
	Context actContext;
public:
	Client() {
		
	}
	~Client() {

	}
};

int main() {
	Client c();

	return 0;
}
