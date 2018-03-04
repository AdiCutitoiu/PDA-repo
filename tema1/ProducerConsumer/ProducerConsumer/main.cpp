#include <queue>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <functional>
#include <cassert>
#include <algorithm>
#include <stdio.h>

using namespace std;

template<typename T>
class Queue
{
public:
  template<typename U>
  void Push(U && aU)
  {
    unique_lock<mutex> pushLock;

    while (true)
    {
      unique_lock<mutex> checkLock(mMutex);

      if (!IsFull())
      {
        checkLock.swap(pushLock);
        break;
      }
    }

    mQueue.push(move(aU));

    printf("Pushed, size = %d\n", mQueue.size());
  }

  T Pop()
  {
    unique_lock<mutex> popLock;

    while (true)
    {
      unique_lock<mutex> checkLock(mMutex);

      if (!IsEmpty())
      {
        checkLock.swap(popLock);
        break;
      }
    }

    auto elem = move(mQueue.front());
    mQueue.pop();

    printf("Popped, size = %d\n", mQueue.size());

    return elem;
  }

private:
  mutex mMutex;
  queue<T> mQueue;

  bool IsEmpty()
  {
    return mQueue.empty();
  }

  bool IsFull()
  {
    return mQueue.size() == 20;
  }
};

template<typename T>
class Consumer
{
public:
  Consumer(Queue<T> & aQueue, int aID)
    : mQueue{ aQueue }
    , mID { aID }
  {
  }

  void operator()()
  {
    for (int i = 0; i < 100; i++)
    {
      auto popped = mQueue.Pop();

      Sleep(1000);

      printf("Consumer %d consumed an item\n", mID);
    }
  }
private:
  Queue<T> & mQueue;
  int mID;
};

template<typename T>
class Producer
{
public:
  Producer(Queue<T> & aQueue, int aID, function<T()> aGenerator)
    : mQueue{ aQueue }
    , mID { aID }
    , mGenerator{ move(aGenerator) }
  {
    assert(mGenerator);
  }

  void operator()()
  {
    for (int i = 0; i < 100; i++)
    {
      Sleep(1000);

      printf("Producer %d produced an item\n", mID);

      mQueue.Push(i);
    }
  }
private:
  Queue<T> & mQueue;
  int mID;
  function<T()> mGenerator;
};

int main()
{
  Queue<int> queue;

  vector<thread> threads;
  threads.emplace_back(Producer<int>(queue, 1, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 2, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 3, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 4, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, 5, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Consumer<int>(queue, 1));
  threads.emplace_back(Consumer<int>(queue, 2));
  threads.emplace_back(Consumer<int>(queue, 3));


  for (auto &thr : threads)
  {
    thr.join();
  }

  return 0;
}
