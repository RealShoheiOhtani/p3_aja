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

        while (!q.empty()) {    //until empty, iterates through all nodes O(n)
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

            q.push(curr->left); //if neither were nullptr, push child nodes
            q.push(curr->right);
        }
    }
    void heapifyUp(Node* node) {    //while there is parent and parent is less than curr node, swap
        //O(logn) as it only iterates through one path from leaf to root
        while (node->parent && node->movie.getRATING() > node->parent->movie.getRATING()) {
            swapNodes(node, node->parent);  //swap the parent with the node
            node = node->parent;    //move to parent to check its parent
        }
    }
    Node* getLastNode() { //similar to insert, goes left and right to find last spot
        if (root == nullptr) {
            return root;
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
        return last;//return last when its at the last node without children
    }

    void swapNodes(Node* node1, Node *node2) { //creates temp movie object to swap root with a node.
        Movie tempMovie = node1->movie; //O(1) operation as it is simply accessing
        node1->setMovie(node2->movie);
        node2->setMovie(tempMovie);
    }

    void heapifyDown() {  //when root is swapped with last node and popped need to put new root in correct place
        //O(logn) as it goes down one path from root to leaf and tree is always complete
        Node *node=root;
        while (node) {
            Node* largest = node;

            if (node->left && node->left->movie.getRATING() > largest->movie.getRATING()) {
                largest = node->left;   //if there is a left node and its rating is higher than largest, move largest to left
            }
            if (node->right && node->right->movie.getRATING() > largest->movie.getRATING()) {
                largest = node->right;   //if there is a right node and its rating is higher than largest, move largest to right
            }

            if (largest == node) {
                break;  //breaks if neither of the nodes was larger as thats how a maxHeap should be
            }

            swapNodes(node, largest);   //swap movie with largest node (parent)
            node = largest; //move node to the larger one and loop again
        }
    }

    void popHead() {//swaps root with last node, runs heapify down
        //O(logn) + O(logn) = O(logn)
        Node *lastNode = getLastNode();//o(logn)

        if (root == lastNode) {
            delete root;
            return;//if only 1 node
        }

        swapNodes(root, lastNode);  //swap root with last node

        if (lastNode==lastNode->parent->left) {
            lastNode->parent->left=nullptr;
        }
        if (lastNode==lastNode->parent->right) {
            lastNode->parent->right=nullptr;
        }//set parent's children to nullptr as lastNode is about to get deleted

        delete lastNode;//deletes the node that was last
        heapifyDown();//heapify down from the root that was swapped || O(logn)
    }

    void printTop10() {   //prints the top 10
        if (root==nullptr) {//if empty print that there were no movies.
            std::cout<<"No movies from that year"<<std::endl;
            return;
        }   //prints top 10 by popping the head and heapify down 10 times.
        for (int i=0; i<10; i++) {
            if (root == getLastNode() && i<9) {//if there were less than 10 movies, ensures it prints the correct amount
                i=9;
            }
            std::cout << root->movie.getTITLE() << " ("<<root->movie.getRATING()<<") "<<std::endl;
            popHead(); //print and pop root.
        }
    }

    public:
    MaxHeap() : root(NULL) {}   //default constructor, root is null

    void insertPub(Movie m) {   //insert into heap
        insert(m);
    }
    void printTop5() {  //print the top5
        printTop10();
    }
};
#endif //MAXHEAP_H
