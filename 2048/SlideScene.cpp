//
// Created by kier on 11/24/15.
//

#include "SlideScene.h"

#include <bud/inc.h>
#include <iostream>

using namespace std;
using namespace bud;

SlideScene::SlideScene(bud::Renderer *bud_renderer)
        : Scene(bud_renderer) {

    m_font.load("assert/font/consola.ttf", 20);

    ensureLayerSize(10);

    LocalNode *map_node = new LocalNode();
    map_node->selfDelete(true);
    LocalNode *score_node = new LocalNode();
    map_node->selfDelete(true);

    map_node->whenUpdate([=] {
        if (animation_count > 0) return;
        if (Input::trigger(KEY::W) || Input::trigger(KEY::UP)) {
            if (move(DIRECTION::UP, false))
                generate();
        } else if (Input::trigger(KEY::S) || Input::trigger(KEY::DOWN)) {
            if (move(DIRECTION::DOWN, false))
                generate();
        } else if (Input::trigger(KEY::A) || Input::trigger(KEY::LEFT)) {
            if (move(DIRECTION::LEFT, false))
                generate();
        } else if (Input::trigger(KEY::D) || Input::trigger(KEY::RIGHT)) {
            if (move(DIRECTION::RIGHT, false))
                generate();
        }
        if (!can_move()) gameover();
    });
    map_node->whenRender([=] {
        // 绘制背景
        for (size_t row = 0; row < ROW; ++row) {
            for (int col = 0; col < COL; ++col) {
                Point<int> pos = map_pos + block_size * Point<int>(col, row);
                Rect fill_rect(pos + border, block_size - 2 * border);
                getRenderer()->setDrawColor(Color(0xd5c59f));
                getRenderer()->fill(fill_rect);
            }
        }
        // 动画没完成则不绘制前景
        if (animation_count > 0) return;
        for (size_t row = 0; row < ROW; ++row) {
            for (int col = 0; col < COL; ++col) {
                // 绘制背景
                int num = map[row][col];
                // 如果小于0则不回绘制
                if (num <= 0) continue;
                Point<int> pos = map_pos + block_size * Point<int>(col, row);
                Rect fill_rect(pos + border, block_size - 2 * border);
                getRenderer()->setDrawColor(get_num_color(num));
                getRenderer()->fill(fill_rect);
                // 绘制数字
                Texture *num_text = get_num_text(num);
                getRenderer()->copy(num_text, fill_rect.center() - num_text->rect().size() / 2);
            }
        }
    });

    score_node->whenRender([=] {
        getRenderer()->text(B("Score: ", m_score), &m_font, 400, 80, Color(0xaa2116));
    });

    addNode(map_node, 1);
    addNode(score_node, 4);

    // 绘制说明，1, 基本单元绘制
    // 2, 生成的动画绘制
    // 3. 合并和移动的动画绘制
}

SlideScene::~SlideScene() {
}

void SlideScene::onLoad() {
    Scene::onLoad();
    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            map[row][col] = 0;
        }
    }
    blank_size = ROW * COL;
    animation_count = 0;
    m_score = 0;

    generate();
    generate();
}

void SlideScene::onShow() {
    Scene::onShow();
    if (m_over) {
        onExit();
        onLoad();
        m_over = false;
    }
}

void SlideScene::onHide() {
    Scene::onHide();
}

void SlideScene::onExit() {
    Scene::onExit();
}

bool SlideScene::generate(int num) {
    if (blank_size <= 0) return false;
    bool generated = false;
    int rnd = bud::random.next(1, blank_size);
    int row, col;
    for (row = 0; row < ROW; ++row) {
        for (col = 0; col < COL; ++col) {
            if (map[row][col] == 0) {
                rnd--;
                if (rnd == 0) {
                    map[row][col] = num;
                    blank_size--;
                    generated = true;
                    break;
                }
            }
        }
        if (col < COL) break;
    }
    return generated;
}

bud::Texture *SlideScene::get_num_text(int num) {
    Texture *num_text;
    auto it = m_num_hash.find(num);
    if (it == m_num_hash.end()) {
        num_text = new Texture(getRenderer(), &m_font, B(1 << num), Color::Brown);
        m_num_hash.insert(pair<int, Texture *>(num, num_text));
    } else {
        num_text = it->second;
    }
    return num_text;
}

bud::Color SlideScene::get_num_color(int num) {
    switch (num) {
        default:
            return Color(0x27342b);
        case 1:
            return Color(0xcde6c7);
        case 2:
            return Color(0x65c294);
        case 3:
            return Color(0x77ac98);
        case 4:
            return Color(0x007d65);
        case 5:
            return Color(0x84bf96);
        case 6:
            return Color(0x45b97c);
        case 7:
            return Color(0x225a1f);
        case 8:
            return Color(0x367459);
        case 9:
            return Color(0x007947);
        case 10:
            return Color(0x40835e);
        case 11:
            return Color(0x2b6447);
        case 12:
            return Color(0x005831);
        case 13:
            return Color(0x006c54);
        case 14:
            return Color(0x375830);
        case 15:
            return Color(0x274d3d);
        case 16:
            return Color(0x375830);
        case 17:
            return Color(0x27342b);
    }
}

