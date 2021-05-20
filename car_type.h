#ifndef CAR_TYPE_H_
#define CAR_TYPE_H_

#include "avl_tree.h"
#include "car_model.h"
#include <stdlib.h>
#include <iostream>

using namespace DataStructures;

typedef int ModelId;
namespace DataStructures {
    class CarType {
        int       type_id;
        int       num_of_models;
    public:
        int       total_sales;
        int       best_seller_model;                         //by sales!!!!!!!!!!!!!!
        int       num_of_sales_of_best_seller_model;         //by sales!!!!!!!!!!!!!!

        AVLTree<CarModel>*  zero_scored_models;
        
        CarType(int type_id, int num_of_models);
        CarType(const CarType&);
        ~CarType();
        int getNumOfModels() const;
        int get_type_id() const;
        void addSale();
        void updateBestSeller(int modelID, int num_of_sales);
        void operator=(const CarType& type);
        bool operator==(const CarType&) const;
        bool operator>(const CarType&) const;
        friend std::ostream& operator<<(std::ostream&, const CarType&);
    };
    std::ostream& operator<<(std::ostream&, const CarType&);
    bool operator!=(const CarType&, const CarType&);
    bool operator<=(const CarType&, const CarType&);
    bool operator>=(const CarType&, const CarType&);  
    bool operator<(const CarType&, const CarType&);
}


#endif /* CAR_TYPE_H_ */
