import tkinter as tk
from tkinter import Canvas, Entry, Button, Label

# Các thông tin của một bài nhạc
class Music:
    def __init__(self, nameMusic, singer, year, rating):
        self.nameMusic = nameMusic
        self.singer = singer
        self.year = year
        self.rating = rating
        self.left = None
        self.right = None

# Cây nhị phân tìm kiếm (BST)
class CayNhiPhanTimKiem:
    def __init__(self):
        self.root = None  #  Khởi tạo cây rỗng

    def ThemNodeVaoCay(self, nameMusic, singer, year, rating):
        if not self.root:     #cây rỗng
            self.root = Music(nameMusic, singer, year, rating)
        else:
            self._ThemNodeVaoCay(self.root, nameMusic, singer, year, rating)

    def _ThemNodeVaoCay(self, root, nameMusic, singer, year, rating):
        if year < root.year:  # so sánh theo năm phát hành
            if root.left:
                self._ThemNodeVaoCay(root.left, nameMusic, singer, year, rating)
            else:
                root.left = Music(nameMusic, singer, year, rating)
        elif year > root.year:
            if root.right:
                self._ThemNodeVaoCay(root.right, nameMusic, singer, year, rating)
            else:
                root.right = Music(nameMusic, singer, year, rating)

    def XoaNode(self, year):
        self.root = self._XoaNode(self.root, year)

    def _XoaNode(self, root, year):
        if not root:   # cây rỗng
            return None
        if year < root.year:
            root.left = self._XoaNode(root.left, year)
        elif year > root.year:
            root.right = self._XoaNode(root.right, year)
        else:    # node cần xóa
            if not root.left:   # node chỉ có cây con phải
                return root.right
            if not root.right: # node chỉ có cây con trái
                return root.left
            temp = self._min_year_node(root.right)  #temp là node nhỏ nhất của cây con phải
            root.year = temp.year
            root.right = self._XoaNode(root.right, temp.year)
        return root

    def _min_year_node(self, node):
        while node.left:
            node = node.left
        return node

    def pre_order(self, node, result):
        if node:
            result.append((node.nameMusic, node.singer, node.year, node.rating))
            self.pre_order(node.left, result)
            self.pre_order(node.right, result)

# Minh họa CayNhiPhanTimKiem với giao diện người dùng
class CayNhiPhanTimKiemMinhHoa:
    def __init__(self, bst):
        self.bst = bst
        self.window = tk.Tk()
        self.window.title("Minh hoa cay nhi phan tim kiem") 
        
        # Khung chứa cây (Canvas)
        self.canvas_frame = tk.Frame(self.window)
        self.canvas_frame.pack(side=tk.LEFT, padx=10) 

        # Mở rộng kích thước canvas
        self.canvas = Canvas(self.canvas_frame, width=800, height= 1000, bg="white")  
        self.canvas.pack()

        # Khung chứa các nút điều khiển 
        self.control_frame = tk.Frame(self.window)
        self.control_frame.pack(side=tk.RIGHT, padx=10)  # Đặt các nút và hiển thị ở bên phải

        # Nhập thông tin bài nhạc
        self.label_name = Label(self.control_frame, text="Tên bài nhạc:", font=("Arial", 12))
        self.label_name.pack()
        self.entry_name = Entry(self.control_frame)
        self.entry_name.pack()

        self.label_singer = Label(self.control_frame, text="Tên ca sĩ:", font=("Arial", 12))
        self.label_singer.pack()
        self.entry_singer = Entry(self.control_frame)
        self.entry_singer.pack()

        self.label_year = Label(self.control_frame, text="Năm:", font=("Arial", 12))
        self.label_year.pack()
        self.entry_year = Entry(self.control_frame)
        self.entry_year.pack()

        self.label_rating = Label(self.control_frame, text="Đánh giá:", font=("Arial", 12))
        self.label_rating.pack()
        self.entry_rating = Entry(self.control_frame)
        self.entry_rating.pack()

        # Các nút bấm
        Button(self.control_frame, text="ThemNodeVaoCay", command=self.ThemNodeVaoCay_node).pack(pady=5)
        Button(self.control_frame, text="XoaNode", command=self.XoaNode_node).pack(pady=5)
        Button(self.control_frame, text="PreOrder", command=self.HienThiPreOrder).pack(pady=5)

        self.pre_order_label = Label(self.control_frame, text="", font=("Arial", 12))
        self.pre_order_label.pack()
        
    # Thêm thông tin vào cây qua giao diện người dùng
    def ThemNodeVaoCay_node(self):
        try:
            nameMusic = self.entry_name.get()
            singer = self.entry_singer.get()
            year = int(self.entry_year.get())
            rating = float(self.entry_rating.get())
            self.bst.ThemNodeVaoCay(nameMusic, singer, year, rating)
            self.veLaiCay()
            self.xoaNhap()
        except ValueError:
            pass

    # Xóa thông tin khỏi cây qua giao diện người dùng
    def XoaNode_node(self):
        try:
            year = int(self.entry_year.get())
            self.bst.XoaNode(year)
            self.veLaiCay()
            self.xoaNhap()
        except ValueError:
            pass
    # Hiển thị thông tin theo thứ tự PreOrder
    def HienThiPreOrder(self):
        result = []
        self.bst.pre_order(self.bst.root, result)
        pre_order_text = "\n".join([f"{name} | {singer} ({year}) - Rating: {rating}" for name, singer, year, rating in result])
        self.pre_order_label.config(text="PreOrder:\n" + pre_order_text)

    def veLaiCay(self):
        self.canvas.delete("all")
        self.draw_tree()

    def draw_tree(self):
        if self.bst.root:
            self._draw_node(self.bst.root, 300, 50, 150) 

    def _draw_node(self, node, x, y, dx):
        if node:
            r = 20
            self.canvas.create_oval(x - r, y - r, x + r, y + r, fill="lightblue")
            self.canvas.create_text(x, y, text=str(node.year), font=("Arial", 12))
            if node.left:
                self.canvas.create_line(x, y, x - dx, y + 50)
                self._draw_node(node.left, x - dx, y + 50, dx / 2)
            if node.right:
                self.canvas.create_line(x, y, x + dx, y + 50)
                self._draw_node(node.right, x + dx, y + 50, dx / 2)

    # Xóa thông tin nhập sau khi đã thêm hoặc xóa
    def xoaNhap(self):
        self.entry_name.delete(0, tk.END)
        self.entry_singer.delete(0, tk.END)
        self.entry_year.delete(0, tk.END)
        self.entry_rating.delete(0, tk.END)

    def run(self):
        self.draw_tree()
        self.window.mainloop()

bst = CayNhiPhanTimKiem()
dsNhac = [
    ("See you again", "Charlie Puth", 2015, 9.2),
    ("Baby", "Justin Bieber", 2010, 9.4),
    ("We Don't Talk Anymore", "Charlie Puth", 2016, 8.6),
    ("Co hen voi thanh xuan", "MONSTAR", 2021, 9.0),
    ("Di de tro ve 1", "SOOBIN HOANG SON", 2017, 9.5),
    ("Di de tro ve 2", "SOOBIN HOANG SON", 2018, 8.8),
    ("Di de tro ve 3", "SOOBIN HOANG SON", 2019, 8.5),
    ("Waiting for you", "MONO", 2022, 9.3),
]
for music in dsNhac:
    bst.ThemNodeVaoCay(*music)

CayNhiPhanTimKiemMinhHoa(bst).run()
