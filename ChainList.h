#ifndef CHAINLIST_CHAINLIST_H
#define CHAINLIST_CHAINLIST_H

#include "Node.h"
#include <iostream>

using namespace std;
template<typename KEY, typename DATA>
class ChainList{
private:
    Node<KEY, DATA>* head;
    Node<KEY, DATA>* tail;

    Node<KEY, DATA>* findPosition(KEY k){
        if(k < head->getKey()){
            return head;
        }
        Node<KEY, DATA>* tmp = this->head;
        while(!tmp){
            if(tmp->getKey() == k){
                return tmp;
            }
            if(tmp->getKey() > k){
                return tmp->getLeftSon();
            }
            tmp = tmp->getRightSon();
        }
        return tail;
    }

public:
    ChainList():head(nullptr), tail(nullptr){
    }

    Node<KEY, DATA>* getHead(){
        return head;
    }

    ChainList(Node<KEY, DATA>* new_node){
        this->head = new_node;
        this->tail = this->head;
    }

    void insert(KEY k, DATA* d){
        Node<KEY, DATA>* new_one = new Node<KEY, DATA>(k, d);
        if(!head){
            head = new_one;
            this->tail = this->head;
        }
        else{
            Node<KEY, DATA>* pre = findPosition(k);
            if(pre == head){
                new_one->setRightSon(head);
                head = new_one;
            }
            else if(pre == tail){
                new_one->setLeftSon(tail);
                tail = new_one;
            }
            else{
                Node<KEY, DATA>* next = pre->getRightSon();
                new_one->setLeftSon(pre);
                new_one->setRightSon(next);
                pre->setRightSon(new_one);
                next->setLeftSon(new_one);
            }
        }
    }

    void remove(KEY k){
        if(!head){
            return;
        }
        Node<KEY, DATA>* tmp = findPosition(k);
        if(tmp->getKey() != k){
            return;
        }
        else{
            Node<KEY, DATA>* pre = tmp->getLeftSon();
            Node<KEY, DATA>* next = tmp->getRightSon();
            pre->setRightSon(next);
            next->setLeftSon(pre);
            tmp->setRightSon(nullptr);
            tmp->setLeftSon(nullptr);
            delete tmp;
        }
    }

    Node<KEY, DATA>* search(KEY k){
        Node<KEY, DATA>* tmp = findPosition(k);
        if(tmp->getKey() == k){
            return tmp;
        }
        return nullptr;
    }

    void display(){
        Node<KEY, DATA>* tmp = head;
        while(tmp){
            if(tmp != tail) {
                cout << tmp->getKey() << ", ";
            }
            else{
                cout << tmp->getKey() << ".";
            }
            tmp = tmp->getRightSon();
        }
    }

    ~ChainList(){
        while(!head){
            Node<KEY, DATA>* tmp = head;
            head = head->getRightSon();
            delete tmp;
        }
    }

};

#endif //CHAINLIST_CHAINLIST_H
