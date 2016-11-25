//
// Created by kier on 11/21/15.
//

#ifndef TYPING_PAUSESCENE_H
#define TYPING_PAUSESCENE_H

#include <bud/Scene.h>
#include <bud/Tile.h>

class PauseScene : public bud::Scene {

public:
    PauseScene(bud::Renderer *bud_renderer);

    virtual ~PauseScene();

    void setParentScene(bud::Scene *scene);

private:
    bud::Font *m_menu_font;
    bud::Tile m_resume_tile;
    bud::Tile m_main_menu_tile;

    bud::Tile m_background;
};


#endif //TYPING_PAUSESCENE_H
