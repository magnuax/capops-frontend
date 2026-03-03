#pragma once

#include <QWidget>

class QMenu;

class GridSector : public QWidget
{
    Q_OBJECT

public:
    explicit GridSector(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void createContextMenu();

private:
    int _id;
    int _row;
    int _col;

    QMenu *_contextMenu;
    QAction *_renameAction;
    QAction *_deleteAction;
    QAction *_customAction;
};