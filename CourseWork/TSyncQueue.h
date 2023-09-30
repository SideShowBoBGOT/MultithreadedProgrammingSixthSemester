#ifndef COURSEWORK_TSYNCQUEUE_H
#define COURSEWORK_TSYNCQUEUE_H

#include <queue>
#include <mutex>
#include <optional>

template<typename T>
class TSyncQueue {
    public:
    TSyncQueue()=default;
    virtual ~TSyncQueue()=default;

    public:
    void Push(const T& el) {
        std::lock_guard<std::mutex> guard(m_xMutex);
        m_qQueue.push(el);
    }

    std::optional<T> Pop() {
        std::lock_guard<std::mutex> guard(m_xMutex);
        if(m_qQueue.empty()) return std::nullopt;
        auto el = std::move(m_qQueue.front());
        m_qQueue.pop();
        return el;
    }

    protected:
    std::mutex m_xMutex;
    std::queue<T> m_qQueue;
};

#endif //COURSEWORK_TSYNCQUEUE_H
