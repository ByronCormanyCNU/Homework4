#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

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
		cout << "Waiting " << endl;
		while(!b_WaiterIsFinished){
			cout << "Starting " << endl;
			cv_order_inQ.wait(lck);
		}
	}

	while(order_in_Q.size() > 0 || b_WaiterIsFinished == true){

	//unique_lock<mutex> lck(mutex_order_outQ);
    lock_guard<mutex> lck(mutex_order_outQ);

	ORDER temp = order_in_Q.front();
	order_in_Q.pop();
	cout << order_in_Q.size() << endl;


	Baker::bake_and_box(temp);

	//while(order_in_Q.size() == 0 || b_WaiterIsFinished == false){
	//	cv_order_inQ.wait(lck);
	//}

	order_out_Vector.push_back(temp);

	if(order_out_Vector.size() == 5){
		b_WaiterIsFinished = false;
	}

	}

	lock_guard<mutex> lck(mutex_order_outQ);
	b_WaiterIsFinished = false;
	cout << "Baker " << id << " is done" << endl;
	cout << order_out_Vector.size() << endl;
}
