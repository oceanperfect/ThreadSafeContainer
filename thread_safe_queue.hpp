/************************************************************************************************
	File Name	:	thread_safe_queue.hpp
	Version		: 	Initial Draft
	Author		:	yang yang
	Created		:	2017.06.08
	Last Modified:	2017.06.08
	Description	:	A thread safe queue.
************************************************************************************************/

#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <queue>
#include <boost/thread.hpp>
#include <boost/lambda/lambda.hpp>

template <typename T> class ThreadSafeQueue
{
    private:
        std::queue<T> queue;
        boost::mutex mutex;
    public:
        ThreadSafeQueue(){};
        ThreadSafeQueue(const ThreadSafeQueue &other)
        {
            boost::mutex::scoped_lock lock(other.mutex);
            queue = other.queue;
        }
        ~ThreadSafeQueue(){};

        void Push(T &new_value)
        {
            boost::mutex::scoped_lock lock(mutex);
            queue.push(new_value);
            condition.notify_one();
        }
        void Pop(T &value)
        {
            boost::mutex::scoped_lock lock(mutex);
            value = queue.front();
            queue.pop();
        }

        bool TryPop(T &value)
        {
            boost::mutex::scoped_lock lock(mutex);
            if( queue.empty())
            {
                return false;
            }
            else
            {
                value = queue.front();
                queue.pop();
                return true;
            }
        }
        
        bool Empty()
        {
            boost::mutex::scoped_lock lock(mutex);
            return queue.empty();
        }
};

#endif
