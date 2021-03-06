#include "menu_wall_cell.h"

MenuWallCell::MenuWallCell(int x, int y, int row, int col, int size, bool hasWall, QPixmap pixmap) : MenuCell{x, y,row,col,size,false, false, pixmap}, hasWall{hasWall} {
    if (hasWall) setWall(getPosition().x() % 2 == 0);
}

void MenuWallCell::setWall(bool isHorizontal) {
    if (isHorizontal) setCellPixmap(QPixmap(wallHorizontal_png.c_str()));
    else setCellPixmap(QPixmap(wallVertical_png.c_str()));
}

void MenuWallCell::unsetWall() {
    setCellPixmap(QPixmap(no_wall_png.c_str()));
}