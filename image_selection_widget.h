//
// Created by irouva on 25/07/2021.
//

#ifndef IMAGE_SELECTOR_IMAGE_SELECTION_WIDGET_H
#define IMAGE_SELECTOR_IMAGE_SELECTION_WIDGET_H

#include <QWidget>

class LabelClickable;
class QRadioButton;

#define PDF_SNAP_WIDTH 320
#define PDF_SNAP_HEIGHT 480
#define PDF_IMAGE_BYTE 4
#define PDF_WIDTH_COUNT 3


class ImageSelectionWidget : public QWidget {

    Q_OBJECT

public:
    explicit ImageSelectionWidget(int index, int imageWidth, int imageHeight, QWidget* parent = nullptr);
    void setPixmap(const QPixmap& pixmap);
    bool isSelected();
    int getIndex();

private:
    LabelClickable* image;
    QRadioButton *radioButton;
    int index;

public slots:
    void selectionChanged(bool isSelected);

};


#endif //IMAGE_SELECTOR_IMAGE_SELECTION_WIDGET_H
