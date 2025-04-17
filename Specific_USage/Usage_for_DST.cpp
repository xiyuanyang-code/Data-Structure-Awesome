/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-17 14:52:33
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-17 16:27:11
 * @FilePath: /Data_structure/Specific_USage/Usage_for_DST.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>

int generate_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);
    return dis(gen);
}

std::string generate_random_string(size_t length = 20) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[dis(gen)];
    }
    return random_string;
}

void test_set(int size = 1000) {
    std::set<int> s;
    std::set<int>::iterator p;
    int element, target;
    long long count = 0;
    std::cout << "Test for STL: set BEGIN \n\n\n";

    // insert elements in the set
    for (int i = 0; i < size; i++) {
        element = generate_random();
        s.insert(element);
        std::cout << element << " is inserted in the set." << std::endl;
    }

    // find elements in the set
    for (int i = 0; i < size * 10; i++) {
        target = generate_random();
        if (s.find(target) != s.end()) {
            // find the element
            std::cout << target << " is found in the set." << std::endl;
            count++;
        } else {
            std::cout << target << " is not found in the set." << std::endl;
        }
    }
    std::cout << count << " elements is found in the set." << std::endl;

    // delete elements in the set
    for (auto iter = s.begin(); iter != s.end();) {
        std::cout << *iter << " is in the set." << std::endl;
        iter = s.erase(iter);
        if (s.empty()) {
            std::cout << "The set is empty now!" << std::endl;
        }
    }

    std::cout << "Test for STL: set END \n\n\n";
}

void test_map(int size = 1000) {
    std::cout << "Test for STL: map BEGIN \n\n\n";
    std::map<int, std::string> s;
    int element;
    std::string element_string;
    long long count = 0;

    // insert element in s
    for (int i = 0; i < size; i++) {
        element = generate_random();
        element_string = generate_random_string();
        s.insert(std::pair<int, std::string>{element, element_string});
        std::cout << element_string << " with the value " << element << " is inserted in the map." << std::endl;
    }

    // find elements in s
    for (int i = 0; i < 10 * size; i++) {
        element = generate_random();
        auto iter = s.find(element);
        if (iter != s.end()) {
            // find the element!
            std::string target_string = iter->second;
            std::cout << target_string << " is found with the key value of " << element << std::endl;
            count++;
        } else {
            std::cout << "Key value " << element << " is not found in this map." << std::endl;
        }
    }
    std::cout << count << " elements are found in the map." << std::endl;

    // delete element in the map
    for (auto iter = s.begin(); iter != s.end();) {
        std::cout << iter->second << " with the string value " << iter->first << " is found in the map." << std::endl;
        iter = s.erase(iter);
        if (s.empty()) {
            std::cout << "The map is empty now!" << std::endl;
        }
    }

    std::cout << "Test for STL: map END \n\n\n";
}

int main() {
    // test for stl dynamic search
    test_set();
    test_map();
    return 0;
}