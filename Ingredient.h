#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <string>

using namespace std;

// [Requirement 1: Class with Constructor] (Class 1)
class Ingredient {
protected:
    string name;
    int baseCarbon;
    bool isImported;
    bool isFrozenOrFactory;
    bool isPackaged;

public:
    Ingredient(string n, int c, bool imp, bool frozen, bool pack) 
        : name(n), baseCarbon(c), isImported(imp), isFrozenOrFactory(frozen), isPackaged(pack) {}
    
    virtual ~Ingredient() {}

    string getName() const { return name; }
    bool getIsImported() const { return isImported; }
    bool getIsFrozen() const { return isFrozenOrFactory; }
    bool getIsPackaged() const { return isPackaged; }

    // [Requirement 3: Inheritance + Polymorphism] (Pure Virtual Function)
    virtual int getCarbon() = 0; 
};

// [Requirement 3: Derived Class - วัตถุดิบท้องถิ่น]
class LocalIngredient : public Ingredient {
public:
    LocalIngredient(string n, int c, bool frozen = false, bool pack = false) 
        : Ingredient(n, c, false, frozen, pack) {}

    int getCarbon() override {
        return baseCarbon; // ท้องถิ่น ไม่บวกเพิ่ม
    }
};

// [Requirement 3: Derived Class - วัตถุดิบนำเข้า]
class ImportedIngredient : public Ingredient {
private:
    int importPenalty = 30; // คาร์บอนจากการขนส่ง
public:
    ImportedIngredient(string n, int c, bool frozen = false, bool pack = false) 
        : Ingredient(n, c, true, frozen, pack) {}

    int getCarbon() override {
        return baseCarbon + importPenalty; // นำเข้าบวกเพิ่ม 30
    }
};

// [Requirement 3: Derived Class - วัตถุดิบสำเร็จรูป/แปรรูป]
class ProcessedIngredient : public Ingredient {
private:
    int factoryPenalty = 20; // คาร์บอนจากโรงงาน
public:
    ProcessedIngredient(string n, int c, bool imp = false, bool pack = true) 
        : Ingredient(n, c, imp, true, pack) {}

    int getCarbon() override {
        return baseCarbon + factoryPenalty + (isImported ? 30 : 0);
    }
};

#endif