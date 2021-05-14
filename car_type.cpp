#include "car_type.h"

CarType::CarType(int type_id, int num_of_models) :  type_id(type_id), num_of_models(num_of_models), total_sales(0),
                                                    best_seller_model(0), num_of_sales_of_best_seller_model(0){
                                                        
    CarModel* temp_arr = new CarModel[num_of_models];
    for(int i=0; i<num_of_models; i++){                 
        temp_arr[i] = CarModel(type_id, num_of_models - 1 - i);
    }
    
    //zero_scored_models = new AVLTree<CarModel>(temp_arr, num_of_models);
    zero_scored_models = AVLTree<CarModel>(temp_arr, num_of_models);
    delete[] temp_arr;
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
    if (this->num_of_sales_of_best_seller_model <= num_of_sales)
    {
        if (this->num_of_sales_of_best_seller_model == num_of_sales)
        {
            if (this->best_seller_model < modelID)
            {
                this->best_seller_model = modelID;
            }
            return;
        }
        this->num_of_sales_of_best_seller_model = num_of_sales;
        this->best_seller_model = modelID;
    }
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
