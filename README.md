
# He_thong_dat_ve_xe# 🎬 Hệ Thống Đặt Vé Xe

Dự án cá nhân mô phỏng hệ thống đặt vé xe sử dụng 4 cấu trúc dữ liệu thủ công (không dùng STL): BST, Priority Queue, Stack, Linked List — viết bằng C++17.

---

## Cấu trúc dữ liệu sử dụng

- **BST (Binary Search Tree):** dùng để lưu trữ và tìm kiếm khách hàng theo mã KH, vì mã KH là chuỗi có thể so sánh thứ tự tự nhiên — phù hợp làm khóa cây, cho phép thêm/tìm với độ phức tạp trung bình O(log n) mà không cần dịch chuyển dữ liệu như mảng.

- **Priority Queue (Hàng đợi ưu tiên):** dùng để quản lý danh sách chờ khi hết ghế, vì khách VIP và người cao tuổi cần được ưu tiên phục vụ trước bất kể thời điểm đến — Queue thường (FIFO) không đáp ứng được yêu cầu này.

- **Stack (Ngăn xếp):** dùng để ghi lại lịch sử giao dịch và hỗ trợ hủy vé, vì giao dịch gần nhất luôn cần xử lý trước (LIFO) — đây là bản chất tự nhiên của nghiệp vụ hoàn tác/xem lịch sử.

- **Linked List (Danh sách liên kết):** dùng để biểu diễn sơ đồ ghế ngồi, vì số lượng ghế có thể thay đổi khi mở rộng (thêm hàng, thêm phòng chiếu) mà không cần cấp phát lại vùng nhớ liên tục như mảng tĩnh.

---

## Compile và chạy

**Chương trình chính (menu):**
```bash
g++ -std=c++17 src/main.cpp src/functions.cpp -o app && ./app
```

**Chạy test case độc lập (không cần file khác):**
```bash
g++ -std=c++17 src/test.cpp -o test_app && ./test_app
```

> ⚠️ Không build `main.cpp` và `test.cpp` cùng lúc vì cả hai đều có hàm `main()`.

---

## Chức năng

1. **Hiển thị sơ đồ ghế (ASCII)** — Vẽ lưới ghế 5 hàng × 6 cột, ký hiệu `[ ]` ghế trống và `[X]` ghế đã đặt, hàng A được đánh dấu VIP.
2. **Đặt vé** — Tự động tìm ghế phù hợp (ưu tiên hàng A cho khách VIP); nếu hết ghế thì tự động đưa khách vào Priority Queue theo mức ưu tiên.
3. **Hủy vé** — Ghi nhận vào Stack lịch sử; tự động cấp ghế vừa trống cho người đứng đầu hàng chờ (ưu tiên cao nhất).
4. **Tìm kiếm khách hàng** — Tìm theo mã KH qua BST với O(log n); tìm theo tên bằng cách duyệt toàn cây.
5. **Xuất danh sách đã đặt vé** — Liệt kê tất cả ghế đang được giữ kèm mã khách hàng và giá vé.
6. **Thống kê** — Hiển thị số ghế trống/đã đặt, số người đang chờ và tổng doanh thu hiện tại.

> Ngoài ra: xem danh sách chờ (Priority Queue) và xem lịch sử giao dịch (Stack) qua menu phụ.

---

## Test cases

| # | Mô tả | Kết quả kỳ vọng |
|---|---|---|
| 1 | Đặt vé cho khách thường `KH01` | Vé được cấp, ghế chuyển sang `[X]` trên sơ đồ |
| 2 | Đặt vé cho khách VIP `KH02` | Được ưu tiên ghế hàng A (VIP), hiển thị đúng trên danh sách |
| 3 | Lấp đầy 30 ghế rồi cho thêm 3 khách (thường → cao tuổi → VIP) vào hàng chờ | Thứ tự hàng chờ đúng: VIP (`KH33`) → Cao tuổi (`KH32`) → Thường (`KH31`) |
| 4 | Hủy vé ghế `A1` của `KH01` khi đang có người trong hàng chờ | Ghế `A1` tự động được cấp cho `KH33` (VIP, đầu hàng chờ), Stack ghi nhận đúng 2 giao dịch liên tiếp |
| 5 | Tìm kiếm khách hàng mẫu `KH001` theo mã và `Tran Thi Bich` theo tên; tìm mã không tồn tại `KH99` | BST trả về đúng thông tin 2 trường hợp có, thông báo không tìm thấy cho `KH99` |

---

## Cấu trúc file

```
src/
  main.cpp        — Menu điều khiển chương trình chính
  functions.h     — Khai báo struct (Ghe, KhachHang, NguoiCho, GiaoDich)
                    và nguyên mẫu hàm của class CinemaSystem
  functions.cpp   — Cài đặt chi tiết toàn bộ logic: BST, Priority Queue,
                    Stack, Linked List và các chức năng nghiệp vụ
  test.cpp        — File kiểm thử độc lập (tự chứa toàn bộ code,
                    không include functions.h, build riêng một mình)
```

---

## Dữ liệu mẫu có sẵn

Hệ thống khởi động với 5 khách hàng được nạp sẵn vào BST:

| Mã KH | Họ tên | Loại |
|---|---|---|
| KH001 | Nguyễn Văn An | VIP |
| KH002 | Trần Thị Bích | Cao tuổi |
| KH003 | Lê Hoàng Nam | Thường |
| KH004 | Phạm Thị Hoa | Thường |
| KH005 | Đỗ Văn Minh | VIP |

---

## Ghi chú kỹ thuật

- Toàn bộ cấu trúc dữ liệu được cài đặt thủ công bằng `new`/`delete`, **không sử dụng STL** (`vector`, `map`, `stack`, `queue`, `priority_queue`...).
- Sơ đồ ghế: **5 hàng (A–E) × 6 cột = 30 ghế**; hàng A giá **120.000 VNĐ** (VIP), hàng B–E giá **75.000 VNĐ**.
- BST không tự cân bằng — trường hợp xấu nhất tìm kiếm là O(n) nếu mã KH được thêm theo thứ tự tăng dần liên tục.
