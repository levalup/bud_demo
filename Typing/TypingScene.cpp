//
// Created by kier on 11/21/15.
//

#include "TypingScene.h"
#include "Global.h"
#include <bud/inc.h>
#include "PauseScene.h"
#include "GameoverScene.h"

using namespace bud;
using namespace std;

void TypingScene::onLoad() {
    Scene::onLoad();

    m_rand = new Random();

    m_falling_letter_font = new Font("assert/font/consola.ttf", 64);

    for (int i : range(26)) {
        m_letter_texture[i] = new Texture(getRenderer(), m_falling_letter_font, string() + char('A' + i), Color::Black);
    }

    m_blast_texture = new Texture(getRenderer(), "assert/graphics/Fire2.png");

    m_score_font = new Font("assert/font/consola.ttf", 20);

    m_heart_texture = new Texture(getRenderer(), "assert/graphics/heart.png");

    m_score = 0;
    m_heart = 3;

    LocalNode *node = new LocalNode();
    node->selfDelete(true);
    node->whenRender([=]() {
        this->getRenderer()->setDrawColor(Color::Red);
        this->getRenderer()->line({0, g_window->height() - 20}, {g_window->width(), g_window->height() - 20});

        for (int i : range(this->m_heart)) {
            this->getRenderer()->copy(this->m_heart_texture, Point<int>(10, 10) + this->m_heart_texture->rect().col(i));
        }

        Texture score_text(this->getRenderer(), m_score_font, B("Score: ", m_score), Color::Yellow);
        this->getRenderer()->copy(&score_text, g_window->width() - score_text.width() - 10, 10);
    });
    addNode(node, 2);
}

void TypingScene::onShow() {
    Scene::onShow();
    if (m_over)
    {
        onExit();
        onLoad();
        m_over = false;
    }
}

void TypingScene::onHide() {
    Scene::onHide();
}

void TypingScene::onExit() {
    Scene::onExit();

    clearNode();
    clearComponent();

    delete m_falling_letter_font;

    for (int i : range(26)) {
        delete m_letter_texture[i];
    }

    delete m_blast_texture;

    delete m_score_font;

    delete m_heart_texture;

    delete m_rand;
}

void TypingScene::update() {
    Node::update();

    if (Input::trigger(KEY::ESCAPE)) {
        g_pauseScene->setParentScene(this);
        g_master->pushScene(g_pauseScene);
    }

    if (updateTimes() % 30 == 0) {
        int id = m_rand->next() % 26;

        Sprite *newSprite = new Sprite(m_letter_texture[id]);
        newSprite->selfDelete(true);
        newSprite->pos.y = 0 - newSprite->rect().h;
        newSprite->pos.x = m_rand->next() % (g_window->width() - newSprite->rect().w);
        newSprite->motion.v.y = 1;

        LocalMotion *motion = new LocalMotion();
        motion->selfDelete(true);
        motion->finishKill(true);
        motion->whenUpdate([=](Sprite *sprite) {
            if (Input::trigger(KEY((int)KEY::A + id))) {
                sprite->kill();
                this->m_score++;
                Sprite *blast = new Sprite(this->m_blast_texture);
                blast->selfDelete(true);
                blast->clip = this->m_blast_texture->rect() / Point<int>(5, 3);
                blast->pos = sprite->rect().center() - blast->rect().base().center();
                AnimationMotion *animation = new AnimationMotion(ANIMATION_LAYOUT::H, 12, 2, 1);
                animation->selfDelete(true);
                animation->finishKill(true);
                blast->motion.run(animation);
                addNode(blast, 1);
            } else if (sprite->pos.y + sprite->rect().h > g_window->height()) {
                motion->finish();
            }
        });
        motion->whenFinish([=]() {
            if (this->m_heart > 0) {
                this->m_heart--;
                if (this->m_heart <= 0)
                    gameover();
            }
        });

        newSprite->motion.run(motion);

        addNode(newSprite, 0);
    }
}

void TypingScene::gameover() {
    m_over = true;
    g_gameoverScene->setParentScene(this);
    g_master->pushScene(g_gameoverScene);
}
