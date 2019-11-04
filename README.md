# ImageFiltering
Làm trơn ảnh và phát hiện biên cạnh trên miền không gian

## Cài đặt
  - Tải OpenCV 4.1.1 và giải nén vào C:/OpenCV/
  - Build project ở chế độ debug x64 (phiên bản release sẽ được cập nhật sau)
  - Chép tất cả file trong C:/OpenCV/opencv/build/x64/vc15/bin/ vào thư mục chứa file thực thi (.exe) đã được build
  
## Cách sử dụng
  - Mở Command Prompt hoặc Windows PowerShell và nhập lệnh:
  ```bash
  path/to/executable/file/<exeFile>.exe --taskName path/to/input/file/<imageFile>.jpg <taskArgument1> <taskArgument2>
  ```
**1. Lọc nhiễu trung bình:**
  - task = mean
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  ImageFiltering.exe --mean test.jpg 3 3
  ```
**2. Lọc nhiễu trung vị:**
  - task = median
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  ImageFiltering.exe --median test.jpg 3 3
  ```
**3. Lọc nhiễu Gaussian:**
  - task = gauss
  - taskArgument1 = chiều rộng kernel
  - taskArgument2 = chiều cao kernel
  ```bash
  ImageFiltering.exe --gauss test.jpg 3 3
  ```
  
**4. Lọc biên cạnh Sobel:**
  - task = sobel
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  ImageFiltering.exe --sobel test.jpg
  ```
  
**5. Lọc biên cạnh Prewitt:**
  - task = prewitt
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  ImageFiltering.exe --prewitt test.jpg
  ```
**6. Lọc biên cạnh Laplacian:**
  - task = laplace
  - taskArgument1 = taskArgument2 = NULL
  ```bash
  ImageFiltering.exe --laplace test.jpg
  ```
  
## Giấy phép
  - Phần mềm đơn giản phục vụ mục đích học tập
  - Không vì bất kì lý do kinh doanh nào
