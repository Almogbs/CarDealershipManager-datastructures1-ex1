#ifndef CAR_TYPE_H_
#define CAR_TYPE_H_

#include "avl_tree.h"

typedef int ModelId;

class CarType {
    int type_id;
    int num_of_models;
    int total_sales;
    int best_car;
    AVLTree<ModelId> zero_scored_models;
public:
    CarType(int type_id, int num_of_model);
    ~CarType();
    friend bool operator==(const CarType&, const CarType&);
    friend bool operator>(const CarType&, const CarType&);

};


#endif /* CAR_TYPE_H_ */
