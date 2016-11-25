//
// Created by kier on 11/21/15.
//

#ifndef TYPING_GLOBAL_H
#define TYPING_GLOBAL_H

#include <bud/Window.h>
#include <bud/SceneMaster.h>

class TypingScene;
class StartScene;
class MenuScene;
class GameoverScene;
class PauseScene;

extern bud::Window *g_window;
extern bud::SceneMaster *g_master;

extern StartScene *g_startScene;
extern TypingScene *g_typingScene;
extern MenuScene *g_menuScene;
extern GameoverScene *g_gameoverScene;
extern PauseScene *g_pauseScene;

#endif //TYPING_GLOBAL_H
