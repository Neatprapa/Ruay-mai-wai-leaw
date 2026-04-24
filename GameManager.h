#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <string>
#include "Menu.h"

// [Requirement 2: Linked List Node]
struct DaySummary {
    int dayNumber;
    int totalCarbonUsed;
    bool missionPassed;
    string missionName;
    DaySummary* next;
};

// [Requirement 1: Class with Constructor] (Class 3)
class GameManager {
private:
    int totalCarbon7Days;
    int maxCarbonCeiling;
    int missionsCompleted;
    DaySummary* headSummary;

public:
    GameManager(int ceiling) : totalCarbon7Days(0), maxCarbonCeiling(ceiling), missionsCompleted(0), headSummary(nullptr) {}

    ~GameManager() {
        DaySummary* current = headSummary;
        while (current != nullptr) {
            DaySummary* next = current->next;
            delete current;
            current = next;
        }
    }

    // [Requirement 4: Implement with Sorting (Linked List Insertion Sort)]
    void addDailyRecordSorted(int day, int carbon, bool missionPassed, string mName) {
        totalCarbon7Days += carbon;
        if (missionPassed && mName != "No Mission") {
            missionsCompleted++;
        }

        DaySummary* newNode = new DaySummary{day, carbon, missionPassed, mName, nullptr};

        if (headSummary == nullptr || headSummary->totalCarbonUsed > carbon) {
            newNode->next = headSummary;
            headSummary = newNode;
        } else {
            DaySummary* current = headSummary;
            while (current->next != nullptr && current->next->totalCarbonUsed <= carbon) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    bool checkMission(int missionID, Menu* selectedMenu, int& dayCarbon) {
        if (missionID == 0) return true; // วันที่ไม่มีภารกิจถือว่าผ่านฟรี
        int importCount = 0, factoryCount = 0, packageCount = 0;
        
        for (auto ing : selectedMenu->selectedIngredients) {
            if (ing->getIsImported()) importCount++;
            if (ing->getIsFrozen()) factoryCount++;
            if (ing->getIsPackaged()) packageCount++;
        }

        bool passed = false;
        switch (missionID) {
            case 1: passed = (importCount <= 1); 
                    if(passed) dayCarbon = dayCarbon * 0.85; // ลดคาร์บอน 15%
                    break;
            case 2: passed = (factoryCount <= 2); break;
            case 3: passed = selectedMenu->isLowFlame; break;
            case 4: passed = selectedMenu->isVegan && (selectedMenu->calculateTotalCarbon() < 60); break;
            case 5: passed = (packageCount == 0); break;
        }
        return passed;
    }

    /*bool showFinalSummary() {
        // [UI & Formatting: ASCII Art]
        std::cout << R"(
   ____                       ____  _     _               __  __                                   
  / ___|_ __ ___  ___ _ __   | __ )(_)___| |_ _ __ ___   |  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
 | |  _| '__/ _ \/ _ \ '_ \  |  _ \| / __| __| '__/ _ \  | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
 | |_| | | |  __/  __/ | | | | |_) | \__ \ |_| | | (_) | | |  | | (_| | | | | (_| | (_| |  __/ |   
  \____|_|  \___|\___|_| |_| |____/|_|___/\__|_|  \___/  |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                                                   |___/    
                                                                                                                                                                                                                                                             \$$$$$$                     
        )";

        std::cout << "\n============= 7 DAYS RANKING SUMMARY =============\n";
        std::cout << "Sorted by Best Carbon Day (Lowest to Highest)\n";
        std::cout << "--------------------------------------------------\n";
        
        DaySummary* temp = headSummary;
        while (temp != nullptr) {
            std::cout << "Day " << temp->dayNumber 
                      << " | Carbon: " << temp->totalCarbonUsed 
                      << " | Mission (" << temp->missionName << "): " 
                      << (temp->missionPassed ? "[PASS]" : "[FAIL]") << "\n";
            temp = temp->next;
        }
        std::cout << "--------------------------------------------------\n";
        std::cout << "Total Carbon: " << totalCarbon7Days << " / " << maxCarbonCeiling << "\n";
        std::cout << "Missions Completed: " << missionsCompleted << " / 3\n\n";

        // Logic จัด Rank ตาม Requirement
        if (totalCarbon7Days <= (maxCarbonCeiling * 0.7) && missionsCompleted == 3) {
            std::cout << R"(
  ______    _______                     __       
.' ____ \  |_   __ \                   [  |  _   
| (___ \_|   | |__) |   ,--.   _ .--.   | | / ]  
 _.____`.    |  __ /   `'_\ : [ `.-. |  | '' <   
| \____) |  _| |  \ \_ // | |, | | | |  | |`\ \  
 \______.' |____| |___|\'-;__/[___||__][__|  \_] 
                                                 
            )" << "\nPerfect Eco-Bistro! You saved the world!\n";
            return true;
        } else if (totalCarbon7Days <= maxCarbonCeiling && missionsCompleted == 3) {
            std::cout << R"(
      _        _______                     __       
     / \      |_   __ \                   [  |  _   
    / _ \       | |__) |   ,--.   _ .--.   | | / ]  
   / ___ \      |  __ /   `'_\ : [ `.-. |  | '' <   
 _/ /   \ \_   _| |  \ \_ // | |, | | | |  | |`\ \  
|____| |____| |____| |___|\'-;__/[___||__][__|  \_]    

            )" << "\nGreat Job! You met the goals.\n";
            return true;
        } else if (totalCarbon7Days <= maxCarbonCeiling * 1.2) {
            std::cout << R"(
 ______     _______                     __       
|_   _ \   |_   __ \                   [  |  _   
  | |_) |    | |__) |   ,--.   _ .--.   | | / ]  
  |  __'.    |  __ /   `'_\ : [ `.-. |  | '' <   
 _| |__) |  _| |  \ \_ // | |, | | | |  | |`\ \  
|_______/  |____| |___|\'-;__/[___||__][__|  \_]   

            )" << "\nNeeds some improvement, but you survived.\n";
            return true;
        } else {
            std::cout << R"(
 ________   _______                     __       
|_   __  | |_   __ \                   [  |  _   
  | |_ \_|   | |__) |   ,--.   _ .--.   | | / ]  
  |  _|      |  __ /   `'_\ : [ `.-. |  | '' <   
 _| |_      _| |  \ \_ // | |, | | | |  | |`\ \  
|_____|    |____| |___|\'-;__/[___||__][__|  \_] 
                                                 
            )" << "\nGAME OVER! The authorities closed your bistro due to extreme pollution. You MUST restart.\n";
            return false; // บังคับเริ่มใหม่
        }
    }*/


    bool showFinalSummary() {
        cout << "\n\n";
        cout << "╔════════════════════════════════════════════════════════════════════════════════════════════╗" << endl;
        cout << "║                               📅 7-DAY SUSTAINABILITY REPORT                               ║" << endl;
        cout << "╠═══════════╦═══════════════╦════════════════════════════════════════════════════╦═══════════╣" << endl;
        cout << "║    DAY    ║     CARBON    ║                      MISSION                       ║   STATUS  ║" << endl;
        cout << "╠═══════════╬═══════════════╬════════════════════════════════════════════════════╬═══════════╣" << endl;   

        DaySummary* temp = headSummary;
        while (temp != nullptr) {
            string icon = temp->missionPassed ? " ✅" : " ❌";
            string statusText = temp->missionPassed ? "PASS" : "FAIL";
            string color = temp->missionPassed ? "\033[32m" : "\033[31m";

            printf("║   Day %-3d ║  %4d units   ║ ", 
                temp->dayNumber, temp->totalCarbonUsed);

            string mName = temp->missionName;
            if(mName == "") mName = "No Mission";
            printf("%-50s ║ ", mName.substr(0, 50).c_str());

            printf("%s%s %-5s\033[0m ║\n", color.c_str(), icon.c_str(), statusText.c_str());

            temp = temp->next;
        }
        cout << "╚═══════════╩═══════════════╩════════════════════════════════════════════════════╩═══════════╝" << endl;

        std::cout << "----------------------------------------------------------------------------------------------\n";
        std::cout << "Total Carbon: " << totalCarbon7Days << " / " << maxCarbonCeiling << "\n";
        std::cout << "Missions Completed: " << missionsCompleted << " / 3\n\n";
    

        if (totalCarbon7Days <= (maxCarbonCeiling * 0.7) && missionsCompleted == 3) {
            std::cout << R"(
  ______    _______                     __       
.' ____ \  |_   __ \                   [  |  _   
| (___ \_|   | |__) |   ,--.   _ .--.   | | / ]  
 _.____`.    |  __ /   `'_\ : [ `.-. |  | '' <   
| \____) |  _| |  \ \_ // | |, | | | |  | |`\ \  
 \______.' |____| |___|\'-;__/[___||__][__|  \_] 
                                                 
            )" << "\nPerfect Eco-Bistro! You saved the world!\n";
            return true;
        } else if (totalCarbon7Days <= maxCarbonCeiling && missionsCompleted == 3) {
            std::cout << R"(
      _        _______                     __       
     / \      |_   __ \                   [  |  _   
    / _ \       | |__) |   ,--.   _ .--.   | | / ]  
   / ___ \      |  __ /   `'_\ : [ `.-. |  | '' <   
 _/ /   \ \_   _| |  \ \_ // | |, | | | |  | |`\ \  
|____| |____| |____| |___|\'-;__/[___||__][__|  \_]    

            )" << "\nGreat Job! You met the goals.\n";
            return true;
        } else if (totalCarbon7Days <= maxCarbonCeiling * 1.2) {
            std::cout << R"(
 ______     _______                     __       
|_   _ \   |_   __ \                   [  |  _   
  | |_) |    | |__) |   ,--.   _ .--.   | | / ]  
  |  __'.    |  __ /   `'_\ : [ `.-. |  | '' <   
 _| |__) |  _| |  \ \_ // | |, | | | |  | |`\ \  
|_______/  |____| |___|\'-;__/[___||__][__|  \_]   

            )" << "\nNeeds some improvement, but you survived.\n";
            return true;
        } else {
            std::cout << R"(
 ________   _______                     __       
|_   __  | |_   __ \                   [  |  _   
  | |_ \_|   | |__) |   ,--.   _ .--.   | | / ]  
  |  _|      |  __ /   `'_\ : [ `.-. |  | '' <   
 _| |_      _| |  \ \_ // | |, | | | |  | |`\ \  
|_____|    |____| |___|\'-;__/[___||__][__|  \_] 
                                                 
            )" << "\nGAME OVER! The authorities closed your bistro due to extreme pollution. You MUST restart.\n";
            return false; // บังคับเริ่มใหม่
        }
}

};

#endif