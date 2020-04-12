
/*
==========================================
  Copyright (c) 2017-2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "dynamic_static/graphics/application.hpp"

namespace dst {
namespace gfx {

Application::~Application()
{
    mRunning = false;
}

void Application::start()
{
    setup();
    mRunning = true;
    mClock.update();
    while (mRunning) {
        dst::sys::Window::poll_events();
        mClock.update();
        const auto& input = mWindow.input();
        pre_update(mClock, input);
        update(mClock, input);
        post_update(mClock, input);
        pre_render(mClock);
        render(mClock);
        post_render(mClock);
    }
    shutdown();
}

void Application::stop()
{
    mRunning = false;
}

void Application::setup()
{
}

void Application::shutdown()
{
}

void Application::pre_update(
    const dst::Clock& clock,
    const dst::sys::Input& input
)
{
}

void Application::update(
    const dst::Clock& clock,
    const dst::sys::Input& input
)
{
}

void Application::post_update(
    const dst::Clock& clock,
    const dst::sys::Input& input
)
{
}

void Application::pre_render(const dst::Clock& clock)
{
}

void Application::render(const dst::Clock& clock)
{
}

void Application::post_render(const dst::Clock& clock)
{
}

} // namespace gfx
} // namespace dst