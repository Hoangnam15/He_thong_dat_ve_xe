/*
    main.cpp
    Chương trình chính: Menu quản lý hệ thống đặt vé xem phim.
    (Cac test case da duoc tach rieng sang file test.cpp)
*/

#include "functions.h"
#include <iostream>
#include <limits>
using namespace std;

void hienThiMenu() {
    cout << "\n=============================================\n";
    cout << "   HE THONG DAT VE XEM PHIM\n";
    cout << "=============================================\n";
    cout << "1. Hien thi so do ghe (trong / da dat)\n";
    cout << "2. Dat ve\n";
    cout << "3. Huy ve\n";
    cout << "4. Tim kiem khach hang (theo ma hoac ten)\n";
    cout << "5. Xuat danh sach da dat ve\n";
    cout << "6. Thong ke (ghe trong, doanh thu)\n";
    cout << "7. Xem danh sach cho (Priority Queue)\n";
    cout << "8. Xem lich su giao dich (Stack)\n";
    cout << "0. Thoat\n";
    cout << "=============================================\n";
    cout << "Chon chuc nang: ";
}

void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// =================================================================
//                          HÀM MAIN
// =================================================================
int main() {
    CinemaSystem he;
    int luaChon;

    do {
        hienThiMenu();
        cin >> luaChon;

        if (cin.fail()) {
            xoaBoDem();
            cout << "Vui long nhap so hop le!\n";
            continue;
        }

        switch (luaChon) {
            case 1:
                he.hienThiSoDoGhe();
                break;
            case 2: {
                string maKH, tenKH;
                int chonVIP, chonCaoTuoi;
                cout << "Nhap ma khach hang: ";
                cin >> maKH;
                xoaBoDem();
                cout << "Nhap ten khach hang: ";
                getline(cin, tenKH);
                cout << "Khach hang co la VIP khong? (1: Co, 0: Khong): ";
                cin >> chonVIP;
                cout << "Khach hang co la nguoi cao tuoi khong? (1: Co, 0: Khong): ";
                cin >> chonCaoTuoi;
                he.datVe(maKH, tenKH, chonVIP == 1, chonCaoTuoi == 1);
                break;
            }
            case 3: {
                string maGhe;
                cout << "Nhap ma ghe can huy (vd: A1, B3): ";
                cin >> maGhe;
                he.huyVe(maGhe);
                break;
            }
            case 4: {
                int kieuTim;
                cout << "Tim theo: 1. Ma khach hang   2. Ten khach hang\nChon: ";
                cin >> kieuTim;
                xoaBoDem();
                if (kieuTim == 1) {
                    string maKH;
                    cout << "Nhap ma khach hang: ";
                    cin >> maKH;
                    he.timKiemTheoMa(maKH);
                } else {
                    string ten;
                    cout << "Nhap ten khach hang: ";
                    getline(cin, ten);
                    he.timKiemTheoTen(ten);
                }
                break;
            }
            case 5:
                he.xuatDanhSachDaDat();
                break;
            case 6:
                he.thongKe();
                break;
            case 7:
                he.xemDanhSachCho();
                break;
            case 8:
                he.xemLichSuGiaoDich();
                break;
            case 0:
                cout << "Cam on da su dung he thong. Tam biet!\n";
                break;
            default:
                cout << "Lua chon khong hop le. Vui long thu lai!\n";
        }
    } while (luaChon != 0);

    return 0;
}