bool SlideScene::generate() {
    if (bud::random.binomial(0.5))
        return generate(1);
    else
        return generate(2);
}

bool SlideScene::move(bud::DIRECTION dir, bool test) {
    bool moved = false;
    int tmap[ROW][COL];
    int add_score = 0;
    if (dir == DIRECTION::UP) {
        int trow = 0, tcol = 0;
        for (int col = 0; col < COL; ++col, ++tcol) {
            trow = 0;
            for (int row = 0; row < ROW;) {
                if (map[row][col] == 0) {
                    row++;
                } else {
                    int nrow = row + 1;
                    while (nrow < ROW && map[nrow][col] == 0) nrow++;
                    if (nrow < ROW && map[nrow][col] == map[row][col]) {
                        tmap[trow][tcol] = map[row][col] + 1;
                        add_score += 1 << tmap[trow][tcol];
                        // 合并动画
                        if (!test)
                            animation_merge({col, row}, {col, nrow}, {tcol, trow}, map[row][col], tmap[trow][tcol]);
                        blank_size++;
                        row = nrow + 1;
                    } else {
                        tmap[trow][tcol] = map[row][col];
                        // 移动动画
                        if (!test) animation_move({col, row}, {tcol, trow}, map[row][col]);
                        row = nrow;
                    }
                    trow++;
                }
            }
            for (; trow < ROW; ++trow)
                tmap[trow][tcol] = 0;
        }
    } else if (dir == DIRECTION::DOWN) {
        int trow = ROW - 1, tcol = COL - 1;
        for (int col = COL - 1; col >= 0; --col, --tcol) {
            trow = ROW - 1;
            for (int row = ROW - 1; row >= 0;) {
                if (map[row][col] == 0) {
                    row--;
                } else {
                    int nrow = row - 1;
                    while (nrow >= 0 && map[nrow][col] == 0) nrow--;
                    if (nrow >= 0 && map[nrow][col] == map[row][col]) {
                        tmap[trow][tcol] = map[row][col] + 1;
                        add_score += 1 << tmap[trow][tcol];
                        if (!test)
                            animation_merge({col, row}, {col, nrow}, {tcol, trow}, map[row][col], tmap[trow][tcol]);
                        blank_size++;
                        row = nrow - 1;
                    } else {
                        tmap[trow][tcol] = map[row][col];
                        if (!test) animation_move({col, row}, {tcol, trow}, map[row][col]);
                        row = nrow;
                    }
                    trow--;
                }
            }
            for (; trow >= 0; --trow)
                tmap[trow][tcol] = 0;
        }
    } else if (dir == DIRECTION::LEFT) {
        int trow = 0, tcol = 0;
        for (int row = 0; row < ROW; ++row, ++trow) {
            tcol = 0;
            for (int col = 0; col < COL;) {
                if (map[row][col] == 0) {
                    col++;
                } else {
                    int ncol = col + 1;
                    while (ncol < COL && map[row][ncol] == 0) ncol++;
                    if (ncol < COL && map[row][ncol] == map[row][col]) {
                        tmap[trow][tcol] = map[row][col] + 1;
                        add_score += 1 << tmap[trow][tcol];
                        if (!test)
                            animation_merge({col, row}, {ncol, row}, {tcol, trow}, map[row][col], tmap[trow][tcol]);
                        blank_size++;
                        col = ncol + 1;
                    } else {
                        tmap[trow][tcol] = map[row][col];
                        if (!test) animation_move({col, row}, {tcol, trow}, map[row][col]);
                        col = ncol;
                    }
                    tcol++;
                }
            }
            for (; tcol < COL; ++tcol)
                tmap[trow][tcol] = 0;
        }
    } else if (dir == DIRECTION::RIGHT) {
        int trow = ROW - 1, tcol = COL - 1;
        for (int row = ROW - 1; row >= 0; --row, --trow) {
            tcol = COL - 1;
            for (int col = COL - 1; col >= 0;) {
                if (map[row][col] == 0) {
                    col--;
                } else {
                    int ncol = col - 1;
                    while (ncol >= 0 && map[row][ncol] == 0) ncol--;
                    if (ncol >= 0 && map[row][ncol] == map[row][col]) {
                        tmap[trow][tcol] = map[row][col] + 1;
                        add_score += 1 << tmap[trow][tcol];
                        if (!test)
                            animation_merge({col, row}, {ncol, row}, {tcol, trow}, map[row][col], tmap[trow][tcol]);
                        blank_size++;
                        col = ncol - 1;
                    } else {
                        tmap[trow][tcol] = map[row][col];
                        if (!test) animation_move({col, row}, {tcol, trow}, map[row][col]);
                        col = ncol;
                    }
                    tcol--;
                }
            }
            for (; tcol >= 0; --tcol)
                tmap[trow][tcol] = 0;
        }
    }
    for (int trow = 0; trow < ROW; ++trow) {
        for (int tcol = 0; tcol < COL; ++tcol) {
            if (map[trow][tcol] != tmap[trow][tcol])
                moved = true;
            map[trow][tcol] = tmap[trow][tcol];
        }
    }
    if (!test && add_score > 0) animation_score(add_score);
    return moved;
}

