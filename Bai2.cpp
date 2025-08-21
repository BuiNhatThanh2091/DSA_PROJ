#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct Music {
    string nameMusic;
    string singer;
    int year;
    double rating;
    Music *pLeft;
    Music *pRight;
};

typedef Music NODE;
typedef NODE* TREE;

// Hàm khởi tạo cây
void KhoiTaoCay(TREE &t) {
    t = NULL;
}

// Hàm tạo 1 node
NODE* TaoMotNode(Music x) {
    NODE *p = new NODE;
    p->nameMusic = x.nameMusic;
    p->singer = x.singer;
    p->year = x.year;
    p->rating = x.rating;
    p->pLeft = NULL;
    p->pRight = NULL;
    return p;
}

// Hàm thêm 1 phần tử vào cây (Dùng đệ quy)
void ThemNodeVaoCay1(TREE &t, Music x) {
    if (t == NULL) {
        t = TaoMotNode(x);
    } else {
        if (x.year < t->year) {
            ThemNodeVaoCay1(t->pLeft, x);
        } else if (x.year > t->year) {
            ThemNodeVaoCay1(t->pRight, x);
        } else {
            cout << "Bai nhac nam nay da ton tai!" << endl;
        }
    }
}

// Hàm thêm 1 phần tử vào cây (không dùng đệ quy) --> dùng vòng lặp
NODE* ThemNodeVaoCay2(TREE t, Music x) {
    NODE* newMusic = TaoMotNode(x);
    if (t == NULL)
        return newMusic;
    NODE *parent = NULL, *curr = t;  // parent: con trỏ giữ vị trí của node cha node cần chèn, curr: con trỏ duyệt cây
    while (curr != NULL) {
        parent = curr;
        if (curr->year > x.year)
            curr = curr->pLeft;
        else if (curr->year < x.year)
            curr = curr->pRight;
        else {
            cout << "Bai nhac nam nay da ton tai!" << endl;
            delete newMusic;
            return t;
        }
    }
    // đã tìm được vị trí chèn node mới
    if (parent->year > x.year)
        parent->pLeft = newMusic;
    else
        parent->pRight = newMusic;
    return t;
}

NODE* TimPhanTuTheMang(TREE t) {
    if (t == NULL) {
        cout << "Cay rong!" << endl;
        return NULL;
    }
    t = t->pRight;
    while (t != NULL && t->pLeft != NULL)
        t = t->pLeft;
    return t;
}

// Hàm xóa 1 bài nhạc trong cây (dùng đệ quy)
NODE* XoaNode1(TREE &t, int year) {
    if (t == NULL) {
        cout << "Khong tim thay bai nhac can xoa!" << endl;
        return t;
    }
    if (year < t->year) {
        t->pLeft = XoaNode1(t->pLeft, year);
    } else if (year > t->year) {
        t->pRight = XoaNode1(t->pRight, year);
    } else {
        if (t->pLeft == NULL) {
            NODE* temp = t->pRight;  // Node chỉ có con phải
            delete t;
            return temp;
        } else if (t->pRight == NULL) {
            NODE* temp = t->pLeft;  // Node chỉ có con trái
            delete t;
            return temp;
        } else {
            // Node có cả con trái và con phải
            NODE* theMang = TimPhanTuTheMang(t);  // Tìm node thế mạng
            t->year = theMang->year;
            t->nameMusic = theMang->nameMusic;
            t->singer = theMang->singer;
            t->rating = theMang->rating;
            t->pRight = XoaNode1(t->pRight, theMang->year);  // Xóa node thế mạng
        }
    }
    return t;
}

// Hàm xóa 1 bài nhạc trong cây (Không dùng đệ quy)
NODE* XoaNode2(TREE& t, int year)
{
    // Tìm node cần xóa
    NODE* current = t;              // con trỏ để duyệt
    NODE* parent = NULL;            // con trỏ để lưu vị trí cha của node cần xóa
    while (current != NULL && current->year != year) {
        parent = current;
        if (year < current->year)
            current = current->pLeft;
        else
            current = current->pRight;
    } // sau khi duyệt xong current sẽ trỏ vào node cần xóa, parent sẽ trỏ vào node cha của current
    if (current == NULL) // Không tìm thấy node cần xóa
        return t;
    //Trường hợp nút cần xóa có 1 con hoặc không có con nào
    if (current->pLeft == NULL || current->pRight == NULL) {
        NODE* newCurrent;
        if (current->pLeft == NULL)   
            newCurrent = current->pRight;
        else
            newCurrent = current->pLeft;
        if (parent == NULL)   
            return newCurrent;
        // Kiểm tra vị trí của current so với parent
        if (current == parent->pLeft)
            parent->pLeft = newCurrent;
        else
            parent->pRight = newCurrent;
        delete current;
    }
    // Trường hợp nút cần xóa có hai nhánh con
    else {
        // temp: nút thay thế (nhỏ nhất nhánh phải)
        NODE* p = NULL; // p: cha của temp
        NODE* temp = current->pRight;
        while (temp->pLeft != NULL) {
            p = temp;
            temp = temp->pLeft;
        }
        if (p != NULL)
            p->pLeft = temp->pRight; // nối nhánh phải của temp vào cha của temp
        else
        current->pRight = temp->pRight;
        current->year = temp->year;
        current->nameMusic = temp->nameMusic;
        current->singer = temp->singer;
        current->rating = temp->rating;
        delete temp;    // xóa nút thay thế
    }
    return t;
}

