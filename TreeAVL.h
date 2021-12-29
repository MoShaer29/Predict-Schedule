#ifndef UNTITLED1_TREEAVL_H
#define UNTITLED1_TREEAVL_H

#include <iostream>
#include "Node.h"

using namespace std;

template<typename T, typename R>
class TreeAVL
{
    Node<T, R>* root;

    Node<T, R>* search(T key, Node<T, R>* current){
        if(!current){
            return nullptr;
        }
        if(current->getKey() == key){
            return current;
        }
        else if(current->getKey() < key){
            return search(key, current->getRightSon());
        }
        else{
            return search(key, current->getLeftSon());
        }
    }

    Node<T, R>* searchSmaller(T key, Node<T, R>* current){
        if(!current){
            return nullptr;
        }
        Node<T, R>* max_smaller;
        if(key < current->getKey()){
           return searchSmaller(key, current->getLeftSon());
        }
        if(key > current->getKey()){
            max_smaller = searchSmaller(key, current->getRightSon());
            if(!max_smaller){
                return current;
            }
            if(max_smaller->getKey() > current->getKey()){
                return max_smaller;
            }
            return current;
        }
        return nullptr;
    }

    Node<T, R>* searchSmallerEqual(T key, Node<T, R>* current){
        Node<T, R>* tmp = this->search(key, root);
        if(tmp){
           return tmp;
        }
        else{
            return searchSmaller(key, root);
        }
    }

    void makeEmpty(Node<T, R>* t){
        if(t == NULL)
            return;
        makeEmpty(t->getLeftSon());
        makeEmpty(t->getRightSon());
        delete t;
    }

    Node<T, R>* insert(R* data, T key, Node<T, R>* t){
        if(t == nullptr){
            t = new Node<T, R>(key, data);
        }
        else if(key < t->getKey()){
            t->setLeftSon(insert(data, key, t->getLeftSon()));
            if(height(t->getLeftSon()) - height(t->getRightSon()) == 2)
            {
                if(key < t->getLeftSon()->getKey())
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if(key > t->getKey())
        {
            t->setRightSon(insert(data, key, t->getRightSon()));
            if(height(t->getRightSon()) - height(t->getLeftSon()) == 2)
            {
                if(key > t->getRightSon()->getKey())
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->setHeight(max(height(t->getLeftSon()), height(t->getRightSon()))+1);
        return t;
    }

    Node<T, R>* singleRightRotate(Node<T, R>* t)
    {
        Node<T, R>* u = t->getLeftSon();
        t->setLeftSon(u->getRightSon());
        u->setRightSon(t);
        t->setHeight(max(height(t->getLeftSon()), height(t->getRightSon()))+1);
        u->setHeight(max(height(u->getLeftSon()), t->getHeight())+1);
        return u;
    }

    Node<T, R>* singleLeftRotate(Node<T, R>* t)
    {
        Node<T, R>* u = t->getRightSon();
        t->setRightSon(u->getLeftSon());
        u->setLeftSon(t);
        t->setHeight(max(height(t->getLeftSon()), height(t->getRightSon()))+1);
        u->setHeight(max(height(t->getRightSon()), t->getHeight())+1);
        return u;
    }

    Node<T, R>* doubleLeftRotate(Node<T, R>* t)
    {
        t->setRightSon(singleRightRotate(t->getRightSon()));
        return singleLeftRotate(t);
    }

    Node<T, R>* doubleRightRotate(Node<T, R>* t)
    {
        t->setLeftSon(singleLeftRotate(t->getLeftSon()));
        return singleRightRotate(t);
    }

    Node<T, R>* findMin(Node<T, R>* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->getLeftSon() == NULL)
            return t;
        else
            return findMin(t->getLeftSon());
    }

    Node<T, R>* findMax(Node<T, R>* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->getRightSon() == NULL)
            return t;
        else
            return findMax(t->getRightSon());
    }

    Node<T, R>* remove(T key, Node<T, R>* t)
    {
        Node<T, R>* temp;

        // Element not found
        if(t == NULL)
            return NULL;

            // Searching for element
        else if(key < t->getKey())
            t->setLeftSon(remove(key, t->getLeftSon()));
        else if(key > t->getKey())
            t->setRightSon(remove(key, t->getRightSon()));

            // Element found
            // With 2 children
        else if(t->getLeftSon() && t->getRightSon())
        {
            temp = findMin(t->getRightSon());
            t->setData(temp->getData());
            t->setKey(temp->getKey());
            t->setRightSon(remove(t->getKey(), t->getRightSon()));
        }
            // With one or zero child
        else
        {
            temp = t;
            if(t->getLeftSon() == NULL)
                t = t->getRightSon();
            else if(t->getRightSon() == NULL)
                t = t->getLeftSon();
            delete temp;
        }
        if(t == NULL)
            return t;

        t->setHeight(max(height(t->getLeftSon()), height(t->getRightSon()))+1);

        // If Node is unbalanced
        // If left Node is deleted, right case
        if(height(t->getLeftSon()) - height(t->getRightSon()) == 2)
        {
            // right right case
            if(height(t->getLeftSon()->getLeftSon()) - height(t->getLeftSon()->getRightSon()) == 1)
                return singleLeftRotate(t);
                // right left case
            else
                return doubleLeftRotate(t);
        }
            // If right Node is deleted, left case
        else if(height(t->getRightSon()) - height(t->getLeftSon()) == 2)
        {
            // left left case
            if(height(t->getRightSon()->getRightSon()) - height(t->getRightSon()->getLeftSon()) == 1)
                return singleRightRotate(t);
                // left right case
            else
                return doubleRightRotate(t);
        }
        return t;
    }

    int height(Node<T, R>* t)
    {
        return (t == NULL ? -1 : t->getHeight());
    }

    int getBalance(Node<T, R>* t)
    {
        if(t == NULL)
            return 0;
        else
            return height(t->getLeftSon()) - height(t->getRightSon());
    }

    void inorder(Node<T, R>* t)
    {
        if(t == NULL)
            return;
        inorder(t->getLeftSon());
        cout << t->getKey() << " ";
        inorder(t->getRightSon());
    }

public:
    TreeAVL()
    {
        root = NULL;
    }

    ~TreeAVL(){
        makeEmpty(root);
    }

    void insert(T key, R* data)
    {
        root = insert(data, key, root);
    }

    void remove(T key)
    {
        root = remove(key, root);
    }

    R* search(T key){
        Node<T, R>* tmp = this->search(key, root);
        if(!tmp){
            return nullptr;
        }
        return tmp->getData();
    }

    Node<T, R>* searchSmallerEqual(T key){
        return this->searchSmallerEqual(key, root);
    }

    bool isEmpty(){
        if(!root){
            return true;
        }
        return false;
    }

    void display()
    {
        inorder(root);
        cout << endl;
    }

    Node<T, R>* getRoot(){
        return this->root;
    }
};

#endif //UNTITLED1_TREEAVL_H
