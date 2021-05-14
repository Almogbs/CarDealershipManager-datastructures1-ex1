#include "cdm.h"

#define UNINITIALIZED -1
#define ADD_TO_ZST 0
#define REMOVE_FROM_ZST 1
#define SCORE_BEFOR_ZREO -10

CDM::CDM() : best_seller_type(UNINITIALIZED), best_seller_model(UNINITIALIZED), best_seller_sales(UNINITIALIZED), total_num_of_models(0) {};

static bool isValidNumOfModels(int num){
    return num > 0;
}

static bool isValidModelId(int id){
    return id >= 0;
}

static bool isValidTypeId(int id){
    return id > 0;
}

//O(logM)
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


/*
void CDM::updateBestSellerOfType(int typeID){
    ty
}
*/

//O(LogM)+O(logn)?????

void CDM::InsertOrRemoveFromZeroScoredTypesTree(CarType car_type, int action)
{
    if (action == ADD_TO_ZST)
    {
        this->zero_scored_types_tree.insert(car_type);
        //car_type.zero_scored_models = AVLTree<CarModel>(); //really need ?
    }
    else if (action == REMOVE_FROM_ZST)
    {
        this->zero_scored_types_tree.remove(car_type);
        //car_type.zero_scored_models.~AVLTree();            //really need ?
    }
    return;
}

//score wasnt zero and now turned to zero
void CDM::addModelToZeroScroredModels(CarType car_type, CarModelByGrade car_model_by_grade){
    //type exists in zero_scored_type_tree
    try{
        CarType* insert_to = zero_scored_types_tree.getNode(car_type)->get_key();
        insert_to->zero_scored_models.insert(CarModel(car_model_by_grade.get_type_id(),car_model_by_grade.get_model_id(), 
                                                      0, car_model_by_grade.get_total_sales()));
    }
    //type not exists in zero_scored_type_tree
    catch(const DataStructures::NodeNotExist &e){
        InsertOrRemoveFromZeroScoredTypesTree(car_type, ADD_TO_ZST);
        CarType* insert_to = zero_scored_types_tree.getNode(car_type)->get_key();
        insert_to->zero_scored_models.insert(CarModel(car_model_by_grade.get_type_id(),car_model_by_grade.get_model_id(), 
                                                      0, car_model_by_grade.get_total_sales()));
    }
    models_by_score_tree.remove(car_model_by_grade);  //not forget to search by old_score
}

//score was zero and now it isnt
void CDM::addToModelsByScoreTree(CarType* car_type, CarModel car_model){
    CarModelByGrade car_model_by_grade =  CarModelByGrade(car_model.get_type_id(), car_model.get_model_id(),
                                                          car_model.get_score(), car_model.get_total_sales());
    car_type->zero_scored_models.remove(car_model);                                                      
    models_by_score_tree.insert(car_model_by_grade);
    if (car_type->zero_scored_models.getSize() == 0){
        InsertOrRemoveFromZeroScoredTypesTree(*car_type, REMOVE_FROM_ZST);
    }
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
    total_num_of_models += numOfModels;
    return SUCCESS;
}

