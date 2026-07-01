/*
    functions.cpp
    Cài đặt chi tiết các chức năng của hệ thống đặt vé xem phim.
*/

#include "functions.h"
#include <iostream>
#include <iomanip>
using namespace std;

// =================================================================
//                      KHỞI TẠO & HỦY
// =================================================================
CinemaSystem::CinemaSystem() {
    goc = nullptr;
    dauHangCho = nullptr;
    dinhStack = nullptr;
    tongDoanhThu = 0;
    danhSachGhe = nullptr;

    // Khởi tạo sơ đồ ghế: 5 hàng (A-E) x 6 cột, hàng A là VIP
    Ghe* duoi = nullptr;
    for (int h = 0; h < SO_HANG; h++) {
        char tenHang = 'A' + h;
        for (int c = 1; c <= SO_COT; c++) {
            Ghe* g = new Ghe();
            g->hang = tenHang;
            g->cot = c;
            g->maGhe = string(1, tenHang) + to_string(c);
            g->daDat = false;
            g->maKhachHang = "";
            g->giaVe = (tenHang == 'A') ? GIA_VE_VIP : GIA_VE_THUONG;
            g->tiep = nullptr;

            if (danhSachGhe == nullptr) danhSachGhe = g;
            else duoi->tiep = g;
            duoi = g;
        }
    }

    // Tạo sẵn 5 khách hàng mẫu trong BST (chưa đặt ghế nào)
    khoiTaoKhachHangMau();
}

// =================================================================
//        5 KHÁCH HÀNG MẪU CÓ SẴN TRONG HỆ THỐNG (DỮ LIỆU MẪU)
// =================================================================
void CinemaSystem::khoiTaoKhachHangMau() {
    struct DuLieuMau {
        string maKH, tenKH;
        bool isVIP, isCaoTuoi;
    };

    DuLieuMau danhSachMau[5] = {
        {"KH001", "Nguyen Van An",     true,  false}, // khach VIP
        {"KH002", "Tran Thi Bich",     false, true},  // khach cao tuoi
        {"KH003", "Le Hoang Nam",      false, false}, // khach thuong
        {"KH004", "Pham Thi Hoa",      false, false}, // khach thuong
        {"KH005", "Do Van Minh",       true,  false}  // khach VIP
    };

    for (int i = 0; i < 5; i++) {
        KhachHang* khMoi = new KhachHang();
        khMoi->maKH = danhSachMau[i].maKH;
        khMoi->tenKH = danhSachMau[i].tenKH;
        khMoi->isVIP = danhSachMau[i].isVIP;
        khMoi->isCaoTuoi = danhSachMau[i].isCaoTuoi;
        khMoi->soVeDaMua = 0;
        khMoi->trai = khMoi->phai = nullptr;
        goc = themBST(goc, khMoi);
    }

    cout << ">> Da khoi tao 5 khach hang mau co san trong he thong (BST).\n";
}

void CinemaSystem::xoaCayBST(KhachHang* node) {
    if (node == nullptr) return;
    xoaCayBST(node->trai);
    xoaCayBST(node->phai);
    delete node;
}

CinemaSystem::~CinemaSystem() {
    // Giải phóng danh sách ghế
    Ghe* g = danhSachGhe;
    while (g != nullptr) {
        Ghe* tmp = g;
        g = g->tiep;
        delete tmp;
    }
    // Giải phóng BST khách hàng
    xoaCayBST(goc);
    // Giải phóng hàng chờ ưu tiên
    NguoiCho* nc = dauHangCho;
    while (nc != nullptr) {
        NguoiCho* tmp = nc;
        nc = nc->tiep;
        delete tmp;
    }
    // Giải phóng stack giao dịch
    GiaoDich* gd = dinhStack;
    while (gd != nullptr) {
        GiaoDich* tmp = gd;
        gd = gd->tiep;
        delete tmp;
    }
}

// =================================================================
//              LINKED LIST: SƠ ĐỒ GHẾ NGỒI (CHỨC NĂNG 1)
// =================================================================
Ghe* CinemaSystem::timGhe(const string& maGhe) {
    Ghe* p = danhSachGhe;
    while (p != nullptr) {
        if (p->maGhe == maGhe) return p;
        p = p->tiep;
    }
    return nullptr;
}

