#ifndef CAR_MODEL_H_
#define CAR_MODEL_H_


#include "avl_tree.h"

using namespace DataStructures;

namespace DataStructures 
{
    class CarModel {
    protected:
        int     model_id;
        int     type_id;
        int     sales;
        int     score;
    public:
        CarModel(int type_id, int model_id, int score = 0, int sells = 0);
        CarModel() = default;
        int get_score() const;
        int get_total_sales() const;
        int get_model_id() const;
        int get_type_id() const;
        void addSale();
        void complain(int t);
        virtual bool operator==(const CarModel&) const;
        virtual bool operator>(const CarModel&) const;
        friend std::ostream& operator<<(std::ostream&, const CarModel&);
    };
    std::ostream& operator<<(std::ostream&, const CarModel&);
    bool operator!=(const CarModel&, const CarModel&);   
    bool operator>=(const CarModel&, const CarModel&);
    bool operator<(const CarModel&, const CarModel&);
    bool operator<=(const CarModel&, const CarModel&);


    class CarModelByGrade : public CarModel{
    public:
        bool operator==(const CarModel&) const override;
        bool operator>(const CarModel&) const override;
    };
}


#endif /* CAR_MODEL_H_ */
