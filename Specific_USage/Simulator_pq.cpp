/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-03 15:42:00
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-03 15:43:13
 * @FilePath: /Data_structure/Specific_USage/Simulator_pq.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <cstddef>
class simulator{
    private:
        std::size_t server_num;
        std::size_t custom_num;
        std::size_t arrival_low;
        std::size_t arrival_high;
        std::size_t service_time_high;
        std::size_t service_time_low;
};