#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Menu.h"
#include "GameManager.h"

using namespace std;

vector<Menu*> setupDay1Menus();
vector<Menu*> setupDay2Menus();
vector<Menu*> setupDay3Menus();
vector<Menu*> setupDay4Menus();
vector<Menu*> setupDay5Menus();
vector<Menu*> setupDay6Menus();
vector<Menu*> setupDay7Menus();

int main() {
    srand(time(0));
    string missionNames[] = {
        "No Mission",
        "Locavore Legend (Max 1 Imported)",
        "Fresh Over Factory (Max 2 Frozen/Processed)",
        "Low-Flame Master (Use Low Energy)",
        "Plant-Based Power (Vegan & < 60 Carbon)",
        "Packaging Purge (0 Packaged Items)"
    };

    bool restartGame;
    do {
        restartGame = false;
        GameManager game(800); // สมมติเพดานที่ 800

        std::cout << R"(
 ____      ____      __                                     
|_  _|    |_  _|    [  |                                    
  \ \  /\  / /.---.  | |  .---.   .--.   _ .--..--.  .---.  
   \ \/  \/ // /__\\ | | / /'`\]/ .'`\ \[ `.-. .-. |/ /__\\ 
    \  /\  / | \__., | | | \__. | \__. | | | | | | || \__., 
     \/  \/   '.__.'[___]'.___.' '.__.' [___||__||__]'.__.' 
                                                            
        )" << "\nWelcome to Green Bistro Manager!\n\n";

        // สุ่ม 3 ภารกิจ ลงใน 3 วันจาก 7 วัน
        int missionDays[7] = {0};
        int assigned = 0;
        while(assigned < 3) {
            int rDay = rand() % 7;
            if (missionDays[rDay] == 0) {
                missionDays[rDay] = (rand() % 5) + 1;
                assigned++;
            }
        }

        // Loop 7 วัน
        for (int day = 1; day <= 7; day++) {
            cout << "\n==============================\n";
            cout << "          DAY " << day << "          \n";
            cout << "==============================\n";
            
            int todayMission = missionDays[day-1];
            cout << ">> Today's Mission: " << missionNames[todayMission] << " <<\n\n";

            // ดึงเมนูประจำวันตามเลข Day
            vector<Menu*> todaysMenus;
            switch(day) {
                case 1: todaysMenus = setupDay1Menus(); break;
                case 2: todaysMenus = setupDay2Menus(); break;
                case 3: todaysMenus = setupDay3Menus(); break;
                case 4: todaysMenus = setupDay4Menus(); break;
                case 5: todaysMenus = setupDay5Menus(); break;
                case 6: todaysMenus = setupDay6Menus(); break;
                case 7: todaysMenus = setupDay7Menus(); break;
            }

            cout << "Select a menu to cook today:\n";
            for (size_t i = 0; i < todaysMenus.size(); i++) {
                cout << i + 1 << ". " << todaysMenus[i]->menuName << "\n";
            }
            
            int menuChoice;
            do {
                cout << "Enter choice (1-4): ";
                cin >> menuChoice;
            } while(menuChoice < 1 || menuChoice > 4);

            Menu* selectedMenu = todaysMenus[menuChoice - 1];
            selectedMenu->letPlayerChoose(); // ให้ผู้เล่นเลือกวัตถุดิบย่อย

            int dayCarbon = selectedMenu->calculateTotalCarbon();
            bool missionPassed = game.checkMission(todayMission, selectedMenu, dayCarbon);

            if(todayMission != 0) {
                cout << "\nMission Result: " << (missionPassed ? "PASSED!" : "FAILED!") << "\n";
            }
            cout << "Total Carbon Generated Today: " << dayCarbon << "\n";

            // เพิ่มลง Linked List
            game.addDailyRecordSorted(day, dayCarbon, missionPassed, missionNames[todayMission]);

            // Clean up memory
            for(auto m : todaysMenus) delete m;
        }

        // จบ 7 วัน ประเมิน Rank
        bool canEndGame = game.showFinalSummary();

        if (!canEndGame) {
            cout << "\nPress Enter to restart...";
            cin.ignore(); cin.get();
            restartGame = true; // บังคับ F Rank เริ่มใหม่
        }

    } while (restartGame);

    cout << "Thank you for playing Green Bistro Manager!\n";
    return 0;
}

