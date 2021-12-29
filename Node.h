#ifndef NODE_LIBRARY_H
#define NODE_LIBRARY_H

template<typename KEY, typename DATA>
class Node{
private:
    KEY key;
    DATA* data;
    Node* left;
    Node* right;
    int height;
public:
    Node(KEY k, DATA* d):key(k), data(d), left(nullptr), right(nullptr), height(0){
    }

    ~Node(){
        delete data;
    }

    DATA* getData(){
        if(!this){
            return nullptr;
        }
        return this->data;
    }

    KEY getKey(){
        return this->key;
    }

    void setKey(KEY new_key){
        if(!this){
            return;
        }
        this->key = new_key;
    }

    Node* getLeftSon(){
        if(!this){
            return nullptr;
        }
        return this->left;
    }

    Node* getRightSon(){
        if(!this){
            return nullptr;
        }
        return this->right;
    }

    void setLeftSon(Node<KEY, DATA>* new_son){
        if(!this){
            return;
        }
        this->left = new_son;
    }
    void setRightSon(Node<KEY, DATA>* new_son){
        if(!this){
            return;
        }
        this->right = new_son;
    }

    void setData(DATA* data){
        if(!this){
            return;
        }
        this->data = data;
    }

    int getHeight(){
        return this->height;
    }

    void setHeight(int height){
        this->height = height;
    }
};


#endif //NODE_LIBRARY_H
