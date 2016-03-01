/********************************************************************
Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>

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
#ifndef WAYLAND_SERVER_SEAT_INTERFACE_H
#define WAYLAND_SERVER_SEAT_INTERFACE_H

#include <QObject>
#include <QPoint>
#include <QMatrix4x4>

#include <KWayland/Server/kwaylandserver_export.h>
#include "global.h"
#include "keyboard_interface.h"
#include "pointer_interface.h"
#include "touch_interface.h"

struct wl_client;
struct wl_resource;

namespace KWayland
{
namespace Server
{

class DataDeviceInterface;
class Display;
class SurfaceInterface;

/**
 * @brief Represents a Seat on the Wayland Display.
 *
 * A Seat is a set of input devices (e.g. Keyboard, Pointer and Touch) the client can connect
 * to. The server needs to announce which input devices are supported and passes dedicated input
 * focus to a SurfaceInterface. Only the focused surface receives input events.
 *
 * The SeatInterface internally handles enter and release events when setting a focused surface.
 * Also it handles input translation from global to the local coordination, removing the need from
 * the user of the API to track the focused surfaces and can just interact with this class.
 *
 * To create a SeatInterface use @link Display::createSeat @endlink. Then one can set up what is
 * supported. Last but not least create needs to be called.
 *
 * @code
 * SeatInterface *seat = display->createSeat();
 * // set up
 * seat->setName(QStringLiteral("seat0"));
 * seat->setHasPointer(true);
 * seat->setHasKeyboard(true);
 * seat->setHasTouch(false);
 * // now fully create
 * seat->create();
 * @endcode
 *
 * To forward input events one needs to set the focused surface, update time stamp and then
 * forward the actual events:
 *
 * @code
 * // example for pointer
 * seat->setFocusedPointerSurface(surface, QPointF(100, 200)); // surface at it's global position
 * seat->setTimestamp(100);
 * seat->setPointerPos(QPointF(350, 210)); // global pos, local pos in surface: 250,10
 * seat->setTimestamp(110);
 * seat->pointerButtonPressed(Qt::LeftButton);
 * seat->setTimestamp(120);
 * seat->pointerButtonReleased(Qt::LeftButton);
 * @endcode
 *
 * @see KeyboardInterface
 * @see PointerInterface
 * @see TouchInterface
 * @see SurfaceInterface
 **/