// ------------------- DAY 1 -------------------
vector<Menu*> setupDay1Menus() {
    vector<Menu*> day1;

    // 1. สลัดเต้าหู้ (Vegan, LowFlame)
    Menu* m1 = new Menu("Tofu Salad", true, true);
    m1->addCategory("Protein", {new LocalIngredient("Fresh White Tofu", 5), new ProcessedIngredient("Packaged Tofu", 10)});
    m1->addCategory("Veggies", {new ProcessedIngredient("Hydroponic Veggies", 15, false, false), new ImportedIngredient("Imported Veggies", 20), new LocalIngredient("Market Veggies", 5)});
    m1->addCategory("Dressing", {new ImportedIngredient("Japanese Sesame", 15, false, true), new ProcessedIngredient("Clear Sauce", 10), new LocalIngredient("Homemade", 5)});
    day1.push_back(m1);

    // 2. สเต็กหมู (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Pork Steak", false, false);
    m2->addCategory("Meat", {new LocalIngredient("Local Farm Pork", 20), new ImportedIngredient("Imported Kurobuta", 30), new ProcessedIngredient("Supermarket Pork", 25)});
    m2->addCategory("Sauce", {new ProcessedIngredient("Ready-made Black Pepper", 15), new LocalIngredient("Homemade Sauce", 5), new LocalIngredient("Salt & Pepper", 2)});
    m2->addCategory("Veggies", {new LocalIngredient("Fresh Mashed Potato", 10), new ProcessedIngredient("Instant Mashed Potato", 20), new ImportedIngredient("Imported Asparagus", 25)});
    m2->addCategory("Fat/Oil", {new ProcessedIngredient("Butter", 15), new ProcessedIngredient("Vegetable Oil", 10), new ImportedIngredient("Imported Olive Oil", 20)});
    day1.push_back(m2);

    // 3. ข้าวหมกไก่
    Menu* m3 = new Menu("Chicken Biryani", false, false);
    m3->addCategory("Rice", {new LocalIngredient("Local Sao Hai Rice", 10), new LocalIngredient("Jasmine Rice", 12), new ImportedIngredient("Imported Basmati", 25)});
    m3->addCategory("Meat", {new LocalIngredient("Local Chicken", 15), new LocalIngredient("Closed-farm Chicken", 20), new ImportedIngredient("Frozen Imported Chicken", 25, true, true)});
    m3->addCategory("Spices", {new LocalIngredient("Fresh Market Spices", 5), new ProcessedIngredient("Instant Biryani Powder", 15), new ImportedIngredient("Premium Middle-East Spices", 25)});
    day1.push_back(m3);

    // 4. ข้าวกะเพราหมูสับไข่ดาว
    Menu* m4 = new Menu("Basil Minced Pork with Fried Egg", false, false);
    m4->addCategory("Rice", {new ProcessedIngredient("White Rice", 10, false, false), new LocalIngredient("Brown Rice", 8), new LocalIngredient("Riceberry", 8)});
    m4->addCategory("Meat", {new LocalIngredient("Closed-farm Minced Pork", 20), new LocalIngredient("Fattened Beef", 30), new ImportedIngredient("Premium Imported Minced Beef", 40)});
    m4->addCategory("Egg", {new ProcessedIngredient("No.0 Egg", 10), new LocalIngredient("Free-range Duck Egg", 5), new LocalIngredient("Organic Egg", 5)});
    day1.push_back(m4);

    return day1;
}

