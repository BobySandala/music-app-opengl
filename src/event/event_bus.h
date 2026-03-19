#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <queue>
#include <mutex>
#include <iostream>

class EventBus
{
public:

    EventBus() = default;

    template<typename EventType>
    using Handler = std::function<void(const EventType&)>;

    template<typename EventType>
    size_t subscribe(Handler<EventType> handler)
    {
        auto& handlers = subscribers[typeid(EventType)];

        handlers.push_back(
            [handler](const void* event)
            {
                handler(*static_cast<const EventType*>(event));
            }
        );

        return handlers.size() - 1; // return ID
    }

    // Unsubscribe: works for ANY event type
    template<typename EventType>
    void unsubscribe(size_t index)
    {
        auto it = subscribers.find(typeid(EventType));
        if (it == subscribers.end()) return;

        auto& handlers = it->second;

        if (index < handlers.size())
        {
            handlers[index] = nullptr; // invalidate safely
        }
        std::cout << "[Event bus] Unsubscribe" << std::endl;
    }

    template<typename EventType>
    void emit(const EventType& event)
    {
        std::lock_guard<std::mutex> lock{queueMutex};

        EventType* copy{new EventType{event}};

        eventQueue.push(
            {
                typeid(EventType),
                copy
            }
        );
    }

    void process()
    {
        std::queue<QueuedEvent> localQueue;

        {
            std::lock_guard<std::mutex> lock{queueMutex};
            std::swap(localQueue, eventQueue);
        }

        while(!localQueue.empty())
        {
            auto q{localQueue.front()};
            localQueue.pop();

            auto it{subscribers.find(q.type)};

            if(it != subscribers.end())
            {
                for(auto& handler : it->second)
                {
                    handler(q.data);
                }
            }

            delete static_cast<char*>(q.data);
        }
    }

private:

    struct QueuedEvent
    {
        std::type_index type;
        void* data;
    };

    std::unordered_map<
        std::type_index,
        std::vector<std::function<void(const void*)>>
    > subscribers;

    std::queue<QueuedEvent> eventQueue;

    std::mutex queueMutex;
};