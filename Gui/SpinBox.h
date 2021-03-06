/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2013-2017 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef NATRON_GUI_FEEDBACKSPINBOX_H
#define NATRON_GUI_FEEDBACKSPINBOX_H

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#endif

#include "Engine/DimensionIdx.h"
#include "Engine/ViewIdx.h"
#include "Gui/LineEdit.h"
#include "Gui/GuiFwd.h"


NATRON_NAMESPACE_ENTER;

struct SpinBoxPrivate;

class SpinBox
    : public LineEdit
{

    GCC_DIAG_SUGGEST_OVERRIDE_OFF
    Q_OBJECT
    GCC_DIAG_SUGGEST_OVERRIDE_ON
public:

    enum SpinBoxTypeEnum
    {
        eSpinBoxTypeInt = 0,
        eSpinBoxTypeDouble
    };

    explicit SpinBox(QWidget* parent = 0,
                     SpinBoxTypeEnum type = eSpinBoxTypeInt);

    virtual ~SpinBox() OVERRIDE;

    void setType(SpinBoxTypeEnum type);

    ///Set the digits after the decimal point.
    void decimals(int d);

    ///Min/Max of the spinbox
    void setMaximum(double t);
    void setMinimum(double b);

    double value()
    {
        return text().toDouble();
    }

    ///If OLD_SPINBOX_INCREMENT is defined in SpinBox.cpp, this function does nothing
    ///as the increments are relative to the position of the cursor in the spinbox.
    void setIncrement(double d);

    ///Get a pointer to the right click menu, this can be used to add extra entries. @see KnobGuiTypes.cpp
    QMenu* getRightClickMenu();


    /**
     * @brief Set an optional validator that will validate numbers instead of the regular double/int validator.
     * The spinbox takes ownership of the validator and will destroy it.
     **/
    void setValidator(SpinBoxValidator* validator);

    double getLastValidValueBeforeValidation() const;

protected:

    void increment(int delta, int shift);

    virtual void wheelEvent(QWheelEvent* e) OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* e) OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* e) OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* e) OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* e) OVERRIDE;
    virtual void focusInEvent(QFocusEvent* e) OVERRIDE;
    virtual void focusOutEvent(QFocusEvent* e) OVERRIDE;
    virtual void paintEvent(QPaintEvent* e) OVERRIDE FINAL;

    bool validateText();

    bool validateInternal();

    bool validateWithCustomValidator(const QString& txt);

Q_SIGNALS:

    void valueChanged(double d);

public Q_SLOTS:

    void setValue(double d);

    void setValue(int d);

    /*Used internally when the user pressed enter*/
    void interpretReturn();

private:


    void setValue_internal(double d, bool reformat);

    void setText(const QString &str, int cursorPos);

protected:

    bool ignoreWheelEvent;

private:

    boost::scoped_ptr<SpinBoxPrivate> _imp;
};

class KnobSpinBox
    : public SpinBox
{
public:

    KnobSpinBox(QWidget* parent,
                SpinBoxTypeEnum type,
                const KnobGuiPtr& knob,
                DimIdx dimension,
                ViewIdx view);

    virtual ~KnobSpinBox();

private:

    virtual void wheelEvent(QWheelEvent* e) OVERRIDE FINAL;
    virtual void enterEvent(QEvent* e) OVERRIDE FINAL;
    virtual void leaveEvent(QEvent* e) OVERRIDE FINAL;
    virtual void keyPressEvent(QKeyEvent* e) OVERRIDE FINAL;
    virtual void keyReleaseEvent(QKeyEvent* e) OVERRIDE FINAL;
    virtual void mousePressEvent(QMouseEvent* e) OVERRIDE FINAL;
    virtual void mouseMoveEvent(QMouseEvent* e) OVERRIDE FINAL;
    virtual void mouseReleaseEvent(QMouseEvent* e) OVERRIDE FINAL;
    virtual void dragEnterEvent(QDragEnterEvent* e) OVERRIDE FINAL;
    virtual void dragMoveEvent(QDragMoveEvent* e) OVERRIDE FINAL;
    virtual void dropEvent(QDropEvent* e) OVERRIDE FINAL;
    virtual void focusInEvent(QFocusEvent* e) OVERRIDE FINAL;
    virtual void focusOutEvent(QFocusEvent* e) OVERRIDE FINAL;

private:
    KnobGuiWPtr knob;
    DimIdx dimension;
    ViewIdx view;
    boost::shared_ptr<KnobWidgetDnD> _dnd;
};

NATRON_NAMESPACE_EXIT;

#endif /* defined(NATRON_GUI_FEEDBACKSPINBOX_H_) */
