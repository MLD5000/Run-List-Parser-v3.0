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
#include <cmath>

//--- Default constructor --------------------
//--------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

//--- Default destructor ---
//--------------------------
MainWindow::~MainWindow()
{
    delete ui;
}




//--- Opens text file data in the input text box, from context menu -
//-------------------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

       if (!fileName.isEmpty())
       {
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




//--- Saves output data to text file from context menu -------
//------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
               tr("Word Document (*.doc);;C++ Files (*.cpp *.h)"));

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




//--- Button that executes parse action ------------------
//--------------------------------------------------------
void MainWindow::on_parseRunListButton_clicked()
{
    QTextStream stream(&runList);
    stream << ui->textEdit->toPlainText();
    stream.flush();

    getLastByte();

    removeSpaces();

    parseRunList();

    printRunValues();

    printFragments();

    runList = "";
    newRunList = "";
    index2 = 0;
    headerIndex = 0;
    endCluster = 0;

}




//--- store input into string variable and outputs it to lower text box ---
//-------------------------------------------------------------------------
void MainWindow::on_actionParse_Run_List_triggered()
{
    QTextStream stream(&runList);
    stream << ui->textEdit->toPlainText();
    stream.flush();

    getLastByte();

    removeSpaces();

    parseRunList();

    printRunValues();

    printFragments();

    runList = "";
    newRunList = "";
    index2 = 0;
    headerIndex = 0;
    endCluster = 0;
}



//--- Loads instructions to information window from context menu ---------
//------------------------------------------------------------------------
void MainWindow::on_actionInstructions_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Instructions");
    msgBox.setText("Enter a complete hexadecimal runlist string with"
                   " or without spaces and be sure  to end the run with "
                   "00 and press Enter.\n\n"
                   "Enter the runlist as you see it in the hex editor, "
                   "the program will account for endianess.\n\n"
                   "Hint: Copy the hex string from the hex editor and "
                   "paste it into the console.");
    msgBox.exec();
}




//--- launches "about" window with general information ------------------------------------
//-----------------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Run List Parser v 3.0.\n"
                                             "Parses hexadecimal run list strings "
                                             "to identify data fragments of "
                                             "deleted files in NTFS systems.\n\n"
                                             "Copyright 2014.\n"
                                             "Matt Danner, mattdanner.cf@gmail.com\n"));

}




//--- checks for run list end byte signature 00. ---------------------------
//--------------------------------------------------------------------------
void MainWindow::getLastByte()
{

    runListLength = runList.length();

    // gets last two bytes of run list
    lastRunByte = runList[runListLength - 2];
    lastRunByte += runList[runListLength - 1];

    if (lastRunByte != "00")
    {
        QMessageBox::warning(this, tr("Warning"), tr("NOTE - Last byte is not 00."));
        runList += " 00";
    }
}



//--- Removes spaces from run list input -------
//----------------------------------------------
void MainWindow::removeSpaces()
{
    for (int i = 0; i < runList.length(); i++)
    {
        // remove spaces from input
        if (runList[i] != ' ')
        {
            newRunList += runList[i];
        }
    }
}


