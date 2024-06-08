#include "Application.h"

#include "utils/Timer.h"

BEGIN_NS_SCENCE_GRAPH

static Application* s_instance = nullptr;

Application* Application::getInstance() {
    CCASSERT(s_instance != nullptr, "please create application");

    return s_instance;
}
Application::Application(const std::string& name, Window* window) : m_name(name) {
    CCASSERT(s_instance == nullptr, "only one application can been create");
    s_instance = this;
    setWindow(window);
}

Application::~Application() {
    if (m_window) {
        m_window->delRef();
        m_window = nullptr;
    }
}

void Application::setWindow(Window* window) {
    if (m_window) {
        m_window->delRef();
        m_window = nullptr;
    }
    m_window = window;
    if (m_window) {
        m_window->addRef();
    }

    onUpdateWindow(m_window);
}

void Application::run() {
    m_lasttime    = Timer::get_time_stamp_microsec();
    m_gameRunTime = 0;
    m_frameCount  = 0;
    while (!m_exit) {
        time_t curtime = Timer::get_time_stamp_microsec();
        float dt       = (curtime - m_lasttime) / 1000000.f;
        if (dt > m_fps) {
            m_gameRunTime += dt;
            m_lasttime = curtime;

            if (m_window) {
                m_window->updateEvents();
            }
            update(dt);
            m_frameCount++;
        }
    }
}

void Application::update(float dt) {
    onUpdate(dt);

    foreach ([&](System* sys) { sys->update(dt); });
}

void Application::exit() { m_exit = true; }

END_NS_SCENCE_GRAPH