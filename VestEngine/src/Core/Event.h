#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <type_traits>

namespace Vest {

enum class EventType {
    None = 0,
    WindowClose,
    WindowResize
};

class Event {
public:
    virtual ~Event() = default;

    bool handled = false;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const { return GetName(); }
};

class WindowCloseEvent : public Event {
public:
    static EventType StaticType() { return EventType::WindowClose; }

    EventType GetEventType() const override { return StaticType(); }
    const char* GetName() const override { return "WindowClose"; }
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int w, unsigned int h) : m_Width(w), m_Height(h) {}

    static EventType StaticType() { return EventType::WindowResize; }

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }

    EventType GetEventType() const override { return StaticType(); }
    const char* GetName() const override { return "WindowResize"; }

    std::string ToString() const override {
        std::ostringstream ss;
        ss << "WindowResize: " << m_Width << "x" << m_Height;
        return ss.str();
    }

private:
    unsigned int m_Width;
    unsigned int m_Height;
};

class EventDispatcher {
public:
    explicit EventDispatcher(Event& event) : m_Event(event) {}

    template <typename T, typename F>
    bool Dispatch(const F& func) {
        static_assert(std::is_base_of_v<Event, T>, "Can only dispatch Event types");
        if (m_Event.GetEventType() == T::StaticType()) {
            m_Event.handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

}  // namespace Vest
