#ifndef CDM_H_
#define CDM_H_

#include "library.h"
#include "avl_tree.h"
#include "node.h"
#include "car_type.h"
#include "car_model.h"


class CDM {
public:
    AVLTree<CarType>            types_tree;                 //AVL Tree of all Types, sorted by TypeID
    AVLTree<CarType>            zero_scored_types_tree;     //AVL Tree of all Types which hold at least one Model with the Score: 0, sorted by TypeID
    AVLTree<CarModel>           models_by_type_tree;        //AVL Tree of all Models, sorted by TypeID>ModelID
    AVLTree<CarModelByGrade>    models_by_score_tree;       //AVL Tree of all Models, sorted by Score>TypeID>ModelID
    AVLTree<CarModelBySales>    models_by_sales_tree;       //AVL Tree of all Models, sorted by Sales>TypeID>ModelID
    int                         best_seller_type;           //Store the best Seller Model's type
    int                         best_seller_model;          //Store the best Seller Model's model
    int                         best_seller_sales;          //Store the best Seller Model's sales
    int                         total_num_of_models;        //total models in CDM (M)



    //builds or deletes ZSM for car_type
    void InsertOrRemoveFromZeroScoredTypesTree(CarType car_type, int action);
    void addModelToZeroScroredModels(CarType car_type, CarModelByGrade car_model_by_grade);
    void addToModelsByScoreTree(CarType* car_type, CarModel car_model);


    //update the global top seller model in cdm
    void updateBestSeller();
    //update the top seller car in the typeID node of types_tree
    //void updateBestSellerOfType(int typeID);
    void updateAllModelTrees(CarModel car_model);

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
