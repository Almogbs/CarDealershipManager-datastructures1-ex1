#ifndef CDM_H_
#define CDM_H_

#include "library.h"
#include "avl_tree.h"
#include "node.h"
#include "car_type.h"
#include "car_model.h"


class CDM {
    AVLTree<CarType>            types_tree;
    AVLTree<CarType>            zero_scored_types_tree;
    AVLTree<CarModel>           models_by_type_tree;
    AVLTree<CarModelByGrade>    models_by_score_tree;
    CarType*                    best_seller_type;

public:
    CDM();
    ~CDM() = default;
    StatusType AddCarType(int typeID, int numOfModels);
    StatusType RemoveCarType(int typeID);
    StatusType sellCar(int typeID, int modelID);
    StatusType makeComplaint(int typeID, int modelID, int t);
    StatusType GetBestSellerModelByType(int typeID, int * modelID);
    StatusType GetWorstModels(int numOfModels, int *types, int *models);
};

#endif /* CDM_H_ */
