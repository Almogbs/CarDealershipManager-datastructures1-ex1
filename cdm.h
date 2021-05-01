#ifndef CDM_H_
#define CDM_H_

#include "library.h"


class CDM {
public:
    CDM();
    ~CDM();
    StatusType AddCarType(int typeID, int numOfModels);
    StatusType RemoveCarType(int typeID);
    StatusType sellCar(int typeID, int modelID);
    StatusType makeComplaint(int typeID, int modelID, int t);
    StatusType GetBestSellerModelByType(int typeID, int * modelID);
    StatusType GetWorstModels(int numOfModels, int *types, int *models);
};


#endif /* CDM_H_ */
