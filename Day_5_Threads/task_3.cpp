#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

constexpr int RING_SIZE = 10;

class Semaphore {
private:
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
        m_cv.wait(lock, [this]() { return m_s != 0; });
        m_s -= 1;
    }
};

int main() {

    Semaphore sem_input(RING_SIZE);
    Semaphore sem_transform(0);
    Semaphore sem_output(0);

    char input[RING_SIZE];
    char output[RING_SIZE];

    std::thread thread_input([&input, &sem_input, &sem_transform]() {
        char c;
        int idx = 0;

        while (true) {
            std::cin >> c;

            sem_input.wait();

            input[idx] = c;
            idx = (idx + 1) % RING_SIZE;
            
            sem_transform.signal();
        }
    });

    std::thread thread_transform([&input, &output, &sem_transform, &sem_output]() {
        int idx = 0;

        while (true) {
            sem_transform.wait();

            output[idx] = toupper(input[idx]);
            idx = (idx + 1) % RING_SIZE;

            sem_output.signal();
        }
    });
    
    std::thread thread_output([&output, &sem_output, &sem_input]() {
        int idx = 0;

        while (true) {
            sem_output.wait();

            std::cout << output[idx];
            idx = (idx + 1) % RING_SIZE;

            sem_input.signal();
        }
    });

    thread_input.join();
    thread_transform.join();
    thread_output.join();

    return 0;

}