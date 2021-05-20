#include "car_type.h"

CarType::CarType(int type_id, int num_of_models) :  type_id(type_id), num_of_models(num_of_models), total_sales(0),
                                                    best_seller_model(0), num_of_sales_of_best_seller_model(0){
    if(num_of_models != 0){                                   
        CarModel temp_arr[num_of_models];
        for(int i=0; i<num_of_models; i++){                 
            temp_arr[i] = CarModel(type_id, i);
        }

        //zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
        zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
        //delete[] temp_arr;
    }
    else zero_scored_models = new AVLTree<CarModel>();
}

void CarType::operator=(const CarType& type){
    delete zero_scored_models;
    type_id = type.type_id;
    num_of_models = type.num_of_models;
    total_sales = type.total_sales;
    best_seller_model = type.best_seller_model;                         
    num_of_sales_of_best_seller_model = type.num_of_sales_of_best_seller_model;
    zero_scored_models = new AVLTree<CarModel>();
    CarModel* to_insert = type.zero_scored_models->InOrderGetFirst();
    for(int i = 0; i < type.zero_scored_models->getSize(); i++){
        zero_scored_models->insert(*to_insert);
        to_insert = type.zero_scored_models->InOrderGetNext();
    }
}

CarType::CarType(const CarType& type){
    type_id = type.type_id;
    num_of_models = type.num_of_models;
    total_sales = type.total_sales;
    best_seller_model = type.best_seller_model;                         
    num_of_sales_of_best_seller_model = type.num_of_sales_of_best_seller_model;
    if(num_of_models != 0){                                   
        CarModel temp_arr[num_of_models];
        for(int i=0; i<num_of_models; i++){                 
            temp_arr[i] = CarModel(type_id, i);
        }
        //zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
        zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
        //delete[] temp_arr;
    }
    else {zero_scored_models = new AVLTree<CarModel>();}
}

CarType::~CarType(){
    //if(zero_scored_models != nullptr)
        delete zero_scored_models;
}

int CarType::getNumOfModels() const{
    return num_of_models;    
}

int CarType::get_type_id() const {
    return type_id;    
}

std::ostream& DataStructures::operator<<(std::ostream& os, const CarType& type){
    os << type.get_type_id();
    return os;
}


void CarType::addSale()
{
    this->total_sales++;
}

void CarType::updateBestSeller(int modelID, int num_of_sales)
{
    if (this->num_of_sales_of_best_seller_model < num_of_sales){
        best_seller_model = modelID;
        num_of_sales_of_best_seller_model = num_of_sales;
        return;
    }
    else if(this->num_of_sales_of_best_seller_model == num_of_sales && best_seller_model > modelID){
        best_seller_model = modelID;
        num_of_sales_of_best_seller_model = num_of_sales;
        return;

    }    
}

bool CarType::operator==(const CarType& type) const{
    return type_id == type.type_id; 
}  

bool CarType::operator>(const CarType& type) const{
    return type_id > type.type_id; 
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
