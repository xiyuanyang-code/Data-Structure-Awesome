/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-17 17:30:47
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-17 19:29:17
 * @FilePath: /Data_structure/Specific_USage/Bucket_usage.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <iostream>
#include <random>
#include <cstring>

const int MAXSIZE = 100001;
int arr[MAXSIZE];

int generate_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);
    return dis(gen);
}

int main(){
    // algorithms: given an array, for each query, reach O(1) time complexity
    int arr_length;
    int random_input = 0;
    std::cin >> arr_length;
    std::memset(arr, 0, sizeof(arr));

    for(int i = 0; i < arr_length; i++){
        random_input = generate_random();
        arr[random_input] ++;
    }

    // for the query part
    int query_num = 0;
    int query_value = 0;
    int not_found = 0;
    std::cin >> query_num;
    for(int i = 0; i < query_num; i++){
        query_value = generate_random();
        if(arr[query_value] >= 1){
            // the value is found
            std::cout << query_value << " found." << std::endl;
        }else{
            // std::cout <<  query_value << " not found" << std::endl;
            not_found ++;
        }
    }
    std::cout << "There are " << not_found << " elements that are not found." << std::endl;

    return 0;
}