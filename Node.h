//
// Created by aidan on 4/17/2025.
//

#ifndef NODE_H
#define NODE_H
#include "Movie.h"

class Node {        //simple node class with student object as data, a left node and a right node
public:
    Movie movie;
    Node* left;
    Node* right;
    Node* parent;

    Node(Movie m) : movie(m), left(nullptr), right(nullptr), parent(nullptr) {} //constructor with movie
    Node(Movie m, Node* p) : movie(m), left(nullptr), right(nullptr), parent(p) {}  //constructor with movie and parent
    void setMovie(Movie m) {    //set movie;
        movie = m;
    }
};
#endif //NODE_H
