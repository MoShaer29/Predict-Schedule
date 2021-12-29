#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "ChainList.h"
#include "Element.h"
using namespace std;

#define NUM_DAYS 7
#define SEC_IN_WEEK 604800
#define SEC_IN_DAY 86400

double calcWeight(string str){
    int point_place = str.find_first_of('.');
    double left = 0, right = 0;
    if(point_place != string::npos){
        left = atoi(str.substr(0, point_place-1).c_str());
        right = atoi(str.substr(point_place+1).c_str());
        right = right / pow(10, str.substr(point_place+1).length());
        return left+right;
    }
    else{
        return atoi(str.c_str());
    }
}

void configureData(string *string_data, ChainList<string, Element>** data_saver){
    int cur = 0, len = 0, last = 0;
    double weight = 0;
    time_t cur_time;
    time(&cur_time);
    string tmp;
    for (int i = 0; i < NUM_DAYS; ++i) {
        cur = 0; len = 0; last = 0;
        if(string_data[i].compare("empty") == 0){
            continue;
        }
        while((cur <= string_data[i].length()) && (last != string::npos)) {
            len = string_data[i].find_first_of('=', cur);
            tmp = string_data[i].substr(cur, len);
            last = string_data[i].find_first_of(' ', len + 1);
            Element *cur_data = new Element();
            weight = calcWeight(string_data[i].substr(len + 1, last));
            cur_data->setWeight(weight);
            cur_data->updateTime(cur_time+(i*SEC_IN_DAY));
            data_saver[i]->insert(tmp, cur_data);
            cur = last + 1;
        }
    }
}

bool beenAWeek(time_t* cur, time_t* adding_time){
    time(cur);
    if(difftime(*cur, *adding_time) >= SEC_IN_WEEK){
        return true;
    }
    return false;
}

void addNewData(ChainList<string, Element>** data_saver, TreeAVL<string, Node<string, Element>*>* tree, TreeAVL<double, WeightTreeElement>* predict_tree){
    for (int i = 0; i < NUM_DAYS; ++i) {
        ChainList<string, Element>* tmp_data = data_saver[i];
        Node<string, Element>* cur_data = tmp_data->getHead();
        while(cur_data){
            Node<string, Element>** cur_node_search_tree = tree->search(cur_data->getKey());
            if(cur_node_search_tree) {
                Node<string, Element> *copy_cur_node_search_tree = *cur_node_search_tree;
                Element *cur_elem = copy_cur_node_search_tree->getData();
                WeightTreeElement* weight_node = predict_tree->search(copy_cur_node_search_tree->getData()->getWeight());
                weight_node->byKey->remove(copy_cur_node_search_tree->getKey());
                if(weight_node->byKey->isEmpty()){
                    predict_tree->remove(copy_cur_node_search_tree->getData()->getWeight());
                }
                cur_elem->updateElement(cur_data->getData());
                WeightTreeElement* tmp_weight_tree = predict_tree->search(cur_elem->getWeight());
                if(tmp_weight_tree){
                    tmp_weight_tree->byKey->insert(cur_data->getKey(), cur_elem);
                }
                else{
                    WeightTreeElement* tmp_weight = new WeightTreeElement();
                    tmp_weight->byKey->insert(cur_data->getKey(), cur_elem);
                    predict_tree->insert(cur_elem->getWeight(), tmp_weight);
                }
                *cur_node_search_tree = predict_tree->search(cur_elem->getWeight())->byKey->searchSmallerEqual(cur_data->getKey());
            }
            else{
                cur_data->getData()->inc();
                WeightTreeElement* weight_node = predict_tree->search(cur_data->getData()->getWeight());
                if(weight_node){
                    /*RangeTreeElement* range_node = weight_node->byRange->search(0);
                    if(range_node){
                        range_node->byID->insert(cur_data->getKey(), cur_data->getData());
                    }
                    else{*/
                        //RangeTreeElement* tmp = new RangeTreeElement();
                        weight_node->byKey->insert(cur_data->getKey(), cur_data->getData());
                        //weight_node->byRange->insert(0, tmp);
                    //}
                } else{
                    WeightTreeElement* tmp_weight = new WeightTreeElement();
                    //RangeTreeElement* tmp_range = new RangeTreeElement();
                    //tmp_range->byID->insert(cur_data->getKey(), cur_data->getData());
                    tmp_weight->byKey->insert(cur_data->getKey(), cur_data->getData());
                    predict_tree->insert(cur_data->getData()->getWeight(), tmp_weight);
                }
                Node<string, Element>* tmp = predict_tree->search(cur_data->getData()->getWeight())->byKey->searchSmallerEqual(cur_data->getKey());
                tree->insert(tmp->getKey(), &tmp);
            }
            cur_data = cur_data->getRightSon();
        }
    }
}

