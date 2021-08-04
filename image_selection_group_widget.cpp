//
// Created by irouva on 25/07/2021.
//

#include "image_selection_group_widget.h"
#include "image_selection_widget.h"

#include <QScrollArea>
#include <QImage>
#include <QPixmap>
#include <QGridLayout>

ImageSelectionGroupWidget::ImageSelectionGroupWidget(QWidget* parent): QScrollArea(parent) {

    container = new QWidget(this);
    gridLayout = new QGridLayout(container);

    container->setLayout(gridLayout);
    setWidget(container);
}

QList<int> ImageSelectionGroupWidget::getSelected() {
    QList<int> list;
    int count = gridLayout->count();
    for (int i = 0; i < count; i++) {
        auto* item = gridLayout->itemAt(i);
        auto* widget = item->widget();
        auto* imageSelection = dynamic_cast<ImageSelectionWidget*>(widget);
        if (imageSelection && imageSelection->isSelected()) {
            list.push_back(imageSelection->getIndex());
        }
    }
    return list;
}

void ImageSelectionGroupWidget::addImage(const QPixmap &pixmap, int index) {
    auto* imageSelectionWidget = new ImageSelectionWidget(index, PDF_SNAP_WIDTH, PDF_SNAP_HEIGHT, container);
    imageSelectionWidget->setPixmap(pixmap);
    int currentSize = gridLayout->count();
    gridLayout->addWidget(imageSelectionWidget, currentSize / PDF_WIDTH_COUNT, currentSize % PDF_WIDTH_COUNT);
}

void ImageSelectionGroupWidget::setSize(int size) {
    int pair = (size / PDF_WIDTH_COUNT) + 1;
    int width = PDF_WIDTH_COUNT * PDF_SNAP_WIDTH + 5 * PDF_WIDTH_COUNT;
    int height = PDF_SNAP_HEIGHT * pair + 20 * pair;
    container->setFixedSize(width, height);
}