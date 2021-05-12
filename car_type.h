#ifndef CAR_TYPE_H_
#define CAR_TYPE_H_

#include "avl_tree.h"
#include "car_model.h"
#include "linked_list.h"
//#include <iostream>

using namespace DataStructures;

typedef int ModelId;
namespace DataStructures {
    class CarType {
        int       type_id;
        int       num_of_models;
    public:
        int       total_sales;
        int       best_car;

        //ListNode<AVLTree<CarModel>*>* list_node_ptr;
        AVLTree<CarModel>  zero_scored_models;
        
        CarType(int type_id, int num_of_models);
        ~CarType();
        int get_num_of_models() const;
        int get_type_id() const;
        bool operator==(const CarType&) const;
        bool operator>(const CarType&) const;
        friend std::ostream& operator<<(std::ostream&, const CarType&);
    };
    std::ostream& operator<<(std::ostream&, const CarType&);
    bool operator!=(const CarType&, const CarType&);
    bool operator<=(const CarType&, const CarType&);
    bool operator>=(const CarType&, const CarType&);  
    bool operator<(const CarType&, const CarType&);

    //class :     
}


#endif /* CAR_TYPE_H_ */
