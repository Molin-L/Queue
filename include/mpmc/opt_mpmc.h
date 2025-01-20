#pragma once
#include <vector>
#include <atomic>
#include <optional>

template <typename T>
class OptimizedMPMCQueue {
public:
    explicit OptimizedMPMCQueue(size_t capacity)
        : m_capacity(capacity), m_queue(capacity), m_head(0), m_tail(0) {}

    // Push an item into the queue. Returns false if the queue is full.
    bool push(const T& item) {
        size_t tail;
        while (true) {
            tail = m_tail.load(std::memory_order_relaxed);
            size_t head = m_head.load(std::memory_order_acquire);
            if (size(tail, head) >= m_capacity) {
                return false; // Queue is full.
            }
            if (m_tail.compare_exchange_weak(tail, (tail + 1) % m_capacity, std::memory_order_release)) {
                break;
            }
        }

        // Write the item to the queue.
        m_queue[tail] = item;
        return true;
    }

    // Pop an item from the queue. Returns std::nullopt if the queue is empty.
    std::optional<T> pop() {
        size_t head;
        while (true) {
            head = m_head.load(std::memory_order_relaxed);
            size_t tail = m_tail.load(std::memory_order_acquire);
            if (size(tail, head) == 0) {
                return std::nullopt; // Queue is empty.
            }
            if (m_head.compare_exchange_weak(head, (head + 1) % m_capacity, std::memory_order_release)) {
                break;
            }
        }

        // Read the item from the queue.
        return m_queue[head];
    }

    // Get the current size of the queue.
    size_t size() const {
        size_t tail = m_tail.load(std::memory_order_acquire);
        size_t head = m_head.load(std::memory_order_acquire);
        return size(tail, head);
    }

    // Check if the queue is empty.
    bool empty() const {
        return size() == 0;
    }

    // Check if the queue is full.
    bool full() const {
        return size() >= m_capacity;
    }

private:
    size_t size(size_t tail, size_t head) const {
        if (tail >= head) {
            return tail - head;
        }
        return m_capacity - (head - tail);
    }

private:
    const size_t m_capacity;           // Maximum number of items in the queue.
    std::vector<T> m_queue;            // Circular buffer for storing items.
    std::atomic<size_t> m_head;        // Atomic index of the front of the queue.
    std::atomic<size_t> m_tail;        // Atomic index of the back of the queue.
};