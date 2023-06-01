#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include <chrono>

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include <iostream>

using namespace std::chrono_literals;

auto start = std::chrono::high_resolution_clock::now();

void sig_handler(int signum){

    printf("Inside handler function\n");
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Waited " << elapsed << '\n';
}


int main(){

    signal(SIGALRM,sig_handler); // Register signal handler

    alarm(7);  // Scheduled alarm after 2 seconds
    start = std::chrono::high_resolution_clock::now();

    for(int i=1;;i++){

        printf("%d : Inside main function\n",i);
        sleep(1);  // Delay for 1 second
    }
    return 0;
}
