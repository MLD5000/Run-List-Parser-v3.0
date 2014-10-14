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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

       if (!fileName.isEmpty()) {
           QFile file(fileName);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
               return;
           }
           QTextStream in(&file);
           ui->textEdit->setText(in.readAll());
           file.close();
}
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
               tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

       if (!fileName.isEmpty()) {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               // error message
           } else {
               QTextStream stream(&file);
               stream << ui->textEdit_2->toPlainText();
               stream.flush();
               file.close();
           }
       }
}

void MainWindow::on_parseRunListButton_clicked()
{
    QTextStream stream(&runList);
    stream << ui->textEdit->toPlainText();
    stream.flush();

    ui->textEdit_2->setPlainText(QString(runList));

    getLastByte();

    runList = "";

}

void MainWindow::on_actionInstructions_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Instructions");
    msgBox.setText("Enter a complete hexadecimal runlist string with or without spaces and be sure  to end the run with 00 and press Enter.\n\n"
                   "Enter the runlist as you see it in the hex editor, the program will account for endianess.\n\n"
                   "Hint: Copy the hex string from the hex editor and paste it into the console.");
    msgBox.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Run List Parser v 3.0.\n"
                                             "Parses hexadecimal run list strings to identify data fragments of "
                                             "deleted files in NTFS systems.\n\n"
                                             "Copyright 2014.\n"
                                             "Matt Danner, mattdanner.cf@gmail.com\n"));

}

void MainWindow::on_actionParse_Run_List_triggered()
{
    QTextStream stream(&runList);
    stream << ui->textEdit->toPlainText();
    stream.flush();

    ui->textEdit_2->setPlainText(QString(runList));

    getLastByte();

    runList = "";
}

void MainWindow::getLastByte()
{

    runListLength = runList.length();
    lastRunByte = runList[runListLength - 2];
    lastRunByte += runList[runListLength - 1];

    if (lastRunByte != "00")
    {
        QMessageBox::warning(this, tr("Warning"), tr("NOTE - Last byte is not 00."));
    }
}

