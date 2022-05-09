#pragma once
#include <string>
#include <sstream>

#define BIT(x) (1 << x)

namespace LG
{
    enum EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        Application = BIT(0),
        Input =       BIT(1),
        KeyBoard =    BIT(2),
        Mouse =       BIT(3),
        MouseButton = BIT(14),
    };

    #define EVENT_CLASS_TYPE(type)                                          \
        static EventType GetStaticType() { return EventType::##type; }      \
        virtual EventType GetEventType() const { return GetStaticType(); }  \
        virtual const char* GetName() const { return #type; }               \

    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() { return category; } 

    class Event
    {
        friend class EventDispatcher;

        public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }
        bool Handled() const { return m_Handled; }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    protected:
        bool m_Handled = false;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event)
        : m_event(event){}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if(m_event.GetEventType() == T::GetStaticType())
            {
                m_event.m_Handled = func(*(T*)&m_event;)
                return true; 
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Key Events //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() { return m_keyCode; }

        EVENT_CLASS_CATEGORY(EventCategory::KeyBoard | EventCategory::Input)

    protected:
        KeyEvent(int keyCode) : m_keyCode(keyCode){}
        int m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode)
            , m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_RepeatCount; }

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Mouse Events ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_mouseX(x)
            , m_mouseY(y) {}

        inline float GetX() const { return m_mouseX; }
        inline float GetY() const { return m_mouseY; }

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << "," << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

    private:
        float m_mouseX;
        float m_mouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_xOffset(xOffset)
            , m_yOffset(yOffset) {}

        inline float GetXOffset() const { return m_xOffset; }
        inline float GetYOffset() const { return m_yOffset; }

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_xOffset << "," << m_yOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

    private:
        float m_xOffset;
        float m_yOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

    protected:
        MouseButtonEvent(int button)
            : m_button(button) {}

        int m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:

        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button)
        {}

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressed: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:

        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) {}

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Application Events //////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width(width)
            , m_height(height) {}

        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }

        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    private:
        unsigned int m_width;
        unsigned int m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class AppTickEvent : public Event
    {
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class  AppUpdateEvent : public Event
    {
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class AppRenderEvent : public Event
    {
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };
} // namespace LG
