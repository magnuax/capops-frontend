#pragma once

#include <QFrame>

enum SectorState {
    NORMAL,
    CONGESTED,
    AT_RISK
};

class QMenu;

class GridSector : public QFrame
{
    Q_OBJECT

public:
    explicit GridSector(int row, int col, QWidget *parent = nullptr);

    void setState(SectorState state);

signals:
    void clicked(int row, int col);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void createContextMenu();

private:
    int _id;
    int _row;
    int _col;
    SectorState _state;

    QMenu *_contextMenu;
    QAction *_renameAction;
    QAction *_deleteAction;
    QAction *_customAction;
};