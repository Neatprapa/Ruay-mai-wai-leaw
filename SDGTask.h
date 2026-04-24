#ifndef SDGTASK_H
#define SDGTASK_H
#include <string>

class Mission {
public:
    std::string name;
    Mission(std::string n) : name(n) {}
    // เช็ค: คาร์บอนรวม, จำนวนของนำเข้า, จำนวนของสำเร็จรูป, เป็นเมนูเจ/เต้าหู้ไหม
    virtual bool isSuccess(int carbon, int imports, int factory, bool isVeg) = 0;
    virtual ~Mission() {}
};

class LocavoreHero : public Mission {
public:
    LocavoreHero() : Mission("Locavore Hero") {}
    bool isSuccess(int c, int i, int f, bool v) override { return i <= 1; }
};

class ZeroProcessing : public Mission {
public:
    ZeroProcessing() : Mission("Zero Processing") {}
    bool isSuccess(int c, int i, int f, bool v) override { return f <= 2; }
};

class PlantBasedDay : public Mission {
public:
    PlantBasedDay() : Mission("Plant-Based Day") {}
    bool isSuccess(int c, int i, int f, bool v) override { return v == true; }
};
// เพิ่มคลาสอื่นๆ ได้ตามต้องการในลักษณะเดียวกัน
#endif