/*
    test.cpp
    -----------------------------------------------------------
                       FILE KIEM THU DOC LAP
    -----------------------------------------------------------
*/

#include <iostream>
#include <string>
using namespace std;

// ====================== HẰNG SỐ HỆ THỐNG ======================
const int SO_HANG = 5;          // Hàng ghế: A -> E
const int SO_COT = 6;           // Cột ghế: 1 -> 6
const int TONG_SO_GHE = SO_HANG * SO_COT;
const long GIA_VE_THUONG = 75000;
const long GIA_VE_VIP = 120000;

// ====================== 1. LINKED LIST: SƠ ĐỒ GHẾ ======================
struct Ghe {
    string maGhe;
    char hang;
    int cot;
    bool daDat;
    string maKhachHang;
    long giaVe;
    Ghe* tiep;
};

// ====================== 2. BST: KHÁCH HÀNG ======================
struct KhachHang {
    string maKH;
    string tenKH;
    bool isVIP;
    bool isCaoTuoi;
    int soVeDaMua;
    KhachHang* trai;
    KhachHang* phai;
};

// ====================== 3. PRIORITY QUEUE: DANH SÁCH CHỜ ======================
struct NguoiCho {
    string maKH;
    string tenKH;
    bool isVIP;
    bool isCaoTuoi;
    int doUuTien;
    NguoiCho* tiep;
};

// ====================== 4. STACK: LỊCH SỬ GIAO DỊCH ======================
struct GiaoDich {
    string loaiGD;
    string maKH;
    string maGhe;
    long soTien;
    GiaoDich* tiep;
};

// ====================== LỚP QUẢN LÝ HỆ THỐNG ======================
class CinemaSystem {
private:
    Ghe* danhSachGhe;
    KhachHang* goc;
    NguoiCho* dauHangCho;
    GiaoDich* dinhStack;
    long tongDoanhThu;

    KhachHang* themBST(KhachHang* node, KhachHang* moi) {
        if (node == nullptr) return moi;
        if (moi->maKH < node->maKH) node->trai = themBST(node->trai, moi);
        else if (moi->maKH > node->maKH) node->phai = themBST(node->phai, moi);
        return node;
    }

    KhachHang* timBST(KhachHang* node, const string& maKH) {
        if (node == nullptr) return nullptr;
        if (maKH == node->maKH) return node;
        if (maKH < node->maKH) return timBST(node->trai, maKH);
        return timBST(node->phai, maKH);
    }

    void timTheoTenBST(KhachHang* node, const string& ten, bool& timThay) {
        if (node == nullptr) return;
        if (node->tenKH == ten) {
            timThay = true;
            cout << " -> Ma KH: " << node->maKH
                 << " | Ten: " << node->tenKH
                 << " | VIP: " << (node->isVIP ? "Co" : "Khong")
                 << " | Cao tuoi: " << (node->isCaoTuoi ? "Co" : "Khong")
                 << " | So ve da mua: " << node->soVeDaMua << "\n";
        }
        timTheoTenBST(node->trai, ten, timThay);
        timTheoTenBST(node->phai, ten, timThay);
    }

    void xoaCayBST(KhachHang* node) {
        if (node == nullptr) return;
        xoaCayBST(node->trai);
        xoaCayBST(node->phai);
        delete node;
    }

    Ghe* timGhe(const string& maGhe) {
        Ghe* p = danhSachGhe;
        while (p != nullptr) {
            if (p->maGhe == maGhe) return p;
            p = p->tiep;
        }
        return nullptr;
    }

    // Chèn vào hàng chờ ưu tiên: ưu tiên giảm dần, cùng mức thì FIFO
    void chenVaoHangCho(NguoiCho* moi) {
        if (dauHangCho == nullptr || moi->doUuTien > dauHangCho->doUuTien) {
            moi->tiep = dauHangCho;
            dauHangCho = moi;
            return;
        }
        NguoiCho* p = dauHangCho;
        while (p->tiep != nullptr && p->tiep->doUuTien >= moi->doUuTien) {
            p = p->tiep;
        }
        moi->tiep = p->tiep;
        p->tiep = moi;
    }

    void pushGiaoDich(const string& loai, const string& maKH,
                       const string& maGhe, long soTien) {
        GiaoDich* gd = new GiaoDich();
        gd->loaiGD = loai;
        gd->maKH = maKH;
        gd->maGhe = maGhe;
        gd->soTien = soTien;
        gd->tiep = dinhStack;
        dinhStack = gd;
    }

