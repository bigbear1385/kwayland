/********************************************************************
Copyright 2015  Martin Gräßlin <mgraesslin@kde.org>
Copyright 2015  Marco Martin <mart@kde.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#ifndef KWAYLAND_SERVER_BLUR_INTERFACE_H
#define KWAYLAND_SERVER_BLUR_INTERFACE_H

#include "global.h"
#include "resource.h"

#include <QObject>

#include <KWayland/Server/kwaylandserver_export.h>

namespace KWayland
{
namespace Server
{

class Display;

/**
 * @brief Represents the Global for org_kde_kwin_blur_manager interface.
 *
 * This class creates BlurInterfaces and attaches them to SurfaceInterfaces.
 *
 * @see BlurInterface
 * @see SurfaceInterface
 * @since 5.5
 **/
class KWAYLANDSERVER_EXPORT BlurManagerInterface : public Global
{
    Q_OBJECT
public:
    virtual ~BlurManagerInterface();

private:
    explicit BlurManagerInterface(Display *display, QObject *parent = nullptr);
    friend class Display;
    class Private;
};

/**
 * @brief Represents the Resource for the org_kde_kwin_blur interface.
 *
 * Instances of this class are only generated by the BlurManagerInterface.
 * The BlurInterface gets attached to a SurfaceInterface and can be assessed
 * from there using @link SurfaceInterface::blur() @endlink. Please note that
 * the BlurInterface is only available on the SurfaceInterface after it has been
 * committed.
 *
 * @see BlurManagerInterface
 * @see SurfaceInterface
 * @since 5.5
 **/
class KWAYLANDSERVER_EXPORT BlurInterface : public Resource
{
    Q_OBJECT
public:
    virtual ~BlurInterface();

    /**
     * @returns The region or the SurfaceInterface which should be blurred, null Region implies complete surface.
     **/
    QRegion region();

private:
    explicit BlurInterface(BlurManagerInterface *parent, wl_resource *parentResource);
    friend class BlurManagerInterface;

    class Private;
    Private *d_func() const;
};

}
}

#endif
