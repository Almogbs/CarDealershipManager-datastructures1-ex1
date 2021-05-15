#ifndef CAR_MODEL_H_
#define CAR_MODEL_H_


#include "avl_tree.h"


#define SCORE_ARG_COMPLAIN 100
#define SCORE_ARG_SELL 10

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
        //~CarModel();
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
        CarModelByGrade(int type_id, int model_id, int score = 0, int sales = 0);
        bool operator==(const CarModel&) const override;
        bool operator>(const CarModel&) const override;
        
    };
    bool operator!=(const CarModelByGrade&, const CarModelByGrade&);   
    bool operator>=(const CarModelByGrade&, const CarModelByGrade&);
    bool operator<(const CarModelByGrade&, const CarModelByGrade&);
    bool operator<=(const CarModelByGrade&, const CarModelByGrade&);

    class CarModelBySales : public CarModel{
    public:
        CarModelBySales(int type_id, int model_id, int score = 0, int sales = 0);
        bool operator==(const CarModel&) const override;
        bool operator>(const CarModel&) const override;
    };
    bool operator!=(const CarModelBySales&, const CarModelBySales&);   
    bool operator>=(const CarModelBySales&, const CarModelBySales&);
    bool operator<(const CarModelBySales&, const CarModelBySales&);
    bool operator<=(const CarModelBySales&, const CarModelBySales&);
}


#endif /* CAR_MODEL_H_ */
