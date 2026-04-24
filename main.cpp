#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "SDGTask.h"
#include "BistroSystem.h"

using namespace std;

// ฟังก์ชันแสดงรูปภาพ (แยกไว้ข้างนอก main ได้)
void displayMenuArt(int menuChoice) {
    if (menuChoice == 1) {
        cout << "\n[ ผัดผักรวมมิตรเจ ]\n" << "          ⣀⣀⣀⣀⣀\n" << "    ⣰⣶⢶⣏⡿⠿⠿⠹⠏⠷⠏⠿⠿⢏⣷⣶⣀\n";
    } else if (menuChoice == 2) {
        cout << "\n[ ซาลาเปาหมูสับ ]\n" << "    ⣴⣾⡿⢟⣙⣹⣿⠟⢋⠝⣽⢻⡿⢻⣯⣦\n";
    } else if (menuChoice == 3) {
        cout << "\n[ โรตีมะตะบะไก่ ]\n" << "      ⣀⡀⢰⢶⠆⠶⣀⠉⠉⠉⠷⠶⢶\n";
    } else {
        cout << "\n[ ราเมนซุปกระดูกหมู ]\n" << "             ⣀⣠⣤⣶⣾⣯⣿⡇\n";
    }
}

int main() {
    BistroManager bistro;
    vector<Mission*> missions = { new LocavoreHero(), new ZeroProcessing(), new PlantBasedDay() };
    int totalCarbon = 0;
    int tasksWon = 0;

    string dailyMenus[7][4] = {
        {"สลัดเต้าหู้", "สเต็กหมู", "ข้าวหมกไก่", "ข้าวกะเพราหมูสับ"},
        {"ข้าวหน้าหมูสามชั้น", "ซุปหางวัว", "แซนวิชแฮมชีส", "ซุปฟักทอง"},
        {"ผัดไทยเต้าหู้", "เบอร์เกอร์หมู", "มัสมั่นไก่", "ไก่ทอดเกาหลี"},
        {"ข้าวผัดอเมริกัน", "สะตอผัดกุ้ง", "แพนเค้ก", "ต้มยำเห็ดเจ"},
        {"ผัดผักรวมมิตรเจ", "ซาลาเปาหมูสับ", "โรตีมะตะบะไก่", "ราเมนซุปกระดูกหมู"},
        {"พิซซ่า", "เนื้อย่างจิ้มแจ่ว", "บาร์บีคิว", "กล้วยบวชชี"},
        {"แกงจืดเต้าหู้", "ซี่โครงหมูย่าง", "สลัดแขก", "เต้าหู้คั่วพริกเกลือ"}
    };

    for (int day = 1; day <= 7; day++) {
        cout << "\n--- DAY " << day << " ---" << endl;
        for(int i=0; i<4; i++) cout << i+1 << ". " << dailyMenus[day-1][i] << endl;

        int menuChoice;
        // --- ส่วนที่ต้องอยู่ใน main เพื่อดัก Error ---
        while (true) {
            cout << "\nเลือกเมนู (1-4): ";
            if (!(cin >> menuChoice)) {
                cout << "STUPID!! You need to input integers only!" << endl; // ตามภาพที่คุณส่งมา
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (menuChoice >= 1 && menuChoice <= 4) break;
            else cout << "ERROR: เลือกได้แค่ 1-4 เท่านั้น!" << endl;
        }

        displayMenuArt(menuChoice);
        
        // คำนวณ Carbon เบื้องต้น
        int dailyCarbon = 20 + (menuChoice * 15); 
        bistro.addDay(day, dailyCarbon);
        totalCarbon += dailyCarbon;
        
        cout << "วันนี้ใช้ Carbon ไป: " << dailyCarbon << " pts" << endl;
    }

    // สรุป Rank
    cout << "\n=== FINAL SUMMARY ===" << endl;
    cout << "Rank: " << bistro.calculateRank(totalCarbon, tasksWon) << endl;
    bistro.showBestDays();

    return 0;
}