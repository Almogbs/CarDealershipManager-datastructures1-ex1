#include "cdm.h"
#include "avl_tree.h"
#include "car_type.h"
#include "car_model.h"
#include <iostream>
#include <stdlib.h>

int main(void)
{
    CDM Tesla;
    Tesla.AddCarType(5,6);
    Tesla.AddCarType(2,7);
    Tesla.zero_scored_types_tree.InOrderGetFirst()->zero_scored_models->printInorder();
    Tesla.zero_scored_types_tree.InOrderGetNext()->zero_scored_models->printInorder();

    return 0;
}