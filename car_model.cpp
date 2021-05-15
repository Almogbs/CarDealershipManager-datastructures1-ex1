#include "car_model.h"


using namespace DataStructures;
/*
bool isValidModelId(int model_id)
{
    return !(model_id < 0);
}
*/

CarModel::CarModel(int type_id, int model_id, int score, int sales) :
        model_id(model_id), type_id(type_id), sales(sales), score(score) {};

void CarModel::addSale(){
    sales++;
    score += SCORE_ARG_SELL;
}

int CarModel::get_score() const{
    return score;
}

int CarModel::get_total_sales() const{
    return sales;
}

int CarModel::get_model_id() const{
    return model_id;
}

int CarModel::get_type_id() const{
    return type_id;
}

std::ostream& DataStructures::operator<<(std::ostream& os, const CarModel& model){
    os << "Type: " << model.get_type_id() << " Model: " << model.get_model_id();
    return os;
}


void CarModel::complain(int t){
    //t should not be 0 here
    if(t == 0) return;
    //sales = sales - 1;
    score -= (SCORE_ARG_COMPLAIN/t);
}

bool CarModel::operator==(const CarModel& model) const{
    return (model_id == model.get_model_id() && type_id == model.get_type_id());
}   

bool DataStructures::operator!=(const CarModel& first, const CarModel& second){
    return !(first == second);
}   

bool CarModel::operator>(const CarModel& model) const{
    if(type_id > model.get_type_id()) return true;
    if(type_id == model.get_type_id()) return model_id > model.get_model_id();
    return false;
}

bool DataStructures::operator>=(const CarModel& first, const CarModel& second){
    return ((first > second) || first == second);
}

bool DataStructures::operator<=(const CarModel& first, const CarModel& second){
    return !(first > second);
}

bool DataStructures::operator<(const CarModel& first, const CarModel& second){
    return !(first >= second);
}

CarModelByGrade::CarModelByGrade(int type_id, int model_id, int score, int sales) : CarModel(type_id, model_id, score, sales){};

bool CarModelByGrade::operator==(const CarModel& model) const {
    return(score == model.get_score() && type_id == model.get_type_id() && model_id == model.get_model_id());
}

bool CarModelByGrade::operator>(const CarModel& model) const {
    if(score > model.get_score()) return true;
    else if(score == model.get_score()){
        if(type_id > model.get_type_id()) return true;
        else if(type_id == model.get_type_id()) return model_id > model.get_model_id(); 
        else return false;
    }
    return false;
}

bool DataStructures::operator!=(const CarModelByGrade& first, const CarModelByGrade& second){
    return !(first == second);
}   

bool DataStructures::operator>=(const CarModelByGrade& first, const CarModelByGrade& second){
    return ((first > second) || first == second);
}

bool DataStructures::operator<=(const CarModelByGrade& first, const CarModelByGrade& second){
    return !(first > second);
}

bool DataStructures::operator<(const CarModelByGrade& first, const CarModelByGrade& second){
    return !(first >= second);
}

CarModelBySales::CarModelBySales(int type_id, int model_id, int score, int sales) : CarModel(type_id, model_id, score, sales){};

bool CarModelBySales::operator==(const CarModel& model) const {
    return(sales == model.get_total_sales() && type_id == model.get_type_id() && model_id == model.get_model_id());
}

bool CarModelBySales::operator>(const CarModel& model) const {
    if(score > model.get_total_sales()) return true;
    else if(score == model.get_total_sales()){
        if(type_id > model.get_type_id()) return true;
        else if(type_id == model.get_type_id()) return model_id > model.get_model_id(); 
        else return false;
    }
    return false;
}

bool DataStructures::operator!=(const CarModelBySales& first, const CarModelBySales& second){
    return !(first == second);
}   

bool DataStructures::operator>=(const CarModelBySales& first, const CarModelBySales& second){
    return ((first > second) || first == second);
}

bool DataStructures::operator<=(const CarModelBySales& first, const CarModelBySales& second){
    return !(first > second);
}

bool DataStructures::operator<(const CarModelBySales& first, const CarModelBySales& second){
    return !(first >= second);
}

