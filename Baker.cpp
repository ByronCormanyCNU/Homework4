#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;
bool isDone = false;
Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	//check the number of donuts to see how many boxes are needed
	Box box1;
	DONUT plain;
	int donutNum = anOrder.number_donuts;

	if(donutNum > 0 && donutNum <= 12){
		for(int i = 0; i < donutNum; i++){
			box1.addDonut(plain);
		}
		anOrder.boxes.push_back(box1);
	}
	else if(donutNum > 12){
		Box box2;
		for(int i = 0; i < 12; i++){
			box1.addDonut(plain);
		}

		anOrder.boxes.push_back(box1);

		for(int i = 12; i < donutNum; i++){
			box2.addDonut(plain);
		}

		anOrder.boxes.push_back(box2);
	}
	else{
		//throw "no donut";
	}
}

void Baker::beBaker() {

	{
		unique_lock<mutex> lck(mutex_order_outQ);
		cout << "Waiting " << id << endl;
		while(!b_WaiterIsFinished){
			cv_order_inQ.wait(lck);
			cout << "Starting " << id << endl;
		}
	}


	while(true){

	unique_lock<mutex> lck(mutex_order_outQ);


		if(order_in_Q.size() > 0){
			lck.unlock();
			ORDER temp = order_in_Q.front();
			order_in_Q.pop();
			cout << "Baker " << id << " is doing work" << endl;
			Baker::bake_and_box(temp);
			lck.lock();
			order_out_Vector.push_back(temp);

		}
		else{
			isDone = true;
		}


		if(order_in_Q.size() == 0 && isDone == true){
			cout << "Baker " << id << " is done" << endl;
			break;
		}

	}

}
