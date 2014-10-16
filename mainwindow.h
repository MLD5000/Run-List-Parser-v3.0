/**
    Run List Parser v 3.0
    Parses hexadecimal run list strings to identify data fragments of
    deleted files in NTFS systems.

    Copyright (C) 2014  Matt Danner, mattdanner.cf at gmail dot com.

    This file is part of Run List Parser v 3.0.

    Run List Parser v 3.0 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Run List Parser v 3.0 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Run List Parser v 3.0.  If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void getLastByte();

    void removeSpaces();

    void parseRunList();

    int convertHex(QString);

    void printRunValues();

    void printFragments();

private slots:

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionInstructions_triggered();

    void on_actionAbout_triggered();

    void on_actionParse_Run_List_triggered();

    void on_parseRunListButton_clicked();

private:
    Ui::MainWindow *ui;

    static const int LEN = 1000;

    QString runList,
            newRunList,
            lastRunByte,
            runHeaderL,
            runHeaderR,
            run[LEN],
            runString,
            jumpValueHex[LEN],
            numClustersHex[LEN];



    int runListLength,
        headerIndex = 0,
        index = 0,
        index2 = 0,
        clusterLengthBytes[LEN],
        startingExtentBytes[LEN],
        jumpValue[LEN],
        numClusters[LEN],
        mask,
        startCluster = 0,
        endCluster;


};

#endif // MAINWINDOW_H
