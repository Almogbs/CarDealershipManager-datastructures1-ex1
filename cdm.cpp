#include "cdm.h"

#define UNINITIALIZED -1
#define ADD_TO_ZST 0
#define REMOVE_FROM_ZST 1
#define SCORE_BEFOR_ZREO -10

CDM::CDM() : best_seller_type(UNINITIALIZED), best_seller_model(UNINITIALIZED), best_seller_sales(UNINITIALIZED), num_of_models(0) {};

static bool isValidNumOfModels(int num){
    return num > 0;
}

static bool isValidModelId(int id){
    return id >= 0;
}

static bool isValidTypeId(int id){
    return id > 0;
}

void CDM::updateBestSeller(){
    if(models_by_sales_tree.getSize() != 0 ){
        CarModelBySales* temp = models_by_sales_tree.getMaxNode()->get_key();
        best_seller_model = temp->get_model_id();
        best_seller_sales = temp->get_score();
        best_seller_type = temp->get_type_id();
    }
    best_seller_type = types_tree.getMinNode()->get_key()->get_type_id();
    best_seller_sales = 0;
    best_seller_model = 0;
}

//O(LogM)+O(logn)?????

void CDM::updateZST(CarType car_type, int action)
{
    if (action == ADD_TO_ZST)
    {
        this->zero_scored_types_tree.insert(car_type);
        car_type.zero_scored_models = AVLTree<CarModel>(); //really need ?
    }
    else if (REMOVE_FROM_ZST)
    {
        this->zero_scored_types_tree.remove(car_type);
        car_type.zero_scored_models.~AVLTree();            //really need ?
    }
    return;
}

void CDM::addToZSM(CarType car_type, AVLTree<CarModel>* zero_scored_models, CarModelByGrade car_model_by_grade)
{
    if (car_type.zero_scored_models == nullptr)
    {
        updateZST(car_type, ADD_TO_ZST);
    }
    CarModel car_model = CarModel(car_model_by_grade.get_type_id(),car_model_by_grade.get_model_id(), 
                                  car_model_by_grade.get_score(),car_model_by_grade.get_total_sales());
    car_type.zero_scored_models.insert(car_model);
    this->models_by_score_tree.remove(car_model_by_grade);

    return;
}

void CDM::addToMBS(CarType car_type, AVLTree<CarModel>* zero_scored_models, CarModel car_model)
{
    CarModelByGrade car_model_by_grade =  CarModelByGrade(car_model.get_type_id(), car_model.get_model_id(),
                                                          car_model.get_score(), car_model.get_total_sales());
    this->models_by_score_tree.insert(car_model_by_grade);
    car_type.zero_scored_models.remove(car_model);
    if (car_type.zero_scored_models.getSize() == 0 )
    {
        updateZST(car_type, REMOVE_FROM_ZST);
    }
    
    return;
}

