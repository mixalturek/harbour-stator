/*
 * Copyright (C) 2015 Michal Turek
 *
 * This file is part of Stator application.
 * https://github.com/mixalturek/harbour-stator/
 *
 * Stator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Stator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include "locationreader.h"

#define LOG_FILE "/home/nemo/stator.log"

#ifdef LOG_FILE
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QFile outFile(LOG_FILE);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&outFile);

    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    switch (type) {
        case QtDebugMsg:
            stream << " DEBUG ";
            break;

        case QtWarningMsg:
            stream << " WARN  ";
            break;

        case QtCriticalMsg:
            stream << " CRIT  ";
            break;

        case QtFatalMsg:
            stream << " FATAL ";
            abort();
            break;

        default:
            stream << " ????? ";
            break;
    }

    stream << msg << endl;
}
#endif // LOG_FILE


int main(int argc, char *argv[])
{
#ifdef LOG_FILE
    qInstallMessageHandler(customMessageHandler);
#endif // LOG_FILE

    qDebug() << "===================== STARTING APPLICATION =====================";


    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationName("stator");

    qmlRegisterType<LocationReader>("harbour.stator.LocationReader", 1, 0, "LocationReader");

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathTo("qml/harbour-stator.qml"));
    view->showFullScreen();

    return app->exec();
}
