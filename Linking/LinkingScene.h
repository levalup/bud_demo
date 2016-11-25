//
// Created by kier on 11/24/15.
//

#ifndef LINKING_TYPINGSCENE_H
#define LINKING_TYPINGSCENE_H

#include <bud/Scene.h>
#include <bud/Font.h>

class LinkingScene : public bud::Scene {
public:
    LinkingScene(bud::Renderer *bud_renderer);
    virtual ~LinkingScene();

    virtual void onLoad() override ;
    virtual void onShow() override ;
    virtual void onHide() override ;
    virtual void onExit() override ;

    static const int COL = 10;
    static const int ROW = 10;
    int map[ROW][COL];
    int level = 5;
private:
    bool m_over = false;

    bud::Font m_font;
    bud::Point<int> map_pos = {45, 45};
    bud::Point<int> block_size = {40, 40};

    bud::Point<int> select_block = {-1, -1};
    bud::Point<int> mouse_on_block = {-1, -1};

    void generate();
    void shuffle();
    bool can_solve();
    bool link(int row1, int col1, int row2, int col2, bool test = false);
    bool line(int row1, int col1, int row2, int col2);

    void animation_link(int row1, int col1,
                        int row2, int col2,
                        int row3, int col3,
                        int row4, int col4,
                        int data1, int data2);
};


#endif //LINKING_TYPINGSCENE_H