void CinemaSystem::hienThiSoDoGhe() const {
    cout << "\n================ SO DO GHE NGOI (ASCII) ================\n";
    cout << "        ";
    for (int c = 1; c <= SO_COT; c++) cout << " " << c << "  ";
    cout << "\n";

    Ghe* p = danhSachGhe;
    char hangHienTai = '\0';
    for (int h = 0; h < SO_HANG; h++) {
        char tenHang = 'A' + h;
        cout << "  Hang " << tenHang << " : ";
        Ghe* q = danhSachGhe;
        while (q != nullptr) {
            if (q->hang == tenHang) {
                cout << (q->daDat ? "[X] " : "[ ] ");
            }
            q = q->tiep;
        }
        cout << (tenHang == 'A' ? "  (VIP)" : "") << "\n";
    }
    cout << "==========================================================\n";
    cout << "Chu thich: [ ] Ghe trong   [X] Ghe da dat   Hang A = VIP\n";
}

// =================================================================
//                 BST: KHÁCH HÀNG (CHỨC NĂNG 4)
// =================================================================
KhachHang* CinemaSystem::themBST(KhachHang* node, KhachHang* moi) {
    if (node == nullptr) return moi;
    if (moi->maKH < node->maKH) node->trai = themBST(node->trai, moi);
    else if (moi->maKH > node->maKH) node->phai = themBST(node->phai, moi);
    // Nếu trùng mã KH thì bỏ qua (không thêm trùng)
    return node;
}

KhachHang* CinemaSystem::timBST(KhachHang* node, const string& maKH) {
    if (node == nullptr) return nullptr;
    if (maKH == node->maKH) return node;
    if (maKH < node->maKH) return timBST(node->trai, maKH);
    return timBST(node->phai, maKH);
}

void CinemaSystem::timTheoTenBST(KhachHang* node, const string& ten, bool& timThay) {
    if (node == nullptr) return;
    if (node->tenKH == ten) {
        timThay = true;
        cout << " -> Ma KH: " << node->maKH
             << " | Ten: " << node->tenKH
             << " | VIP: " << (node->isVIP ? "Co" : "Khong")
             << " | Cao tuoi: " << (node->isCaoTuoi ? "Co" : "Khong")
             << " | So ve da mua: " << node->soVeDaMua << "\n";
    }
    // Tên không phải khóa sắp xếp của BST nên phải duyệt toàn bộ cây
    timTheoTenBST(node->trai, ten, timThay);
    timTheoTenBST(node->phai, ten, timThay);
}

void CinemaSystem::timKiemTheoMa(const string& maKH) const {
    KhachHang* kq = const_cast<CinemaSystem*>(this)->timBST(goc, maKH);
    if (kq == nullptr) {
        cout << "Khong tim thay khach hang co ma: " << maKH << "\n";
    } else {
        cout << "Tim thay (BST): Ma KH: " << kq->maKH
             << " | Ten: " << kq->tenKH
             << " | VIP: " << (kq->isVIP ? "Co" : "Khong")
             << " | Cao tuoi: " << (kq->isCaoTuoi ? "Co" : "Khong")
             << " | So ve da mua: " << kq->soVeDaMua << "\n";
    }
}

void CinemaSystem::timKiemTheoTen(const string& ten) const {
    bool timThay = false;
    const_cast<CinemaSystem*>(this)->timTheoTenBST(goc, ten, timThay);
    if (!timThay) cout << "Khong tim thay khach hang co ten: " << ten << "\n";
}

void CinemaSystem::duyetGiuaBST(KhachHang* node) const {
    if (node == nullptr) return;
    duyetGiuaBST(node->trai);
    cout << " - " << node->maKH << " | " << node->tenKH
         << " | So ve: " << node->soVeDaMua << "\n";
    duyetGiuaBST(node->phai);
}

// =================================================================
//        PRIORITY QUEUE: DANH SÁCH CHỜ ƯU TIÊN (cài bằng linked list)
// =================================================================
// Chèn theo thứ tự ưu tiên giảm dần; cùng độ ưu tiên thì FIFO (vào trước ra trước)
static void chenVaoHangCho(NguoiCho*& dau, NguoiCho* moi) {
    if (dau == nullptr || moi->doUuTien > dau->doUuTien) {
        moi->tiep = dau;
        dau = moi;
        return;
    }
    NguoiCho* p = dau;
    while (p->tiep != nullptr && p->tiep->doUuTien >= moi->doUuTien) {
        p = p->tiep;
    }
    moi->tiep = p->tiep;
    p->tiep = moi;
}

void CinemaSystem::xemDanhSachCho() const {
    if (dauHangCho == nullptr) {
        cout << "Hang cho dang trong.\n";
        return;
    }
    cout << "\n--- DANH SACH CHO (Priority Queue) ---\n";
    NguoiCho* p = dauHangCho;
    int i = 1;
    while (p != nullptr) {
        string loaiUT = p->isVIP ? "VIP" : (p->isCaoTuoi ? "Cao tuoi" : "Thuong");
        cout << i++ << ". " << p->maKH << " - " << p->tenKH
             << " (Uu tien: " << loaiUT << ")\n";
        p = p->tiep;
    }
}

