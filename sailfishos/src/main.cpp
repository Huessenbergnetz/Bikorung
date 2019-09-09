/* Bikorung - Qt based server service checker
 * Copyright (C) 2019 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Bikorung
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <memory>

#include <QQmlContext>
#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QQmlEngine>

#ifndef CLAZY
#include <sailfishapp.h>
#endif

#include "hbnsc.h"
#include "hbnsciconprovider.h"
#include "licensesmodel.h"
#include "languagesmodel.h"
#include "servermodel.h"
#include "serverfiltermodel.h"
#include "server.h"

int main(int argc, char *argv[])
{
#ifndef CLAZY
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));
#else
    auto app = std::make_unique<QGuiApplication>(argc, argv);
#endif

    app->setApplicationName(QStringLiteral("harbour-bikorung"));
    app->setApplicationDisplayName(QStringLiteral("Bikorung"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

    {
        qDebug("Loading translations from %s", TRANSLATIONS_DIR);
        const QLocale locale;
        qDebug("Detected locale: %s", qUtf8Printable(locale.name()));

        for (const QString &name : {QStringLiteral("bikorung"), QStringLiteral("hbnsc")}) {
            auto trans = new QTranslator(app.get());
            if (Q_LIKELY(trans->load(locale, name, QStringLiteral("_"), QStringLiteral(TRANSLATIONS_DIR), QStringLiteral(".qm")))) {
                if (Q_UNLIKELY(!app->installTranslator(trans))) {
                    qWarning("Can not install translator for component \"%s\" and locale \"%s\".", qUtf8Printable(name), qUtf8Printable(locale.name()));
                }
            } else {
                qWarning("Can not load translations for component \"%s\" and locale \"%s\".", qUtf8Printable(name), qUtf8Printable(locale.name()));
            }
        }
    }

    qmlRegisterType<LanguagesModel>("harbour.bikorung", 1, 0, "LanguageModel");
    qmlRegisterType<LicensesModel>("harbour.bikorung", 1, 0, "LicensesModel");
    qmlRegisterType<Server>("harbour.bikorung", 1, 0, "Server");
    qmlRegisterUncreatableType<ServerModel>("harbour.bikorung", 1, 0, "ServerModel", QStringLiteral("ServerModel is not a creatable type!"));
    qmlRegisterUncreatableType<ServerFilterModel>("harbour.bikorung", 1, 0, "ServerFilterModel", QStringLiteral("ServerFilterModel is not a creatable type!"));

#ifndef CLAZY
    std::unique_ptr<QQuickView> view(SailfishApp::createView());
#else
    auto view = std::make_unique<QQuickView>();
#endif

    auto hbnscIconProvider = Hbnsc::HbnscIconProvider::createProvider(view->engine());

    auto servers = new ServerFilterModel(app.get());

    view->rootContext()->setContextProperty(QStringLiteral("appLauncherIcon"), Hbnsc::getLauncherIcon({86,108,128,150,172}));
    view->rootContext()->setContextProperty(QStringLiteral("servers"), servers);

#ifndef CLAZY
    view->setSource(SailfishApp::pathToMainQml());
#endif

    view->showFullScreen();

    return app->exec();
}
