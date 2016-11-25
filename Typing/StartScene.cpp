//
// Created by kier on 11/22/15.
//

#include "StartScene.h"
#include "Global.h"
#include <bud/inc.h>
#include "TypingScene.h"

using namespace bud;

StartScene::StartScene(bud::Renderer *bud_renderer)
        : Scene(bud_renderer) {
    m_menu_font = new Font("assert/font/msyh.ttc", 24);
    m_start_tile.updateTexture(new Texture(getRenderer(), m_menu_font, "开始游戏", Color::Green));
    m_exit_tile.updateTexture(new Texture(getRenderer(), m_menu_font, "结束游戏", Color::Green));

    m_start_tile.pos.set(10, 10);
    m_exit_tile.pos.set(10, m_start_tile.rect().h + 20);

    addNode(&m_start_tile, 1);
    addNode(&m_exit_tile, 1);

    LocalNode *start = new LocalNode();
    start->selfDelete(true);
    start->whenUpdate([=]() {
        bool in = m_start_tile.rect().contain(Input::mouse());
        start->show(in);
        if (Input::trigger(MOUSE::LEFT) && in)
            g_master->pushScene(g_typingScene);
    });
    start->whenRender([=]() {
        this->getRenderer()->setDrawColor(Color::Blue);
        this->getRenderer()->rect(m_start_tile.rect());
    });
    addNode(start, 2);

    LocalNode *exit = new LocalNode();
    exit->selfDelete(true);
    exit->whenUpdate([=]() {
        bool in = m_exit_tile.rect().contain(Input::mouse());
        exit->show(in);
        if (Input::trigger(MOUSE::LEFT) && in)
            this->back();
    });
    exit->whenRender([=]() {
        this->getRenderer()->setDrawColor(Color::Blue);
        this->getRenderer()->rect(m_exit_tile.rect());
    });
    addNode(exit, 2);

}

StartScene::~StartScene() {
    delete m_start_tile.setTexture(nullptr);
    delete m_exit_tile.setTexture(nullptr);
    delete m_menu_font;
}
