/*
    functions.h
    ----------------------------------------------------
    Đề tài: HỆ THỐNG ĐẶT VÉ XEM PHIM
    Cấu trúc dữ liệu sử dụng:
        - BST          : lưu & tìm kiếm khách hàng (theo mã KH)
        - Priority Queue (cài đặt thủ công bằng linked list có thứ tự)
                       : danh sách chờ ưu tiên (VIP, người cao tuổi)
        - Stack (cài đặt thủ công bằng linked list - LIFO)
                       : lịch sử giao dịch, hỗ trợ hủy vé
        - Linked List  : sơ đồ ghế ngồi
    Ghi chú: KHÔNG sử dụng các container có sẵn của STL
             (vector, map, stack, queue, priority_queue...).
             Tất cả được cài đặt thủ công bằng new/delete.
    ----------------------------------------------------
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
using namespace std;

// ====================== HẰNG SỐ HỆ THỐNG ======================
const int SO_HANG = 5;          // Hàng ghế: A -> E
const int SO_COT = 6;           // Cột ghế: 1 -> 6
const int TONG_SO_GHE = SO_HANG * SO_COT;
const long GIA_VE_THUONG = 75000;   // đơn giá vé thường (VNĐ)
const long GIA_VE_VIP = 120000;     // đơn giá vé cho hàng VIP (hàng A)

// ====================== 1. LINKED LIST: SƠ ĐỒ GHẾ ======================
struct Ghe {
    string maGhe;       // VD: "A1", "B3"...
    char hang;          // 'A' .. 'E'
    int cot;            // 1 .. 6
    bool daDat;         // true nếu đã được đặt
    string maKhachHang; // mã KH đang giữ ghế này (nếu daDat = true)
    long giaVe;         // giá vé của ghế này
    Ghe* tiep;          // con trỏ tới ghế tiếp theo trong danh sách liên kết
};

// ====================== 2. BST: KHÁCH HÀNG ======================
struct KhachHang {
    string maKH;         // khóa của BST (duy nhất)
    string tenKH;
    bool isVIP;
    bool isCaoTuoi;
    int soVeDaMua;
    KhachHang* trai;
    KhachHang* phai;
};

// ====================== 3. PRIORITY QUEUE: DANH SÁCH CHỜ ======================
// Độ ưu tiên: VIP = 2, Người cao tuổi = 1, Thường = 0 (số càng lớn ưu tiên càng cao)
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
    string loaiGD;      // "DAT_VE" hoặc "HUY_VE"
    string maKH;
    string maGhe;
    long soTien;
    GiaoDich* tiep;
};

// ====================== LỚP QUẢN LÝ HỆ THỐNG ======================
class CinemaSystem {
private:
    Ghe* danhSachGhe;            // đầu danh sách liên kết các ghế
    KhachHang* goc;              // gốc cây BST khách hàng
    NguoiCho* dauHangCho;        // đầu hàng đợi ưu tiên (priority queue)
    GiaoDich* dinhStack;         // đỉnh stack lịch sử giao dịch
    long tongDoanhThu;

    // ---- Hàm hỗ trợ nội bộ cho BST ----
    KhachHang* themBST(KhachHang* node, KhachHang* moi);
    KhachHang* timBST(KhachHang* node, const string& maKH);
    void timTheoTenBST(KhachHang* node, const string& ten, bool& timThay);
    void duyetGiuaBST(KhachHang* node) const;
    void xoaCayBST(KhachHang* node);

    // ---- Hàm hỗ trợ nội bộ cho Linked List ghế ----
    Ghe* timGhe(const string& maGhe);

public:
    CinemaSystem();
    ~CinemaSystem();

    // 1. Hiển thị sơ đồ ghế dạng ASCII
    void hienThiSoDoGhe() const;

    // 2. Đặt vé - nếu hết ghế trống tự động đưa vào Priority Queue
    void datVe(const string& maKH, const string& tenKH, bool isVIP, bool isCaoTuoi);

    // 3. Hủy vé - ghi vào Stack, tự động cấp ghế cho người chờ ưu tiên cao nhất
    void huyVe(const string& maGhe);

    // 4. Tìm kiếm khách hàng theo mã hoặc tên (BST)
    void timKiemTheoMa(const string& maKH) const;
    void timKiemTheoTen(const string& ten) const;

    // 5. Xuất danh sách đã đặt vé
    void xuatDanhSachDaDat() const;

    // 6. Thống kê: số ghế trống, doanh thu
    void thongKe() const;

    // Thêm: xem lịch sử giao dịch (Stack)
    void xemLichSuGiaoDich() const;

    // Thêm: xem danh sách chờ (Priority Queue)
    void xemDanhSachCho() const;

    // Hàm hỗ trợ gán ghế trống cho người đứng đầu hàng chờ (dùng nội bộ khi hủy vé)
    bool capGheChoNguoiCho(Ghe* ghe);

    // Khởi tạo 5 khách hàng mẫu có sẵn (đưa thẳng vào BST, chưa đặt vé)
    void khoiTaoKhachHangMau();
};

#endif
