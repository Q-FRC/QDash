// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "QDashApplication.h"

int main(int argc, char *argv[])
{
// TODO
#ifdef __linux__
    qputenv("QT_QPA_PLATFORMTHEME", "gtk3");
#endif

    QDashApplication app(argc, argv);

    return app.run();
}
