#include <cstring>
#include <iostream>

const int MAXM = 100;
const int MAXN = 100;

inline int lowbit(int x){
    return x & (-x);
}

struct fenwick_1
//
{
    int N;
    long long C[MAXN];

    void init(int n){
        N = n;
        memset(C, 0, sizeof(C));
    }

    void addSingle(int pos, long long val) {
        for(int i = pos; i <= N; i += lowbit(i)){
            C[i] += val;
        }
    }

    long long rangeAsk(int pos) {
        long long ans = 0;
        for(int i = pos; i > 0; i -= lowbit(i)){
            ans += C[i];
        }
        return ans;
    }

    long long rangeAsk2(int l, int r) {
        return rangeAsk(r) - rangeAsk(l - 1);
    }
};

struct fenwick_2
{
    int N;
    long long C1[MAXN];
    long long C2[MAXN];

    void init(int n){
        N = n;
        memset(C1, 0, sizeof(C1));
        memset(C2, 0, sizeof(C2));
    }

    void addRange1(int pos, long long val){
        //对于差分数组的单点修改操作
        for(int i = pos; i <= N; i += lowbit(i)){
            C1[i] += val;
            C2[i] += val * pos;
        }
    }

    void addRange2(int posl, int posr, long long val){
        //对差分数组使用两次单点修改操作，就可以得到区间修改
        addRange1(posl, val);
        addRange1(posr + 1, -val);
    }

    long long askRange1(int pos){
        //从1到pos的区间求和
        long long ans = 0;
        for(int i = pos; i > 0; i -= lowbit(i)) {
            ans += (pos + 1)*C1[i] - C2[i];
        }
        return ans;
    }

    long long askRange2(int l, int r){
        return askRange1(r) - askRange1(l - 1);
    }

    //退化为单点的修改和查询
    void Addsingle(int pos, long long val){
        addRange2(pos, pos, val);
    }

    long long Asksingle(int pos){
        return askRange2(pos, pos);
    }

};



/**
 * @brief only for two operations: modify a single point and sum a submatrix
 * 
 */
struct fenwick_2d_1{
    int m,n;//size
    long long C[MAXM][MAXN];//fenwick tree

    fenwick_2d_1(int m_, int n_){
        m = m_;
        n = n_;
        memset(C, 0, sizeof(C));
    }
    
    /**
     * @brief Modify a single element
     * 
     * @param x x-index (1-based)
     * @param y y-index (1-based)
     * @param val plus val
     */
    void add(int x, int y, long long val){
        for(int i = x; i <= m; i += lowbit(i)){
            for(int j = y; j < n; j += lowbit(j)){
                C[i][j] += val;
            }
        }
    }

    //ask for the submatrix (1,1) -> (x,y)
    long long ask(int x, int y){
        long long ans = 0;
        for(int i = x; i > 0; i -= lowbit(i)){
            for(int j = y; j > 0; j -= lowbit(j)){
                ans += C[i][j];
            }
        }

        return ans;
    }

    //(p,q) -> (x,y)
    long long ask_sub(int p, int q, int x, int y){
        return ask(x, y)-ask(p - 1, y)-ask(q - 1, y)+ask(p-1, q-1);
    }
};

struct fenwick_2d_2
{
    int m, n;
    long long C[MAXM][MAXN];//此处维护差分数组
    void init(int m_, int n_){
        m = m_;
        n = n_;
        memset(C, 0, sizeof(C));
    }

    /**
     * @brief submatrix(1,1) -> (x,y) all add val
     * 
     * @param x 
     * @param y 
     * @param val 
     * 这里的代码和上一个代码的add完全相同，但是意义不一样，因为这里的C维护的是差分数组
     */
    void add(int x, int y, long long val){
        for(int i = x; i <= m; i += lowbit(i)){
            for(int j = y; j <= n; j += lowbit(j)){
                C[i][j] += val;
            }
        }
    }

    /**
     * @brief submatrix(x1, y1) -> (x2, y2) all adds x
     * 
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     * @param x 
     */
    void range_add(int x1,int y1,int x2,int y2,long long x) {
        add(x1,y1,x);
        add(x1,y2+1,-x);
        add(x2+1,y1,-x);
        add(x2+1,y2+1,x);
    }

    /**
     * @brief return the value of index (x,y)
     * 
     * @param x 
     * @param y 
     * @return long long 
     */
    long long ask(int x,int y)
    {
        long long ret=0;
        for(int i = x; i > 0; i -= lowbit(i)){
            for(int j = y; j > 0;j -= lowbit(j)){
                ret += C[i][j];
            }
        }
            
        return ret;
    }
};


struct fenwick_2d_3
{
    int m, n;
    long long C1[MAXM][MAXN];
    long long C2[MAXM][MAXN];
    long long C3[MAXM][MAXN];
    long long C4[MAXM][MAXN];

    void init(int m_,int n_)
    {
        m = m_;
        n = n_;
        memset(C1,0,sizeof(C1));
        memset(C2,0,sizeof(C2));
        memset(C3,0,sizeof(C3));
        memset(C4,0,sizeof(C4));
    }

    //区间修改
    /**
     * @brief Add val for single element (x,y)
     * 
     * @param x 
     * @param y 
     * @param val 
     */
    void add(int x, int y, long long val) {
        for(int i = x; i <= m; i += lowbit(i))
        {
            for(int j = y; j <= n; j += lowbit(j))
            {
                C1[i][j] += val;
                C2[i][j] += val * x;
                C3[i][j] += val * y;
                C4[i][j] += val * x * y;
            }
        }
    }

