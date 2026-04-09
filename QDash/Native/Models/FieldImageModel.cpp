// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FieldImageModel.h"

#include <cmath>

FieldImageModel::FieldImageModel(QObject *parent) : QObject(parent) {}

// ── Public invokable ─────────────────────────────────────────────────────────

void FieldImageModel::updatePose(double x, double y, double angle)
{
    m_xMeters = x;
    m_yMeters = y;
    m_angleDeg = angle;
    recompute();
}

// ── Private ──────────────────────────────────────────────────────────────────

void FieldImageModel::recompute()
{
    if (m_fieldWidth <= 0.0)
        return;

    const double meterRatio =
        (m_useVerticalField ? m_paintedFieldWidth : m_paintedFieldHeight) / m_fieldWidth;

    const double rW = m_robotLengthMeters * meterRatio;
    const double rH = m_robotWidthMeters * meterRatio;

    const double xPixels = (m_useVerticalField ? -m_yMeters : m_xMeters) * meterRatio;
    const double yPixels = (m_useVerticalField ? m_xMeters : m_yMeters) * meterRatio;

    const double realFieldX = m_fieldImgX + (m_fieldImgWidth - m_paintedFieldWidth) / 2.0;
    const double realFieldY = m_fieldImgY + (m_fieldImgHeight - m_paintedFieldHeight) / 2.0;

    double startX, startY;
    if (m_useVerticalField) {
        startX = realFieldX + m_paintedFieldWidth - rW;
        startY = realFieldY + m_paintedFieldHeight;
    } else {
        startX = realFieldX;
        startY = realFieldY + m_paintedFieldHeight;
    }

    m_robotX = startX + xPixels - (m_useVerticalField ? -rH : rW) / 2.0;
    m_robotY = startY - yPixels - (m_useVerticalField ? rW : rH) / 2.0;
    m_robotW = rW;
    m_robotH = rH;
    m_robotRotation = -m_angleDeg + (m_useVerticalField ? 270.0 : 0.0);

    emit layoutChanged();
}

// ── Input getters ─────────────────────────────────────────────────────────────

double FieldImageModel::xMeters() const { return m_xMeters; }
double FieldImageModel::yMeters() const { return m_yMeters; }
double FieldImageModel::angleDeg() const { return m_angleDeg; }
double FieldImageModel::robotWidthMeters() const { return m_robotWidthMeters; }
double FieldImageModel::robotLengthMeters() const { return m_robotLengthMeters; }
double FieldImageModel::fieldWidth() const { return m_fieldWidth; }
bool FieldImageModel::useVerticalField() const { return m_useVerticalField; }
double FieldImageModel::paintedFieldWidth() const { return m_paintedFieldWidth; }
double FieldImageModel::paintedFieldHeight() const { return m_paintedFieldHeight; }
double FieldImageModel::fieldImgX() const { return m_fieldImgX; }
double FieldImageModel::fieldImgY() const { return m_fieldImgY; }
double FieldImageModel::fieldImgWidth() const { return m_fieldImgWidth; }
double FieldImageModel::fieldImgHeight() const { return m_fieldImgHeight; }

// ── Output getters ────────────────────────────────────────────────────────────

double FieldImageModel::robotX() const { return m_robotX; }
double FieldImageModel::robotY() const { return m_robotY; }
double FieldImageModel::robotW() const { return m_robotW; }
double FieldImageModel::robotH() const { return m_robotH; }
double FieldImageModel::robotRotation() const { return m_robotRotation; }

// ── Input setters ─────────────────────────────────────────────────────────────

void FieldImageModel::setXMeters(double v)
{
    if (qFuzzyCompare(m_xMeters, v))
        return;
    m_xMeters = v;
    emit xMetersChanged();
    recompute();
}

void FieldImageModel::setYMeters(double v)
{
    if (qFuzzyCompare(m_yMeters, v))
        return;
    m_yMeters = v;
    emit yMetersChanged();
    recompute();
}

void FieldImageModel::setAngleDeg(double v)
{
    if (qFuzzyCompare(m_angleDeg, v))
        return;
    m_angleDeg = v;
    emit angleDegChanged();
    recompute();
}

void FieldImageModel::setRobotWidthMeters(double v)
{
    if (qFuzzyCompare(m_robotWidthMeters, v))
        return;
    m_robotWidthMeters = v;
    emit robotWidthMetersChanged();
    recompute();
}

void FieldImageModel::setRobotLengthMeters(double v)
{
    if (qFuzzyCompare(m_robotLengthMeters, v))
        return;
    m_robotLengthMeters = v;
    emit robotLengthMetersChanged();
    recompute();
}

void FieldImageModel::setFieldWidth(double v)
{
    if (qFuzzyCompare(m_fieldWidth, v))
        return;
    m_fieldWidth = v;
    emit fieldWidthChanged();
    recompute();
}

void FieldImageModel::setUseVerticalField(bool v)
{
    if (m_useVerticalField == v)
        return;
    m_useVerticalField = v;
    emit useVerticalFieldChanged();
    recompute();
}

void FieldImageModel::setPaintedFieldWidth(double v)
{
    if (qFuzzyCompare(m_paintedFieldWidth, v))
        return;
    m_paintedFieldWidth = v;
    emit paintedFieldWidthChanged();
    recompute();
}

void FieldImageModel::setPaintedFieldHeight(double v)
{
    if (qFuzzyCompare(m_paintedFieldHeight, v))
        return;
    m_paintedFieldHeight = v;
    emit paintedFieldHeightChanged();
    recompute();
}

void FieldImageModel::setFieldImgX(double v)
{
    if (qFuzzyCompare(m_fieldImgX, v))
        return;
    m_fieldImgX = v;
    emit fieldImgXChanged();
    recompute();
}

void FieldImageModel::setFieldImgY(double v)
{
    if (qFuzzyCompare(m_fieldImgY, v))
        return;
    m_fieldImgY = v;
    emit fieldImgYChanged();
    recompute();
}

void FieldImageModel::setFieldImgWidth(double v)
{
    if (qFuzzyCompare(m_fieldImgWidth, v))
        return;
    m_fieldImgWidth = v;
    emit fieldImgWidthChanged();
    recompute();
}

void FieldImageModel::setFieldImgHeight(double v)
{
    if (qFuzzyCompare(m_fieldImgHeight, v))
        return;
    m_fieldImgHeight = v;
    emit fieldImgHeightChanged();
    recompute();
}
