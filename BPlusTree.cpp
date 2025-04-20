#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Movie.h"
using namespace std;

const int DEGREE = 64; // Maximum number of keys per node before a split

// Definition of a B+ Tree node
struct node {
    bool isLeaf;                          // True if node is a leaf
    vector<int> keys;                     // Keys (movie years)
    vector<vector<Movie*>> values;       // Values for leaf nodes: list of Movie pointers per year
    vector<node*> children;              // Pointers to child nodes (internal nodes only)
    node* next;                          // Pointer to next leaf node for fast range traversal

    // Constructor for creating a new node
    node(bool leaf) {
        isLeaf = leaf;
        next = nullptr;
    }
};

// B+ Tree class for indexing movies by release year
class BPlusTree {
private:
    node* root; // Root of the B+ Tree

    // Helper function to insert an internal key and child into a parent node
    void insert(int key, node* current, node* child) {
        // Find position to insert key to maintain sorted order
        auto pos = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
        current->keys.insert(current->keys.begin() + pos, key);
        current->children.insert(current->children.begin() + pos + 1, child);

        // If node overflows, split it
        if (current->keys.size() >= DEGREE) {
            node* newNode = new node(false); // New internal node
            int mid = DEGREE / 2;            // Midpoint for split

            // Move half of keys and children to the new node
            newNode->keys.assign(current->keys.begin() + mid + 1, current->keys.end());
            newNode->children.assign(current->children.begin() + mid + 1, current->children.end());
            int upKey = current->keys[mid]; // Key to be pushed up

            // Resize current node after split
            current->keys.resize(mid);
            current->children.resize(mid + 1);

            // If current is root, create new root
            if (current == root) {
                root = new node(false);
                root->keys.push_back(upKey);
                root->children.push_back(current);
                root->children.push_back(newNode);
            } else {
                // Recursively insert the upKey to the parent
                insert(upKey, parent(root, current), newNode);
            }
        }
    }

    // Helper function to find the parent of a given child node
    node* parent(node* current, node* child) {
        if (!current || current->isLeaf) return nullptr;

        for (auto& c : current->children) {
            if (c == child) return current;
            node* res = parent(c, child);
            if (res) return res;
        }
        return nullptr;
    }

public:
    // Constructor: initialize tree with an empty leaf root
    BPlusTree() {
        root = new node(true);
    }

    // Insert a movie into the B+ Tree
    void insert(Movie* m) {
        node* curr = root;

        // Traverse to appropriate leaf node
        while (!curr->isLeaf) {
            int i = upper_bound(curr->keys.begin(), curr->keys.end(), m->YEAR) - curr->keys.begin();
            curr = curr->children[i];
        }

        // Check if the year already exists in leaf
        auto it = find(curr->keys.begin(), curr->keys.end(), m->YEAR);
        if (it != curr->keys.end()) {
            int idx = distance(curr->keys.begin(), it);
            curr->values[idx].push_back(m);
        } else {
            // Insert new year and initialize list with this movie
            int pos = upper_bound(curr->keys.begin(), curr->keys.end(), m->YEAR) - curr->keys.begin();
            curr->keys.insert(curr->keys.begin() + pos, m->YEAR);
            curr->values.insert(curr->values.begin() + pos, vector<Movie*>{m});
        }

        // Split leaf if it overflows
        if (curr->keys.size() >= DEGREE) {
            node* newLeaf = new node(true);
            int mid = DEGREE / 2;

            // Move half of the key-value pairs to new leaf
            newLeaf->keys.assign(curr->keys.begin() + mid, curr->keys.end());
            newLeaf->values.assign(curr->values.begin() + mid, curr->values.end());

            curr->keys.resize(mid);
            curr->values.resize(mid);

            // Link the new leaf into the list
            newLeaf->next = curr->next;
            curr->next = newLeaf;

            // Create a new root or propagate split upwards
            if (curr == root) {
                root = new node(false);
                root->keys.push_back(newLeaf->keys[0]);
                root->children.push_back(curr);
                root->children.push_back(newLeaf);
            } else {
                insert(newLeaf->keys[0], parent(root, curr), newLeaf);
            }
        }
    }

    // Query for the top 10 movies from a given year, sorted by rating
    vector<Movie*> query(int year) {
        node* curr = root;

        // Traverse down to the appropriate leaf
        while (!curr->isLeaf) {
            int i = upper_bound(curr->keys.begin(), curr->keys.end(), year) - curr->keys.begin();
            curr = curr->children[i];
        }

        // Search for the year in the leaf node
        for (int i = 0; i < curr->keys.size(); ++i) {
            if (curr->keys[i] == year) {
                auto result = curr->values[i];

                // Sort movies by descending rating
                sort(result.begin(), result.end(), [](Movie* a, Movie* b) {
                    return a->RATING > b->RATING;
                });

                // Return top 10 movies
                if (result.size() > 10) result.resize(10);
                return result;
            }
        }

        return {}; // No movies found for that year
    }

    // Destructor: deallocate all nodes
    ~BPlusTree() {
        destroyTree(root);
    }

    // Recursively delete all nodes in the tree
    void destroyTree(node* n) {
        if (!n) return;
        if (!n->isLeaf) {
            for (auto child : n->children) {
                destroyTree(child);
            }
        }
        delete n;
    }
};
