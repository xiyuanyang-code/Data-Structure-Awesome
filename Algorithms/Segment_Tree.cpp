#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

const int MAXN = 10000;
int arr[MAXN];
class segmentTree {
    struct TreeNode {
        int left, right;
        int lson, rson;
        int lazy_tag;
        long long data;

        TreeNode(int left_ = 0, int right_ = 0, int lson_ = 0, int rson_ = 0, long long data_ = 0) : left(left_), right(right_), lson(lson_), rson(rson_), data(data_) {}
    };

private:
    TreeNode seg_arr[4 * MAXN];

public:
    segmentTree() {
        // build the initial tree (all empty)
        std::memset(seg_arr, 0, sizeof(seg_arr));
    }

    /**
     * @brief Build the initial tree
     * 
     * @param number The index of the tree (array: seg_arr)
     * @param s the left index
     * @param t the right index
     */
    void build(int number, int s, int t) {
        // build the tree
        seg_arr[number].left = s;
        seg_arr[number].right = t;
        seg_arr[number].lazy_tag = 0;
        seg_arr[number].lson = number << 1;
        seg_arr[number].rson = seg_arr[number].lson + 1;


        if (s == t) {
            // finish the end, return
            seg_arr[number].data = arr[s];
            return;
        }

        // not finish, go deeper
        int mid = (s + t) / 2;
        build(seg_arr[number].lson, s, mid);
        build(seg_arr[number].rson, mid + 1, t);

        // update the value of the father node
        push_up(number);
    }

    /**
     * @brief update the value of the father node (Traceback the two son nodes)
     * 
     * @param node father node index
     */
    void push_up(int node) {
        auto &cur = seg_arr[node];
        cur.data = std::max(seg_arr[cur.lson].data,
                            seg_arr[cur.rson].data);
    }

    /**
     * @brief Ask fot the maximum value of [l, r]
     * 
     * @param node (begin with index 1), the seg_arr index
     * @param l the left index
     * @param r the right index
     * @return int 
     */
    int query(int node, int l, int r) {
        auto &cur = seg_arr[node];
        if (l <= cur.left && cur.right <= r) {
            // be totally covered, just return as the end of the recursion
            return cur.data;
        }

        // the son will be get, thus update the lazy tag
        push_down(node);

        auto mid = (cur.left + cur.right) >> 1;
        int res = std::numeric_limits<int>::min();
        if (l <= mid) {
            res = std::max(res, query(cur.lson, l, r));
        }
        if (r > mid) {
            res = std::max(res, query(cur.rson, l, r));
        }
        return res;
    }

    /**
     * @brief Update single value, be banned (it does not use lazy tag)
     * 
     * @param node 
     * @param pos 
     * @param val 
     */
    void updateSingle(int node, int pos, int val) {
        auto &cur = seg_arr[node];
        auto mid = (cur.left + cur.right) >> 1;
        if (cur.lson == cur.rson) {
            // find the leaf node, return!
            cur.data = val;
            return;
        }
        if (pos <= mid) {
            updateSingle(cur.lson, pos, val);
        }
        if (pos > mid) {
            updateSingle(cur.rson, pos, val);
        }
        push_up(node);
    }

    /**
     * @brief push down the lazy tag (for only one layer)
     * 
     * @param node 
     */
    void push_down(int node) {
        auto &cur = seg_arr[node];
        if (cur.left == cur.right) {
            // for the leaf node, do not push down
            return;
        }
        if (cur.lazy_tag != 0) {
            seg_arr[cur.lson].data += cur.lazy_tag;// for the maximum value
            seg_arr[cur.rson].data += cur.lazy_tag;
            seg_arr[cur.lson].lazy_tag += cur.lazy_tag;
            seg_arr[cur.rson].lazy_tag += cur.lazy_tag;
            cur.lazy_tag = 0;
        }
    }

    /**
     * @brief Add val for all in [l, r]
     * 
     * @param node begin from index 1
     * @param l 
     * @param r 
     * @param val 
     */
    void modify(int node, int l, int r, int val) {
        auto &cur = seg_arr[node];
        if (cur.left >= l && cur.right <= r) {
            cur.data += val;
            cur.lazy_tag += val;
            return;
        }

        // no return? It means it will search the child, push down before searching (update the lazy tag)
        push_down(node);
        auto mid = (cur.left + cur.right) >> 1;
        if (l <= mid) {
            modify(cur.lson, l, r, val);
        }
        if (r > mid) {
            modify(cur.rson, l, r, val);
        }
        push_up(node);
    }
};


int main() {
    const int n = 10;
    segmentTree segTree;
    for (int i = 1; i <= n; ++i) {
        arr[i] = i;
    }

    segTree.build(1, 1, n);


    std::cout << "Initial maximum in range [1, 5]: " << segTree.query(1, 1, 5) << std::endl;
    std::cout << "Initial maximum in range [1, 3]: " << segTree.query(1, 1, 3) << std::endl;


    segTree.modify(1, 2, 4, 10);

    std::cout << "After modifying range [2, 4] by +10:" << std::endl;
    std::cout << "Maximum in range [1, 5]: " << segTree.query(1, 1, 5) << std::endl;
    std::cout << "Maximum in range [3, 3]: " << segTree.query(1, 3, 3) << std::endl;

    segTree.modify(1, 3, 10, 22);
    std::cout << "After modifying range [3, 10] by +22:" << std::endl;
    std::cout << "Maximum in range [1, 5]: " << segTree.query(1, 1, 5) << std::endl;
    std::cout << "Maximum in range [3, 3]: " << segTree.query(1, 3, 3) << std::endl;
    std::cout << "Maximum in range [1, 10]: " << segTree.query(1, 1, 10) << std::endl;
    std::cout << "Maximum in range [6, 10]: " << segTree.query(1, 6, 10) << std::endl;

    segTree.modify(1, 1, 10, 5);
    std::cout << "After modifying range [1, 10] by +5:" << std::endl;
    std::cout << "Maximum in range [1, 10]: " << segTree.query(1, 1, 10) << std::endl;
    std::cout << "Maximum in range [6, 10]: " << segTree.query(1, 6, 10) << std::endl;

    return 0;
}