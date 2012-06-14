/*
 * qdoubleedit.h
 *
 *  Created on: 24 jul 2010
 *      Author: Mark Hurenkamp
 */

#ifndef QDOUBLEEDIT_H_
#define QDOUBLEEDIT_H_

#include <QLineEdit>
#include <QDoubleValidator>

class QDoubleEdit : public QLineEdit
{
public:
    QDoubleEdit(const double input, QWidget *parent) : QLineEdit(QVariant(input).toString(), parent)
    {
        setValidator(new QDoubleValidator(this));
    }
    double number()                    { return text().toDouble(); }
    void setNumber(const double value) { setText(QVariant(value).toString()); }
};

#endif /* QDOUBLEEDIT_H_ */
