#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

const int DEGREE = 64;

struct Movie {
    int id;
    string name;
    int year;
    double rating;

    Movie(int i, const string& n, int y, double r) {
        id = i;
        name = n;
        year = y;
        rating = r;
    }
};

struct node {
    bool isLeaf;
    vector<int> keys;
    vector<vector<Movie*>> values; // only in leaves
    vector<node*> children;
    node* next;

    node(bool leaf) {
        isLeaf = leaf;
        next = nullptr;
    }
};

class BPlusTree {
private:
    node* root;

    void insert(int key, node* current, node* child) {
        auto pos = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
        current->keys.insert(current->keys.begin() + pos, key);
        current->children.insert(current->children.begin() + pos + 1, child);

        if (current->keys.size() >= DEGREE) {
            node* newNode = new node(false);
            int mid = DEGREE / 2;

            newNode->keys.assign(current->keys.begin() + mid + 1, current->keys.end());
            newNode->children.assign(current->children.begin() + mid + 1, current->children.end());
            int upKey = current->keys[mid];

            current->keys.resize(mid);
            current->children.resize(mid + 1);

            if (current == root) {
                root = new node(false);
                root->keys.push_back(upKey);
                root->children.push_back(current);
                root->children.push_back(newNode);
            } else {
                insert(upKey, parent(root, current), newNode);
            }
        }
    }

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
    BPlusTree() {
        root = new node(true);
    }

    void insert(Movie* m) {
        node* curr = root;
        while (!curr->isLeaf) {
            int i = upper_bound(curr->keys.begin(), curr->keys.end(), m->year) - curr->keys.begin();
            curr = curr->children[i];
        }

        auto it = find(curr->keys.begin(), curr->keys.end(), m->year);
        if (it != curr->keys.end()) {
            int idx = distance(curr->keys.begin(), it);
            curr->values[idx].push_back(m);
        } else {
            int pos = upper_bound(curr->keys.begin(), curr->keys.end(), m->year) - curr->keys.begin();
            curr->keys.insert(curr->keys.begin() + pos, m->year);
            curr->values.insert(curr->values.begin() + pos, vector<Movie*>{m});
        }

        if (curr->keys.size() >= DEGREE) {
            node* newLeaf = new node(true);
            int mid = DEGREE / 2;

            newLeaf->keys.assign(curr->keys.begin() + mid, curr->keys.end());
            newLeaf->values.assign(curr->values.begin() + mid, curr->values.end());

            curr->keys.resize(mid);
            curr->values.resize(mid);

            newLeaf->next = curr->next;
            curr->next = newLeaf;

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

    vector<Movie*> query(int year) {
        node* curr = root;
        while (!curr->isLeaf) {
            int i = upper_bound(curr->keys.begin(), curr->keys.end(), year) - curr->keys.begin();
            curr = curr->children[i];
        }

        for (int i = 0; i < curr->keys.size(); ++i) {
            if (curr->keys[i] == year) {
                auto result = curr->values[i];
                sort(result.begin(), result.end(), [](Movie* a, Movie* b) {
                    return a->rating > b->rating;
                });
                if (result.size() > 10) result.resize(10);
                return result;
            }
        }

        return {};
    }
    ~BPlusTree() {
        destroyTree(root);
    }

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