    /**
     * @brief Add val for all elements in the submatrix 
     * 
     * @param x 
     * @param y 
     * @param val 
     */
    void add_submatrix(int x1, int y1, int x2, int y2, long long val) {
        add(x1,y1,val);
        add(x1,y2+1,-val);
        add(x2+1,y1,-val);
        add(x2+1,y2+1,val);
    }

    //区间查询
    /**
     * @brief the sum of the submatrix (1,1) -> (x,y)
     * 
     * @param x 
     * @param y 
     * @return long long 
     */
    long long rangeAsk1(int x, int y){
        long long ans = 0;
        for(int i = x; i > 0; i -= lowbit(i)){
            for(int j = y; j > 0; j -= lowbit(j)){
                ans += (x + 1)*(y + 1)*C1[i][j];
                ans -= (y + 1)*C2[i][j] + (x + 1)*C3[i][j];
                ans += C4[i][j];
            }
        }
        return ans;
    }

    long long rangeAsk2(int x1, int y1, int x2, int y2){
        return rangeAsk1(x2,y2) - rangeAsk1(x1-1,y2) - rangeAsk1(x2,y1-1) + rangeAsk1(x1-1,y1-1);
    }

    //单点查询
    long long ask(int x,int y)
    {
        return rangeAsk2(x, y, x, y);
    }

    //单点修改的退化情况
    void modifySingle(int x, int y, long long val){
        add_submatrix(x, y, x, y, val);
    }
};




/*int main for testing only*/
int main() {
    {
    // 初始化
    fenwick_1 fw1;
    fenwick_2 fw2;
    int N = 10; // 数组大小
    fw1.init(N);
    fw2.init(N);

    // 测试 fenwick_1
    std::cout << "Testing fenwick_1:\n";
    fw1.addSingle(3, 5); // 在位置 3 增加 5
    fw1.addSingle(6, 7); // 在位置 6 增加 7
    std::cout << "Range sum [1, 5]: " << fw1.rangeAsk2(1, 5) << "\n"; // 查询 [1, 5] 的区间和
    std::cout << "Range sum [4, 8]: " << fw1.rangeAsk2(4, 8) << "\n"; // 查询 [4, 8] 的区间和

    // 测试 fenwick_2
    std::cout << "\nTesting fenwick_2:\n";
    fw2.addRange2(2, 5, 3); // 在区间 [2, 5] 增加 3
    fw2.addRange2(1, 7, 2); // 在区间 [4, 7] 增加 2
    std::cout << "Range sum [1, 6]: " << fw2.askRange2(1, 6) << "\n"; // 查询 [1, 6] 的区间和
    std::cout << "Range sum [3, 8]: " << fw2.askRange2(3, 8) << "\n"; // 查询 [3, 8] 的区间和

    // 测试单点查询
    std::cout << "Single point query at position 4: " << fw2.Asksingle(3) << "\n"; // 查询位置 4 的值
    }

    {
    // 初始化二维树状数组
    int M = 10, N = 10; // 矩阵大小为 10x10
    fenwick_2d_1 fw1(M, N);
    fenwick_2d_2 fw2;
    fenwick_2d_3 fw3;
    fw2.init(M, N);
    fw3.init(M, N);

    // 测试 fenwick_2d_1
    std::cout << "Testing fenwick_2d_1:\n";
    fw1.add(3, 4, 5); // 在位置 (3, 4) 增加 5
    fw1.add(6, 7, 3); // 在位置 (6, 7) 增加 3
    std::cout << "Submatrix sum (1,1) -> (5,5): " << fw1.ask_sub(1, 1, 5, 5) << "\n"; // 查询子矩阵 (1,1) -> (5,5)
    std::cout << "Submatrix sum (4,4) -> (8,8): " << fw1.ask_sub(4, 4, 8, 8) << "\n"; // 查询子矩阵 (4,4) -> (8,8)

    // 测试 fenwick_2d_2
    std::cout << "\nTesting fenwick_2d_2:\n";
    fw2.range_add(2, 2, 6, 6, 3); // 在子矩阵 (2,2) -> (5,5) 增加 3
    fw2.range_add(1, 1, 7, 7, 2); // 在子矩阵 (4,4) -> (7,7) 增加 2
    std::cout << "Value at (3,3): " << fw2.ask(3, 3) << "\n"; // 查询单点 (3,3)
    std::cout << "Value at (6,6): " << fw2.ask(6, 6) << "\n"; // 查询单点 (6,6)

    // 测试 fenwick_2d_3
    std::cout << "\nTesting fenwick_2d_3:\n";
    fw3.add_submatrix(1, 1, 4, 4, 5); // 在子矩阵 (1,1) -> (4,4) 增加 5
    fw3.add_submatrix(3, 3, 6, 6, 2); // 在子矩阵 (3,3) -> (6,6) 增加 2
    std::cout << "Submatrix sum (1,1) -> (5,5): " << fw3.rangeAsk2(1, 1, 5, 5) << "\n"; // 查询子矩阵 (1,1) -> (5,5)
    std::cout << "Submatrix sum (2,2) -> (7,7): " << fw3.rangeAsk2(2, 2, 7, 7) << "\n"; // 查询子矩阵 (2,2) -> (7,7)
    std::cout << "Value at (2,3): " << fw3.ask(2, 3) << "\n"; // 查询单点 (4,4)
    }

    return 0;
} 