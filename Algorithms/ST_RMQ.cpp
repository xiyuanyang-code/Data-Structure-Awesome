/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-01 21:20:31
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-01 21:36:17
 * @FilePath: /Data_structure/Other_files/ST_RMQ.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <iostream>
#include <cmath>
using namespace std;

const int MAXN = 1e6 + 5; // 输入数组的最大长度
const int LOG = 20;       // log2(MAXN) 的上界

int st[MAXN][LOG]; // ST表，st[i][j]表示从i开始长度为2^j的区间的最小值
int arr[MAXN];     // 输入数组
int n;             // 数组长度

// 预处理函数
void preprocess() {
    // 初始化长度为 2^0 的区间
    for (int i = 0; i < n; ++i) {
        st[i][0] = arr[i];
    }

    // 动态规划填充表格
    for (int j = 1; j < LOG; ++j) { // 区间长度从 2^1 到 2^(LOG-1)
        for (int i = 0; i + (1 << j) <= n; ++i) { // 确保区间不越界
            st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }
}

// 查询 [L, R] 区间的最小值
int query(int L, int R) {
    int len = R - L + 1; // 查询区间的长度
    int k = log2(len);   // 找到最大的 k 满足 2^k <= len
    return min(st[L][k], st[R - (1 << k) + 1][k]);
}

int main() {
    // 输入数组长度和元素
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    // 预处理 ST 表
    preprocess();

    // 查询操作
    int q; // 查询次数
    cin >> q;
    while (q--) {
        int L, R;
        cin >> L >> R; // 查询区间 [L, R]
        cout << query(L, R) << endl; // 输出区间最小值
    }

    return 0;
}