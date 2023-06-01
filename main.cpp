#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <csignal>

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

std::atomic<bool> stopFlag(false);
std::mutex mutex;
std::condition_variable cv;

void alarmHandler(int) {
    std::unique_lock<std::mutex> lock(mutex);
    stopFlag.store(true);
    cv.notify_all();
}

int countIterations(int numThreads, int testTime) {
    std::atomic<int> totalIterations(0);
    std::vector<std::thread> threads;

    std::signal(SIGALRM, alarmHandler);

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&totalIterations]() {
                int count = 0;
                for (int j = 2; ; j++) {
                if (isPrime(j)) {
                count++;
                }
                if (stopFlag.load()) {
                break;
                }
                }

                totalIterations += count;
                });
    }

    for (auto& thread : threads) {
        thread.join();
    }
    alarm((unsigned int)testTime); // Установка таймера на 5 секунд

    return totalIterations.load();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Неверное количество аргументов.\n\
            Использование: ./main <количество_потоков> <кол-во секунд>\n";
        return 1;
    }

    int numThreads = std::stoi(argv[1]);
    int testTime = std::stoi(argv[2]);

    int totalIterations = countIterations(numThreads, testTime);

    std::cout << "Общее количество выполненных итераций: " << totalIterations << std::endl;

    return 0;
}

