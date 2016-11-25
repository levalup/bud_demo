//
// Created by kier on 11/22/15.
//

#ifndef TYPING_GAMEOVERSCENE_H
#define TYPING_GAMEOVERSCENE_H

#include <bud/Scene.h>
#include <bud/Font.h>
#include <bud/Tile.h>

class GameoverScene : public bud::Scene {
public:
    GameoverScene(bud::Renderer *bud_renderer);

    virtual ~GameoverScene();

    void setParentScene(bud::Scene *scene);

private:
    bud::Font *m_menu_font;
    bud::Font *m_gameover_font;
    bud::Tile m_gameover_tile;
    bud::Tile m_try_again_tile;
    bud::Tile m_main_menu_tile;

    bud::Tile m_background;
};


#endif //TYPING_GAMEOVERSCENE_H
