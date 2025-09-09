
#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define fi first
#define se second

/*
    MAXN là số lượng đỉnh tối đa
    G, Gr lần lượt là danh sách kề của đồ thị G và của đồ thị ngược của G (lật ngược chiều các cạnh)
    visit, visit2 là mảng đánh dấu đã duyệt qua của DFS và DFS2
    timeDFS dùng để đếm thời điểm vào và ra khởi đỉnh
    start là thời điểm vào đỉnh
    post là thời điểm ra đỉnh
    DS có kiểu pair<int, int> với first là post[i], second là đỉnh i
    TPLTM là số thành phần liên thông mạnh
    SCP[id] là danh sách các đỉnh thuộc TPLTM id
    tplt_id[i] là thành phần liên thông mà đỉnh i thuộc về
    cntC[kt] là mảng đếm số lần xuất hiện của ký tự kt phục vụ việc sắp xếp xâu
    par[i] là đỉnh nối đến i trong một đường đi ngắn nhất.
*/

const int MAXN = 5757 + 1;
vector<int> G[MAXN], Gr[MAXN];
bool visit[MAXN];
bool visit2[MAXN];
int timeDFS = 0;
int start[MAXN], post[MAXN];
pair<int, int> DS[MAXN];
int TPLTM = 0;
vector<int> SCP[MAXN];
int tplt_id[MAXN];
string str[5758];
int cntC[27];
int par[MAXN];

void DFS(int u) {
    visit[u] = 1;
    start[u] = ++timeDFS;
    for(auto v : Gr[u]) {
        if(visit[v]) continue;
        DFS(v);
    }
    post[u] = ++timeDFS;
}

void DFS2(int u) {
    SCP[TPLTM].pb(u);
    tplt_id[u] = TPLTM;
    visit2[u] = 1;
    for(auto v : G[u]) {
        if(visit2[v]) continue;
        DFS2(v);
    }
    return;
}

/*
    Hàm cmp là hàm so sánh phục vụ hàm sort.
    Nếu A.fi > B.fi thì không cần đổi vị trí (sắp xếp giảm dần)
*/
bool cmp(pair<int, int> A, pair<int, int> B) {
    return A.fi > B.fi;
}

struct Trie{
    struct Node{
        Node* child[26];
        int exist;
        vector<int> nxt;

        Node() {
            for (int i = 0; i < 26; i++) child[i] = NULL;
            exist = -1;
        }
    };

    int cur;
    Node* root;
    Trie(){
        root = new Node();
    };

    /*
        Hàm dùng cho cả T1 và T2
        Riêng với T2 thì p -> nxt chỉ có đúng 1 phần tử do các xâu là phân biệt
    */

    void add_string(int name, string s) {
        Node* p = root;
        for (auto f : s) {
            int c = f - 'a';
            if (p->child[c] == NULL) p->child[c] = new Node();
            p = p->child[c];
        }
        (p->nxt).pb(name);
        p->exist = name;
    }

    /*
        Hàm này chỉ dùng trong T2 với chức năng tìm tất cả các xâu thỏa mãn (như mô tả trong README.pdf)
        và thực hiện xây dựng đồ thị luôn
    */
    int find_string(int u, string s) {
        Node* p = root;
        for (auto f : s) {
            int c = f - 'a';
            if (p->child[c] == NULL) return false;
            p = p->child[c];
        }
        for(auto v : p -> nxt) G[u].pb(v);
        if(p -> exist == -1) return 0;
        return p -> exist;
    }

    /*
        Hàm này chỉ dùng trong T2 với mục đích tìm số thứ tự (ID) của xâu s. Do các xâu chỉ xuất hiện đúng 1 lần trong tập.
    */
    int name_string(string s) {
        Node* p = root;
        for (auto f : s) {
            int c = f - 'a';
            if (p->child[c] == NULL) return false;
            p = p->child[c];
        }
        if(p -> exist == -1) return 0;
        return p -> exist;
    }
};

