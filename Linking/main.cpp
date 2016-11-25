#include <iostream>

#include "Global.h"
#include "LinkingScene.h"

#include <bud/inc.h>

using namespace std;
using namespace bud;

int main() {
    BUD _init;

    g_window = new Window("Linking", 640, 480);
    g_master = new SceneMaster(g_window->getRenderer());
    g_start_scene = new LinkingScene(g_master->getRenderer());
    LocalComponent component;
    component.whenQuit([=]() { g_master->clearScene(); });
    g_master->addComponent(&component);

    g_master->pushScene(g_start_scene);

    g_master->showFPS(true);
    g_master->loop();

    deleteAllGlobal();

    return 0;
}