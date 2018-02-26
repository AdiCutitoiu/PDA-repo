#include <iostream>
#include <queue>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <functional>
#include <cassert>
#include <algorithm>

using namespace std;

template<typename T>
class Queue
{
public:
  template<typename U>
  void Push(U && aU)
  {
    lock_guard<mutex> lg(mMutex);

    mQueue.push(aU);
  }

  T Pop()
  {
    lock_guard<mutex> lg(mMutex);

    auto elem = move(mQueue.front());
    mQueue.pop();

    return elem;
  }

  bool IsEmpty()
  {
    return mQueue.empty();
  }

  bool IsFull()
  {
    return !IsEmpty();
  }

private:
  mutex mMutex;
  queue<T> mQueue;
};

template<typename T>
class Consumer
{
public:
  Consumer(Queue<T> & aQueue)
    : mQueue{ aQueue }
  {
  }

  void operator()()
  {
    for (int i = 0; i < 100; i++)
    {
      while (mQueue.IsEmpty())
      {
      }

      auto popped = mQueue.Pop();

      Sleep(500);

      cout << popped << "\n";
    }
  }
private:
  Queue<T> & mQueue;
};

template<typename T>
class Producer
{
public:
  Producer(Queue<T> & aQueue, function<T()> aGenerator)
    : mQueue{ aQueue }
    , mGenerator{ move(aGenerator) }
  {
    assert(mGenerator);
  }

  void operator()()
  {
    for (int i = 0; i < 100; i++)
    {
      Sleep(1000);

      while (mQueue.IsFull())
      {
      }

      mQueue.Push(i);
    }
  }
private:
  Queue<T> & mQueue;
  function<T()> mGenerator;
};

int main()
{
  Queue<int> queue;
  Producer<int> producer(queue, [n = 0]() mutable { return n++; });
  Consumer<int> consumer(queue);

  thread producerThread(producer);
  thread consumerThread(consumer);

  producerThread.join();
  consumerThread.join();

  return 0;
}
