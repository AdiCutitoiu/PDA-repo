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

    cout << "Pushed\n";
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

    cout << "Popped\n";

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
    return mQueue.size() == 5;
  }
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
      auto popped = mQueue.Pop();

      Sleep(2000);

      cout << "Consumed\n";
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
      Sleep(4000);

      cout << "Produced\n";

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

  vector<thread> threads;
  threads.emplace_back(Producer<int>(queue, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Producer<int>(queue, [n = 0]() mutable { return n++; }));
  threads.emplace_back(Consumer<int>(queue));
  threads.emplace_back(Consumer<int>(queue));
  threads.emplace_back(Consumer<int>(queue));
  threads.emplace_back(Consumer<int>(queue));
  threads.emplace_back(Consumer<int>(queue));
  threads.emplace_back(Consumer<int>(queue));


  for (auto &thr : threads)
  {
    thr.join();
  }

  return 0;
}
