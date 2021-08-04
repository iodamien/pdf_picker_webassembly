//
// Created by irouva on 25/07/2021.
//

#ifndef IMAGE_SELECTOR_LABEL_CLICKABLE_H
#define IMAGE_SELECTOR_LABEL_CLICKABLE_H

#include <QLabel>

class QMouseEvent;

class LabelClickable : public QLabel {

    Q_OBJECT
public:
    LabelClickable(QWidget* parent = nullptr);

    signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};


#endif //IMAGE_SELECTOR_LABEL_CLICKABLE_H