void BFS(int u, int v) {
    memset(visit, 0, sizeof(visit));
    queue<int> qu;
    qu.push(u);
    visit[u] = 1;
    while(qu.size()) {
        int u = qu.front();
        qu.pop();
        for(auto v : G[u]) {
            if(visit[v]) continue;
            visit[v] = 1;
            qu.push(v);
            par[v] = u;
        }
    }
    if(visit[v] == 0) cout << "-2";
    else {
        // Tạo lại một đường đi ngắn nhất từ u đến v
        int tmp = v;
        string route = "";
        while(tmp != u) {
            route = str[tmp] + " " + route;
            tmp = par[tmp];
        }
        route = str[tmp] + " " + route;
        cout << route << '\n';
    }
}

int main() {
    for(int i = 1; i <= 5757; ++i) {
        cin >> str[i];
    }
    /*
        Thuật toán xây dựng đồ thị như mô tả trong README.pdf
        Trie T1 dùng để lưu trữ các xâu đã qua xử lý như trong mô tả thuật toán
        Bước 1:
    */
    Trie T1;
    for(int i = 1; i <= 5757; ++i) {
        memset(cntC, 0, sizeof(cntC));
        for(int j = 0; j < str[i].length(); ++j) cntC[str[i][j] - 'a'] ++;
        string str2 = "";
        // Sắp xếp
        for(int j = 0; j < 26; ++j) {
            while(cntC[j] > 0) {
                str2 += j + 'a';
                --cntC[j];
            }
        }
        for(int j = 0; j < str[i].length(); ++j) {
            string str3 = str2;
            str3.erase(j, 1);
            T1.add_string(i, str3);
        }
    }
    /*
        Bước 2:
    */
    for(int i = 1; i <= 5757; ++i) {
        memset(cntC, 0, sizeof(cntC));
        for(int j = 1; j < str[i].length(); ++j) cntC[str[i][j] - 'a'] ++;
        string str2 = "";
        for(int j = 0; j < 26; ++j) {
            while(cntC[j] > 0) {
                str2 += j + 'a';
                --cntC[j];
            }
        }
        T1.find_string(i, str2);
    }
    // Xây dựng danh sách kề của đồ thị ngược
    for(int i = 1; i <= 5757; ++i) {
        for(auto v : G[i]) Gr[v].pb(i);
    }

    // Thực hiện duyệt DFS để xây dựng cây DFS
    int N = 5757;
    memset(visit, 0, sizeof(visit));
    for(int i = 1; i <= N; ++i) {
        if(!visit[i]) DFS(i);
    }
    // Xây dựng mảng DS và sắp xếp theo DS giảm dần
    for(int i = 1; i <= N; ++i) {
        DS[i] = {post[i], i};
    }
    sort(DS + 1, DS + 1 + N, cmp);
    // Thực hiện duyệt DFS lần hai để tìm TPLT mạnh
    memset(visit2, 0, sizeof(visit2));
    for(int i = 1; i <= N; ++i) {
        int u = DS[i].se;
        if(visit2[u]) continue;
        ++TPLTM;
        DFS2(u);
    }
    cout << TPLTM << '\n';
    // Trie T2 dùng để lưu các xâu trong tập, với mục đích tìm số thứ tự của chúng
    struct Trie T2;
    for(int i = 1; i <= N; ++i) T2.add_string(i, str[i]);
    string S3;
    cin >> S3;
    int u = T2.name_string(S3);
    //Tìm các từ cùng thành phần liên thông với u
    if(u == 0) {
        cout << -1 << '\n';
    }
    else {
        int id = tplt_id[u];
        for(auto v : SCP[id]) cout << str[v] << ' ';
        cout << '\n';
    }
    // TÌm đường đi ngắn nhất từ S1 (thứ tự s) đến S2 (thứ tự e)
    string S1, S2;
    cin >> S1 >> S2;
    int s = T2.name_string(S1), e = T2.name_string(S2);
    if(s == 0 || e == 0) cout << -1 << '\n';
    else BFS(s, e);

    return 0;
}



