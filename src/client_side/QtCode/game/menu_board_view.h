
#ifndef MENUBAORDVIEW_H
#define MENUBAORDVIEW_H

#include <menu_board_scene.h>
#include <QGraphicsView>
#include <QMouseEvent>
#include <iostream>
#include <QPoint>
#include "board_fetcher.h"

class MenuBoardView : public QGraphicsView {
    Q_OBJECT
    
    bool playerSelected=false;
    bool wallSelected=false;
    MenuCell* ancientCell=nullptr;
    MenuCell* firstWall=nullptr;
    int game_id;
    std::shared_ptr<bool> should_continue_fetching_;

    bool SendMoveToServer(DIRECTION moveDirection, DIRECTION winningDirection) const;
    
    void drawScene();

    void outputBoardToTerminal() const;
    
public:
    BoardFetcher *boardFetcher;
    MenuBoardView(int game_id, QVector<QPoint> pawns_={}, QVector<QPoint> walls_={});
    void playMove(MenuCell*);
    bool verifyMove(QPoint,QPoint);
    void placeWall(MenuCell*, MenuCell*) const;
    bool verifyWall(QPoint, QPoint) const;
    virtual ~MenuBoardView();
  public slots:
    void mousePressEvent(QMouseEvent *event);
    void redrawScene();
};

#endif