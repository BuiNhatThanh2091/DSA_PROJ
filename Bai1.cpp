#include <iostream>
#include <vector>
#define ElementType int
using namespace std;

typedef struct tagNode {
    ElementType Info;  
    tagNode* pNext;   
} Node;

typedef struct tagStack {
    Node* Top;
} Stack;

void initstack(Stack &s) {
    s.Top = NULL; 
}

Node* CreateNode(ElementType x) {
    Node* p = new Node;
    if (p == NULL) return NULL;
    p->Info = x;
    p->pNext = NULL;
    return p;
}

bool isEmpty(Stack &s) {
    return s.Top == NULL;
}

void pushstack(Stack &s, ElementType x) {
    Node* p = CreateNode(x);
    if (p == NULL) {
        cout << "Khong the tao node moi" << endl;
        return;
    }
    p->pNext = s.Top;    
    s.Top = p; 
}

ElementType popstack(Stack &s) {
    if (!isEmpty(s)) {
        Node* temp = s.Top;  
        ElementType data = temp->Info; 
        s.Top = s.Top->pNext;
        delete temp; 
        return data; 
    } else {
        cout << "Stack rong" << endl; 
        return -1; 
    }
}

void nhap(vector<Node*> &adj, int n, int m) {
    for (int i = 0; i < m; i++) {
        ElementType x, y;
        cin >> x >> y;
        Node* x1 = CreateNode(y);
        x1->pNext = adj[x];
        adj[x] = x1;

        Node* y1 = CreateNode(x);
        y1->pNext = adj[y];
        adj[y] = y1;
    }
}

void DFS(vector<Node*> &adj, ElementType u, vector<bool>& visited) {
    Stack s;
    initstack(s);
    
    pushstack(s, u);
    visited[u] = true;

    while (!isEmpty(s)) {
        ElementType v = popstack(s);
        cout << v << " ";

        for (Node* w = adj[v]; w != NULL; w = w->pNext) {
            if (!visited[w->Info]) {
                pushstack(s, w->Info);
                visited[w->Info] = true;
            }
        }
    }
}

int main() {
    int n, m; 
    cout << "Nhap so dinh (n) va so canh (m): ";
    cin >> n >> m;

    vector<Node*> adj(n + 1, nullptr); 
    nhap(adj, n, m);

    vector<bool> visited(n + 1, false);

    cout << "Duyet do thi theo chieu sau : ";
    DFS(adj, 1, visited);

    return 0;
}
