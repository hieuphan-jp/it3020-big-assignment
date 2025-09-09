
#include <bits/stdc++.h>

using namespace std;

#define pb push_back

/*
    khai báo kiểu struct Node
    với a, b, c lần lượt là lượng nước trong các bình 1, 2, 3
*/

typedef struct {
    int a, b, c;
} Node;

/*
    MAXV là số trạng thái tối đa
    visited[a][b][c] là mảng để đánh dấu đã duyệt qua trạng thái với a, b, c lần lượt là lượng nước ở bình 1, 2, 3.
    vertice[i] là mảng kiểu Node chứa trạng thái tại đỉnh được đánh số i
    dinh để đếm số đỉnh.
*/

const int MAXV = 101;
bool visited[11][8][5];
Node vertice[MAXV];
int dinh = 0;

/*
    Hàm gồm 3 đối a, b, c trả ra kiểu dữ liệu node (a, b, c)
    với mục đích gán vào biến kiểu Node
*/

Node create(int a, int b, int c) {
    Node v;
    v.a = a;
    v.b = b;
    v.c = c;
    return v;
}

void explore(int pre, int space, Node u) {
    visited[u.a][u.b][u.c] = 1;
    ++dinh;
    int id = dinh;
    vertice[id] = u;
    int a = u.a, b = u.b, c = u.c;
    /*
        Xây dựng danh sách kề next cho đỉnh u
        Lần lượt từng dòng là:
        đổ b vào a
        đỏ c vào a
        đổ a vào b
        đổ c vào b
        đổ a vào c
        đổ b vào c
    */
    vector<Node> next;
    Node v;
    v = create(min(a + b, 10), max(a + b - 10, 0), c);
    next.pb(v);
    v = create(min(a + c, 10), b, max(a + c - 10, 0));
    next.pb(v);
    v = create(max(a + b - 7, 0), min(a + b, 7), c);
    next.pb(v);
    v = create(a, min(b + c, 7), max(b + c - 7, 0));
    next.pb(v);
    v = create(max(a + c - 4, 0), b, min(a + c, 4));
    next.pb(v);
    v = create(a, max(b + c - 4, 0), min(b + c, 4));
    next.pb(v);
    //duyệt qua danh sách kề
    for(auto v : next) {
        if(visited[v.a][v.b][v.c]) continue;
        explore(id, space + 1, v);
    }
    return ;
}

int main() {
    Node u = create(0, 7, 4);
    memset(visited, 0, sizeof(visited));
    explore(-1, 0, u);
    // In ra tất cả các kết quả.
    for(int i = 1; i <= dinh; ++i) {
        Node u = vertice[i];
        if(u.b == 2 || u.c == 2) {
            cout << u.a << ' ' << u.b << ' ' << u.c << '\n';
        }
    }
    return 0;
}