// =================================================================
//             STACK: LỊCH SỬ GIAO DỊCH (cài bằng linked list)
// =================================================================
static void pushGiaoDich(GiaoDich*& dinh, const string& loai, const string& maKH,
                          const string& maGhe, long soTien) {
    GiaoDich* gd = new GiaoDich();
    gd->loaiGD = loai;
    gd->maKH = maKH;
    gd->maGhe = maGhe;
    gd->soTien = soTien;
    gd->tiep = dinh;
    dinh = gd;
}

void CinemaSystem::xemLichSuGiaoDich() const {
    if (dinhStack == nullptr) {
        cout << "Chua co giao dich nao.\n";
        return;
    }
    cout << "\n--- LICH SU GIAO DICH (Stack - moi nhat tren cung) ---\n";
    GiaoDich* p = dinhStack;
    int i = 1;
    while (p != nullptr) {
        cout << i++ << ". [" << p->loaiGD << "] KH: " << p->maKH
             << " | Ghe: " << p->maGhe << " | So tien: " << p->soTien << " VND\n";
        p = p->tiep;
    }
}

// =================================================================
//                  CHỨC NĂNG 2: ĐẶT VÉ
// =================================================================
void CinemaSystem::datVe(const string& maKH, const string& tenKH, bool isVIP, bool isCaoTuoi) {
    // Tìm ghế trống đầu tiên (ưu tiên hàng VIP nếu khách là VIP, ngược lại ghế thường trước)
    Ghe* gheTrong = nullptr;
    Ghe* p = danhSachGhe;
    while (p != nullptr) {
        if (!p->daDat) {
            if (isVIP && p->hang == 'A') { gheTrong = p; break; }
            if (!gheTrong && !(p->hang == 'A' && !isVIP)) gheTrong = p;
        }
        p = p->tiep;
    }
    // Nếu chưa tìm được ghế phù hợp ở trên, lấy đại ghế trống bất kỳ còn lại
    if (gheTrong == nullptr) {
        p = danhSachGhe;
        while (p != nullptr) {
            if (!p->daDat) { gheTrong = p; break; }
            p = p->tiep;
        }
    }

    // Thêm/khách hàng vào BST (nếu mã KH chưa tồn tại)
    KhachHang* daTon = timBST(goc, maKH);
    if (daTon == nullptr) {
        KhachHang* khMoi = new KhachHang();
        khMoi->maKH = maKH;
        khMoi->tenKH = tenKH;
        khMoi->isVIP = isVIP;
        khMoi->isCaoTuoi = isCaoTuoi;
        khMoi->soVeDaMua = 0;
        khMoi->trai = khMoi->phai = nullptr;
        goc = themBST(goc, khMoi);
        daTon = khMoi;
    }

    if (gheTrong == nullptr) {
        // HẾT GHẾ -> đưa vào Priority Queue (danh sách chờ)
        NguoiCho* nc = new NguoiCho();
        nc->maKH = maKH;
        nc->tenKH = tenKH;
        nc->isVIP = isVIP;
        nc->isCaoTuoi = isCaoTuoi;
        nc->doUuTien = isVIP ? 2 : (isCaoTuoi ? 1 : 0);
        nc->tiep = nullptr;
        chenVaoHangCho(dauHangCho, nc);
        cout << ">> Rap da het ghe trong! Khach hang " << tenKH
             << " (" << maKH << ") da duoc them vao DANH SACH CHO uu tien.\n";
        return;
    }

    // Đặt ghế
    gheTrong->daDat = true;
    gheTrong->maKhachHang = maKH;
    daTon->soVeDaMua++;
    tongDoanhThu += gheTrong->giaVe;

    // Ghi vào stack lịch sử giao dịch
    pushGiaoDich(dinhStack, "DAT_VE", maKH, gheTrong->maGhe, gheTrong->giaVe);

    cout << ">> Dat ve thanh cong! Khach hang " << tenKH << " (" << maKH
         << ") - Ghe: " << gheTrong->maGhe
         << " - Gia: " << gheTrong->giaVe << " VND\n";
}

