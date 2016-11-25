//
// Created by kier on 11/22/15.
//

#ifndef TYPING_STARTSCENE_H
#define TYPING_STARTSCENE_H

#include <bud/Scene.h>
#include <bud/Font.h>
#include <bud/Tile.h>

class StartScene : public bud::Scene {
public:
    StartScene(bud::Renderer *bud_renderer);

    virtual ~StartScene();

private:
    bud::Font *m_menu_font;
    bud::Tile m_start_tile;
    bud::Tile m_exit_tile;
};


#endif //TYPING_STARTSCENE_H
