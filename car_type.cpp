#include "car_type.h"

CarType::CarType(int type_id, int num_of_models) : type_id(type_id), num_of_models(num_of_models), total_sales(0), best_car(0){
    CarModel* temp_arr = new CarModel[num_of_models];
    for(int i=0; i<num_of_models; i++){                 
        temp_arr[i] = CarModel(type_id, num_of_models - 1 - i);
    }
    
    //zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
    zero_scored_models = AVLTree<CarModel>(temp_arr, num_of_models);
    delete[] temp_arr;
}
/*
CarType::~CarType(){
    delete zero_scored_models;
}
*/

int CarType::get_num_of_models() const{
    return num_of_models;    
}

int CarType::get_type_id() const {
    return type_id;    
}

std::ostream& DataStructures::operator<<(std::ostream& os, const CarType& type){
    os << type.get_type_id();
    return os;
}


bool CarType::operator==(const CarType& type) const{
    return type_id == type.type_id; 
}  

bool CarType::operator>(const CarType& type) const{
    return type_id < type.type_id; 
}

bool DataStructures::operator!=(const CarType& type1, const CarType& type2){
    return !(type1 == type2);
} 

bool DataStructures::operator<=(const CarType& type1, const CarType& type2){
    return !(type1 > type2); 
}

bool DataStructures::operator>=(const CarType& type1, const CarType& type2){
    return !(type1 < type2);
}

bool DataStructures::operator<(const CarType& type1, const CarType& type2){
        return (type1 <= type2) && (type1 != type2); 

}