StatusType CDM::RemoveCarType(int typeID){
    if(!isValidTypeId(typeID)) return INVALID_INPUT;
    CarType type = CarType(typeID, 0);
    //delete the Type from the types_tree
    try{
        Node<CarType>* to_delete = types_tree.getNode(type);
        int num_of_models = to_delete->get_key()->getNumOfModels();
        types_tree.remove(type);
        total_num_of_models -= num_of_models;
        
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
                models_by_sales_tree.remove(CarModelBySales(typeID, i, current_model_score));
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

    //checks if the type exist nad
    try{
        CarType* car_type_node = zero_scored_types_tree.getNode(CarType(typeID, 0))->get_key();
        if(car_type_node->getNumOfModels() <= modelID) return INVALID_INPUT;
    }
    catch (const DataStructures::NodeNotExist &e){
        return FAILURE;
    }

    try{
        //model score is zero
        Node<CarType>* car_type_node = zero_scored_types_tree.getNode(CarType(typeID, 0));
        AVLTree<CarModel>* zero_score_models = &(car_type_node->get_key()->zero_scored_models);
        CarModel to_sell = CarModel(typeID, modelID);
        try
        {
            CarModel to_delete = *(zero_score_models->getNode(to_sell)->get_key());
            int old_score = to_delete.get_score();
            int old_sales = to_delete.get_total_sales();
            to_delete.addSale();
            int new_sales = to_delete.get_total_sales();
            int new_score = to_delete.get_score();
            if(new_score != SCORE_ARG_SELL) throw Assert();
            
            addToModelsByScoreTree(car_type_node->get_key(), to_delete);
            //update sells tree
            //add to scored tree
            if(old_sales == 0){
                models_by_sales_tree.insert(CarModelBySales(typeID, modelID, new_score, new_sales));
            }
            //remove and re-insert to sells tree
            else{
                models_by_sales_tree.remove(CarModelBySales(typeID, modelID, old_score, old_sales));
                models_by_sales_tree.insert(CarModelBySales(typeID, modelID, new_score, new_sales));
            }
            //update types tree
            CarType* current_type = types_tree.getNode(CarType(typeID, 0))->get_key();
            current_type->addSale();
            current_type->updateBestSeller(modelID, new_sales);

            //update models_by_type_tree
            try{
                //CarModel* current_model ;   ///'''''.....  --------------------UNUSED---------------------------
            }
            catch(const DataStructures::NodeNotExist &e){

            }
        }
        catch (const DataStructures::NodeNotExist &e){}
        catch(const std::bad_alloc &e){
            return ALLOCATION_ERROR;
        }
    }
    catch (const DataStructures::NodeNotExist &e){
        //model score is not 0  
        CarModelByGrade to_sell = *(models_by_score_tree.getNode(CarModelByGrade(typeID, modelID))->get_key());
        int old_score = to_sell.get_score();
        int old_sales = to_sell.get_total_sales();
        to_sell.addSale();
        int new_score = to_sell.get_score();
        int new_sales = to_sell.get_total_sales();
        //int new_sales = to_sell.get_total_sales();
        if (old_score == SCORE_BEFOR_ZREO)  //(exrreme case---> score == -10  SCORE_BEFOR_ZREO)
        {
            addModelToZeroScroredModels(CarType(typeID,0), to_sell);
            if (new_score != 0) throw Assert();
        }
        CarType* car_type = types_tree.getNode(CarType(typeID, 0))->get_key(); //ORIGINAL CAR_TYPE
        car_type->updateBestSeller(modelID, new_sales);
        car_type->addSale();

        //update sells tree
        //add to scored tree
        if(old_sales == 0){
            models_by_sales_tree.insert(CarModelBySales(typeID, modelID, new_score, new_sales));
        }
        //remove and re-insert to sells tree
        else{
            models_by_sales_tree.remove(CarModelBySales(typeID, modelID, old_score, old_sales));
            models_by_sales_tree.insert(CarModelBySales(typeID, modelID, new_score, new_sales));
        }

            //update models_by_type_tree
            //..

    }
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}



StatusType CDM::makeComplaint(int typeID, int modelID, int t){
    if ( !isValidTypeId(typeID) || !isValidModelId(modelID) ) return INVALID_INPUT;
    //checks if the type exist and the validity of the input
    try{
        CarType* car_type_node = types_tree.getNode(CarType(typeID, 0))->get_key();
        if(car_type_node->getNumOfModels() <= modelID) return INVALID_INPUT;
    }
    catch (const DataStructures::NodeNotExist &e){
        return FAILURE;
    }
    
    //checks if model exists in models_by_type_tree
    //model is used(sell>0)
    try{
        CarModel* model = models_by_type_tree.getNode(CarModel(typeID, modelID))->get_key();
        //save old score and old sales
        int old_score = model->get_score();
                         //int old_sales = model->get_total_sales();  --------------------UNUSED---------------------------
        model->complain(t);
        int new_score = model->get_score();
        int new_sales = model->get_total_sales();
        //extere case: old score is 0!!!!!!!
        if(old_score == 0){
            //search in zero_scored_types_tree, remove it and insert is to models_by_score_tree
            addToModelsByScoreTree(zero_scored_types_tree.getNode(CarType(typeID, 0))->get_key(), *model);
        }
        else if (new_score == 0)
        {
            //exetreme case: new score is 0!!!! > DO NOT INSERT to models_by_score_tree
            addModelToZeroScroredModels(CarType(typeID, 0), CarModelByGrade(typeID, old_score) );
        }
        else{
            //delete model from models_by_score_tree
            models_by_score_tree.remove(CarModelByGrade(typeID, modelID, old_score));
            //re-insert to models_by_score_tree with the updateed info
            
            models_by_score_tree.insert(CarModelByGrade(typeID, modelID, new_score, new_sales));
        }
        //if modelid was best > check again
    }
    ///model doesnt exist in the models_by_type_tree
    //model is unused (0 sells, 0 score)
    catch (const DataStructures::NodeNotExist &e){
        //should not reach here
        throw Assert();
    }
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType CDM::GetBestSellerModelByType(int typeID, int * modelID){
    if(typeID == 0) *modelID = best_seller_model;
    else if(!isValidTypeId(typeID)) return INVALID_INPUT;
    try{
        Node<CarType>* result = types_tree.getNode(CarType(typeID, 0));
        *modelID = result->get_key()->best_seller_model;
    }
    catch(const DataStructures::NodeNotExist &e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType CDM::GetWorstModels(int numOfModels, int *types, int *models){
    if(numOfModels <= 0) return INVALID_INPUT;
    else if(numOfModels > total_num_of_models) return FAILURE;

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
    
