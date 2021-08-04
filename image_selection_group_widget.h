
//
// Created by irouva on 25/07/2021.
//

#ifndef IMAGE_SELECTOR_IMAGE_SELECTION_GROUP_WIDGET_H
#define IMAGE_SELECTOR_IMAGE_SELECTION_GROUP_WIDGET_H

#include "image_selection_widget.h"
#include <QScrollArea>
#include <QList>

class QGridLayout;
class QPixmap;

class ImageSelectionGroupWidget : public QScrollArea {

    Q_OBJECT

public:
    ImageSelectionGroupWidget(QWidget* parent = nullptr);
    QList<int> getSelected();
    void addImage(const QPixmap& pixmap, int index);
    void setSize(int size);

//    signals:
//        int selectionHasChanged(int count);

private:
    QWidget *container;
    QGridLayout *gridLayout;
};


#endif //IMAGE_SELECTOR_IMAGE_SELECTION_GROUP_WIDGET_H
