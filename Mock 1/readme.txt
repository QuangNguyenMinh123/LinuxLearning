Cách test: cmd -> cd tới thư mục
	   Đối với server: gõ make server
	   Đối với client: gõ make client temp=<nhiệt độ muốn gửi>
	   		   VD:make client temp=0 (k có dấu cách giữa temp và <nhiệt độ>)
		Khi đó, client sẽ gửi data tới server sau mỗi 1s. Giá trị data từ khoảng <nhiệt độ> -> <nhiệt độ> + 99, giá trị là float
		VD: <nhiệt độ> = 0; client sẽ gửi dãy số 0.0, 1.0, 2.0 ... 99.0 đến server. Mỗi lần gửi cách nhau 1s.
		Nếu argument không có temp=<nhiệt độ>, make file sẽ cấp 1 giá trị temp mặc định.
Số lượng client gửi đến server đồng thời cùng 1 lúc mà server vẫn nhận được data và xử lý kịp: 14
Nghĩa là 14 sensor có thể gửi đến server cùng 1 lúc mà server vẫn xử lý đầy đủ và k bỏ sót chút nào.
Sau khi client gửi đủ 100 data, nó sẽ tự disconnect tới server, server nhận được thông báo disconnect cũng sẽ ghi vào log và cũng sẽ hiển thị lên màn hình
Disconnect from IP <192.168.0.106>, port <49148>
Remaining connection: <x>
Cả Client và server có thể tự động shutdown bằng cách nhấn Ctrl + C trên terminal.
Lưu ý cho server: nên đợi client gửi hết data và Shutdown bằng Ctrl + C trước khi muốn đọc file database.db để đảm bảo toàn vẹn data.
/*******************************************************************/
Test cho SQL: dùng phần mềm bất kì hỗ trợ mở file .db
Sẽ có 2 loại bảng: 
Loại bảng 1: Tên bảng: "Connection"
	Hiện thông tin về connection giữa server và node. Thông tin bao gồm : Time, Ip của node, port của server kết nối đến node, trạng thái kết nối (connedted/disconnected)
Loại bảng 2: Tên bảng: "Node<x>"   với <x> là index của node. Nếu có 5 node kết nối tới server thì sẽ có tổng cộng 5 bảng: Node1, Node2, Node3, Node4, Node5
	Hiện thông tin về data client gửi tới server theo thời gian. 
	Thông tin bao gồm : Time, Ip của node, port của server kết nối đến node, nhiệt độ
	Nếu 
/*******************************************************************/
Req 1 : pass. main_server, dòng 154
Req 2: pass. main_server, dòng 282-285
Req 3: pass. Tuy nhiên, không cần phải khởi tạo port cho server hoặc cho client. Client  khi xin kết nối tới server cũng không cần phải khởi tạo port. Mỗi khi client gửi kết nối tới server, server sẽ tự động tìm port để kết nối với client.
Req 4: Pass
Req 5: Đã tính được nhiệt độ trung bình. main_server: dòng 81-107
Req 6: Pass : sql.c: dòng 44-56
Req 7: Đang dùng pipe thay vì FIFO vì dùng FIFO nó lại tạo ra file trên ổ đĩa, nên là xóa đi khá là bất tiện, mặc dù tốc độ cả 2 tương đương nhau
	https://stackoverflow.com/questions/26486015/what-is-faster-pipefifo#:~:text=A%20fifo%20shares%20the%20same,data%20(use%20a%20loop)%20.
Req 8: Pass
Req 9:
	1.a: Pass
	1.b: Pass
	2.a: not yet
	2.b: not yet
	2.c: not yet
	3.a: Pass: sql.c: dòng 60
	3.b: Pass: sql.c: dòng 108
	3.c: Pass: sql.c: dòng 90->96. Chỉ khả năng đọc ghi lên SQL là bị mất, những hoạt động khác như log file, đọc data vẫn bình thường
	3.d: Pass: sql.c: dòng 101
/*******************************************************************/
bug chưa fix: Ở phần SQL, tất cả các bảng data Node<x> đều k có lỗi, nhưng có 1 bảng cuối cùng (<x> có giá trị lớn nhất) thì nhận được 101 data trong khi thực tế
thì client luôn chỉ gửi 100 data. Data thứ 101 bị lặp lại data 100
VD: Nếu có 14 node thì node có index thứ 14 sẽ bị. Bảng này sẽ hiện 101 data. Trong đó, data ở hàng 101 và 100 giống hệt nhau.
    Nếu data client gửi là từ 1400-> 1499 thì 2 giá trị ở hàng 101 và 100 là 1499. Trong khi các hàng khác đều khác nhau.
/*******************************************************************/
Làm thêm: Log sẽ ghi lại toàn bộ data được gửi từ tất cả các node
