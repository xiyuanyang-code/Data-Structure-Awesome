/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-02-20 15:58:05
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-05 01:46:15
 * @FilePath: /Data_structure/Other_files/Maxsum_of_array.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */

/*
Algorithms: Longest Consecutive Subsequence Problem.
*/

#include <iostream>
#include <cmath>
#include <vector>

/*
Several Algorithms:
- force algorithms O(N^3) (Ignored)
- O(N^2)
- O(N logN): Divide and Conquer
- O(N): prefixsum
- O(N): Dynamic Programming
 */

long long maxSubsequence1(std::vector<int> arr, int& start, int& end){
    // O(N^2)
    long long max_sum = 0;
    int size = arr.size();

    for(int i = 0; i < size; i++){
        long long currentsum = 0;
        for(int j = i; j < size; j++){
            // permutate for the subsequence's begin and end
            currentsum += arr[j];
            if(currentsum > max_sum){
                // update the maxumize value
                max_sum = currentsum;
                start = i;
                end = j;
            }
        }
    }

    // Or you can just use the prefix sum!
    return max_sum;
}

// Using divide and conquer

long long maxSum(std::vector<int> arr, int left, int right, int& start, int& end){
    long long maxLeft = 0, maxRight = 0, center = 0;
    long long left_sum = 0;
    long long right_sum = 0;
    long long maxLeft_tmp = 0, maxRight_tmp = 0;
    int startL = 0, startR = 0, endL = 0, endR = 0;

    // where recursion ends...
    if(left == right){
        start = end = right;
        return std::max(arr[left], 0);
    }

    center = (left + right)/2;

    // the sequence from the beginning part
    maxLeft = maxSum(arr, left, center, startL, endL);

    // the sequence from the end part
    maxRight = maxSum(arr, center + 1, right, startR, endR);

    // finding the sequence in the middle
    start = center;
    for(int i = center; i >= left; --i){
        left_sum += arr[i];
        if(left_sum > maxLeft_tmp){
            maxLeft_tmp = left_sum;
            start = i;
        }
    }
    end = center + 1;
    for(int i = center + 1; i <= right; ++i){
        right_sum += arr[i];
        if(right_sum > maxRight_tmp){
            maxRight_tmp = right_sum;
            end = i;
        }
    }

    // finding the max of the three value
        // finding the max of the three values
        long long maxMiddle = maxLeft_tmp + maxRight_tmp; // Sum of the middle sequence
        if (maxLeft >= maxRight && maxLeft >= maxMiddle) {
            // The maximum subsequence is in the left part
            start = startL;
            end = endL;
            return maxLeft;
        } else if (maxRight >= maxLeft && maxRight >= maxMiddle) {
            // The maximum subsequence is in the right part
            start =startR;
            end = endR;
            return maxRight;
        } else {
            // The maximum subsequence is in the middle part
            return maxMiddle;
        }
}

int maxSubsequence2(std::vector<int> arr, int& start, int& end){
    return maxSum(arr, 0, arr.size() - 1, start, end);
}

/**
 * @brief Using sliding windows
 * 
 * @param arr 
 * @param start 
 * @param end 
 * @return int 
 */
int maxSubsequence3(std::vector<int> arr, int& start, int& end){
    long long maxSum, starttmp, currentSum;
    start = end = maxSum = starttmp = currentSum = 0;
    int size = arr.size();

    for(int i = 0; i < size; i++){
        currentSum += arr[i];
        if(currentSum < 0){
            // exceed the forward sequence
            currentSum = 0;
            starttmp = i + 1;
        }else if(currentSum > maxSum){
            maxSum = currentSum;
            start = starttmp;
            end = i;
        }
    }

    return maxSum;
}

/**
 * @brief Using the prefix sum: Just find the maximum value and the minimum value!
 * 
 * @param arr 
 * @param start 
 * @param end 
 * @return int 
 */
int maxSubsequence4(std::vector<int> arr, int& start, int& end) {
    int size = arr.size();
    long long maxSum = 0, prefixSum = 0, minPrefixSum = 0;
    int minPrefixIndex = -1; // Tracks the index of the minimum prefix sum

    start = end = 0;

    for (int i = 0; i < size; i++) {
        prefixSum += arr[i];

        // Check if the current subarray (from minPrefixIndex + 1 to i) has the maximum sum
        if (prefixSum - minPrefixSum > maxSum) {
            maxSum = prefixSum - minPrefixSum;
            start = minPrefixIndex + 1;
            end = i;
        }

        // Update the minimum prefix sum and its index
        if (prefixSum < minPrefixSum) {
            minPrefixSum = prefixSum;
            minPrefixIndex = i;
        }
    }

    return maxSum;
}

int main() {
    // Test array
    std::vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int start = 0, end = 0;

    // Test maxSubsequence1 (O(N^2))
    std::cout << "Testing maxSubsequence1 (O(N^2))..." << std::endl;
    long long result1 = maxSubsequence1(arr, start, end);
    std::cout << "Maximum Subsequence Sum: " << result1 << std::endl;
    std::cout << "Start Index: " << start << ", End Index: " << end << std::endl;

    // Test maxSubsequence2 (O(N log N) - Divide and Conquer)
    std::cout << "\nTesting maxSubsequence2 (O(N log N))..." << std::endl;
    start = end = 0; // Reset start and end
    long long result2 = maxSubsequence2(arr, start, end);
    std::cout << "Maximum Subsequence Sum: " << result2 << std::endl;
    std::cout << "Start Index: " << start << ", End Index: " << end << std::endl;

    // Test maxSubsequence3 (O(N) - Dynamic Programming)
    std::cout << "\nTesting maxSubsequence3 (O(N))..." << std::endl;
    start = end = 0; // Reset start and end
    long long result3 = maxSubsequence3(arr, start, end);
    std::cout << "Maximum Subsequence Sum: " << result3 << std::endl;
    std::cout << "Start Index: " << start << ", End Index: " << end << std::endl;

    // Test maxSubsequence4 (O(N) - Prefix Sum)
    std::cout << "\nTesting maxSubsequence4 (O(N))..." << std::endl;
    start = end = 0; // Reset start and end
    long long result4 = maxSubsequence4(arr, start, end);
    std::cout << "Maximum Subsequence Sum: " << result4 << std::endl;
    std::cout << "Start Index: " << start << ", End Index: " << end << std::endl;

    return 0;
}