// Tính chiều cao
int ChieuCaoCay (TREE t) {
    if (t==NULL) return 0;
    int hl = ChieuCaoCay(t->pLeft);
    int hr = ChieuCaoCay(t->pRight);
    if (hl > hr)
        return (1 + hl);
    else
        return (1 + hr);
}

// In cây theo PreOrder (NLR) 
void PreOrder(TREE t) {
    if (t != NULL) {
        cout << "| " 
             << left << setw(24) << t->nameMusic 
             << " | " << left << setw(16) << t->singer 
             << " | " << right << setw(4) << t->year 
             << " | " << right << setw(8) << fixed << setprecision(1) << t->rating 
             << " |" << endl;
        PreOrder(t->pLeft);
        PreOrder(t->pRight);
    }
}

// Hàm in bảng nhạc
void InBang(TREE t) {
    cout << "+--------------------------+------------------+------+--------+------" << endl;
    cout << "| Ten Bai Nhac             | Ca Si            | Nam  | Danh gia|" << endl;
    cout << "+--------------------------+------------------+------+--------+------" << endl;
    PreOrder(t);
    cout << "+--------------------------+------------------+------+--------+-----" << endl;
}

int main() {
    TREE t;
    KhoiTaoCay(t);
    Music dsNhac[] = {
        {"See you again", "Charlie Puth", 2015, 9.2},
        {"Beautiful Girl","Michael Jackson", 2004, 9.4},
        {"Baby", "Justin Bieber", 2010, 9.4},
        {"We Don't Talk Anymore", "Charlie Puth", 2016, 8.6},
        {"Co hen voi thanh xuan", "MONSTAR", 2021, 9.0},
        {"Di de tro ve 1", "SOOBIN HOANG SON", 2017, 9.5},
        {"Di de tro ve 2", "SOOBIN HOANG SON", 2018, 8.8},
        {"Di de tro ve 3", "SOOBIN HOANG SON", 2019, 8.5},
        {"Waiting for you", "MONO", 2022, 9.3},
    };
    for (int i = 0; i < sizeof(dsNhac) / sizeof(dsNhac[0]); ++i) {
        ThemNodeVaoCay1(t, dsNhac[i]);
    }
    cout << "Danh sach nhac ban dau: " << endl;
    InBang(t);
    while(true) {
        cout<<"====================MENU===================="<<endl;
        cout<<"1. Them bai nhac(Dung de quy)"<<endl;
        cout<<"2. Them bai nhac(Khong dung de quy)"<<endl;
        cout<<"3. Xoa bai nhac(Dung de quy)"<<endl;
        cout<<"4. Xoa bai nhac(Khong dung de quy)"<<endl;
        cout<<"5. Tinh chieu cao cay"<<endl;
        cout<<"6. Thoat"<<endl;
        int choice;
        cout<<"Nhap lua chon: ";
        cin>>choice;
        cin.ignore();
        if(choice==1)
        {
            Music x;
            cout<<"Nhap ten bai nhac: ";
            getline(cin, x.nameMusic);
            cout<<"Nhap ten ca si: ";
            getline(cin, x.singer);
            cout<<"Nhap nam phat hanh: ";
            cin>>x.year;
            cout<<"Nhap danh gia: ";
            cin>>x.rating;
            ThemNodeVaoCay1(t, x);
            cout<<"Danh sach nhac sau khi them: "<<endl;
            InBang(t);
        }
        else if(choice==2)
        {
            Music x;
            cout<<"Nhap ten bai nhac: ";
            getline(cin, x.nameMusic);
            cout<<"Nhap ten ca si: ";
            getline(cin, x.singer);
            cout<<"Nhap nam phat hanh: ";
            cin>>x.year;
            cout<<"Nhap danh gia: ";
            cin>>x.rating;
            ThemNodeVaoCay2(t, x);
            cout<<"Danh sach nhac sau khi them: "<<endl;
            InBang(t);
        }
        else if(choice==3)
        {
            int year;
            cout<<"Nhap nam bai nhac can xoa: ";
            cin>>year;
            t=XoaNode1(t, year);
            cout<<"Danh sach nhac sau khi xoa: "<<endl;
            InBang(t);
        }
        else if(choice==4)
        {
            int year;
            cout<<"Nhap nam bai nhac can xoa: ";
            cin>>year;
            t = XoaNode2(t, year);
            cout<<"Danh sach nhac sau khi xoa: "<<endl;
            InBang(t);
        }
        else if(choice==5)
        {
            cout<<"Chieu cao cay: "<<ChieuCaoCay(t)<<endl;
        }
        else if(choice==6)
        {
            break;
        }
    }
    return 0;
}
