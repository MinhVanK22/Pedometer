# Pedometer By Group 6
## Tên các thành viên:
- Nguyễn Minh Văn - 20020739
- Lục Văn Tuyên - 20020349
## Tính năng:
- Đặc điểm kỹ thuật chính:
``+ Vi điều khiển: STM32F103C8T6
+ Cảm biến gia tốc: MPU6050
+ Màn hình: LCD 1602A
+ Giao diện ngoại vi: GPIO (General Purpose Input/Output) và I2C
+ Cấp nguồn: 3.3V DC cho vi điều khiển, 5V DC cho LCD và MPU6050``

- Tính năng và chức năng:
``+ Đo giá trị của gia tốc rồi xử lý tín hiệu để phát hiện bước chân
+ Lưu trữ giá trị đếm vào bộ nhớ và truyền qua giao thức I2C đến màn hình LCD
+ Hỗ trợ chế độ hoạt động, tạm dừng và đặt lại đếm
+ Sử dụng 2 LED: LED xanh báo trạng thái hoạt động và LED đỏ báo trạng thái
tạm dừng
+ Sử dụng 2 nút bấm: Nút 1 để thay đổi trạng thái hoạt động hay tạm dừng. Nút 2
để đặt lại giá trị bộ đếm về 0
+ Phát hiện được bước chân khi đi bộ ở tốc độ tiêu chuẩn 5km/h``

- Điều kiện và môi trường hoạt động:
``+ Được cấp đủ nguồn điện theo thông số kỹ thuật phía trên
+ Hoạt động ổn định trong điều kiện nhiệt độ, độ ẩm bình thường và đường đi
bằng phẳng``