Element* findMatchRange(Node<string, Element>* cur, time_t cur_time){
    if(!cur){
        return nullptr;
    }
    if(difftime(cur_time, cur->getData()->getTmpTime()) >= (cur->getData()->getRange())){
        return cur->getData();
    }
    findMatchRange(cur->getLeftSon(), cur_time);
    findMatchRange(cur->getRightSon(), cur_time);
}

ChainList<string, Element>** predict(double max_weight, int working_days, TreeAVL<double, WeightTreeElement>* predict_tree){
    ChainList<string, Element>** cur_week = new ChainList<string, Element>* [working_days];
    for (int k = 0; k < working_days; ++k) {
        cur_week[k] = new ChainList<string, Element>();
    }
    double* left_weight_per_day = new double[working_days];
    for (int i = 0; i < working_days; ++i) {
        left_weight_per_day[i] = max_weight;
    }
    TreeAVL<string, Element>* keys_tree;
    Node<double, WeightTreeElement>* weight_node;
    for (int j = 0; j < working_days; ++j) {
        while(left_weight_per_day[j] > 0){
            weight_node = predict_tree->searchSmallerEqual(left_weight_per_day[j]);
            if(!weight_node){
                break;
            }
            keys_tree = weight_node->getData()->byKey;
            if(!keys_tree){
                cout << "There is no Data to base the prediction on." << endl;
                delete [] left_weight_per_day;
                delete [] cur_week;
                return nullptr;
            }
            Node<string, Element>* predict_root = keys_tree->getRoot();
            Element* tmp = findMatchRange(predict_root, time(nullptr));
            if(!tmp){
                left_weight_per_day--;
                continue;
            }
            else{
                cur_week[j]->insert(tmp->getID(), tmp);
                left_weight_per_day[j] -= tmp->getWeight();
            }
        }
    }
    delete [] left_weight_per_day;
    return cur_week;
}

void printPrediction(ChainList<string, Element>** predicted_data, int working_days){
    for (int i = 0; i < working_days; ++i) {
        if(predicted_data[i]){
            predicted_data[i]->display();
        }
    }
}

int main() {
    int num_working_days = -1;
    double max_weight_per_day = -1;
    char op = ' ';
    auto globalTree = new TreeAVL<string, Node<string, Element>*>();
    auto predictionTree = new TreeAVL<double, WeightTreeElement>;
    string tmp_data[NUM_DAYS];
    time_t adding_time, cur_time;
    ChainList<string, Element>* cur_week[NUM_DAYS];
    for (int j = 0; j < NUM_DAYS; ++j) {
        cur_week[j] = new ChainList<string, Element>();
    }
    cout << "Welcome to Mohammad Shaer's Prediction Project." << endl;
    if(num_working_days == -1) {
        cout << "How money working days there is in a week?" << endl;
        cin >> num_working_days;
    }
    if(max_weight_per_day == -1) {
        cout << "What is the max weight per day?" << endl;
        cin >> max_weight_per_day;
    }
    do {
        cout << "To add values for a week press 'a' and to get my prediction for this week press 'p'." << endl;
        cin >> op;
        if (op == 'p') {
            if(beenAWeek(&cur_time, &adding_time)){
                addNewData(cur_week, globalTree, predictionTree);
            }
            ChainList<string, Element>** predicted_data = predict(max_weight_per_day, num_working_days, predictionTree);
            printPrediction(predicted_data, num_working_days);
            char to_save = ' ';
            cout << "Do you want to save this prediction? [y/n]" ;
            cin >> to_save;
            if(to_save == 'y'){
                addNewData(cur_week, globalTree, predictionTree);
            }

        } else if (op == 'a') {
            time(&adding_time);
            cout << "Please Enter the schedule for a week starting from Sunday" << endl;
            cout << "Enter each day in a new line, also add space between each two elements, ";
            cout << "and for each element please add its weight by separating it from the element with '='; for empty days enter 'empty'." << endl;
            cout << "For example:\napple=10 grape=5 food=2\ncoke=1 water=3\n.\n.\n." << endl;
            for (int i = 0; i < NUM_DAYS; ++i) {
                cin >> tmp_data[i];
            }
            configureData(tmp_data, cur_week);
        } else if (op == 'e') {
            return 0;
        }
    }while ((op != 'a') || (op != 'p') || (op != 'e'));
}
