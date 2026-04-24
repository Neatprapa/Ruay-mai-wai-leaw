#ifndef BISTROSYSTEM_H
#define BISTROSYSTEM_H
#include <iostream>
#include <string>

struct Node {
    int day;
    int carbon;
    Node *left, *right;
    Node(int d, int c) : day(d), carbon(c), left(nullptr), right(nullptr) {}
};

class BistroManager {
private:
    Node* root;
    Node* insert(Node* node, int d, int c) {
        if (!node) return new Node(d, c);
        if (c < node->carbon) node->left = insert(node->left, d, c);
        else node->right = insert(node->right, d, c);
        return node;
    }
    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        std::cout << "Day " << node->day << ": " << node->carbon << " pts" << std::endl;
        inOrder(node->right);
    }
public:
    BistroManager() : root(nullptr) {}
    void addDay(int d, int c) { root = insert(root, d, c); }
    void showBestDays() { inOrder(root); }
    
    std::string calculateRank(int total, int tasks) {
        if (tasks == 3 && total <= 392) return "S";
        if (tasks == 3 && total <= 560) return "A";
        if (tasks >= 1 && total <= 750) return "B";
        return "F";
    }
};
#endif