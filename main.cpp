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
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
