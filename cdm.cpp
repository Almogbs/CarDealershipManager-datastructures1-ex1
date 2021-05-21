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
        best_seller_sales = temp->get_total_sales();
        best_seller_type = temp->get_type_id();
        return;
    }
    if(types_tree.getSize() == 0) {
        best_seller_type = UNINITIALIZED;
        best_seller_sales = UNINITIALIZED;
        best_seller_model = UNINITIALIZED;
        return;
    }
    best_seller_type = types_tree.getMinNode()->get_key()->get_type_id();
    best_seller_sales = 0;
    best_seller_model = 0;
}

void CDM::InsertOrRemoveFromZeroScoredTypesTree(CarType car_type, int action)
{
    if (action == ADD_TO_ZST){
        this->zero_scored_types_tree.insert(CarType(car_type.get_type_id(), 0));
    }
    else if (action == REMOVE_FROM_ZST){
        this->zero_scored_types_tree.remove(car_type);
    }
    return;
}

//score wasnt zero and now turned to zero
void CDM::addModelToZeroScroredModels(CarType car_type, CarModelByGrade car_model_by_grade){
    //type exists in zero_scored_type_tree
    try{
        CarType* insert_to = zero_scored_types_tree.getNode(car_type)->get_key();
        insert_to->zero_scored_models->insert(CarModel(car_model_by_grade.get_type_id(),car_model_by_grade.get_model_id(), 
                                                      0, car_model_by_grade.get_total_sales()));
    }
    //type not exists in zero_scored_type_tree
    catch(const DataStructures::NodeNotExist &e){
        InsertOrRemoveFromZeroScoredTypesTree(car_type, ADD_TO_ZST);
        CarType* insert_to = zero_scored_types_tree.getNode(car_type)->get_key();
        insert_to->zero_scored_models->insert(CarModel(car_model_by_grade.get_type_id(),car_model_by_grade.get_model_id(), 
                                                      0, car_model_by_grade.get_total_sales()));
    }
    models_by_score_tree.remove(car_model_by_grade);  //not forget to search by old_score
}

