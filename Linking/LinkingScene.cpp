//
// Created by kier on 11/24/15.
//

#include "LinkingScene.h"
#include <iostream>
#include <bud/inc.h>

using namespace bud;
using namespace std;

LinkingScene::LinkingScene(bud::Renderer *bud_renderer)
        : Scene(bud_renderer) {
    ensureLayerSize(5);

    m_font.load("/opt/RTP/Font/consola.ttf", 20);

    LocalNode *map_node = new LocalNode();
    map_node->selfDelete(true);

    map_node->whenUpdate([=] {
        Rect map_rect(map_pos, block_size * Point<int>(COL, ROW));
        if (Input::trigger(MOUSE::LEFT)) {
            if (map_rect.contain(Input::mouse())) {
                Point<int> index = (Input::mouse() - map_pos) / block_size;
                if (map[index.y][index.x] >= 0) {
                    if (select_block.x == index.x && select_block.y == index.y) {
                        select_block = {-1, -1};
                    } else if (select_block.x >= 0 && select_block.y >= 0) {
                        // 连接两个方块
                        if (link(select_block.y, select_block.x, index.y, index.x)) {
                            map[select_block.y][select_block.x] = -1;
                            map[index.y][index.x] = -1;
                        }
                        select_block = {-1, -1};
                    } else {
                        select_block = index;
                    }
                } else {
                    select_block = {-1, -1};
                }
            } else {
                select_block = {-1, -1};
            }
        }
        if (Input::trigger(MOUSE::RIGHT)) {
            shuffle();
        }
        if (map_rect.contain(Input::mouse())) {
            Point<int> index = (Input::mouse() - map_pos) / block_size;
            mouse_on_block = index;
        } else {
            mouse_on_block = {-1, -1};
        }
    });
    map_node->whenRender([=] {
        getRenderer()->setDrawColor(Color::Green);
        getRenderer()->rect(Rect(map_pos - Point<int>(2, 2), block_size * Point<int>(COL, ROW) + Point<int>(4, 4)));
        getRenderer()->rect(Rect(map_pos - Point<int>(4, 4), block_size * Point<int>(COL, ROW) + Point<int>(8, 8)));
        for (int row = 0; row < ROW; ++row) {
            for (int col = 0; col < COL; ++col) {
                int seed = map[row][col];
                if (seed < 0) continue;
                Random rand(seed);
                Point<int> pos = map_pos + block_size * Point<int>(col, row);
                Rect fill_rect(pos, block_size);
                getRenderer()->setDrawColor(
                        Color((byte) rand.next(0, 255), (byte) rand.next(0, 255), (byte) rand.next(0, 255)));
                getRenderer()->fill(fill_rect);
            }
        }
        if (select_block.x >= 0 && select_block.y >= 0) {
            Point<int> pos = map_pos + block_size * select_block;
            getRenderer()->setDrawColor(Color::Blue);
            getRenderer()->rect(Rect(pos, block_size));
            getRenderer()->rect(Rect(pos + Point<int>(2, 2), block_size - Point<int>(4, 4)));
        }
        if (mouse_on_block.x >= 0 && mouse_on_block.y >= 0) {
            Point<int> pos = map_pos + block_size * mouse_on_block;
            getRenderer()->setDrawColor(Color::Blue);
            getRenderer()->rect(Rect(pos, block_size));
        }
    });

    addNode(map_node, 0);
}

LinkingScene::~LinkingScene() {
}

void LinkingScene::onLoad() {
    Scene::onLoad();
    generate();
    while (!can_solve()) {
        shuffle();
    }
    select_block = {-1, -1};
    mouse_on_block = {-1, -1};
}

void LinkingScene::onShow() {
    Scene::onShow();
    if (m_over) {
        onExit();
        onLoad();
        m_over = false;
    }
}

void LinkingScene::onHide() {
    Scene::onHide();
}

void LinkingScene::onExit() {
    Scene::onExit();
}

void LinkingScene::generate() {
    // step 1.1: 生成成对的数据
    const int size = ROW * COL;
    int *data = (int *)map;
    int level_size = size / level;
    level_size = (int)ceil((double)level_size / 2) * 2;
    for (int i = 0; i < size; ++i) {
        data[i] = i / level_size;
    }
    // step 1.2: 随机打乱数据
    shuffle();
}

