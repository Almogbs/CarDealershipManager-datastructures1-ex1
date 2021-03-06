#include"library.h"
#include "cdm.h"

void* Init() {
    CDM *DS = new CDM (); 
    return (void*)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels){
    if(DS == NULL) return INVALID_INPUT;
    return ((CDM *)DS)-> AddCarType(typeID, numOfModels);
}

StatusType RemoveCarType(void *DS, int typeID){
    if(DS == NULL) return INVALID_INPUT;
    return ((CDM *)DS)-> RemoveCarType(typeID);
}

StatusType SellCar(void *DS, int typeID, int modelID){
    if(DS == NULL) return INVALID_INPUT;    
    return ((CDM *)DS)-> sellCar(typeID, modelID);
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t){
    if(DS == NULL) return INVALID_INPUT;
    return ((CDM *)DS)-> makeComplaint(typeID, modelID, t);
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID){
    if(DS == NULL) return INVALID_INPUT;
    return ((CDM *)DS)-> GetBestSellerModelByType(typeID, modelID);

}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models){
    if(DS == NULL) return INVALID_INPUT;
    return ((CDM *)DS)-> GetWorstModels(numOfModels, types, models);

}

void Quit(void** DS) {
    if(*DS == NULL) return;
    //((CDM *)(*DS))-> ~CDM();
    //delete *DS;
    delete (CDM*)(*DS);
    *DS = NULL;
}

