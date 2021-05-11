#include "cdm.h"

CDM::CDM() : best_seller_type(nullptr) {};

static bool isValidNumOfModels(int num){
    return num > 0;
}

static bool isValidModelId(int id){
    return id >= 0;
}

static bool isValidTypeId(int id){
    return id > 0;
}

StatusType CDM::AddCarType(int typeID, int numOfModels){
    if(!isValidTypeId(typeID) || !isValidNumOfModels(numOfModels)) return INVALID_INPUT;
    try {
        CarType type = CarType(typeID, numOfModels);
        types_tree.insert(type);
        if(best_seller_type != nullptr && best_seller_type->total_sales == 0 && best_seller_type->get_type_id() > typeID){
            best_seller_type = &(types_tree.getNode(type)->key);
        }
    } 
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }
    catch(const DataStructures::NodeExist &e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType CDM::RemoveCarType(int typeID){
    if(!isValidTypeId(typeID)) return INVALID_INPUT;
    CarType type = CarType(typeID, 0);
    try{
        Node<CarType>* to_delete = types_tree.getNode(type);
        int num_of_models = to_delete->get_key().get_num_of_models();
        types_tree.remove(type);

        try{
            zero_scored_types_tree.remove(type);
        }
        catch(const DataStructures::NodeNotExist &e){}
        for(int i=0; i<num_of_models; i++){
            CarModel current_model = CarModel(typeID, i);
            try{
                
            }
            catch(const DataStructures::NodeNotExist &e){}

            try{
                
            }
            catch(const DataStructures::NodeNotExist &e){}
        }
    }
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }
    catch(const DataStructures::NodeNotExist &e){
        return FAILURE;
    }
    //find new min pls
    //...
    return SUCCESS;
}

StatusType CDM::sellCar(int typeID, int modelID){
    if ( !isValidTypeId(typeID) || !isValidModelId(modelID) ) return INVALID_INPUT;
    try
    {
        CarType current_type = CarType(typeID, 0);
        Node<CarType>* current_type_node = types_tree.getNode(current_type);  
        
    }

}

StatusType CDM::makeComplaint(int typeID, int modelID, int t){

}
StatusType CDM::GetBestSellerModelByType(int typeID, int * modelID){

}
StatusType CDM::GetWorstModels(int numOfModels, int *types, int *models){

}
    
