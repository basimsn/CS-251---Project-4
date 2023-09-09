// priorityqueue.h
//
// TODO:  write this header comment
#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <set>
#include <map>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    
public: 

    priorityqueue() { //Default Constructor
        size = 0;
        root = nullptr;
        curr = nullptr;
    }

    NODE* clone(NODE* cCurr, NODE* cParent){
        if(cCurr == nullptr){ //returns nullptr if pointer is null
            return nullptr;
        }
        NODE* nCurr = new NODE; //Creating a new node and defining each part
        
        nCurr->link = nullptr;
        nCurr->left = nullptr;
        nCurr->right = nullptr;
        nCurr->dup = cCurr->dup;
        nCurr->priority = cCurr->priority;
        nCurr->value = cCurr->value;
        nCurr->parent = cParent;
        size++; 

        if(nCurr->dup == true){ //Checks for duplicates
            NODE* tLink = nCurr;
            NODE* linkedNode = cCurr->link;
            NODE* nlinkedNode = nullptr;
            while(linkedNode != nullptr){ //loops through the links creating a new node and making them all point
                NODE* nLink = new NODE; //to each other in order until theres a nullptr
                nlinkedNode = linkedNode->link;
                nLink->priority = linkedNode->priority;
                nLink->value = linkedNode->value;
                nLink->dup = true;
                nLink->parent = tLink;
                nLink->link = nullptr;
                tLink->link = nLink; //Links them together
                tLink = nLink;
                linkedNode = linkedNode->link; //Moves to next duplicate
                size++; 
            }
        }
        nCurr->left = clone(cCurr->left, nCurr); //PreOrder traversal to make a copy
        nCurr->right = clone(cCurr->right, nCurr);

        return nCurr; //returns the node which is then made into *this
    }

    priorityqueue& operator=(const priorityqueue& other) {
        this->clear(); //clear
        NODE* temp = other.root;
        NODE* temp1 = nullptr;
        NODE* nBST = nullptr;
        nBST = clone(temp, temp1); //sets the new binary search tree to a node
        root = nBST; //set the root to the new binary search tree
        return *this;
    }

    void delClear(NODE* cCurr){
        if(cCurr != nullptr){
            delClear(cCurr->left); //PostOrder traversal for deleting nodes
            delClear(cCurr->right);
            if(cCurr->dup ==true){ //checks duplicates
                NODE* linkedNode = nullptr; 
                NODE* nCurr = cCurr->link;
                while(nCurr != nullptr){ //loops through all the duplicates and deletes them
                    (size)--;
                    linkedNode = nCurr->link;
                    delete nCurr;
                    nCurr = linkedNode;
                }
            }
            (size)--; //delets the curr node
            delete cCurr;
        }
        else{
            return;
        }
    }

    void clear() {
        delClear(root); //calls the delete helper function
    }

    ~priorityqueue() {
        // clear();
    }

    void enqueue(T value, int priority) {
        NODE* nRoot = root;
        NODE* prev = nullptr;
        while(nRoot != nullptr){ //Loops through the BST starting with root
            if(priority == nRoot->priority){ //Checks for duplicates
                nRoot->dup = true;
                NODE* tRoot = nRoot;
                while(tRoot->link != nullptr){ //Loops through the duplicates to have the node start at the end of the link
                    tRoot = tRoot ->link;
                }
                NODE* nCurr = new NODE; //creates a new node 
                nCurr->priority = priority;
                nCurr->value = value;
                nCurr->parent = tRoot;
                nCurr->dup = true;
                nCurr->link = nullptr;
                nCurr->left = nullptr;
                nCurr->right = nullptr;

                tRoot->link = nCurr; //points the new node to the end of the link
                size++;
                return;
            }
            else if(priority> nRoot->priority){ //Greater than goes right
                prev = nRoot;
                nRoot = nRoot->right;
            }
            else{ //Less than goes left
                prev = nRoot;
                nRoot= nRoot->left;
            }
        }
        NODE* nCurr = new NODE; //Creating a new node
        nCurr->priority = priority;
        nCurr->value = value;
        nCurr->parent = prev;
        nCurr->dup = false;
        nCurr->left = nullptr;
        nCurr->right = nullptr;
        nCurr->link = nullptr;
        
        if(prev == nullptr){ //First enqueue is the root
            root = nCurr;
        }
        else if(priority > prev->priority){ //Adds the new node to BST
            prev->right = nCurr;
        }
        else{
            prev->left = nCurr;
        }
        size++; //Increases size
    }

    T dequeue() {
        //Big L 
    }

    int Size() {
        return size;
    }

    void begin() {
        NODE* nRoot = root; //Temp Node
        curr = root;   
        if(nRoot == nullptr){
            curr = nullptr; 
        }
        else{
            if(nRoot->left == nullptr){  //Checks if there are any values left of the root
                curr = root;
            }
            else{
                while(curr->left != nullptr){ //Loops through to find the leftest node
                    curr = curr->left;
                    nRoot = curr;
                }
            }
        }   
    }

    bool nextDup(NODE* cCurr,T& value, int& priority){ //Next Helper function
        if(cCurr->link != nullptr){ //Checks if there has been any duplicates so far
            value = cCurr->value;
            priority = cCurr->priority;
            curr = cCurr->link; //adds to the link
            return true;
        }
        else if(cCurr->link == nullptr){ //If no duplicates 
            value = cCurr->value;
            priority = =cCurr->priority;
            NODE* nParent = cCurr->parent;
            while(nParent->dup != false){ //Loops through the parents 
                nParent = nParent ->parent;
                cCurr = nParent;
            }
            curr = cCurr->parent; //Points curr to the parent
            return true;
        }
        else{
            return false;
        }
    }

    bool next(T& value, int& priority) {
        NODE* tCurr = curr;
        if(tCurr == nullptr){ //check nullptr
            return false;
        }
        else{
            if(tCurr->dup == true){ //Checks if duplicates
                if(nextDup(tCurr, value, priority)){ //Next helper function
                    return true;
                } 
                else{
                    value = curr->value; 
                    priority = curr->priority;
                }
            }
            if(tCurr->right == nullptr){ //Since begins gets the leftest values the next would be the right subtree if any
                NODE* nParent = tCurr->parent;
                value = curr->value;
                priority = curr->priority;
                while(nParent ->priority < tCurr->priority){ //Loops through the right subtree
                    tCurr = nParent;
                    if(nParent-> parent != nullptr){
                        nParent = nParent->parent;
                    }
                    else{
                        return false;
                    }
                }
                curr= nParent;
                return true;
            }
            else{
                curr = tCurr->right;
                return true;
            }
        }
       
    }

    void buildString(NODE* cNode, ostream& output){ 
        if(cNode != nullptr){  // Checks for null
            buildString(cNode->left, output); //Adds to ouutput starting with curr node
            if(cNode->dup == true){ // Checks for duplicates
                NODE* linkedNode = cNode->link;
                NODE* nLink = nullptr;
                output<< cNode->priority<< " value: "<<cNode->value<<"\n"; //Adds first duplicate
                while(linkedNode!= nullptr){
                    output<<linkedNode->priority<< " value: "<<linkedNode->value<<"\n"; //Loops through the link adding each time
                    linkedNode = linkedNode->link;
                }
                linkedNode = nLink;
            }
            else{
                output<<cNode->priority<<" value: "<<cNode->value<<"\n"; //No duplicates
            }
            buildString(cNode->right, output); //Inorder Traversal for printing out the nodes
        }
        else{
            return;
        }
    }

    string toString() {
        string str;
        if(root != nullptr){ 
            stringstream ss; //stringstream to add to string
            buildString(root, ss);
            str = ss.str();
            return str;
        }
        else{ //If null
            str = "";
            return str;
        }
    }

    T peek() {  
        T valueOut;
        NODE* min = root;
        while(min->left != nullptr){ //loops to find leftest value
            min = min->left;
        }
        valueOut = min->value;

        return valueOut; 
    }
    
    bool doubleEqual(NODE* nCurr, const NODE* oCurr) const { //const at the end to call the function with the parameters of the operator==
        if(nCurr == nullptr && oCurr == nullptr){ //both empty BST
            return true;
        }
        else if(nCurr == nullptr || oCurr == nullptr){ //Checks if one is empty
            return false;
        }
        else{
            if(nCurr->link != nullptr && oCurr->link == nullptr){  //Checks if one has duplicates and one doesn't
                return false;
            }
            else if((nCurr->link == nullptr && oCurr->link != nullptr)){
                return false;
            }
            else{
                const NODE* tOCurr = oCurr;
                NODE* tNCurr = nCurr;
                while(tNCurr != nullptr){ 
                    if(tNCurr->value == tOCurr->value){ //Loops through links
                        tNCurr = tNCurr->link;
                        tOCurr = tOCurr->link;
                    }
                    else{
                        return false;
                    }
                }
            }
            if(doubleEqual(nCurr->left, oCurr->left) && doubleEqual(nCurr->right, oCurr->right) ){  // Checks the BSTS recursively
                return true;
            }
            return false; //changed else statement here
        }
    } 

    bool operator==(const priorityqueue& other) const {
        bool equals = false;
        NODE* nRoot = this->root;
        const NODE* oRoot = other.root;
        if(this->size == other.size){
            equals = doubleEqual(this->root, other.root); //Calls helper function
            if(equals == false){
                return false;
            }
            else{
                return true;
            }
        }
        else{
            return false;
        }
    }

    void* getRoot() {
        return root;
    }
};
