//
// Created by kier on 11/24/15.
//

#ifndef LINKING_TYPINGSCENE_H
#define LINKING_TYPINGSCENE_H

#include <bud/Scene.h>
#include <unordered_map>
#include <bud/Texture.h>
#include <bud/Renderer.h>
#include <bud/Font.h>

class SlideScene : public bud::Scene {
public:
    SlideScene(bud::Renderer *bud_renderer);

    virtual ~SlideScene();

    virtual void onLoad() override;

    virtual void onShow() override;

    virtual void onHide() override;

    virtual void onExit() override;

private:
    bud::Texture *get_num_text(int num);

    bud::Color get_num_color(int num);

    bool generate(int num);

    bool generate();

    bool move(bud::DIRECTION dir, bool test = false);

    bool can_move();

    // 生成一个动画，表示从合并f1,f2块到t块，其中数字由fnum合成为tnum
    void animation_merge(const bud::Vector2D<int> &f1, const bud::Vector2D<int> &f2, const bud::Vector2D<int> &t,
                         int fnum, int tnum);

    // 生成一个动画，表示f块移动到t块
    void animation_move(const bud::Vector2D<int> &f, const bud::Vector2D<int> &t, int fnum);

    // 增加分数的动画
    void animation_score(int score);

    void logLayout();

    void gameover();

    static const int ROW = 4;
    static const int COL = 4;

    int map[ROW][COL];
    int blank_size = ROW * COL;

    bud::Point<int> map_pos = {50, 50};
    bud::Vector2D<int> block_size = {80, 80};
    bud::Vector2D<int> border = {2, 2};

    int animation_frame = 8;

    int animation_count = 0;

    int m_score = 0;

    bud::Font m_font;
    std::unordered_map<int, bud::Texture *> m_num_hash;

    bool m_over = false;
};


#endif //LINKING_TYPINGSCENE_H
