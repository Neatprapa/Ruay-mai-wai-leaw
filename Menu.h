#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <iostream>
#include "Ingredient.h"

using namespace std;

// โครงสร้างหมวดหมู่ใน 1 เมนู (เช่น หมวดข้าว, หมวดเนื้อ)
struct Category {
    string name;
    vector<Ingredient*> choices;
};

// [Requirement 1: Class with Constructor] (Class 2)
class Menu {
public:
    string menuName;
    int baseUmamiCarbon; 
    bool isVegan;        
    bool isLowFlame;     
    vector<Category> categories;
    vector<Ingredient*> selectedIngredients;

    Menu(string name, bool vegan, bool lowFlame) 
        : menuName(name), baseUmamiCarbon(20), isVegan(vegan), isLowFlame(lowFlame) {}

    ~Menu() {
        for(auto& cat : categories) {
            for(auto ing : cat.choices) delete ing;
        }
    }

    void addCategory(string catName, vector<Ingredient*> choices) {
        categories.push_back({catName, choices});
    }

    // ฟังก์ชันให้ผู้เล่นเลือกวัตถุดิบ
    void letPlayerChoose() {
        selectedIngredients.clear();
        cout << "\n=== Preparing: " << menuName << " ===\n";
        for (auto& cat : categories) {
            cout << "Select [" << cat.name << "]:\n";
            for (size_t i = 0; i < cat.choices.size(); i++) {
                cout << "  " << i + 1 << ". " << cat.choices[i]->getName() << "\n";
            }
            int choice;
            do {
                cout << "Enter choice (1-" << cat.choices.size() << "): ";
                cin >> choice;
            } while(choice < 1 || choice > cat.choices.size());
            selectedIngredients.push_back(cat.choices[choice - 1]);
        }
    }

    int calculateTotalCarbon() {
        int total = baseUmamiCarbon; // บวก 20 เสมอ
        for (auto ing : selectedIngredients) {
            total += ing->getCarbon(); // [Polymorphism In Action]
        }
        return total;
    }
};

#endif