/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-02-20 15:58:05
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-02-20 23:27:24
 * @FilePath: /Data_structure/Other_files/Maxsum_of_array.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
/*
Problem: Given an array and try to find the maxsum of the sub-sequential of this array.
*/

#include <iostream>
#include <vector>
//Definition

double force_enumination(std::vector<double> array, int& index_left, int& index_right){
    double sum = 0, maxsum = 0;
    int current_left = 0, current_right = 0;
    int array_size = array.size();
    for (int i = 0; i < array_size; i++)
    {
        //i represents the starting index of array
        for(int j = i; j < array_size; j++){
            //j represents the ending index of array
            for(int k = i; k <= j; k++){
                //sum for each sub-array
                sum += array[k];
            }
            if(sum > maxsum){
                maxsum = sum;
                current_left = i;
                current_right = j;
            }
            sum = 0;
        }
    }
    index_left = current_left;
    index_right = current_right;
    return maxsum;
}

double Kadane(std::vector<double> array, int& index_left, int& index_right){
    double current_max = 0;
    double maxEndingHere;

    
    
}



//The main function is used for testing and debugging only.
int main(){
    std::vector<double> testArray = {
        2.5, -1.2, 3.1, 4.0, -2.5, 
        6.7, -3.3, 1.2, 5.5, -0.4, 
        2.1, -1.0, 7.8, -2.1, 4.4, 
        -3.5, 2.6, 3.3, -1.8, 0.0
    };
    int index_l = 0, index_r = 0;
    std::cout << force_enumination(testArray, index_l, index_r) << index_l << index_r << std::endl;
    
    return 0;
}