//score was zero and now it isnt
void CDM::addToModelsByScoreTree(CarType* car_type, CarModel car_model){
    CarModelByGrade car_model_by_grade =  CarModelByGrade(car_model.get_type_id(), car_model.get_model_id(),
                                                          car_model.get_score(), car_model.get_total_sales());
    try{
        car_type->zero_scored_models->remove(car_model); 
    }
    catch(const DataStructures::NodeNotExist &e) {};
    //car_type->zero_scored_models->InOrderGetFirst();                                                    
    models_by_score_tree.insert(car_model_by_grade);
    if (car_type->zero_scored_models->getSize() == 0){
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
        if((best_seller_type == UNINITIALIZED) || ((best_seller_type > typeID) && (best_seller_sales == 0))){
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
            int current_model_sales = 0;
            CarModel current_model = CarModel(typeID, i);

            //remove the current model of the type from the models_by_type_tree, if exists there
            try{
                try{
                    current_model_score = models_by_type_tree.getNode(current_model)->get_key()->get_score();
                    models_by_score_tree.remove(CarModelByGrade(typeID, i, current_model_score));
                }
                catch(const DataStructures::NodeNotExist &e){}
                try{
                    current_model_sales = models_by_type_tree.getNode(current_model)->get_key()->get_total_sales();
                    models_by_sales_tree.remove(CarModelBySales(typeID, i, current_model_score, current_model_sales));
                }
                catch(const DataStructures::NodeNotExist &e){}
                try{
                    models_by_type_tree.remove(current_model);
                }
                catch(const DataStructures::NodeNotExist &e){}
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
    //checks if the type exist and the validity of the input
    try{
        CarType* car_type_node = types_tree.getNode(CarType(typeID, 0))->get_key();
        if(car_type_node->getNumOfModels() <= modelID) return FAILURE;
    }
    catch (const DataStructures::NodeNotExist &e){
        return FAILURE;
    }
    //checks if model exists in models_by_type_tree
    //model is used(sell>0)
    try{
        CarModel* model = models_by_type_tree.getNode(CarModel(typeID, modelID))->get_key();
        //saves old score and old sales
        int old_score = model->get_score();
        int old_sales = model->get_total_sales();
        model->addSale();
        int new_score = model->get_score();
        int new_sales = model->get_total_sales();
        //in case model is in the zero_scored_types_tree Node
        if(old_score == 0){
            //removing from zero_scored_types_tree and inserting to models_by_score_tree
            addToModelsByScoreTree(zero_scored_types_tree.getNode(CarType(typeID, 0))->get_key(), *model);
        }
        //in case model is in the models_by_score_tree and now should be in zero_scored_types_tree
        else if(new_score == 0){
            //removing from models_by_score_tree and inserting to zero_scored_types_tree
            addModelToZeroScroredModels(CarType(typeID, 0), CarModelByGrade(typeID, modelID, old_score, new_sales));
        }
        //just changing the score form non-zero to non-zero 
        else{
            models_by_score_tree.remove(CarModelByGrade(typeID, modelID, old_score, old_sales));
            models_by_score_tree.insert(CarModelByGrade(typeID, modelID, new_score, new_sales));
        }
        //update sells tree
        if(old_sales != 0){
            models_by_sales_tree.remove(CarModelBySales(typeID, modelID, old_score, old_sales));
        }
        models_by_sales_tree.insert(CarModelBySales(typeID, modelID, new_score, new_sales));
        //update types tree
        CarType* type = types_tree.getNode(CarType(typeID, 0))->get_key();
        type->addSale();
        type->updateBestSeller(modelID, new_sales);
        //update cdm best seller
        updateBestSeller();
    }
    //model is not in models_by_type_tree and therefore unused until now
    catch (const DataStructures::NodeNotExist &e){
        CarModel model = CarModel(typeID, modelID);
        model.addSale();
        models_by_type_tree.insert(model);
        addToModelsByScoreTree(zero_scored_types_tree.getNode(CarType(typeID, 0))->get_key(), model);//...
        //insert to models_by_sales_tree
        models_by_sales_tree.insert(CarModelBySales(typeID, modelID, model.get_score(), model.get_total_sales()));
        //update types tree
        CarType* type = types_tree.getNode(CarType(typeID, 0))->get_key();
        type->addSale();
        type->updateBestSeller(modelID, model.get_total_sales());
        //update cdm best seller
        updateBestSeller();
    }
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

StatusType CDM::makeComplaint(int typeID, int modelID, int t){
    if ( !isValidTypeId(typeID) || !isValidModelId(modelID)) {
        return INVALID_INPUT;
    }

    //checks if the type exist and the validity of the input
    try{
        CarType* car_type_node = types_tree.getNode(CarType(typeID, 0))->get_key();
        if(car_type_node->getNumOfModels() <= modelID) return FAILURE;
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
        model->complain(t);
        int new_score = model->get_score();
        int new_sales = model->get_total_sales();
        //exterme case: old score is 0
        if(old_score == 0){
            //search in zero_scored_types_tree, remove it and insert is to models_by_score_tree
            addToModelsByScoreTree(zero_scored_types_tree.getNode(CarType(typeID, 0))->get_key(), *model);
        }
        else if (new_score == 0)
        {
            //exetreme case: new score is 0!!!! > DO NOT INSERT to models_by_score_tree
            addModelToZeroScroredModels(CarType(typeID, 0), CarModelByGrade(typeID, modelID, old_score));
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
        //throw Assert();
    }
    catch(const std::bad_alloc &e){
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

//fix ths \/
StatusType CDM::GetBestSellerModelByType(int typeID, int * modelID){
    if(typeID == 0) {
        if(best_seller_model == UNINITIALIZED){
            return FAILURE;
        }
        *modelID = best_seller_model;
        return SUCCESS;
    }
    else if(!isValidTypeId(typeID)) return INVALID_INPUT;
    try{
        Node<CarType>* result = types_tree.getNode(CarType(typeID, 0));
        if(result->get_key()->best_seller_model == UNINITIALIZED){
            return FAILURE;
        }
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
        //cout << iterator_negative->get_type_id() << " " << iterator_negative->get_model_id() << " " << iterator_negative->get_score() << endl; ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        iterator_negative = models_by_score_tree.InOrderGetNext();
    }
    if(counter == numOfModels) return SUCCESS;
    else if(counter > numOfModels) {
        //cout << counter << " " << numOfModels << endl;
        throw Assert();

    }

    CarType* iterator_zero = zero_scored_types_tree.InOrderGetFirst();
    //go on with the zero scored models
    //get in only if the tree isnt empty
    for(int zero_tree_counter = 0; zero_tree_counter < zero_scored_types_tree.getSize() && counter < numOfModels; zero_tree_counter++){
        AVLTree<CarModel>* current_type = iterator_zero->zero_scored_models;
        CarModel* current_model = current_type->InOrderGetFirst();
        //go on with the zero models inside the current type
        for(int inner_counter = 0; inner_counter < current_type->getSize() && counter < numOfModels; counter++, inner_counter++){
            types[counter] = current_model->get_type_id();
            models[counter] = current_model->get_model_id();
            //cout << current_model->get_type_id() << " "<< current_model->get_model_id() << " "<< current_model->get_score() << endl; ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            current_model = current_type->InOrderGetNext();
        }
        iterator_zero = zero_scored_types_tree.InOrderGetNext();
    }

    if(counter == numOfModels) return SUCCESS;
    else if(counter > numOfModels) throw Assert();
    
    //get in only if the tree sint empty
    //start with the models with positive score first
    for(int inner_counter = 0; inner_counter < models_by_score_tree.getSize() && counter < numOfModels; counter++, inner_counter++){
        types[counter] = iterator_negative->get_type_id();
        models[counter] = iterator_negative->get_model_id();
        //cout << iterator_negative->get_type_id()<< " " << iterator_negative->get_model_id()<< " " << iterator_negative->get_score() << endl; ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        iterator_negative = models_by_score_tree.InOrderGetNext();
    }
    return SUCCESS;
}
    
