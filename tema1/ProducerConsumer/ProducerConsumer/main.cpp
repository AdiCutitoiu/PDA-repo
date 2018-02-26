#include <iostream>
#include <queue>
#include <thread>
#include <Windows.h>
#include <mutex>

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

    auto elem = move(mQueue.top());
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

      cout << popped;
    }
  }
private:
  Queue<T> & mQueue;
};

template<typename T>
class Producer
{
public:
  Producer(Queue<T> & aQueue)
    : mQueue{ aQueue }
  {
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
};

int main()
{

  return 0;
}
