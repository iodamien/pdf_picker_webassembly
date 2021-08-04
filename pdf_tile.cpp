//
//

#include "pdf_tile.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QThreadPool>
#include "render_tile.h"
#include "render_runnable.h"

PDFTile::PDFTile(uint8_t *buff, QSize size, QPoint renderPos, PDFiumDoc* doc, QWidget *parent) : QWidget(parent),
    buffer(buff),
    pdfDoc(doc) {
    setFixedSize(size);
    connect(this, &PDFTile::onRenderReady, this, &PDFTile::updateRender);
    pdfRenderPos = renderPos;
    buffer = new uint8_t[(int) size.width() * (int) size.height() * 4];
    setUpdatesEnabled(false);
}

PDFTile::~PDFTile() noexcept {
    delete buffer;
}

void PDFTile::paintEvent(QPaintEvent *event) {
//    QWidget::paintEvent(event);
    if (pdfDoc && needRefresh) {
        needRefresh = false;
        const QRect & rect = event->rect();
        QPainter painter(this);
        if (buffer != nullptr) {
            QColor color;
//            painter.fillRect(0, 0, size().width(), size().height(), tmpColor);
            int *currentColor = (int*)buffer;
            for (int y = 0; y < size().height(); y++) {
                for (int x = 0; x < size().width(); x++) {
                    color.setRgba(*currentColor);
                    painter.setPen(color);
                    painter.drawPoint(x, y);
                    currentColor++;
                }
            }
        }
    }
}

void PDFTile::updateRender(uint8_t* data) {
    needRefresh = true;
    setUpdatesEnabled(true);
    repaint();
    setUpdatesEnabled(false);
}

void PDFTile::translate(int offsetX, int offsetY) {
    move(x() + offsetX, y() + offsetY);
}

void PDFTile::setPdfRenderPos(const QPoint &pos) {
    pdfRenderPos = pos;
}

void PDFTile::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    tmpColor = Qt::green;
    repaint();
}

void PDFTile::setScale(float newScale) {
    scale = newScale;
}

void PDFTile::refreshRender() {
//    if (!renderRunnable) {
//        disconnect(renderRunnable);
//        delete renderRunnable;
//    }

    if (pdfDoc) {

//        RenderTile tile {
//            .doc = pdfDoc,
//            .pdfPos = pdfRenderPos,
//            .size = size(),
//            .scale = scale
//        };
//        renderRunnable = new RenderRunnable(std::move(tile));
//        connect(renderRunnable, &RenderRunnable::emitRender, this, &PDFTile::updateRender);
        QThreadPool::globalInstance()->tryStart([this] {
            uint8_t* renderData = pdfDoc->render(pdfRenderPos.x(), pdfRenderPos.y(), size().width(), size().height(), scale, buffer);
//            if (renderData) {
//                delete renderData;
//            }
            emit onRenderReady(renderData);
        });
    }
}

void PDFTile::setPDfDoc(PDFiumDoc *doc) {
    pdfDoc = doc;
    doc->initWithRender(buffer, size().width(), size().height());
}

const PDFiumDoc* PDFTile::getPdfDoc() const {
    return pdfDoc;
}