// ------------------- DAY 2 -------------------
vector<Menu*> setupDay2Menus() {
    vector<Menu*> day2;

    // 1. ข้าวหน้าหมูสามชั้นตุ๋น (Not Vegan, Not LowFlame)
    Menu* m1 = new Menu("Braised Pork Belly Rice", false, false);
    m1->addCategory("Rice", {new ProcessedIngredient("White Rice", 10), new LocalIngredient("Japanese Rice (Local)", 12), new ImportedIngredient("Imported Japanese Rice", 25)});
    m1->addCategory("Pork", {new LocalIngredient("Organic Pit Pork", 15), new ProcessedIngredient("Supermarket Pork", 25), new ImportedIngredient("Frozen Imported Pork", 30, true, true)});
    m1->addCategory("Stew Spices", {new LocalIngredient("Fresh Herbs", 5), new ProcessedIngredient("Instant Palo Powder", 15), new ImportedIngredient("Imported Chinese Spices", 25)});
    m1->addCategory("Energy", {new LocalIngredient("Charcoal Stove", 5), new ProcessedIngredient("Gas Stove", 15), new ProcessedIngredient("Electric Slow Cooker", 20)}); // ประยุกต์ใช้คลาสวัตถุดิบเก็บค่าพลังงาน
    day2.push_back(m1);

    // 2. ซุปหางวัว (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Oxtail Soup", false, false);
    m2->addCategory("Meat", {new LocalIngredient("Free-range Oxtail", 20), new ImportedIngredient("Imported Oxtail", 40)});
    m2->addCategory("Soup Base", {new LocalIngredient("Homemade Herbs", 5), new ProcessedIngredient("Halal Beef Cube", 15)});
    m2->addCategory("Veggies", {new LocalIngredient("Local Potato & Tomato", 5), new ImportedIngredient("Imported Potato", 20)});
    m2->addCategory("Energy", {new LocalIngredient("Overnight Charcoal", 10), new ProcessedIngredient("Gas Stove", 15), new ProcessedIngredient("Electric Pressure Cooker", 25)});
    day2.push_back(m2);

    // 3. แซนวิชแฮมชีส (Not Vegan, Not LowFlame)
    Menu* m3 = new Menu("Ham & Cheese Sandwich", false, false);
    m3->addCategory("Meat", {new ImportedIngredient("Imported Ham", 30), new LocalIngredient("Local Ham", 15), new ProcessedIngredient("Processed Chicken Breast", 20)});
    m3->addCategory("Bread", {new LocalIngredient("Local Bakery Whole Wheat", 10), new ProcessedIngredient("Factory Bread", 15), new ImportedIngredient("Imported Bread", 25)});
    m3->addCategory("Cheese", {new ImportedIngredient("Imported Cheddar", 30), new ProcessedIngredient("Processed Cheese Slice", 20), new LocalIngredient("Homemade Cheese", 10)});
    m3->addCategory("Veggies", {new LocalIngredient("Iceberg Lettuce", 5), new LocalIngredient("Tomato", 5), new LocalIngredient("Cucumber", 5)});
    m3->addCategory("Sauce", {new ProcessedIngredient("Mayonnaise", 15), new LocalIngredient("Homemade Sauce", 5)});
    day2.push_back(m3);

    // 4. ซุปฟักทอง (Vegan, LowFlame) -> สมมติให้เป็น Vegan
    Menu* m4 = new Menu("Pumpkin Soup", true, true);
    m4->addCategory("Pumpkin", {new LocalIngredient("Local Pumpkin", 5), new LocalIngredient("Local Japanese Pumpkin", 8), new ProcessedIngredient("Supermarket Pumpkin", 15)});
    m4->addCategory("Liquid", {new ProcessedIngredient("Fresh Milk (Cow)", 15), new LocalIngredient("Soy Milk", 8)}); // ถ้าใช้นมวัวอาจจะไม่ Vegan 100% แต่ใส่ไว้เป็นกิมมิคได้
    m4->addCategory("Fat", {new ProcessedIngredient("Palm Oil", 20), new ProcessedIngredient("Soybean Oil", 15), new ProcessedIngredient("Rice Bran Oil", 10)});
    day2.push_back(m4);

    return day2;
}

