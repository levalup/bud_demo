//
// Created by kier on 11/21/15.
//

#include <bud/inc.h>
#include "Global.h"
#include "PauseScene.h"
#include "StartScene.h"

using namespace bud;

PauseScene::PauseScene(bud::Renderer *bud_renderer)
    : Scene(bud_renderer) {
    m_menu_font = new Font("assert/font/msyh.ttc", 24);
    m_resume_tile.updateTexture(new Texture(getRenderer(), m_menu_font, "返回游戏", Color::Green));
    m_main_menu_tile.updateTexture(new Texture(getRenderer(), m_menu_font, "返回主菜单", Color::Green));

    m_resume_tile.pos.set(10, 10);
    m_main_menu_tile.pos.set(10, m_resume_tile.rect().h + 20);

    addNode(&m_background, 0);
    addNode(&m_resume_tile, 1);
    addNode(&m_main_menu_tile, 1);

    LocalNode *resume = new LocalNode();
    resume->selfDelete(true);
    resume->whenUpdate([=]() {
        bool in = m_resume_tile.rect().contain(Input::mouse());
        resume->show(in);
        if (Input::trigger(KEY::ESCAPE) || (Input::trigger(MOUSE::LEFT) && in))
            this->back();
    });
    resume->whenRender([=]() {
        this->getRenderer()->setDrawColor(Color::Blue);
        this->getRenderer()->rect(m_resume_tile.rect());
    });
    addNode(resume, 2);

    LocalNode *main_menu = new LocalNode();
    main_menu->selfDelete(true);
    main_menu->whenUpdate([=]() {
        bool in = m_main_menu_tile.rect().contain(Input::mouse());
        main_menu->show(in);
        if (Input::trigger(MOUSE::LEFT) && in)
            g_master->activeScene(g_startScene);
    });
    main_menu->whenRender([=]() {
        this->getRenderer()->setDrawColor(Color::Blue);
        this->getRenderer()->rect(m_main_menu_tile.rect());
    });
    addNode(main_menu, 2);
}

PauseScene::~PauseScene() {
    delete m_main_menu_tile.setTexture(nullptr);
    delete m_resume_tile.setTexture(nullptr);
    delete m_menu_font;
    delete m_background.setTexture(nullptr);
}

void PauseScene::setParentScene(bud::Scene *scene) {
    Texture *new_texture = new Texture(scene->getRenderer(), g_window->width(), g_window->height());
    new_texture->setColorMod({200, 200, 200});

    scene->getRenderer()->setTarget(new_texture);
    scene->getRenderer()->clear(Color::White);
    scene->forceRender();  // 这里是绘制所有节点方法
    scene->getRenderer()->setTargetDefault();

    Texture *old_texture = m_background.updateTexture(new_texture);
    delete old_texture;
}

