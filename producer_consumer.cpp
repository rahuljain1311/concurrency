#include<mutex>
#include<iostream>
#include<thread>
#include<condition_variable>
#include<queue>
std::mutex mtx;
std::condition_variable cv;
std::queue<int>q;
std::condition_variable print;

void consume(){
    
    while(true){
        std::unique_lock<std::mutex> lock(mtx);
        while(q.empty()){
            cv.wait(lock);
        }
        int value = q.front();
        q.pop();
        cv.notify_all();
        lock.unlock();
        
        std::unique_lock<std::mutex> printLock(mtx);
        std::cout<<"Consume "<<value*value<<"\n";
        printLock.unlock();
    }
}

void produce(int range){
    
    std::unique_lock<std::mutex> lock(mtx);
    for(int i=0; i<range; i++){
        q.push(i+1);
    }
    cv.notify_all();
}

int main(){
    std::thread consumers[10];
    
    for(int i=0; i<10; i++)
        consumers[i] = std::thread(consume);

    std::thread producer(produce, 10);
    producer.join();
    for(int i=0; i<10; i++)
        consumers[i].join();
    return 0;
}
