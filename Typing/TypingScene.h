//
// Created by kier on 11/21/15.
//

#ifndef TYPING_TYPINGSCENE_H
#define TYPING_TYPINGSCENE_H

#include <bud/Scene.h>
#include <bud/Font.h>
#include <bud/Texture.h>
#include <bud/Random.h>

class TypingScene : public bud::Scene {
public:
    TypingScene(bud::Renderer *bud_renderer) : Scene(bud_renderer) {}
    virtual void onLoad() override ;
    virtual void onShow() override ;
    virtual void onHide() override ;
    virtual void onExit() override ;

    virtual void update() override ;

    void gameover();

private:
    bud::Font *m_falling_letter_font;
    bud::Texture *m_letter_texture[26];
    bud::Texture *m_blast_texture;
    bud::Font *m_score_font;
    bud::Texture *m_heart_texture;

    bud::Random *m_rand;

    int m_score;
    int m_heart;

    bool m_over = false;
};


#endif //TYPING_TYPINGSCENE_H