// ------------------- DAY 3 -------------------
vector<Menu*> setupDay3Menus() {
    vector<Menu*> day3;

    // 1. ผัดไทเต้าหู้เจ (Vegan, Not LowFlame)
    Menu* m1 = new Menu("Vegan Pad Thai", true, false);
    m1->addCategory("Noodles", {new ProcessedIngredient("Instant Pad Thai Noodles", 15), new LocalIngredient("Chanthaburi Noodles", 10), new LocalIngredient("Small Rice Noodles", 10)});
    m1->addCategory("Protein", {new ProcessedIngredient("Fried Tofu", 15), new LocalIngredient("Firm Tofu", 10), new LocalIngredient("White Tofu", 10)});
    m1->addCategory("Veggies", {new ImportedIngredient("Imported Veggies", 25), new ProcessedIngredient("Hydroponic Veggies", 15), new LocalIngredient("Market Veggies", 5)});
    day3.push_back(m1);

    // 2. เบอร์เกอร์หมูชีส (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Pork Cheese Burger", false, false);
    m2->addCategory("Meat", {new LocalIngredient("Local Minced Pork", 15), new ProcessedIngredient("Frozen Minced Pork", 20), new ImportedIngredient("Imported Minced Beef", 35)});
    m2->addCategory("Bread", {new LocalIngredient("Homemade Fresh Bread", 10), new ProcessedIngredient("Supermarket Bread", 15), new ImportedIngredient("Imported Bread", 25)});
    m2->addCategory("Cheese", {new ImportedIngredient("Imported Cheese", 30), new ProcessedIngredient("Normal Cheese Slices", 15)});
    m2->addCategory("Veggies", {new LocalIngredient("Lettuce", 5), new LocalIngredient("Tomato", 5), new LocalIngredient("Onion", 5)});
    m2->addCategory("Sauce", {new ProcessedIngredient("Ketchup", 10), new ProcessedIngredient("Mayonnaise", 15), new LocalIngredient("Special Homemade Sauce", 10)});
    day3.push_back(m2);

    // 3. มัสมั่นไก่ (Not Vegan, Not LowFlame)
    Menu* m3 = new Menu("Chicken Massaman Curry", false, false);
    m3->addCategory("Meat", {new LocalIngredient("Community Farm Chicken Thigh", 15), new LocalIngredient("Organic Chicken", 25), new ProcessedIngredient("Frozen Chicken (Plastic Pack)", 20)});
    m3->addCategory("Curry Paste", {new LocalIngredient("Homemade Garden Herbs Paste", 10), new ProcessedIngredient("Bottled/Bagged Instant Paste", 20)});
    m3->addCategory("Coconut Milk", {new LocalIngredient("Fresh Squeezed Coconut Milk", 15), new ProcessedIngredient("UHT Coconut Milk", 20), new ProcessedIngredient("Canned Coconut Milk", 25)});
    m3->addCategory("Others", {new LocalIngredient("Homemade Roasted Peanuts", 10), new LocalIngredient("Local Potato", 10), new ImportedIngredient("Imported Potato", 20)});
    day3.push_back(m3);

    // 4. ไก่ทอดซอสเกาหลี (Not Vegan, Not LowFlame)
    Menu* m4 = new Menu("Korean Fried Chicken", false, false);
    m4->addCategory("Meat", {new LocalIngredient("Local Chicken Breast", 15), new ProcessedIngredient("CP Frozen Drumsticks", 20), new LocalIngredient("Benja Chicken", 30)});
    m4->addCategory("Sauce", {new LocalIngredient("Homemade Pepper Sauce", 10), new ProcessedIngredient("Thai Gochujang", 20), new ImportedIngredient("Korean Gochujang", 35)});
    m4->addCategory("Flour", {new LocalIngredient("Local Rice Flour", 10), new ProcessedIngredient("Instant Crispy Flour", 15)});
    m4->addCategory("Oil", {new ProcessedIngredient("Vegetable Oil", 10), new ProcessedIngredient("Soybean Oil", 15), new LocalIngredient("Air Fryer (No Oil)", 0)});
    day3.push_back(m4);

    return day3;
}

