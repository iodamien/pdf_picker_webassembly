//
// Created by irouva on 25/07/2021.
//

#include "image_selection_widget.h"

#include "label_clickable.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QVariant>

ImageSelectionWidget::ImageSelectionWidget(int index, int imageWidth, int imageHeight, QWidget* parent):
    QWidget(parent),
    index(index) {
    image = new LabelClickable(this);
    radioButton = new QRadioButton(this);

    image->setFixedWidth(imageWidth);
    image->setMaximumHeight(imageHeight);
    image->setStyleSheet("background: { color: red; }");

    auto* layout = new QVBoxLayout();
    layout->addWidget(image);
    layout->addSpacing(20);
    layout->addWidget(radioButton);
    layout->setAlignment(radioButton, Qt::AlignHCenter);
    setLayout(layout);

    setFixedSize(PDF_SNAP_WIDTH, PDF_SNAP_HEIGHT);
    connect(radioButton, &QRadioButton::toggled, this, &ImageSelectionWidget::selectionChanged);
    connect(image, &LabelClickable::clicked, [this]{
        this->radioButton->setChecked(!this->radioButton->isChecked());
    });
}

void ImageSelectionWidget::setPixmap(const QPixmap &pixmap) {
    image->setPixmap(pixmap);
}

void ImageSelectionWidget::selectionChanged(bool isSelected) {
    if (isSelected) {
        image->setStyleSheet("border:2px solid orange;");
    } else {
        image->setStyleSheet("");
    }
}

bool ImageSelectionWidget::isSelected() {
    return radioButton->isChecked();
}

int ImageSelectionWidget::getIndex() {
    return index;
}
