//
// Created by irouva on 25/07/2021.
//

#include "label_clickable.h"

LabelClickable::LabelClickable(QWidget *parent) : QLabel(parent) {

}

void LabelClickable::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}