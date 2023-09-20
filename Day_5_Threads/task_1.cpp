#include <iostream>
#include <thread>
#include <chrono>

int main() {

    std::thread thread_1([](int x, int s) {
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(s * 1000));
        std::cout << x << std::endl;
    }, 1, 3);

    std::thread thread_2([](int x, std::thread t) {
        t.join();
        std::cout << x << std::endl;
    }, 2, std::move(thread_1));

    std::thread thread_3([](int x, std::thread t) {
        t.join();
        std::cout << x << std::endl;
    }, 3, std::move(thread_2));

    thread_3.join();

    return 0;
}