// ------------------- DAY 4 -------------------
vector<Menu*> setupDay4Menus() {
    vector<Menu*> day4;

    // 1. ข้าวผัดอเมริกัน (Not Vegan, Not LowFlame)
    Menu* m1 = new Menu("American Fried Rice", false, false);
    m1->addCategory("Rice", {new LocalIngredient("Jasmine Rice", 10), new LocalIngredient("Fresh Ketchup Fried Rice", 15), new ProcessedIngredient("Instant Sauce Fried Rice", 20)});
    m1->addCategory("Sausage", {new ProcessedIngredient("Market Grade Sausage", 15), new ImportedIngredient("US Imported Sausage", 35)});
    m1->addCategory("Raisins", {new LocalIngredient("Local Store Raisins", 10), new ImportedIngredient("US Imported Raisins", 25)});
    m1->addCategory("Egg", {new LocalIngredient("Duck Egg", 5), new LocalIngredient("Chicken Egg", 5), new LocalIngredient("Quail Egg", 5), new ImportedIngredient("Japanese Egg (Raw Edible)", 20)});
    m1->addCategory("Chicken", {new LocalIngredient("Fried Chicken Drumstick", 20), new ImportedIngredient("Imported Battered Drumstick", 30)});
    m1->addCategory("Veggies", {new ProcessedIngredient("Canned Peaches", 20), new ProcessedIngredient("Frozen Peas", 15), new LocalIngredient("Nan Fresh Carrot", 10)});
    day4.push_back(m1);

    // 2. สะตอผัดกุ้ง (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Stir-Fried Sator with Shrimp", false, false);
    m2->addCategory("Shrimp", {new LocalIngredient("Local Fishery Banana Shrimp", 30), new LocalIngredient("Farm Shrimp", 25), new ImportedIngredient("Imported Frozen Peeled Shrimp", 40)});
    m2->addCategory("Sator", {new LocalIngredient("Backyard Sator", 10), new ProcessedIngredient("Plastic Packed Peeled Sator", 25)});
    m2->addCategory("Paste", {new LocalIngredient("Community Homemade Halal Kapi", 15), new ProcessedIngredient("Instant Curry Paste", 20)});
    m2->addCategory("Oil", {new ProcessedIngredient("Palm Oil", 10), new LocalIngredient("Cold-Pressed Coconut Oil", 25)});
    day4.push_back(m2);

    // 3. แพนเค้ก (Not Vegan, LowFlame)
    Menu* m3 = new Menu("Pancakes", false, true);
    m3->addCategory("Flour", {new ProcessedIngredient("General Wheat Flour", 15), new ProcessedIngredient("Instant Pancake Mix", 25), new LocalIngredient("Organic Flour", 30)});
    m3->addCategory("Milk", {new LocalIngredient("Local Fresh Milk", 15), new ProcessedIngredient("UHT Milk", 15), new ImportedIngredient("Imported Almond Milk", 35)});
    m3->addCategory("Egg", {new LocalIngredient("Farm Egg", 5), new LocalIngredient("Organic Egg", 10)});
    m3->addCategory("Topping", {new LocalIngredient("Honey", 20), new ImportedIngredient("Imported Maple Syrup", 40), new LocalIngredient("Local Fruits", 15)});
    m3->addCategory("Fat", {new ProcessedIngredient("Butter", 20), new ProcessedIngredient("Vegetable Oil", 10)});
    day4.push_back(m3);

    // 4. ต้มยำเห็ดเจน้ำข้น (Vegan, Not LowFlame)
    Menu* m4 = new Menu("Vegan Creamy Tom Yum Mushroom", true, false);
    m4->addCategory("Mushroom", {new LocalIngredient("Straw Mushroom", 10), new LocalIngredient("Fairy Mushroom", 10), new LocalIngredient("Shiitake Mushroom", 15)});
    m4->addCategory("Herbs", {new ImportedIngredient("Imported Herbs", 30), new LocalIngredient("Market Herbs", 10), new LocalIngredient("Homegrown Herbs", 5)});
    m4->addCategory("Extras", {new ProcessedIngredient("Evaporated Milk", 15), new ProcessedIngredient("Fried Dried Chili", 10), new ProcessedIngredient("Chili Paste", 15)});
    day4.push_back(m4);

    return day4;
}

