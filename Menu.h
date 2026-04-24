#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <iostream>
#include "Ingredient.h"

// for colour
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define BOLD    "\033[1m"

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
        //cout << "\n=== Preparing: " << menuName << " ===\n";
        //เอาไว้คำนวณตัวอักษรเพื่อวาดกรอบ
        string headerText = " 🍳 PREPARING: " + menuName + " ";
        int len = headerText.length();
        //กรอบบน 
        std::cout << "\n╔";
        for (int i = 0; i < len; i++) std::cout << "═";
        std::cout << "╗" << std::endl;
        //ตรงกลาง
        std::cout << "║" << headerText << "  ║" << std::endl;
        //กรอบล่าง
        std::cout << "╚";
        for (int i = 0; i < len; i++) std::cout << "═";
        std::cout << "╝" << std::endl;

        for (auto& cat : categories) {
            //cout << "Select [" << cat.name << "]:\n";
            cout << "\n  [" << BOLD << YELLOW << " SELECT " << cat.name << RESET << " ] 🛒" << endl;
            cout << "  ─────────────────────────────────────────────────────────────────────────" << endl;
            for (size_t i = 0; i < cat.choices.size(); i++) {
                //cout << "  " << i + 1 << ". " << cat.choices[i]->getName() << "\n";
                Ingredient* ing = cat.choices[i];
                string typeInfo = ing->getType();
                printf("   %s(%zu)%s %-40s ", CYAN, i + 1, RESET, ing->getName().c_str());
                string formattedType = "[" + typeInfo + "]";
                printf("%s", formattedType.c_str());
                int typePadding = 15 - formattedType.length(); 
                for (int j = 0; j < typePadding; j++) cout << " ";
                printf("%s+%02d CO2%s\n", YELLOW, ing->getCarbon(), RESET);
            }
            cout<< "  ─────────────────────────────────────────────────────────────────────────"<< endl;
            int choice;
            do {
                /*cout << "Enter choice (1-" << cat.choices.size() << "): ";
                cin >> choice;*/
                cout << "  Enter choice (1-" << cat.choices.size() << "): " << RESET;
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