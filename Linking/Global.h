//
// Created by kier on 11/24/15.
//

#ifndef LINKING_GLOBAL_H
#define LINKING_GLOBAL_H

#include <bud/inc.h>

extern bud::Window *g_window;
extern bud::SceneMaster *g_master;
extern bud::Scene *g_start_scene;

void deleteAllGlobal();

#endif //LINKING_GLOBAL_H