// ------------------- DAY 5 -------------------
vector<Menu*> setupDay5Menus() {
    vector<Menu*> day5;

    // 1. ผัดผักรวมมิตรเจ (Vegan, Not LowFlame)
    Menu* m1 = new Menu("Stir-Fried Mixed Veggies (Vegan)", true, false);
    m1->addCategory("Veggies", {new ImportedIngredient("Imported Veggies", 25), new ProcessedIngredient("Hydroponic Veggies", 15), new LocalIngredient("Market Veggies", 5)});
    m1->addCategory("Protein", {new LocalIngredient("Tofu", 10), new ProcessedIngredient("Textured Vegetable Protein (TVP)", 15)});
    m1->addCategory("Oil", {new ProcessedIngredient("Palm Oil", 10), new ProcessedIngredient("Soybean Oil", 15), new ProcessedIngredient("Rice Bran Oil", 20)});
    day5.push_back(m1);

    // 2. ซาลาเปาหมูสับ (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Minced Pork Bun", false, false);
    m2->addCategory("Flour", {new LocalIngredient("Fresh Dough", 15), new ProcessedIngredient("Instant Flour Mix", 20)});
    m2->addCategory("Meat", {new LocalIngredient("Local Minced Pork", 20), new ProcessedIngredient("Frozen Pork", 20), new ImportedIngredient("Imported Pork", 35)});
    m2->addCategory("Seasoning", {new ProcessedIngredient("Seasoning Sauce", 10), new ProcessedIngredient("Soy Sauce", 10), new LocalIngredient("Homemade Seasoning", 15)});
    m2->addCategory("Veggies", {new LocalIngredient("Mushroom", 10), new LocalIngredient("Carrot", 5), new LocalIngredient("Spring Onion", 5)});
    day5.push_back(m2);

    // 3. โรตีมะตะบะไก่ (Not Vegan, Not LowFlame)
    Menu* m3 = new Menu("Chicken Mataba Roti", false, false);
    m3->addCategory("Dough", {new LocalIngredient("Fresh Hand-kneaded Wheat Dough", 15), new ProcessedIngredient("Frozen Instant Roti Dough", 20)});
    m3->addCategory("Filling", {new LocalIngredient("Open-farm Minced Chicken", 25), new ProcessedIngredient("Industrial Minced Chicken", 15)});
    m3->addCategory("Pickles", {new LocalIngredient("Market Organic Cucumber", 10), new LocalIngredient("Natural Fermented Vinegar", 15), new ProcessedIngredient("Industrial Distilled Vinegar", 10)});
    m3->addCategory("Fat", {new ImportedIngredient("Imported Ghee", 40), new ProcessedIngredient("Margarine", 10), new ProcessedIngredient("Rice Bran Oil", 15)});
    day5.push_back(m3);

    // 4. ราเมนซุปกระดูกหมู (Not Vegan, Not LowFlame)
    Menu* m4 = new Menu("Pork Bone Ramen", false, false);
    m4->addCategory("Noodles", {new LocalIngredient("Homemade Fresh Noodles", 20), new ProcessedIngredient("Dried Noodles", 15), new ImportedIngredient("Imported Japanese Noodles", 35)});
    m4->addCategory("Soup", {new LocalIngredient("12-Hr Slow Cooked Pork Bone Soup", 40), new ProcessedIngredient("Instant Soup Cube", 10), new ProcessedIngredient("Frozen Concentrated Soup", 25)});
    m4->addCategory("Topping", {new LocalIngredient("Local Chashu Pork", 25), new ImportedIngredient("Imported Pickled Bamboo Shoots", 20), new ImportedIngredient("Dried Seaweed", 15)});
    m4->addCategory("Extra Dairy", {new LocalIngredient("Thai Fresh Milk", 15), new ImportedIngredient("New Zealand Pure Butter", 30)});
    day5.push_back(m4);

    return day5;
}