void LinkingScene::shuffle() {
    const int size = ROW * COL;
    int *data = (int *)map;
    for (int i = 0; i < size - 1; ++i) {
        int j = bud::random.next(i, size - 1);
        if (i != j) {
            auto temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
}

bool LinkingScene::can_solve() {
    return true;
}

bool LinkingScene::link(int row1, int col1, int row2, int col2, bool test) {
    int data1 = map[row1][col1];
    int data2 = map[row2][col2];
    if (data1 != data2) return false;
    map[row1][col1] = map[row2][col2] = -1;
    bool result = false;
    // 1 按行处理
    // 1.1 处理中间
    for (int col : xrange(col1, col2)) {
        if (line(row1, col1, row1, col) && line(row1, col, row2, col) && line(row2, col, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row1, col, row2, col, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    // 1.2 处理左边
    for (int col : xrange(col1 - 1, 0)) {
        if (line(row1, col1, row1, col) && line(row1, col, row2, col) && line(row2, col, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row1, col, row2, col, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    // 1.3 处理右边
    for (int col : xrange(col2 + 1, COL - 1)) {
        if (line(row1, col1, row1, col) && line(row1, col, row2, col) && line(row2, col, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row1, col, row2, col, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    // 2 按列处理
    // 2.1 处理中间
    for (int row : xrange(row1, row2)) {
        if (line(row1, col1, row, col1) && line(row, col1, row, col2) && line(row, col2, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row, col1, row, col2, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    // 2.2 处理上边
    for (int row : xrange(row1 - 1, 0)) {
        if (line(row1, col1, row, col1) && line(row, col1, row, col2) && line(row, col2, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row, col1, row, col2, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    // 2.3 处理下边
    for (int row : xrange(row2 + 1, ROW - 1)) {
        if (line(row1, col1, row, col1) && line(row, col1, row, col2) && line(row, col2, row2, col2)) {
            result = true;
            if (!test) animation_link(row1, col1, row, col1, row, col2, row2, col2, data1, data2);
            goto LINK_RESULT;
        }
    }
    LINK_RESULT:
    map[row1][col1] = data1;
    map[row2][col2] = data2;
    return result;
}

bool LinkingScene::line(int row1, int col1, int row2, int col2) {
    if (row1 == row2) {
        int row = row1;
        for (int col : xrange(col1, col2)) {
            if (map[row][col] >= 0) return false;
        }
    } else {
        int col = col1;
        for (int row : xrange(row1, row2)) {
            if (map[row][col] >= 0) return false;
        }
    }
    return true;
}

void LinkingScene::animation_link(int row1, int col1, int row2, int col2, int row3, int col3, int row4, int col4,
                                  int data1, int data2) {
    int link_frame = 20;
    LocalNode *link_node = new LocalNode();
    link_node->selfDelete(true);
    int *data = link_node->alloc<int>(1);
    data[0] = link_frame;

    link_node->whenUpdate([=] {
        int &frame = link_node->allocated_data<int>()[0];
        if (frame <= 0) {
            link_node->kill();
        } else {
            frame--;
        }
    });

    Point<int> pos1 = map_pos + block_size * Point<int>(col1, row1);
    Point<int> pos2 = map_pos + block_size * Point<int>(col2, row2);
    Point<int> pos3 = map_pos + block_size * Point<int>(col3, row3);
    Point<int> pos4 = map_pos + block_size * Point<int>(col4, row4);

    list<Point<int>> lines = {pos1 + block_size / 2, pos2 + block_size / 2, pos3 + block_size / 2, pos4 + block_size / 2};

    int seed = data1;

    link_node->whenRender([=] {
        int &frame = link_node->allocated_data<int>()[0];
        int fi = link_frame - frame;
        Point<int> border = (double)fi / link_frame * block_size / 2;
        Rect fill_rect1(pos1 + border, block_size - 2 * border);
        Rect fill_rect2(pos4 + border, block_size - 2 * border);
        Random rand(seed);
        Color color((byte) rand.next(0, 255), (byte) rand.next(0, 255), (byte) rand.next(0, 255));
        getRenderer()->setDrawColor(color);
        getRenderer()->fill(fill_rect1);
        getRenderer()->fill(fill_rect2);
        getRenderer()->setDrawColor(Color::Red);
        getRenderer()->line(lines);
    });

    addNode(link_node, 2);
}
