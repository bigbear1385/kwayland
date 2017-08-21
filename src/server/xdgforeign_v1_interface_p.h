/****************************************************************************
Copyright 2017  Marco Martin <notmart@gmail.com>

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
****************************************************************************/
#ifndef KWAYLAND_SERVER_XDGFOREIGNV1_P_H
#define KWAYLAND_SERVER_XDGFOREIGNV1_P_H

#include "global.h"
#include "resource.h"

namespace KWayland
{
namespace Server
{

class Display;
class SurfaceInterface;
class XdgExportedUnstableV1Interface;
class XdgImportedUnstableV1Interface;

class Q_DECL_HIDDEN XdgForeignUnstableInterface::Private
{
public:
    Private(Display *display, XdgForeignUnstableInterface *q);

    XdgForeignUnstableInterface *q;
    XdgExporterUnstableV1Interface *exporter;
    XdgImporterUnstableV1Interface *importer;
};

class Q_DECL_HIDDEN XdgExporterUnstableV1Interface : public Global
{
    Q_OBJECT
public:
    virtual ~XdgExporterUnstableV1Interface();

    XdgExportedUnstableV1Interface *exportedSurface(const QString &handle);

Q_SIGNALS:
    void surfaceExported(const QString &handle, XdgExportedUnstableV1Interface *exported);
    void surfaceUnexported(const QString &handle);

private:
    explicit XdgExporterUnstableV1Interface(Display *display, XdgForeignUnstableInterface *parent = nullptr);
    friend class Display;
    friend class XdgForeignUnstableInterface;
    class Private;
    Private *d_func() const;
};

class Q_DECL_HIDDEN XdgImporterUnstableV1Interface : public Global
{
    Q_OBJECT
public:
    virtual ~XdgImporterUnstableV1Interface();

    XdgImportedUnstableV1Interface *importedSurface(const QString &handle);
    SurfaceInterface *transientFor(SurfaceInterface *surface);

Q_SIGNALS:
    void surfaceImported(const QString &handle, XdgImportedUnstableV1Interface *imported);
    void surfaceUnimported(const QString &handle);
    void transientChanged(KWayland::Server::SurfaceInterface *child, KWayland::Server::SurfaceInterface *parent);

private:
    explicit XdgImporterUnstableV1Interface(Display *display, XdgForeignUnstableInterface *parent = nullptr);
    friend class Display;
    friend class XdgForeignUnstableInterface;
    class Private;
    Private *d_func() const;
};

class Q_DECL_HIDDEN XdgExportedUnstableV1Interface : public Resource
{
    Q_OBJECT
public:
    virtual ~XdgExportedUnstableV1Interface();

private:
    explicit XdgExportedUnstableV1Interface(XdgExporterUnstableV1Interface *parent, wl_resource *parentResource);
    friend class XdgExporterUnstableV1Interface;

    class Private;
    Private *d_func() const;
};

class Q_DECL_HIDDEN XdgImportedUnstableV1Interface : public Resource
{
    Q_OBJECT
public:
    virtual ~XdgImportedUnstableV1Interface();

    SurfaceInterface *child() const;

Q_SIGNALS:
    void childChanged(KWayland::Server::SurfaceInterface *child);

private:
    explicit XdgImportedUnstableV1Interface(XdgImporterUnstableV1Interface *parent, wl_resource *parentResource);
    friend class XdgImporterUnstableV1Interface;

    class Private;
    Private *d_func() const;
};

}
}

#endif