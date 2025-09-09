#include <bits/stdc++.h>

using namespace std;

#define pb push_back

/*
    Do có không quá 5757 từ (nghĩa là đường đi ngắn nhất tối đa giữa 2 đỉnh chỉ là 5757)
    nên để oo (vô cùng) như dưới là phù hợp.
*/

const int oo = 1000000000 + 1;

/*
    Cấu trúc dữ liệu Trie:
        Thêm một xâu S vào cây mất O(S.length())
        Tìm kiếm một xâu S trong cây mất O(S.length())

        Số lượng biến cây tạo ra là T(5757 * 5)

*/

struct Trie{
    /*
        Định nghĩa kiểu dữ liệu Node gồm
            mảng child chứa 26 ký tự
            exist để kiểm tra xem có từ nào kết thục tại Node đó hay không (-1 nếu không có, nếu có sẽ là số thứ tự của từ trong tập)
    */
    struct Node{
        Node* child[26];
        int exist;

        Node() {
            for (int i = 0; i < 26; i++) child[i] = NULL;
            exist = -1;
        }
    };

    // Khởi tạo cây Trie bằng một nút root
    Node* root;
    Trie() {
        root = new Node();
    };
    /*
        Hàm có mục đính thêm một xây s, ở vị trí name trong tập vào cây

        Bắt đầu từ Node root, duyệt qua lần lượt từng ký tự để thêm vào cây Trie.

        Kết thúc exist tại Node được trỏ bởi con trỏ p được gán name
    */
    void add_string(int name, string s) {
        Node* p = root;
        for (auto f : s) {
            int c = f - 'a';
            if (p->child[c] == NULL) p->child[c] = new Node();

            p = p->child[c];
        }
        p->exist = name;
    }

    /*
        Kiểm tra xem một xâu có tồn tại trong cây Trie (trong tập) hay không

        Nếu có trả ra số thứ tự trong tập. Còn không thì -1.
    */
    int find_string(string s) {
        Node* p = root;
        for (auto f : s) {
            int c = f - 'a';
            if (p->child[c] == NULL) return 0;
            p = p->child[c];
        }
        if(p -> exist == -1) return 0;
        return p -> exist;
    }
};

/*
    MAXV là số đỉnh tối đa
    str là danh sách các từ
    G là danh sách kề
    visited là mảng đánh dấu đỉnh đã duyệt trong DFS
    trace[u] là cha của đỉnh u trong cây DFS
    TPLT là số thành phần liên thông
    dist[u] là khoảng cách từ đỉnh start đến u (với start là đỉnh bắt đầu BFS)
*/

const int MAXV = 5757;
string str[MAXV + 1];
vector<int> G[MAXV + 1];
bool visited[MAXV + 1];
int trace[MAXV + 1];
int TPLT = 0, dist[MAXV + 1];

void explore(int u) {
    visited[u] = 1;
    for(auto v : G[u]) {
        if(!visited[v]) explore(v);
    }
    return;
}

void BFS(int u) {
    queue<int> qu;
    qu.push(u);
    dist[u] = 1;
    trace[u] = -1;
    while(qu.size()) {
        int u = qu.front();
        qu.pop();
        for(auto v : G[u]) {
            if(dist[v] == +oo) {
                qu.push(v);
                dist[v] = dist[u] + 1;
                trace[v] = u;
            }
        }
    }
    return;
}

int main() {
    struct Trie T1;
    /*
        Thêm các từ trong danh sách vào cấu trúc dữ liệu cây Trie
        Mỗi từ trong INPUT được đánh một số là số thự tự dòng của nó trong đầu vào
    */
    for(int i = 1; i <= 5757; ++i) {
        cin >> str[i];
        T1.add_string(i, str[i]);
    }
    /*
        Phần tạo đồ thị:

        với mỗi từ str[i] thuộc tập, duyệt qua từng vị trí j, thay thế ký tự ở vị trí này
        bằng ký tự mới kt (phải khác ký tự ban đầu) và gọi từ mới đó là str2

        Nếu từ mới đó tồn tại trong tập các xâu (cây Trie) thì có một cạnh giữa str[i] và str[v]
        (với v là vị trí của từ str2 trong tập)

    */
    for(int i = 1; i <= 5757; ++i) {
        for(int j = 0; j < 5; ++j) {
            for(int kt = 0; kt < 26; ++kt) {
                if(kt == str[i][j] - 'a') continue;
                string str2 = str[i];
                str2[j] = kt + 'a';
                if(int v = T1.find_string(str2)) G[i].pb(v);
            }
        }
    }
    /*
        Phần dùng để đếm số thành phần liên thông (TPLT)
    */
    memset(visited, 0, sizeof(visited));
    for(int i = 1; i <= 5757; ++i) {
        if(visited[i] == 0) {
            ++TPLT;
            explore(i);
        }
    }
    cout << TPLT << '\n';
    /*
        Phần dùng để tìm đường đi ngắn nhất từ hai từ
    */
    for(int i = 1; i <= 5757; ++i) dist[i] = +oo;
    string sstr, estr;
    cin >> sstr >> estr;
    int u = T1.find_string(sstr), v = T1.find_string(estr);
    if(u == 0 || v == 0) {
        cout << "-1" << '\n';
        return 0;
    }
    memset(trace, 0, sizeof(trace));
    BFS(u);
    if(dist[v] == +oo) cout << -2 << '\n';
    else {
        cout << dist[v] << '\n';
        // Phần trình bày lại một đường đi ngắn nhất từ u --> v
        int tmp = v;
        string route = "";
        while(tmp != -1) {
            route = str[tmp] + " " + route;
            tmp = trace[tmp];
        }
        cout << route << '\n';
    }
    return 0;
}