bool SlideScene::can_move() {
    int temp_blank_size = blank_size;
    int temp_map[ROW][COL];
    memcpy(temp_map, map, sizeof(int) * ROW * COL);
    for (int i = 0; i < 4; i++) {
        if (move((DIRECTION) (i + 1), true)) {
            blank_size = temp_blank_size;
            memcpy(map, temp_map, sizeof(int) * ROW * COL);
            return true;
        }
    }
    return false;
}

void SlideScene::animation_merge(const bud::Vector2D<int> &f1, const bud::Vector2D<int> &f2,
                                 const bud::Vector2D<int> &t,
                                 int fnum, int tnum) {
    //cout << B("merge:\t", f1, ' ', f2, ' ', t, ' ', fnum, ' ', tnum) << endl;
    LocalNode *node = new LocalNode();
    node->selfDelete(true);
    int *data = node->alloc<int>(1);
    data[0] = animation_frame;

    node->whenUpdate([=] {
        int &frame = node->allocated_data<int>()[0];
        if (frame <= 0) {
            node->kill();
            m_score += 1 << tnum;
            //animation_score(1 << tnum);
            animation_count--;
        } else {
            frame--;
        }
    });

    node->whenRender([=] {
        int &frame = node->allocated_data<int>()[0];
        int fi = animation_frame - frame;
        Point<int> fpos1 = map_pos + block_size * f1 + border;
        Point<int> fpos2 = map_pos + block_size * f2 + border;
        Point<int> tpos = map_pos + block_size * t + border;
        Point<int> draw_pos1 = fpos1 + (tpos - fpos1) * ((double)(fi) / animation_frame);
        Point<int> draw_pos2 = fpos2 + (tpos - fpos2) * ((double)(fi) / animation_frame);
        Rect fill_rect1(draw_pos1, block_size - 2 * border);
        Rect fill_rect2(draw_pos2, block_size - 2 * border);
        getRenderer()->setDrawColor(get_num_color(fnum));
        getRenderer()->fill(fill_rect1);
        getRenderer()->fill(fill_rect2);

        Texture *num_text = get_num_text(fnum);
        getRenderer()->copy(num_text, fill_rect1.center() - num_text->rect().size() / 2);
        getRenderer()->copy(num_text, fill_rect2.center() - num_text->rect().size() / 2);
    });

    addNode(node, 3);
    animation_count++;
}

void SlideScene::animation_move(const bud::Vector2D<int> &f, const bud::Vector2D<int> &t, int fnum) {
    //cout << B("move:\t", f, ' ', t, ' ', fnum) << endl;
    LocalNode *node = new LocalNode();
    node->selfDelete(true);
    int *data = node->alloc<int>(1);
    data[0] = animation_frame;

    node->whenUpdate([=] {
        int &frame = node->allocated_data<int>()[0];
        if (frame <= 0) {
            node->kill();
            animation_count--;
        } else {
            frame--;
        }
    });

    node->whenRender([=] {
        int &frame = node->allocated_data<int>()[0];
        int fi = animation_frame - frame;
        Point<int> fpos = map_pos + block_size * f + border;
        Point<int> tpos = map_pos + block_size * t + border;
        Point<int> draw_pos = fpos + (tpos - fpos) * ((double)(fi) / animation_frame);
        Rect fill_rect(draw_pos, block_size - 2 * border);
        getRenderer()->setDrawColor(get_num_color(fnum));
        getRenderer()->fill(fill_rect);

        Texture *num_text = get_num_text(fnum);
        getRenderer()->copy(num_text, fill_rect.center() - num_text->rect().size() / 2);
    });

    addNode(node, 3);
    animation_count++;
}

void SlideScene::animation_score(int score) {
    int add_score_frame = 20;
    LocalNode *score_node = new LocalNode();
    score_node->selfDelete(true);
    int *data = score_node->alloc<int>(1);
    data[0] = add_score_frame;

    score_node->whenUpdate([=] {
        int &frame = score_node->allocated_data<int>()[0];
        if (frame <= 0) {
            score_node->kill();
        } else {
            frame--;
        }
    });

    score_node->whenRender([=] {
        int &frame = score_node->allocated_data<int>()[0];
        int fi = add_score_frame - frame - 1;
        getRenderer()->text(B("+", score), &m_font, 500, 80 - fi, Color(0xaa2116));
    });

    addNode(score_node, 5);
}

void SlideScene::logLayout() {
    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            cout << map[row][col] << ' ';
        }
        cout << endl;
    }
}

void SlideScene::gameover() {
    cout << "Game Over" << endl;
    m_over = true;
    onShow();
}
