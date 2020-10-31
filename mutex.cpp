#include <iostream>
#include <string>
#include <thread>
#include <mutex>

std::mutex mymutex;

void function1(int id){

    mymutex.lock();
    std::cout<<"thread # "<<id<<"\n";
    mymutex.unlock();
}

int main(){
    std::thread threads[10];

    for(int i=0; i<10; i++){
        threads[i] = std::thread(function1, i+1);
    }
    for(int i=0; i<10; i++){
        threads[i].join();
    }
    return 0;
}