// ------------------- DAY 6 -------------------
vector<Menu*> setupDay6Menus() {
    vector<Menu*> day6;

    // 1. พิซซ่าหน้าดับเบิ้ลชีส (Not Vegan, Not LowFlame)
    Menu* m1 = new Menu("Double Cheese Pizza", false, false);
    m1->addCategory("Dough", {new LocalIngredient("Hand-kneaded Dough", 15), new ProcessedIngredient("Frozen Instant Dough", 25)});
    m1->addCategory("Cheese 1", {new LocalIngredient("Thai Mozzarella", 25), new ImportedIngredient("Italian Mozzarella", 45)});
    m1->addCategory("Cheese 2", {new ProcessedIngredient("Cheddar Cheese", 20), new ImportedIngredient("Parmesan Cheese", 35), new ProcessedIngredient("Vegan Cheese (Nut-based)", 30)});
    m1->addCategory("Sauce", {new ProcessedIngredient("Canned Tomato Paste", 15), new LocalIngredient("Fresh Blended Tomato", 15)});
    m1->addCategory("Baking Method", {new LocalIngredient("Wood Fired Oven", 20), new ProcessedIngredient("Electric Oven", 15), new ProcessedIngredient("Microwave", 5)});
    day6.push_back(m1);

    // 2. เนื้อย่างจิ้มแจ่ว (Halal) (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Grilled Beef with Jaew Sauce (Halal)", false, false);
    m2->addCategory("Meat", {new LocalIngredient("Local Fattened Beef", 35), new ImportedIngredient("Imported A5 Wagyu Beef", 150)});
    m2->addCategory("Marinade", {new LocalIngredient("Natural Fermented Halal Soy Sauce", 20), new ProcessedIngredient("Industrial Seasoning Sauce", 10)});
    m2->addCategory("Sauce", {new LocalIngredient("Homemade Roasted Rice", 5), new LocalIngredient("Homemade Chili Powder", 5), new LocalIngredient("Fresh Lime", 5), new ProcessedIngredient("Bottled Artificial Lime Juice", 5)});
    m2->addCategory("Fuel", {new ProcessedIngredient("Coconut Shell Charcoal Briquettes", 15), new LocalIngredient("General Wood Charcoal", 10), new ProcessedIngredient("Electric Grill", 10)});
    day6.push_back(m2);

    // 3. บาร์บีคิว (Not Vegan, Not LowFlame)
    Menu* m3 = new Menu("BBQ", false, false);
    m3->addCategory("Meat", {new LocalIngredient("Local Pork", 20), new ProcessedIngredient("Frozen Pork", 20), new ImportedIngredient("Imported Pork", 35)});
    m3->addCategory("Sauce", {new ProcessedIngredient("Instant BBQ Sauce", 15), new LocalIngredient("Homemade BBQ Sauce", 20)});
    m3->addCategory("Veggies", {new LocalIngredient("Bell Pepper", 10), new LocalIngredient("Onion", 5), new LocalIngredient("Mushroom", 10)});
    m3->addCategory("Energy", {new LocalIngredient("Wood Charcoal", 10), new ProcessedIngredient("Gas Stove", 10), new ProcessedIngredient("Electric Grill", 10)});
    day6.push_back(m3);

    // 4. กล้วยบวชชี (Vegan, Not LowFlame)
    Menu* m4 = new Menu("Banana in Coconut Milk", true, false);
    m4->addCategory("Banana", {new ProcessedIngredient("Supermarket Namwa Banana", 15), new LocalIngredient("Market Namwa Banana", 10), new LocalIngredient("Homegrown Namwa Banana", 0)});
    m4->addCategory("Coconut Milk", {new ProcessedIngredient("UHT Coconut Milk", 15), new LocalIngredient("Fresh Coconut Milk", 15)});
    m4->addCategory("Sugar", {new ProcessedIngredient("White Sugar", 5), new ProcessedIngredient("Brown Sugar", 10), new LocalIngredient("Coconut Sugar", 15)});
    day6.push_back(m4);

    return day6;
}

