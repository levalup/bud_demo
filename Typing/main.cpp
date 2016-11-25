#include <iostream>

#include <bud/inc.h>

#include "Global.h"
#include "TypingScene.h"
#include "PauseScene.h"
#include "StartScene.h"
#include "GameoverScene.h"

using namespace std;
using namespace bud;

int main() {

    BUD _init;

    g_window = new Window("BUD Test", 640, 480);
    g_master = new SceneMaster(g_window->getRenderer());
    LocalComponent component;
    component.whenQuit([=]() {
        g_master->clearScene();
    });
    g_master->addComponent(&component);

    g_typingScene = new TypingScene(g_master->getRenderer());
    g_pauseScene = new PauseScene(g_master->getRenderer());
    g_startScene = new StartScene(g_master->getRenderer());
    g_gameoverScene = new GameoverScene(g_master->getRenderer());

    g_master->pushScene(g_startScene);
    g_master->showFPS(true);
    g_master->loop();

    delete g_gameoverScene;
    delete g_startScene;
    delete g_pauseScene;
    delete g_typingScene;
    delete g_master;
    delete g_window;

    return 0;
}