#ifndef UNTITLED1_PREDICTIONELEMENTS_H
#define UNTITLED1_PREDICTIONELEMENTS_H

#include "TreeAVL.h"
#include <time.h>

class Element{
    string ID;
    double range;
    int count;
    double weight;
    time_t last_time;
    time_t tmp_last_time;
public:
    Element():ID(""), range(0), count(0), weight(0){
        time(&last_time);
        tmp_last_time = last_time;
    }
    ~Element()=default;
    void setWeight(double new_weight){
        this->weight = new_weight;
    }
    void inc(){
        count++;
    }
    void updateTmpTime(time_t cur_time){
        tmp_last_time = cur_time;
    }

    time_t getTmpTime(){
        return tmp_last_time;
    }

    time_t getLastTime(){
        return last_time;
    }

    double getRange(){
        return range;
    }

    void updateTime(time_t cur_time){
        last_time = cur_time;
    }

    double getWeight(){
        return this->weight;
    }
    void updateElement(Element* base){
        //this->inc();
        double tmp_weight = ((weight*count)+(base->weight))/(count+1);
        this->setWeight(tmp_weight);
        double tmp_range = (((range*count))+(difftime(base->last_time, last_time)))/(count+1);
        range = tmp_range;
        this->inc();
        last_time = base->last_time;
    }
    string getID(){
        return this->ID;
    }
};


class WeightTreeElement{
public:
    TreeAVL<string, Element>* byKey;
};


#endif //UNTITLED1_PREDICTIONELEMENTS_H
