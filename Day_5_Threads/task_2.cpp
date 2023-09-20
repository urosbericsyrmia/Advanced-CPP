#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Semaphore {
    int m_s = 0;
    std::mutex m_mut;
    std::condition_variable m_cv;

public:
    Semaphore() = default;

    Semaphore(int x) : m_s(x) {}

    void signal() {
        std::unique_lock<std::mutex> lock(m_mut);
        m_s += 1;
        m_cv.notify_one();
    }
    
    void wait() {
        std::unique_lock<std::mutex> lock(m_mut);
        m_cv.wait(lock, [this](){ return m_s != 0; });
        m_s -= 1;
    }
};

int main() {
    int var_1 = 0;
    int var_2 = 0;

    std::mutex mutex_1;
    std::mutex mutex_2;

    Semaphore sem_read{1};
    Semaphore sem_write{0};

    std::thread thread_one([&var_1, &var_2, &mutex_1, &mutex_2, &sem_read, &sem_write]() {
        int val = 1;

        while (true) {
            sem_read.wait();

            std::lock(mutex_1, mutex_2);

            var_1 = val;
            var_2 = val + 1;

            mutex_1.unlock();
            mutex_2.unlock();

            sem_write.signal();

            val = (val + 2) % 6;
        }
    });

    std::thread thread_two([&var_1, &var_2, &mutex_1, &mutex_2, &sem_read, &sem_write]() {
        while (true) {
            sem_write.wait();

            std::lock(mutex_1, mutex_2);

            std::cout << var_1 + var_2 << std::endl;

            mutex_1.unlock();
            mutex_2.unlock();

            sem_read.signal();
        }
    });

    thread_one.join();
    thread_two.join();

    return 0;
}