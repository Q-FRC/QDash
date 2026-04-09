// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <qqmlintegration.h>

// Computes the screen-space geometry of the robot rectangle and heading arrow
// for the Field2d widget.  All floating-point arithmetic that was previously
// performed in QML/JavaScript at ~50 Hz is now done here in C++.
class FieldImageModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // ── Inputs: robot pose (best set together via updatePose()) ──────────────
    Q_PROPERTY(double xMeters READ xMeters WRITE setXMeters NOTIFY xMetersChanged FINAL)
    Q_PROPERTY(double yMeters READ yMeters WRITE setYMeters NOTIFY yMetersChanged FINAL)
    Q_PROPERTY(double angleDeg READ angleDeg WRITE setAngleDeg NOTIFY angleDegChanged FINAL)

    // ── Inputs: robot dimensions ─────────────────────────────────────────────
    Q_PROPERTY(double robotWidthMeters READ robotWidthMeters WRITE setRobotWidthMeters
                   NOTIFY robotWidthMetersChanged FINAL)
    Q_PROPERTY(double robotLengthMeters READ robotLengthMeters WRITE setRobotLengthMeters
                   NOTIFY robotLengthMetersChanged FINAL)

    // ── Inputs: field configuration ──────────────────────────────────────────
    Q_PROPERTY(double fieldWidth READ fieldWidth WRITE setFieldWidth NOTIFY fieldWidthChanged FINAL)
    Q_PROPERTY(bool useVerticalField READ useVerticalField WRITE setUseVerticalField
                   NOTIFY useVerticalFieldChanged FINAL)

    // ── Inputs: painted geometry of the field Image element ──────────────────
    Q_PROPERTY(double paintedFieldWidth READ paintedFieldWidth WRITE setPaintedFieldWidth
                   NOTIFY paintedFieldWidthChanged FINAL)
    Q_PROPERTY(double paintedFieldHeight READ paintedFieldHeight WRITE setPaintedFieldHeight
                   NOTIFY paintedFieldHeightChanged FINAL)
    Q_PROPERTY(double fieldImgX READ fieldImgX WRITE setFieldImgX NOTIFY fieldImgXChanged FINAL)
    Q_PROPERTY(double fieldImgY READ fieldImgY WRITE setFieldImgY NOTIFY fieldImgYChanged FINAL)
    Q_PROPERTY(double fieldImgWidth READ fieldImgWidth WRITE setFieldImgWidth
                   NOTIFY fieldImgWidthChanged FINAL)
    Q_PROPERTY(double fieldImgHeight READ fieldImgHeight WRITE setFieldImgHeight
                   NOTIFY fieldImgHeightChanged FINAL)

    // ── Outputs: computed robot rectangle / arrow geometry ───────────────────
    Q_PROPERTY(double robotX READ robotX NOTIFY layoutChanged FINAL)
    Q_PROPERTY(double robotY READ robotY NOTIFY layoutChanged FINAL)
    Q_PROPERTY(double robotW READ robotW NOTIFY layoutChanged FINAL)
    Q_PROPERTY(double robotH READ robotH NOTIFY layoutChanged FINAL)
    Q_PROPERTY(double robotRotation READ robotRotation NOTIFY layoutChanged FINAL)

public:
    explicit FieldImageModel(QObject *parent = nullptr);

    // Batch-updates all three pose values and triggers a single recompute.
    // Call this from the NT update callback (~50 Hz) to avoid three separate
    // recompute() calls.
    Q_INVOKABLE void updatePose(double x, double y, double angle);

    // Input getters
    double xMeters() const;
    double yMeters() const;
    double angleDeg() const;
    double robotWidthMeters() const;
    double robotLengthMeters() const;
    double fieldWidth() const;
    bool useVerticalField() const;
    double paintedFieldWidth() const;
    double paintedFieldHeight() const;
    double fieldImgX() const;
    double fieldImgY() const;
    double fieldImgWidth() const;
    double fieldImgHeight() const;

    // Output getters
    double robotX() const;
    double robotY() const;
    double robotW() const;
    double robotH() const;
    double robotRotation() const;

    // Input setters
    void setXMeters(double v);
    void setYMeters(double v);
    void setAngleDeg(double v);
    void setRobotWidthMeters(double v);
    void setRobotLengthMeters(double v);
    void setFieldWidth(double v);
    void setUseVerticalField(bool v);
    void setPaintedFieldWidth(double v);
    void setPaintedFieldHeight(double v);
    void setFieldImgX(double v);
    void setFieldImgY(double v);
    void setFieldImgWidth(double v);
    void setFieldImgHeight(double v);

signals:
    void xMetersChanged();
    void yMetersChanged();
    void angleDegChanged();
    void robotWidthMetersChanged();
    void robotLengthMetersChanged();
    void fieldWidthChanged();
    void useVerticalFieldChanged();
    void paintedFieldWidthChanged();
    void paintedFieldHeightChanged();
    void fieldImgXChanged();
    void fieldImgYChanged();
    void fieldImgWidthChanged();
    void fieldImgHeightChanged();

    // Emitted after every recompute; QML output bindings listen to this.
    void layoutChanged();

private:
    void recompute();

    // Input state
    double m_xMeters = 0.0;
    double m_yMeters = 0.0;
    double m_angleDeg = 0.0;
    double m_robotWidthMeters = 0.5;
    double m_robotLengthMeters = 0.5;
    double m_fieldWidth = 8.0692752;
    bool m_useVerticalField = false;
    double m_paintedFieldWidth = 0.0;
    double m_paintedFieldHeight = 0.0;
    double m_fieldImgX = 0.0;
    double m_fieldImgY = 0.0;
    double m_fieldImgWidth = 0.0;
    double m_fieldImgHeight = 0.0;

    // Output state
    double m_robotX = 0.0;
    double m_robotY = 0.0;
    double m_robotW = 0.0;
    double m_robotH = 0.0;
    double m_robotRotation = 0.0;
};
