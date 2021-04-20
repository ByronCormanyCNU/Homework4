#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){

	int check = myIO.getNext(anOrder);

	if(check == SUCCESS){
		return SUCCESS;
	}

	else{
		return FAIL;
	}
}

void Waiter::beWaiter() {
	//get the order

	ORDER temp;
	int check = 0;

	while(check == 0){	//check if next gives me success
		lock_guard<mutex> lck(mutex_order_inQ);

		//puts order in q and then signals with cv
		check = getNext(temp);
		if(check != 0){
			break;
		}
		order_in_Q.push(temp);

		//cv_order_inQ.notify_all();
	}

	//tell bakers that waiter is done

	lock_guard<mutex> lck(mutex_order_inQ);

	cv_order_inQ.notify_all();
	b_WaiterIsFinished = true;
	cout << "Waiter " << id << " is done" << endl;
	cout << "Queue Size " << order_in_Q.size() << endl;



}

