# MidtermImageProcessing
Biến đổi màu, biến đổi hình học, làm trơn và phát hiện biên cạnh trên miền không gian

## Cài đặt
  - Tải OpenCV 4.1.1 và giải nén vào C:/OpenCV/
  - Build project ở chế độ debug x64 (phiên bản release sẽ được cập nhật sau)
  - Chép tất cả file trong C:/OpenCV/opencv/build/x64/vc15/bin/ vào thư mục chứa file thực thi (.exe) đã được build
  
## Cách sử dụng
  - Mở Command Prompt hoặc Windows PowerShell và nhập lệnh:
  ```bash
  path/to/executable/file/<exeFile>.exe --taskName path/to/input/file/<imageFile>.jpg <taskArgument1> <taskArgument2>
  ```
**1. Chuyển RGB sang GRAY:**
  - task = rgb2gray
  ```bash
  MidtermImageProcessing.exe --rgb2gray test.jpg
  ```
**2. Chuyển GRAY sang RGB:**
  - task = gray2rgb
  ```bash
  MidtermImageProcessing.exe --gray2rgb test.jpg
  ```
**3. Chuyển RGB sang HSV:**
  - task = rgb2hsv
  ```bash
  MidtermImageProcessing.exe --rgb2hsv test.jpg
  ```
**4. Chuyển HSV sang RGB:**
  - task = hsv2rgb
  ```bash
  MidtermImageProcessing.exe --hsv2rgb test.jpg
  ```
**5. Thay đổi độ sáng:**
  - task = bright
  - taskArgument1 = lượng thay đổi
  ```bash
  MidtermImageProcessing.exe --bright test.jpg 100
  ```
**6. Thay đổi độ tương phản:**
  - task = contrast
  - taskArgument1 = lượng thay đổi
  ```bash
  MidtermImageProcessing.exe --contrast test.jpg 1.5
  ```
**8. Tính histogram:**
  - task = hist
  ```bash
  MidtermImageProcessing.exe --hist test.jpg
  ```
**9. Vẽ histogram:**
  - task = drawhist
  ```bash
  MidtermImageProcessing.exe --drawhist test.jpg
  ```
**10. Cân bằng histogram:**
  - task = equalhist
  ```bash
  MidtermImageProcessing.exe --equalhist test.jpg
  ```
**11. So sánh 2 ảnh:**
  - task = compare
  - taskArgument1: đường dẫn đến file ảnh cần so sánh
  ```bash
  MidtermImageProcessing.exe --compare test.jpg anothertest.jpg
  ```
**12. Lọc nhiễu trung bình:**
  - task = mean
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  MidtermImageProcessing.exe --mean test.jpg 3 3
  ```
**13. Lọc nhiễu trung vị:**
  - task = median
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  MidtermImageProcessing.exe --median test.jpg 3 3
  ```
**14. Lọc nhiễu Gaussian:**
  - task = gauss
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  MidtermImageProcessing.exe --gauss test.jpg 3 3
  ```
  
**15. Lọc biên cạnh Sobel:**
  - task = sobel
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  MidtermImageProcessing.exe --sobel test.jpg
  ```
  
**16. Lọc biên cạnh Prewitt:**
  - task = prewitt
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  MidtermImageProcessing.exe --prewitt test.jpg
  ```
**17. Lọc biên cạnh Laplacian:**
  - task = laplace
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  MidtermImageProcessing.exe --laplace test.jpg
  ```
  
## Giấy phép
  - Phần mềm đơn giản phục vụ mục đích học tập
  - Không vì bất kì lý do kinh doanh nào
