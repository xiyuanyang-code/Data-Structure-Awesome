/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-03 15:42:00
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 13:20:00
 * @FilePath: /Data_structure/Specific_USage/Simulator_pq.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

class simulator {
private:
    std::size_t server_num;
    std::size_t custom_num;
    std::size_t arrival_low;
    std::size_t arrival_high;
    std::size_t service_time_high;
    std::size_t service_time_low;

    struct event {
        size_t time;
        int type;// 0 for arriving, 1 for leaving
        bool operator<(const event &other) const {
            return time > other.time;// 最小堆
        }
    };

public:
    simulator() {
        std::cout << "Enter the number of servers: ";
        std::cin >> server_num;

        std::cout << "Enter the range for arrival time intervals (low high): ";
        std::cin >> arrival_low >> arrival_high;

        std::cout << "Enter the range for service times (low high): ";
        std::cin >> service_time_low >> service_time_high;

        std::cout << "Enter the number of customers to simulate: ";
        std::cin >> custom_num;
    }

    double avgWaitTime() {
        std::size_t server_busy = 0;
        std::size_t total_wait_time = 0;

        std::priority_queue<event> eventQueue;
        std::queue<event> waitQueue;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> arrival_dist(arrival_low, arrival_high);
        std::uniform_int_distribution<size_t> service_dist(service_time_low, service_time_high);

        // Initialize event queue
        std::vector<size_t> arrival_times(custom_num);
        for (size_t i = 0; i < custom_num; ++i) {
            if (i == 0) {
                arrival_times[i] = arrival_dist(rng);
            } else {
                arrival_times[i] = arrival_times[i - 1] + arrival_dist(rng);
            }
            event arrival_event{arrival_times[i], 0};
            eventQueue.push(arrival_event);
        }

        // Simulation
        while (!eventQueue.empty()) {
            event current_event = eventQueue.top();
            eventQueue.pop();

            if (current_event.type == 0) {// Arrival
                if (server_busy < server_num) {
                    ++server_busy;
                    event departure_event{current_event.time + service_dist(rng), 1};
                    eventQueue.push(departure_event);
                } else {
                    waitQueue.push(current_event);
                }
            } else {// Departure
                if (!waitQueue.empty()) {
                    event next_event = waitQueue.front();
                    waitQueue.pop();
                    total_wait_time += (current_event.time - next_event.time);
                    event departure_event{current_event.time + service_dist(rng), 1};
                    eventQueue.push(departure_event);
                } else {
                    --server_busy;
                }
            }
        }

        return static_cast<double>(total_wait_time) / custom_num;
    }
};

int main() {
    simulator sim;
    std::cout << "Average Wait Time: " << sim.avgWaitTime() << std::endl;
    return 0;
}