// =================================================================
//   Cấp ghế trống cho người đứng đầu hàng chờ (dùng khi có người huỷ vé)
// =================================================================
bool CinemaSystem::capGheChoNguoiCho(Ghe* ghe) {
    if (dauHangCho == nullptr) return false;

    NguoiCho* nguoiDuocCap = dauHangCho;
    dauHangCho = dauHangCho->tiep;

    // Đảm bảo khách hàng đã có trong BST
    KhachHang* daTon = timBST(goc, nguoiDuocCap->maKH);
    if (daTon == nullptr) {
        KhachHang* khMoi = new KhachHang();
        khMoi->maKH = nguoiDuocCap->maKH;
        khMoi->tenKH = nguoiDuocCap->tenKH;
        khMoi->isVIP = nguoiDuocCap->isVIP;
        khMoi->isCaoTuoi = nguoiDuocCap->isCaoTuoi;
        khMoi->soVeDaMua = 0;
        khMoi->trai = khMoi->phai = nullptr;
        goc = themBST(goc, khMoi);
        daTon = khMoi;
    }

    ghe->daDat = true;
    ghe->maKhachHang = nguoiDuocCap->maKH;
    daTon->soVeDaMua++;
    tongDoanhThu += ghe->giaVe;

    pushGiaoDich(dinhStack, "DAT_VE (tu hang cho)", nguoiDuocCap->maKH, ghe->maGhe, ghe->giaVe);

    cout << ">> Da tu dong cap ghe " << ghe->maGhe << " cho khach hang dang cho: "
         << nguoiDuocCap->tenKH << " (" << nguoiDuocCap->maKH << ")\n";

    delete nguoiDuocCap;
    return true;
}

// =================================================================
//                  CHỨC NĂNG 3: HỦY VÉ
// =================================================================
void CinemaSystem::huyVe(const string& maGhe) {
    Ghe* g = timGhe(maGhe);
    if (g == nullptr) {
        cout << "Khong tim thay ghe co ma: " << maGhe << "\n";
        return;
    }
    if (!g->daDat) {
        cout << "Ghe " << maGhe << " hien dang trong, khong the huy.\n";
        return;
    }

    string maKHCu = g->maKhachHang;
    long giaVe = g->giaVe;

    // Cập nhật số vé khách đã mua (nếu tìm thấy trong BST)
    KhachHang* kh = timBST(goc, maKHCu);
    if (kh != nullptr && kh->soVeDaMua > 0) kh->soVeDaMua--;

    // Hoàn lại doanh thu
    tongDoanhThu -= giaVe;

    // Ghi nhận giao dịch huỷ vào Stack
    pushGiaoDich(dinhStack, "HUY_VE", maKHCu, maGhe, giaVe);

    // Giải phóng ghế
    g->daDat = false;
    g->maKhachHang = "";

    cout << ">> Da huy ve ghe " << maGhe << " cua khach hang " << maKHCu << "\n";

    // Tự động cấp ghế vừa trống cho người đứng đầu hàng chờ ưu tiên (nếu có)
    capGheChoNguoiCho(g);
}

// =================================================================
//             CHỨC NĂNG 5: XUẤT DANH SÁCH ĐÃ ĐẶT VÉ
// =================================================================
void CinemaSystem::xuatDanhSachDaDat() const {
    cout << "\n========== DANH SACH GHE DA DAT ==========\n";
    Ghe* p = danhSachGhe;
    int dem = 0;
    while (p != nullptr) {
        if (p->daDat) {
            cout << " - Ghe " << p->maGhe << " | KH: " << p->maKhachHang
                 << " | Gia: " << p->giaVe << " VND\n";
            dem++;
        }
        p = p->tiep;
    }
    if (dem == 0) cout << "(Chua co ghe nao duoc dat)\n";
    cout << "Tong so ve da ban: " << dem << "\n";
    cout << "============================================\n";
}

// =================================================================
//           CHỨC NĂNG 6: THỐNG KÊ GHẾ TRỐNG & DOANH THU
// =================================================================
void CinemaSystem::thongKe() const {
    int trong = 0, daDat = 0;
    Ghe* p = danhSachGhe;
    while (p != nullptr) {
        if (p->daDat) daDat++; else trong++;
        p = p->tiep;
    }
    cout << "\n================ THONG KE ================\n";
    cout << "Tong so ghe          : " << TONG_SO_GHE << "\n";
    cout << "So ghe da dat        : " << daDat << "\n";
    cout << "So ghe con trong     : " << trong << "\n";
    cout << "So nguoi dang cho    : ";
    int demCho = 0;
    NguoiCho* nc = dauHangCho;
    while (nc != nullptr) { demCho++; nc = nc->tiep; }
    cout << demCho << "\n";
    cout << "Tong doanh thu       : " << tongDoanhThu << " VND\n";
    cout << "============================================\n";
}
