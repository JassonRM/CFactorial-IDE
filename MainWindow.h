//
// Created by marco on 19/03/18.
//

#ifndef CFACTORIAL_IDE_MAINWINDOW_H
#define CFACTORIAL_IDE_MAINWINDOW_H


#include <QtWidgets>

class MainWindow : public QMainWindow {

public:
    MainWindow();

private slots:

private:
    QWidget* centralWidget;

    QVBoxLayout* vLayout;

    QPlainTextEdit* editor;

    QLabel* stdOut;

    QLabel* applicationLog;

    QTableView* tableView;

    QToolBar* toolbar;

    int const windowWidth = 800;

    int const windowHeight = 600;
};

#endif //CFACTORIAL_IDE_MAINWINDOW_H