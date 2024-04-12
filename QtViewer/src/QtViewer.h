#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtViewer.h"

class QtViewer : public QMainWindow
{
    Q_OBJECT

public:
    QtViewer(QWidget *parent = nullptr);
    ~QtViewer();

private:
    Ui::QtViewerClass ui;
};
