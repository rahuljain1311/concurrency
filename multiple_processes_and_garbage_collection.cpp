#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <semaphore>
using namespace std;



/*
Schedulder scheduler;

scheduler.schedule(task);

scheduler.blockUntilComplete();


*/

class Task {

  public:
  Task(){
  }
  
  run(){
  }
};

class Scheduler {
  
  queue<Task> q;
  mutex mtx;
  condition_variable cv;
  condition_variable taskComplete;
  counting_semaphore runningsTasks;
  
  vector<thread>threads;
  
  void work(){
    
    while(true){
      
      unique_lock<decltype(mutex)> lock(mtx);
      if(q.size() > 0){
        Task t = q.front();
        q.pop();
        runningsTasks++;
        lock.release();
        t.run();
        
        lock.lock();
        runningsTasks--;
        
        if(runningsTasks == 0)
          taskComplete.notify_all(lock);
        lock.release();
      }
      else {
        cv.wait(lock);
      }
    }
  }
  
public:
  Scheduler(){
    const int numThreads = 10;
    runningsTasks = 0;
    threads.resize(numThreads);
    
    for(int i=0; i<numThreads; i++){
      threads[i] = thread(work);
    }
  }
  
  void schedule(Task t){
    unique_lock<decltype(mutex)> lock(mtx);
    q.push(t);
    cv.notify_one(lock);
  }
  
  void blockUntilComplete(){
    unique_lock<decltype(mutex)> lock(mtx);
    
    while(!q.empty() || runningsTasks > 0){
      taskComplete.wait(lock);
    }
  }
};

// To execute C++, please define "int main()"
int main() {
  
  return 0;
}

