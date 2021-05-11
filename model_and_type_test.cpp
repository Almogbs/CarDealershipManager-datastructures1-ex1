
#include "car_type.h"
#include "car_model.h"
#include "avl_tree.h"
#include <vector>
#include <ostream>
#include <random>
#include <chrono>
#include <algorithm>

int main(){
    CarType t1(11, 21);
    CarModel m1 = CarModel(11,2);
    CarModel m2 = CarModel(11,0);
    cout << m1 << endl;
    cout << m2 << endl;
    t1.zero_scored_models->printInorder();
    try{
        t1.zero_scored_models->remove(m1);
        t1.zero_scored_models->remove(m2);
    } catch(DataStructures::NodeNotExist &e){
        std::cout << "somtething wrong!" << std::endl;
    } 
    t1.zero_scored_models->printInorder();
	return 0;
}