    void khoiTaoKhachHangMau() {
        struct DuLieuMau {
            string maKH, tenKH;
            bool isVIP, isCaoTuoi;
        };
        DuLieuMau danhSachMau[5] = {
            {"KH001", "Nguyen Van An",  true,  false},
            {"KH002", "Tran Thi Bich",  false, true},
            {"KH003", "Le Hoang Nam",   false, false},
            {"KH004", "Pham Thi Hoa",   false, false},
            {"KH005", "Do Van Minh",    true,  false}
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

public:
    CinemaSystem() {
        goc = nullptr;
        dauHangCho = nullptr;
        dinhStack = nullptr;
        tongDoanhThu = 0;
        danhSachGhe = nullptr;

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
        khoiTaoKhachHangMau();
    }

    ~CinemaSystem() {
        Ghe* g = danhSachGhe;
        while (g != nullptr) { Ghe* tmp = g; g = g->tiep; delete tmp; }
        xoaCayBST(goc);
        NguoiCho* nc = dauHangCho;
        while (nc != nullptr) { NguoiCho* tmp = nc; nc = nc->tiep; delete tmp; }
        GiaoDich* gd = dinhStack;
        while (gd != nullptr) { GiaoDich* tmp = gd; gd = gd->tiep; delete tmp; }
    }

    // 1. Hiển thị sơ đồ ghế
    void hienThiSoDoGhe() const {
        cout << "\n================ SO DO GHE NGOI (ASCII) ================\n";
        cout << "        ";
        for (int c = 1; c <= SO_COT; c++) cout << " " << c << "  ";
        cout << "\n";
        for (int h = 0; h < SO_HANG; h++) {
            char tenHang = 'A' + h;
            cout << "  Hang " << tenHang << " : ";
            Ghe* q = danhSachGhe;
            while (q != nullptr) {
                if (q->hang == tenHang) cout << (q->daDat ? "[X] " : "[ ] ");
                q = q->tiep;
            }
            cout << (tenHang == 'A' ? "  (VIP)" : "") << "\n";
        }
        cout << "==========================================================\n";
        cout << "Chu thich: [ ] Ghe trong   [X] Ghe da dat   Hang A = VIP\n";
    }

    // 2. Đặt vé
    void datVe(const string& maKH, const string& tenKH, bool isVIP, bool isCaoTuoi) {
        Ghe* gheTrong = nullptr;
        Ghe* p = danhSachGhe;
        while (p != nullptr) {
            if (!p->daDat) {
                if (isVIP && p->hang == 'A') { gheTrong = p; break; }
                if (!gheTrong && !(p->hang == 'A' && !isVIP)) gheTrong = p;
            }
            p = p->tiep;
        }
        if (gheTrong == nullptr) {
            p = danhSachGhe;
            while (p != nullptr) {
                if (!p->daDat) { gheTrong = p; break; }
                p = p->tiep;
            }
        }

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
            NguoiCho* nc = new NguoiCho();
            nc->maKH = maKH;
            nc->tenKH = tenKH;
            nc->isVIP = isVIP;
            nc->isCaoTuoi = isCaoTuoi;
            nc->doUuTien = isVIP ? 2 : (isCaoTuoi ? 1 : 0);
            nc->tiep = nullptr;
            chenVaoHangCho(nc);
            cout << ">> Rap da het ghe trong! Khach hang " << tenKH
                 << " (" << maKH << ") da duoc them vao DANH SACH CHO uu tien.\n";
            return;
        }

        gheTrong->daDat = true;
        gheTrong->maKhachHang = maKH;
        daTon->soVeDaMua++;
        tongDoanhThu += gheTrong->giaVe;
        pushGiaoDich("DAT_VE", maKH, gheTrong->maGhe, gheTrong->giaVe);

        cout << ">> Dat ve thanh cong! Khach hang " << tenKH << " (" << maKH
             << ") - Ghe: " << gheTrong->maGhe
             << " - Gia: " << gheTrong->giaVe << " VND\n";
    }

    bool capGheChoNguoiCho(Ghe* ghe) {
        if (dauHangCho == nullptr) return false;
        NguoiCho* nguoiDuocCap = dauHangCho;
        dauHangCho = dauHangCho->tiep;

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
        pushGiaoDich("DAT_VE (tu hang cho)", nguoiDuocCap->maKH, ghe->maGhe, ghe->giaVe);

        cout << ">> Da tu dong cap ghe " << ghe->maGhe << " cho khach hang dang cho: "
             << nguoiDuocCap->tenKH << " (" << nguoiDuocCap->maKH << ")\n";

        delete nguoiDuocCap;
        return true;
    }

    // 3. Hủy vé
    void huyVe(const string& maGhe) {
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

        KhachHang* kh = timBST(goc, maKHCu);
        if (kh != nullptr && kh->soVeDaMua > 0) kh->soVeDaMua--;

        tongDoanhThu -= giaVe;
        pushGiaoDich("HUY_VE", maKHCu, maGhe, giaVe);

        g->daDat = false;
        g->maKhachHang = "";

        cout << ">> Da huy ve ghe " << maGhe << " cua khach hang " << maKHCu << "\n";

        capGheChoNguoiCho(g);
    }

    // 4. Tìm kiếm khách hàng (BST)
    void timKiemTheoMa(const string& maKH) {
        KhachHang* kq = timBST(goc, maKH);
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

    void timKiemTheoTen(const string& ten) {
        bool timThay = false;
        timTheoTenBST(goc, ten, timThay);
        if (!timThay) cout << "Khong tim thay khach hang co ten: " << ten << "\n";
    }

    // 5. Xuất danh sách đã đặt vé
    void xuatDanhSachDaDat() const {
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

    // 6. Thống kê
    void thongKe() const {
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

    void xemDanhSachCho() const {
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

    void xemLichSuGiaoDich() const {
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
};

// =================================================================
//                  CÁC TEST CASE KIỂM THỬ TỰ ĐỘNG
// =================================================================
void chayTestCase() {
    cout << "\n#################################################\n";
    cout << "#          BAT DAU CHAY CAC TEST CASE          #\n";
    cout << "#################################################\n";

    CinemaSystem he;

    // ---------- TEST 1: Dat ve thanh cong, kiem tra so do ghe ----------
    cout << "\n--- TEST 1: Dat ve cho khach hang thuong ---\n";
    he.datVe("KH01", "Nguyen Van A", false, false);
    he.hienThiSoDoGhe();

    // ---------- TEST 2: Dat ve VIP -> duoc uu tien ghe hang A ----------
    cout << "\n--- TEST 2: Dat ve cho khach VIP (uu tien hang A) ---\n";
    he.datVe("KH02", "Tran Thi B", true, false);
    he.xuatDanhSachDaDat();

    // ---------- TEST 3: Lap day rap (30 ghe) -> khach thu 31 vao hang cho ----------
    cout << "\n--- TEST 3: Lap day toan bo rap va kiem tra Priority Queue ---\n";
    for (int i = 3; i <= 30; i++) {
        string ma = "KH" + (i < 10 ? string("0") + to_string(i) : to_string(i));
        he.datVe(ma, "Khach Hang " + to_string(i), false, false);
    }
    he.thongKe();
    he.datVe("KH31", "Le Van Thuong", false, false);
    he.datVe("KH32", "Pham Thi Gia", false, true);
    he.datVe("KH33", "Hoang VIP", true, false);
    cout << "Ky vong thu tu hang cho: KH33 (VIP) > KH32 (Cao tuoi) > KH31 (Thuong)\n";
    he.xemDanhSachCho();

    // ---------- TEST 4: Huy ve -> ghe trong duoc cap tu dong cho hang cho ----------
    cout << "\n--- TEST 4: Huy ve ghe A1 (cua KH01) va kiem tra cap phat tu dong ---\n";
    he.huyVe("A1");
    cout << "Ky vong: KH33 (dau hang cho, VIP) duoc cap ghe A1 tu dong\n";
    he.xemDanhSachCho();
    he.xuatDanhSachDaDat();

    // ---------- TEST 5: Tim kiem khach hang theo ma va theo ten (BST) ----------
    cout << "\n--- TEST 5: Tim kiem khach hang mau co san bang BST ---\n";
    cout << "Tim theo ma 'KH001' (khach hang mau, VIP):\n";
    he.timKiemTheoMa("KH001");
    cout << "Tim theo ten 'Tran Thi Bich' (khach hang mau, cao tuoi):\n";
    he.timKiemTheoTen("Tran Thi Bich");
    cout << "Tim theo ma 'KH02' (khach vua dat ve trong test 2):\n";
    he.timKiemTheoMa("KH02");
    cout << "Tim ma khong ton tai 'KH99':\n";
    he.timKiemTheoMa("KH99");

    // ---------- TEST 6: Kiem tra Stack luu lich su giao dich ----------
    cout << "\n--- TEST 6: Kiem tra lich su giao dich (Stack - LIFO) ---\n";
    he.xemLichSuGiaoDich();
    cout << "Ky vong: giao dich moi nhat (HUY_VE / DAT_VE tu hang cho) nam tren cung\n";

    // ---------- TEST 7: Thong ke doanh thu cuoi cung ----------
    cout << "\n--- TEST 7: Thong ke tong quan cuoi cung ---\n";
    he.thongKe();

    cout << "\n#################################################\n";
    cout << "#          KET THUC CAC TEST CASE              #\n";
    cout << "#################################################\n";
}

int main() {
    chayTestCase();
    return 0;
}
