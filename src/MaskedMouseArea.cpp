/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "MaskedMouseArea.h"

#include <QStyleHints>
#include <QGuiApplication>
#include <qqmlfile.h>

MaskedMouseArea::MaskedMouseArea(QQuickItem *parent)
    : QQuickItem(parent),
    m_pressed(false),
    m_containsMouse(false),
    m_isLeft(true)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    connect(this, &QQuickItem::widthChanged, this, &MaskedMouseArea::resetPolygon);
    connect(this, &QQuickItem::heightChanged, this, &MaskedMouseArea::resetPolygon);
}

void MaskedMouseArea::setPressed(bool pressed)
{
    if (m_pressed != pressed) {
        m_pressed = pressed;
        emit pressedChanged();
    }
}

void MaskedMouseArea::setContainsMouse(bool containsMouse)
{
    if (m_containsMouse != containsMouse) {
        m_containsMouse = containsMouse;
        emit containsMouseChanged();
    }
}

bool MaskedMouseArea::contains(const QPointF &point) const
{
    if (!QQuickItem::contains(point))
        return false;

    return m_polygon.containsPoint(point, Qt::FillRule::OddEvenFill);
}

void MaskedMouseArea::mousePressEvent(QMouseEvent *event)
{
    setPressed(true);
    m_pressPoint = event->pos();
    emit pressed();
}

void MaskedMouseArea::mouseReleaseEvent(QMouseEvent *event)
{
    setPressed(false);
    emit released();

    const int threshold = qApp->styleHints()->startDragDistance();
    const bool isClick = (threshold >= qAbs(event->position().x() - m_pressPoint.x()) &&
                          threshold >= qAbs(event->position().y() - m_pressPoint.y()));

    if (isClick)
        emit clicked();
}

void MaskedMouseArea::mouseUngrabEvent()
{
    setPressed(false);
    emit canceled();
}

bool MaskedMouseArea::isLeft() const
{
    return m_isLeft;
}

void MaskedMouseArea::setIsLeft(bool newIsLeft)
{
    if (m_isLeft == newIsLeft)
        return;
    m_isLeft = newIsLeft;
    emit isLeftChanged();
}

void MaskedMouseArea::resetPolygon()
{
    m_polygon.clear();

    if (m_isLeft) {
        m_polygon << QPointF(width() / 2, height())
                  << QPointF(width() / 2, 0)
                  << QPointF(0, height());
    } else {
        m_polygon << QPointF(width() / 2, height())
                  << QPointF(width() / 2, 0)
                  << QPointF(width(), height());
    }
}

void MaskedMouseArea::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(true);
}

void MaskedMouseArea::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(false);
}
