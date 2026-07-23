# Build a Thread-Safe Queue

This bounded outline demonstrates mutex, condition variables, shutdown, and move semantics. Production requirements may include cancellation and timeouts.

```cpp
template<class T>
class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity) : capacity_{capacity} {
        if (capacity == 0) throw std::invalid_argument{"zero capacity"};
    }

    bool push(T value) {
        std::unique_lock lock{mutex_};
        not_full_.wait(lock, [&] { return closed_ || queue_.size() < capacity_; });
        if (closed_) return false;
        queue_.push_back(std::move(value));
        lock.unlock();
        not_empty_.notify_one();
        return true;
    }

    std::optional<T> pop() {
        std::unique_lock lock{mutex_};
        not_empty_.wait(lock, [&] { return closed_ || !queue_.empty(); });
        if (queue_.empty()) return std::nullopt;
        T value = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        not_full_.notify_one();
        return value;
    }

    void close() {
        { std::lock_guard lock{mutex_}; closed_ = true; }
        not_empty_.notify_all();
        not_full_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable not_empty_, not_full_;
    std::deque<T> queue_;
    std::size_t capacity_;
    bool closed_{};
};
```

The destructor cannot safely solve external thread lifetime: owners must close the queue and join workers before destroying it. The predicates prevent spurious wakeups from violating invariants. Work is moved out before the lock is released; processing happens outside the critical section.

