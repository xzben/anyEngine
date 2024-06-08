#include <iostream>

#include "CurRunApp.h"

int main() {
    auto app = NS_SG::createRunApp("test", 800, 800);
    app->run();
    delete app;

    return 0;
}