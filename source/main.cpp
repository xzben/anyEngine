#include <iostream>
#include "platform/Window.h"

int main()
{
    Window *win = Window::create("test", 800, 800);

    win->init();

    while (!win->shoudleClose())
    {
        win->updateEvents();
    }

    return 0;
}