class KWAYLANDSERVER_EXPORT SeatInterface : public Global
{
    Q_OBJECT
    /**
     * The name of the Seat
     **/
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /**
     * Whether the SeatInterface supports a pointer device.
     **/
    Q_PROPERTY(bool pointer READ hasPointer WRITE setHasPointer NOTIFY hasPointerChanged)
    /**
     * Whether the SeatInterface supports a keyboard device.
     **/
    Q_PROPERTY(bool keyboard READ hasKeyboard WRITE setHasKeyboard NOTIFY hasKeyboardChanged)
    /**
     * Whether the SeatInterface supports a touch device.
     * @deprecated use touch
     **/
    Q_PROPERTY(bool tourch READ hasTouch WRITE setHasTouch NOTIFY hasTouchChanged)
    /**
     * Whether the SeatInterface supports a touch device.
     **/
    Q_PROPERTY(bool touch READ hasTouch WRITE setHasTouch NOTIFY hasTouchChanged)
    /**
     * The global pointer position.
     **/
    Q_PROPERTY(QPointF pointerPos READ pointerPos WRITE setPointerPos NOTIFY pointerPosChanged)
    /**
     * The current timestamp passed to the input events.
     **/
    Q_PROPERTY(quint32 timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
public:
    virtual ~SeatInterface();

    QString name() const;
    bool hasPointer() const;
    bool hasKeyboard() const;
    bool hasTouch() const;

    void setName(const QString &name);
    void setHasPointer(bool has);
    void setHasKeyboard(bool has);
    void setHasTouch(bool has);

    void setTimestamp(quint32 time);
    quint32 timestamp() const;

    /**
     * @name Drag'n'Drop related methods
     **/
    ///@{
    /**
     * @returns whether there is currently a drag'n'drop going on.
     * @since 5.6
     * @see isDragPointer
     * @see isDragTouch
     * @see dragStarted
     * @see dragEnded
     **/
    bool isDrag() const;
    /**
     * @returns whether the drag'n'drop is operated through the pointer device
     * @since 5.6
     * @see isDrag
     * @see isDragTouch
     **/
    bool isDragPointer() const;
    /**
     * @returns whether the drag'n'drop is operated through the touch device
     * @since 5.6
     * @see isDrag
     * @see isDragPointer
     **/
    bool isDragTouch() const;
    /**
     * @returns The transformation applied to go from global to local coordinates for drag motion events.
     * @see dragSurfaceTransformation
     * @since 5.6
     **/
    QMatrix4x4 dragSurfaceTransformation() const;
    /**
     * @returns The currently focused Surface for drag motion events.
     * @since 5.6
     * @see dragSurfaceTransformation
     * @see dragSurfaceChanged
     **/
    SurfaceInterface *dragSurface() const;
    /**
     * @returns The PointerInterface which triggered the drag operation
     * @since 5.6
     * @see isDragPointer
     **/
    PointerInterface *dragPointer() const;
    /**
     * @returns The DataDeviceInterface which started the drag and drop operation.
     * @see isDrag
     * @since 5.6
     **/
    DataDeviceInterface *dragSource() const;
    /**
     * Sets the current drag target to @p surface.
     *
     * Sends a drag leave event to the current target and an enter event to @p surface.
     * The enter position is derived from @p globalPosition and transformed by @p inputTransformation.
     * @since 5.6
     **/
    void setDragTarget(SurfaceInterface *surface, const QPointF &globalPosition, const QMatrix4x4 &inputTransformation);
    /**
     * Sets the current drag target to @p surface.
     *
     * Sends a drag leave event to the current target and an enter event to @p surface.
     * The enter position is derived from current global position and transformed by @p inputTransformation.
     * @since 5.6
     **/
    void setDragTarget(SurfaceInterface *surface, const QMatrix4x4 &inputTransformation = QMatrix4x4());
    ///@}

    /**
     * @name Pointer related methods
     **/
    ///@{
    /**
     * Updates the global pointer @p pos.
     *
     * Sends a pointer motion event to the focused pointer surface.
     **/
    void setPointerPos(const QPointF &pos);
    /**
     * @returns the global pointer position
     **/
    QPointF pointerPos() const;
    /**
     * Sets the focused pointer @p surface.
     * All pointer events will be sent to the @p surface till a new focused pointer surface gets
     * installed. When the focus pointer surface changes a leave event is sent to the previous
     * focused surface.
     *
     * To unset the focused pointer surface pass @c nullptr as @p surface.
     *
     * Pointer motion events are adjusted to the local position based on the @p surfacePosition.
     * If the surface changes it's position in the global coordinate system
     * use setFocusedPointerSurfacePosition to update.
     * The surface position is used to create the base transformation matrix to go from global
     * to surface local coordinates. The default generated matrix is a translation with
     * negative @p surfacePosition.
     *
     * @param surface The surface which should become the new focused pointer surface.
     * @param surfacePosition The position of the surface in the global coordinate system
     *
     * @see setPointerPos
     * @see focucedPointerSurface
     * @see focusedPointer
     * @see setFocusedPointerSurfacePosition
     * @see focusedPointerSurfacePosition
     * @see setFocusedPointerSurfaceTransformation
     * @see focusedPointerSurfaceTransformation
     **/
    void setFocusedPointerSurface(SurfaceInterface *surface, const QPointF &surfacePosition = QPoint());
    /**
     * Sets the focused pointer @p surface.
     * All pointer events will be sent to the @p surface till a new focused pointer surface gets
     * installed. When the focus pointer surface changes a leave event is sent to the previous
     * focused surface.
     *
     * To unset the focused pointer surface pass @c nullptr as @p surface.
     *
     * Pointer motion events are adjusted to the local position based on the @p transformation.
     * If the surface changes it's position in the global coordinate system
     * use setFocusedPointerSurfaceTransformation to update.
     *
     * @param surface The surface which should become the new focused pointer surface.
     * @param transformation The transformation to transform global into local coordinates
     *
     * @see setPointerPos
     * @see focucedPointerSurface
     * @see focusedPointer
     * @see setFocusedPointerSurfacePosition
     * @see focusedPointerSurfacePosition
     * @see setFocusedPointerSurfaceTransformation
     * @see focusedPointerSurfaceTransformation
     * @since 5.6
     **/
    void setFocusedPointerSurface(SurfaceInterface *surface, const QMatrix4x4 &transformation);
    /**
     * @returns The currently focused pointer surface, that is the surface receiving pointer events.
     * @see setFocusedPointerSurface
     **/
    SurfaceInterface *focusedPointerSurface() const;
    /**
     * @returns The PointerInterface belonging to the focused pointer surface, if any.
     * @see setFocusedPointerSurface
     **/
    PointerInterface *focusedPointer() const;
    /**
     * Updates the global position of the currently focused pointer surface.
     *
     * Updating the focused surface position also generates a new transformation matrix.
     * The default generated matrix is a translation with negative @p surfacePosition.
     * If a different transformation is required a dedicated call to
     * @link setFocusedPointerSurfaceTransformation is required.
     *
     * @param surfacePosition The new global position of the focused pointer surface
     * @see focusedPointerSurface
     * @see setFocusedPointerSurface
     * @see focusedPointerSurfaceTransformation
     * @see setFocusedPointerSurfaceTransformation
     **/
    void setFocusedPointerSurfacePosition(const QPointF &surfacePosition);
    /**
     * @returns The position of the focused pointer surface in global coordinates.
     * @see setFocusedPointerSurfacePosition
     * @see setFocusedPointerSurface
     * @see focusedPointerSurfaceTransformation
     **/
    QPointF focusedPointerSurfacePosition() const;
    /**
     * Sets the @p transformation for going from global to local coordinates.
     *
     * The default transformation gets generated from the surface position and reset whenever
     * the surface position changes.
     *
     * @see focusedPointerSurfaceTransformation
     * @see focusedPointerSurfacePosition
     * @see setFocusedPointerSurfacePosition
     * @since 5.6
     **/
    void setFocusedPointerSurfaceTransformation(const QMatrix4x4 &transformation);
    /**
     * @returns The transformation applied to pointer position to go from global to local coordinates.
     * @see setFocusedPointerSurfaceTransformation
     * @since 5.6
     **/
    QMatrix4x4 focusedPointerSurfaceTransformation() const;
    /**
     * Marks the @p button as pressed.
     *
     * If there is a focused pointer surface a button pressed event is sent to it.
     *
     * @param button The Linux button code
     **/
    void pointerButtonPressed(quint32 button);
    /**
     * @overload
     **/
    void pointerButtonPressed(Qt::MouseButton button);
    /**
     * Marks the @p button as released.
     *
     * If there is a focused pointer surface a button release event is sent to it.
     *
     * @param button The Linux button code
     **/
    void pointerButtonReleased(quint32 button);
    /**
     * @overload
     **/
    void pointerButtonReleased(Qt::MouseButton button);
    /**
     * @returns whether the @p button is pressed
     **/
    bool isPointerButtonPressed(quint32 button) const;
    /**
     * @returns whether the @p button is pressed
     **/
    bool isPointerButtonPressed(Qt::MouseButton button) const;
    /**
     * @returns the last serial for @p button.
     **/
    quint32 pointerButtonSerial(quint32 button) const;
    /**
     * @returns the last serial for @p button.
     **/
    quint32 pointerButtonSerial(Qt::MouseButton button) const;
    void pointerAxis(Qt::Orientation orientation, quint32 delta);
    /**
     * @returns true if there is a pressed button with the given @p serial
     * @since 5.6
     **/
    bool hasImplicitPointerGrab(quint32 serial) const;
    ///@}

    /**
     * @name keyboard related methods
     **/
    ///@{
    void setKeymap(int fd, quint32 size);
    void keyPressed(quint32 key);
    void keyReleased(quint32 key);
    void updateKeyboardModifiers(quint32 depressed, quint32 latched, quint32 locked, quint32 group);
    /**
     * Sets the key repeat information to be forwarded to all bound keyboards.
     *
     * To disable key repeat set a @p charactersPerSecond of @c 0.
     *
     * Requires wl_seat version 4.
     *
     * @param charactersPerSecond The characters per second rate, value of @c 0 disables key repeating
     * @param delay The delay on key press before starting repeating keys
     *
     * @since 5.5
     ***/
    void setKeyRepeatInfo(qint32 charactersPerSecond, qint32 delay);
    quint32 depressedModifiers() const;
    quint32 latchedModifiers() const;
    quint32 lockedModifiers() const;
    quint32 groupModifiers() const;
    quint32 lastModifiersSerial() const;
    int keymapFileDescriptor() const;
    quint32 keymapSize() const;
    bool isKeymapXkbCompatible() const;
    QVector<quint32> pressedKeys() const;
    /**
     * @returns The key repeat in character per second
     * @since 5.5
     * @see setKeyRepeatInfo
     * @see keyRepeatDelay
     **/
    qint32 keyRepeatRate() const;
    /**
     * @returns The delay on key press before starting repeating keys
     * @since 5.5
     * @see keyRepeatRate
     * @see setKeyRepeatInfo
     **/
    qint32 keyRepeatDelay() const;

    void setFocusedKeyboardSurface(SurfaceInterface *surface);
    SurfaceInterface *focusedKeyboardSurface() const;
    KeyboardInterface *focusedKeyboard() const;
    ///@}

    /**
     * @name  touch related methods
     **/
    ///@{
    void setFocusedTouchSurface(SurfaceInterface *surface, const QPointF &surfacePosition = QPointF());
    SurfaceInterface *focusedTouchSurface() const;
    TouchInterface *focusedTouch() const;
    void setFocusedTouchSurfacePosition(const QPointF &surfacePosition);
    QPointF focusedTouchSurfacePosition() const;
    qint32 touchDown(const QPointF &globalPosition);
    void touchUp(qint32 id);
    void touchMove(qint32 id, const QPointF &globalPosition);
    void touchFrame();
    void cancelTouchSequence();
    bool isTouchSequence() const;
    ///@}

    static SeatInterface *get(wl_resource *native);

Q_SIGNALS:
    void nameChanged(const QString&);
    void hasPointerChanged(bool);
    void hasKeyboardChanged(bool);
    void hasTouchChanged(bool);
    void pointerPosChanged(const QPointF &pos);
    void timestampChanged(quint32);

    void pointerCreated(KWayland::Server::PointerInterface*);
    void keyboardCreated(KWayland::Server::KeyboardInterface*);
    void touchCreated(KWayland::Server::TouchInterface*);

    /**
     * Emitted whenever the focused pointer changes
     * @since 5.6
     **/
    void focusedPointerChanged(KWayland::Server::PointerInterface*);

    /**
     * Emitted when a drag'n'drop operation is started
     * @since 5.6
     * @see dragEnded
     **/
    void dragStarted();
    /**
     * Emitted when a drag'n'drop operation ended, either by dropping or canceling.
     * @since 5.6
     * @see dragStarted
     **/
    void dragEnded();
    /**
     * Emitted whenever the drag surface for motion events changed.
     * @since 5.6
     * @see dragSurface
     **/
    void dragSurfaceChanged();

private:
    friend class Display;
    friend class DataDeviceManagerInterface;
    explicit SeatInterface(Display *display, QObject *parent);

    class Private;
    Private *d_func() const;
};

}
}

Q_DECLARE_METATYPE(KWayland::Server::SeatInterface*)

#endif
