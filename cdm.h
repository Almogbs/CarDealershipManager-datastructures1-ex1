#ifndef CDM_H_
#define CDM_H_

#include "library.h"
#include "avl_tree.h"
#include "node.h"
#include "car_type.h"
#include "car_model.h"


class CDM {
    AVLTree<CarType>            types_tree;                 //AVL Tree of all Types, sorted by TypeID
    AVLTree<CarType>            zero_scored_types_tree;     //AVL Tree of all Types which hold at least one Model with the Score: 0, sorted by TypeID
    AVLTree<CarModel>           models_by_type_tree;        //AVL Tree of all Models, sorted by TypeID>ModelID
    AVLTree<CarModelByGrade>    models_by__pos_score_tree;  //AVL Tree of all Models with Positive Scrore, sorted by Score>TypeID>ModelID
    AVLTree<CarModelByGrade>    models_by_neg_score_tree;   //AVL Tree of all Models with Negative Scrore, sorted by Score>TypeID>ModelID
    CarType*                    best_seller_type;           //Pointer to ???

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
