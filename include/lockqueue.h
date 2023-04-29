#pragma once

#include <queue>
#include <mutex>

template <typename T>
class LockQueue {
public:
    void push(const T& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
    }

    T pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.size();
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    void swap(std::queue<T>& q) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.swap(q);
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};