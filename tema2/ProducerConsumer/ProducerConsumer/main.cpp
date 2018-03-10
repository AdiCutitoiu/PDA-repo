#include <queue>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <functional>
#include <cassert>
#include <algorithm>
#include <stdio.h>

using namespace std;

class Semaphore
{
public:
  Semaphore(size_t aInitialCount = 0)
    : mCount { aInitialCount }
  {
  }

  void Up()
  {
    unique_lock<mutex> lock(mMutex);

    mCount++;

    mCondVar.notify_one();
  }

  void Down()
  {
    unique_lock<mutex> lock(mMutex);

    while (mCount == 0)
    {
      mCondVar.wait(lock);
    }

    mCount--;
  }

private:
  size_t             mCount;
  mutex              mMutex;
  condition_variable mCondVar;
};

template<typename T>
class Queue
{
public:
  template<typename U>
  void Push(U && aU)
  {
    unique_lock<mutex> pushLock(mMutex);

    mQueue.push(move(aU));

    printf("Pushed, size = %u\n", mQueue.size());
  }

  T Pop()
  {
    unique_lock<mutex> popLock(mMutex);

    auto elem = move(mQueue.front());
    mQueue.pop();

    printf("Popped, size = %u\n", mQueue.size());

    return elem;
  }

private:
  mutex mMutex;
  queue<T> mQueue;
};

constexpr int kIterations = 100;

template<typename T>
class Consumer
{
public:
  Consumer(Queue<T> & aQueue, int aID, Semaphore & aProdSem, Semaphore & aConSem)
    : mQueue{ aQueue }
    , mID { aID }
    , mProdSem { aProdSem }
    , mConSem { aConSem }
  {
  }

  void operator()()
  {
    for (int i = 0; i < kIterations; i++)
    {
      mConSem.Down();
      auto popped = mQueue.Pop();
      mProdSem.Up();

      Sleep(1000);

      printf("Consumer %d consumed an item\n", mID);
    }
  }
private:
  Queue<T> & mQueue;
  int mID;
  Semaphore & mProdSem;
  Semaphore & mConSem;
};

template<typename T>
class Producer
{
public:
  Producer(Queue<T> & aQueue, int aID, Semaphore & aProdSem, Semaphore & aConSem, function<T()> aGenerator)
    : mQueue{ aQueue }
    , mID { aID }
    , mProdSem { aProdSem }
    , mConSem { aConSem }
    , mGenerator{ move(aGenerator) }
  {
    assert(mGenerator);
  }

  void operator()()
  {
    for (int i = 0; i < kIterations; i++)
    {
      Sleep(1000);

      printf("Producer %d produced an item\n", mID);

      mProdSem.Down();
      mQueue.Push(i);
      mConSem.Up();
    }
  }
private:
  Queue<T> & mQueue;
  int mID;
  Semaphore & mProdSem;
  Semaphore & mConSem;
  function<T()> mGenerator;
};

int main()
{
  Queue<int> queue;

  Semaphore prodSem(20);
  Semaphore conSem(0);

  vector<thread> threads;
  threads.emplace_back(Producer<int>(queue, 1, prodSem, conSem, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 2, prodSem, conSem, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 3, prodSem, conSem, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 4, prodSem, conSem, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 5, prodSem, conSem, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Consumer<int>(queue, 1, prodSem, conSem));
  threads.emplace_back(Consumer<int>(queue, 2, prodSem, conSem));
  threads.emplace_back(Consumer<int>(queue, 3, prodSem, conSem));


  for (auto &thr : threads)
  {
    thr.join();
  }

  return 0;
}