// ------------------- DAY 7 -------------------
vector<Menu*> setupDay7Menus() {
    vector<Menu*> day7;

    // 1. แกงจืดเต้าหู้สาหร่าย (Not Vegan due to egg tofu, Not LowFlame)
    Menu* m1 = new Menu("Clear Soup with Tofu and Seaweed", false, false);
    m1->addCategory("Protein", {new ProcessedIngredient("Egg Tofu", 10), new LocalIngredient("White Tofu", 10)});
    m1->addCategory("Veggies", {new ImportedIngredient("Imported Veggies", 25), new ProcessedIngredient("Hydroponic Veggies", 15), new LocalIngredient("Market Veggies", 5)});
    m1->addCategory("Seaweed", {new ProcessedIngredient("Dried Seaweed", 10), new LocalIngredient("Fresh Seaweed", 15)});
    day7.push_back(m1);

    // 2. ซี่โครงหมูย่าง (Not Vegan, Not LowFlame)
    Menu* m2 = new Menu("Grilled Pork Ribs", false, false);
    m2->addCategory("Meat", {new LocalIngredient("Local Pork Ribs", 25), new ImportedIngredient("Imported Pork Ribs", 45)});
    m2->addCategory("Sauce", {new ProcessedIngredient("Instant BBQ Sauce", 15), new LocalIngredient("Homemade BBQ Sauce", 20)});
    m2->addCategory("Side Dish", {new LocalIngredient("Corn", 10), new LocalIngredient("Side Salad", 15), new LocalIngredient("Potato", 10)});
    m2->addCategory("Energy", {new LocalIngredient("Charcoal Grill", 10), new ProcessedIngredient("Electric Oven", 15)});
    day7.push_back(m2);

    // 3. สลัดแขก (Not Vegan, LowFlame)
    Menu* m3 = new Menu("Muslim Salad", false, true);
    m3->addCategory("Veggies", {new LocalIngredient("Local Organic Lettuce & Tomato", 15), new ProcessedIngredient("Hydroponic Salad Veggies", 20), new ImportedIngredient("Imported Veggies", 35)});
    m3->addCategory("Protein", {new LocalIngredient("Fried Firm Tofu", 10), new LocalIngredient("Free-range Boiled Egg", 10), new LocalIngredient("Shredded Boiled Chicken", 15)});
    m3->addCategory("Dressing", {new LocalIngredient("Homemade Peanut & Coconut Dressing", 20), new ProcessedIngredient("Bottled Muslim Salad Dressing", 25)});
    m3->addCategory("Fried Sweet Potato", {new LocalIngredient("Homegrown Fried Sweet Potato", 10), new ProcessedIngredient("Instant Crispy Potato Chips", 15)});
    day7.push_back(m3);

    // 4. เต้าหู้คั่วพริกเกลือ (Vegan, Not LowFlame)
    Menu* m4 = new Menu("Stir-Fried Tofu with Chili and Salt", true, false);
    m4->addCategory("Tofu", {new LocalIngredient("Market Board Tofu", 10), new ProcessedIngredient("Supermarket Organic Tofu", 20), new ImportedIngredient("Japanese Tofu in Mineral Water", 35)});
    m4->addCategory("Chili & Garlic", {new LocalIngredient("Garden Bird's Eye Chili", 5), new ImportedIngredient("Chinese Garlic", 10), new LocalIngredient("Thai Garlic", 10)});
    m4->addCategory("Seasoning", {new LocalIngredient("Sea Salt", 5), new LocalIngredient("Thai Soy Sauce", 10), new ProcessedIngredient("Umami Seasoning Powder", 10)});
    m4->addCategory("Oil", {new ProcessedIngredient("Rice Bran Oil", 15), new ProcessedIngredient("Vegetable Oil", 10), new LocalIngredient("Teflon Pan (No Oil)", 0)});
    day7.push_back(m4);

    return day7;
}