//
// Created by kier on 11/24/15.
//

#include "Global.h"

using namespace bud;

Window *g_window = nullptr;
SceneMaster *g_master = nullptr;
Scene *g_start_scene = nullptr;

void deleteAllGlobal() {
    delete g_window;
    delete g_master;
    delete g_start_scene;
}