StatusType CDM::AddCarType(int typeID, int numOfModels){
    if(!isValidTypeId(typeID) || !isValidNumOfModels(numOfModels)) return INVALID_INPUT;
    try {
        CarType type = CarType(typeID, numOfModels);
        types_tree.insert(type);
        zero_scored_types_tree.insert(type);
        
        // updated best sellerin CDM
        if((best_seller_type != UNINITIALIZED) && (best_seller_sales == 0) && (best_seller_type > typeID)){
            best_seller_type = typeID;
            best_seller_sales = 0;
            best_seller_model = 0;
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

void CDM::updateAllModelTrees(CarModel car_model)
{
    Node<CarModel>* car_model_node = this->models_by_type_tree.getNode(car_model);
}

StatusType CDM::RemoveCarType(int typeID){
    if(!isValidTypeId(typeID)) return INVALID_INPUT;
    CarType type = CarType(typeID, 0);
    //delete the Type from the types_tree
    try{
        Node<CarType>* to_delete = types_tree.getNode(type);
        int num_of_models = to_delete->get_key()->get_num_of_models();
        types_tree.remove(type);
        
        //delete the type from the zero_score_types_tree, if exist there
        try{
            zero_scored_types_tree.remove(type);
        }
        catch(const DataStructures::NodeNotExist &e){}
        for(int i=0; i<num_of_models; i++){
            int current_model_score = 0;
            CarModel current_model = CarModel(typeID, i);

            //remove the current model of the type from the models_by_type_tree, if exists there
            try{
                current_model_score = models_by_type_tree.getNode(current_model)->get_key()->get_score();
                models_by_type_tree.remove(current_model);
                models_by_score_tree.remove(CarModelByGrade(typeID, i, current_model_score));
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
    updateBestSeller();
    return SUCCESS;
}

StatusType CDM::sellCar(int typeID, int modelID){
    if ( !isValidTypeId(typeID) || !isValidModelId(modelID) ) return INVALID_INPUT;
    try
    {
        //model score is zero
        CarType car_type = CarType(typeID, 0);
        Node<CarType>* car_type_node = zero_scored_types_tree.getNode(car_type);
        AVLTree<CarModel>* zero_score_models = &(car_type_node->get_key()->zero_scored_models);
        CarModel to_sell = CarModel(typeID, modelID);
        if (zero_score_models->search(to_sell)) 
        {  
            //do something
        }
    }
    catch (const DataStructures::NodeNotExist &e){
        //model score is not 0  (exrreme case---> score == -10  SCORE_BEFOR_ZREO)
        //do somthing YAMETEEE
    }
    
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

StatusType CDM::makeComplaint(int typeID, int modelID, int t){
    if ( !isValidTypeId(typeID) || !isValidModelId(modelID) ) return INVALID_INPUT;


}

StatusType CDM::GetBestSellerModelByType(int typeID, int * modelID){
    if(typeID == 0) *modelID = best_seller_model;
    else if(!isValidTypeId(typeID)) return INVALID_INPUT;
    try{
        Node<CarType>* result = types_tree.getNode(CarType(typeID, 0));
        *modelID = result->get_key()->best_car;
    }
    catch(const DataStructures::NodeNotExist &e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType CDM::GetWorstModels(int numOfModels, int *types, int *models){
    if(numOfModels <= 0) return INVALID_INPUT;
    else if(numOfModels > num_of_models) return FAILURE;

    CarModelByGrade* iterator_negative = models_by_score_tree.InOrderGetFirst();
    int counter = 0;//, zero_tree_counter = 0;
    
    //get in only if the tree sint empty
    //start with the models with negative score first
    for(counter = 0; counter < models_by_score_tree.getSize() && counter < numOfModels; counter++){
        if(iterator_negative->get_score() > 0){
            //counter--;
            break;
        }
        types[counter] = iterator_negative->get_type_id();
        models[counter] = iterator_negative->get_model_id();
        iterator_negative = models_by_score_tree.InOrderGetNext();
    }
    if(counter == numOfModels - 1) return SUCCESS;
    else if(counter > numOfModels - 1) throw Assert();

    CarType* iterator_zero = zero_scored_types_tree.InOrderGetFirst();

    //go on with the zero scored models
    //get in only if the tree isnt empty
    for(int zero_tree_counter = 0; zero_tree_counter < zero_scored_types_tree.getSize() && counter < numOfModels; zero_tree_counter++){
        AVLTree<CarModel>* current_type = &(iterator_zero->zero_scored_models);
        CarModel* current_model = current_type->InOrderGetFirst();
        //go on with the zero models inside the current type
        for(int inner_counter = 0; inner_counter < current_type->getSize() && counter < numOfModels; counter++){
            types[counter] = current_model->get_type_id();
            models[counter] = current_model->get_model_id();
            current_model = current_type->InOrderGetNext();
        }
        iterator_zero = zero_scored_types_tree.InOrderGetNext();
    }

    if(counter == numOfModels - 1) return SUCCESS;
    else if(counter > numOfModels - 1) throw Assert();
    
    //get in only if the tree sint empty
    //start with the models with positive score first
    for(counter = counter; counter < models_by_score_tree.getSize() && counter < numOfModels; counter++){
        types[counter] = iterator_negative->get_type_id();
        models[counter] = iterator_negative->get_model_id();
        iterator_negative = models_by_score_tree.InOrderGetNext();
    }
    return SUCCESS;
}
    
