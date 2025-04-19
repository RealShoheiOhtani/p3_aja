//
// Created by aidan on 4/17/2025.
//

#ifndef MAXHEAP_H
#define MAXHEAP_H
#include "Node.h"
#include "Movie.h"
#include <queue>

class MaxHeap {
    Node* root; //only variable this has is a root node, everything will be accessed through that.

    void insert(Movie m) {      //find insert position and insert the movie
        if (root == nullptr) {  // if empty, root is node with movie object
            root = new Node(m);
            return;
        }
        std::queue<Node*> q;    //create queue
        q.push(root);       //push root

        while (!q.empty()) {    //until empty current node is at front
            Node* curr = q.front();
            q.pop();

            if (curr->left == nullptr) {    //if open left spot, enter new node. Heap will always have a left node before right
                curr->left = new Node(m, curr);
                heapifyUp(curr->left);  //heapify
                return;
            }
            if (curr->right == nullptr) {   //check if right is open.
                curr->right = new Node(m, curr);
                heapifyUp(curr->right);     //heapify
                return;
            }

            q.push(curr->left); //push left and right if there exists
            q.push(curr->right);
        }
    }
    void heapifyUp(Node* node) {    //while there is parent and parent is less than curr node, swap
        while (node->parent && node->movie.getRATING() > node->parent->movie.getRATING()) {
            std::swap(node->movie, node->parent->movie);
            node = node->parent;    //move to parent to check its parent
        }
    }
    Node* getLastNode(Node* root) { //similar to insert, goes left and right to find last spot
        if (root == nullptr) {
            return nullptr;
        }
        std::queue<Node*> q;
        q.push(root);
        Node* last = nullptr;

        while (!q.empty()) {
            last = q.front();
            q.pop();

            if (last->left) {   //if node has a left push it
                q.push(last->left);
            }
            if (last->right) {  //if node has right push. this will search the right side first since its pushed second.
                q.push(last->right);    //important because Heaps are always complete.
            }
        }
        return last;//return last when its at the last node without chilren
    }
    void swapRoot(Node* node) { //creates temp movie object to swap root with a node.
        Movie tempMovie = root->movie;
        root->setMovie(node->movie);
        node->setMovie(tempMovie);
    }

    void heapifyDown(Node* node) {  //for after popping the root.
        while (node) {
            Node* largest = node;

            if (node->left && node->left->movie.getRATING() > largest->movie.getRATING()) {
                largest = node->left;
            }
            if (node->right && node->right->movie.getRATING() > largest->movie.getRATING()) {
                largest = node->right;
            }//find if left or right has the higher rating make that largest.

            if (largest == node) {
                break;  //breaks if neither of the nodes was larger as thats how a maxHeap should be
            }

            std::swap(node->movie, largest->movie); //swap movie with largest movie
            node = largest; //move node to the larger one and check if thats in position by going back in loop
        }
    }
    void popHead(Node* node) {
        if (root == getLastNode(root)) {
            return;//if only 1 node
        }
        swapRoot(node);//swap with node.
        Node* temp = getLastNode(node);
        if (getLastNode(node)==getLastNode(node)->parent->left) {
            getLastNode(node)->parent->left=nullptr;
        }
        if (getLastNode(node)==getLastNode(node)->parent->right) {
            getLastNode(node)->parent->right=nullptr;
        }//set parents children to nullptr
        delete temp;//deletes the node that was last
        heapifyDown(root);//heapify down from the root that was swapped
    }

    void printLevels(Node* node) {  //ensuring heap was working as normal, not used in code other than debug
        if (!node) return;

        std::queue<Node*> q;
        q.push(node);

        while (!q.empty()) {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++) {
                Node* node = q.front();
                q.pop();

                std::cout << node->movie.getRATING() << " ";

                if (node->left) {
                    q.push(node->left);
                }
                if (node->right) {
                    q.push(node->right);
                }
            }
            std::cout << std::endl;
        }
    }
    void printTopFive() {
        if (root==nullptr) {//if empty print that there were no movies.
            std::cout<<"No movies from that year"<<std::endl;
            return;
        }   //prints top 10 by popping the head and heapify down 10 times.
        for (int i=0; i<10; i++) {
            if (root == getLastNode(root) && i<9) {//if there were less than 10 movies, ensures it prints the correct amount
                i=9;
            }
            std::cout << root->movie.getTITLE() << " ("<<root->movie.getRATING()<<") "<<std::endl;
            popHead(getLastNode(root)); //print and pop root.
        }
    }


    public:
    MaxHeap() : root(NULL) {}       //default constructor, root is null

    void insertPub(Movie m) {     //insert into heap
        insert(m);
    }
    void printLevels() {//print levels
        printLevels(root);
    }
    void swapHeadLast() {//swap the head with last pos.
        swapRoot(getLastNode(root));
    }
    void getLastNode() {//gets the last node
        std::cout<<getLastNode(root)->movie.getRATING()<<std::endl;
    }
    void popHead() {//removes head
        popHead(getLastNode(root));
    }
    void printTop5() {  //print the top5
        printTopFive();
    }
};
#endif //MAXHEAP_H