//--- Parses bytes of run list string
//--------------------------------------
void MainWindow::parseRunList()
{

    ui->textEdit_2->clear();

    // store run header
    runHeaderL = newRunList[headerIndex];
    runHeaderR = newRunList[headerIndex+1];

    // exit while loop when end of run reached
    while(runHeaderL != "0" && runHeaderR != "0")
    {

        // convert run header from string char to integers
        clusterLengthBytes[index2] = convertHex(runHeaderR);
        startingExtentBytes[index2] = convertHex(runHeaderL);

        // store individual runs for output report
        for ( index = headerIndex; index < (((clusterLengthBytes[index2] * 2) + (startingExtentBytes[index2] * 2) + 2) + headerIndex ); index++ )
        {
            run[index2] += newRunList[index];
        }

        // temp location for each run within run list
        runString = run[index2];

        // reverse the jump value to little endian notation and store each jump value into an array
        for (int j = ( clusterLengthBytes[index2] * 2) + (startingExtentBytes[index2] * 2); j >= (clusterLengthBytes[index2]*2) + 2; j -= 2 )
        {
            jumpValueHex[index2] += runString[j];
            jumpValueHex[index2] += runString[j+1];

            // convert jump value string to integer and store in array

            bool ok;
            jumpValue[index2] = jumpValueHex[index2].toInt(&ok, 16);
        }

        // extract number of clusters in the fragment and store in an array in little endian
        for ( int j = (clusterLengthBytes[index2] * 2); j >= 2; j -= 2 )
        {
            numClustersHex[index2] += runString[j];
            numClustersHex[index2] += runString[j+1];

            // convert number of cluster hex values to int
            bool ok;
            numClusters[index2] = numClustersHex[index2].toInt(&ok, 16);

            numClustersHex[index2] = "";    // clear array for use in next run list
        }


        // mask will be assigned a value appropriate for the size of the run to check for negative integer notation (2's complement)
        mask = pow(2, ((startingExtentBytes[index2] * 8) - 1));

        // determines if most significant bit is 1, then converts integer to negative number by reverse 2's complement
        if ( ((jumpValue[index2] & mask) == mask) && index2 > 0)
        {
            mask = (pow(2, startingExtentBytes[index2] * 8) - 1);
            jumpValue[index2] -= 1;
            jumpValue[index2] = (jumpValue[index2] ^ mask) * -1;
        }

        // points the header index to the next run header in the run list
        headerIndex += ((clusterLengthBytes[index2] * 2) + (startingExtentBytes[index2] * 2) + 2);
        index2++;

        // get next run header values
        runHeaderL = newRunList[headerIndex];
        runHeaderR = newRunList[headerIndex+1];
    }

}



// this function converts the run header values to integers
int MainWindow::convertHex(QString runHeader)
{
    if (runHeader == "0") return 0;

    else if (runHeader == "1") return 1;

    else if (runHeader == "2") return 2;

    else if (runHeader == "3") return 3;

    else if (runHeader == "4") return 4;

    else if (runHeader == "5") return 5;

    else if (runHeader == "6") return 6;

    else if (runHeader == "7") return 7;

    else if (runHeader == "8") return 8;

    else if (runHeader == "9") return 9;

    else return 0;
}



//--- prints run values
//----------------------
void MainWindow::printRunValues()
{
    ui->textEdit_2->append("Individual Runs: \n"
                           "-------------------------------------\n"
                           "-------------------------------------\n");


    for (int j = 0; j < index2; j++)
    {
        ui->textEdit_2->append("Run " + QString::number(j + 1) + ": ");
        //ui->textEdit_2->append("-------------------------------------");
        ui->textEdit_2->append("Run: " + run[j]);

        run[j] = "";

        if (j == 0)
        {
            ui->textEdit_2->append("Starting Cluster in Hex: " + jumpValueHex[j]);
            ui->textEdit_2->append("Starting Cluster in Dec: " + QString::number(jumpValue[j]) + "\n");
        }

        else
        {
            ui->textEdit_2->append("Jump Value in Hex: " + jumpValueHex[j]);
            ui->textEdit_2->append("Jump Value in Dec: " + QString::number(jumpValue[j]) + "\n");
        }

        jumpValueHex[j] = "";
    }

}

//--- Prints fragments for run list
//------------------------------------
void MainWindow::printFragments()
{
    ui->textEdit_2->append("File Fragments: \n"
                           "-------------------------------------\n"
                           "-------------------------------------\n");

    startCluster = jumpValue[0];

    for (int j = 0; j < index2; j++)
    {
        ui->textEdit_2->append("Fragment " + QString::number(j + 1) + ": ");

        if (j == 0)
        {
            endCluster = jumpValue[j];
            endCluster += numClusters[j];
            ui->textEdit_2->append("Clusters " + QString::number(jumpValue[j]) + " - " + QString::number(endCluster) + "\n");
            ui->textEdit_2->append("Total clusters in fragment: " + QString::number(numClusters[j]) + "\n");
        }

        else
        {
            startCluster += jumpValue[j];
            endCluster = startCluster + numClusters[j];
            ui->textEdit_2->append("Clusters " + QString::number(startCluster) + " - " + QString::number(endCluster) + "\n");
            ui->textEdit_2->append("Total clusters in fragment: " + QString::number(numClusters[j]) + "\n");
        }

        jumpValue[j] = 0;
        numClusters[j] = 0;
    }

}
