//
//

#include "pdf_widget.h"
#include "pdf_tile.h"

#include <QPaintEvent>
#include <QPainter>

#include "render_dispatcher.h"
#include <QThreadPool>
#include "pdfium_util.h"
#include "pdfium_doc.h"
#include <QTimer>
#include <QCursor>

int RENDER_WIDTH = 1024;
int RENDER_HEIGHT = 720;

#define SCALE_FACTOR 1.2f
#define MIN_SCALE 0.1f
#define MAX_SCALE 128

PDFWidget::PDFWidget(QWidget *parent) : QWidget(parent), pdfUtil(new PDFiumUtil()) {
    QList<QPoint> points;
    auto size = RenderDispatcher::prepareDispatch(points, RENDER_WIDTH, RENDER_HEIGHT, 1, 0);

    for (QPoint &point: points) {
        auto* pdfTile = new PDFTile(nullptr, size, point, pdfDoc, this);
        pdfTile->move(point);
        tiles.push_back(pdfTile);
    }
    debouncer = new Debouncer(500, this);
    connect(debouncer, &Debouncer::triggered, this, &PDFWidget::onScaleChanged);
}

PDFWidget::~PDFWidget() noexcept {
    for (PDFTile* tile: tiles) {
        delete tile;
    }
    tiles.clear();
    delete pdfDoc;
    delete pdfUtil;
}

void PDFWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
//    QPainter painter(this);
//    QColor color;
//    color.setRgba(0xFF00FFFF);
//    painter.setPen(color);
//    painter.drawPoint(10, 10);
}

void PDFWidget::manageKey(QKeyEvent *event) {
    int keyCode = event->key();
    switch (keyCode) {
        case Qt::Key_Left:
            moveTiles(-1, 0);
            break;
        case Qt::Key_Right:
            moveTiles(1, 0);
            break;
        case Qt::Key_Up:
            moveTiles(0, -1);
            break;
        case Qt::Key_Down:
            moveTiles(0, 1);
            break;
    }
}

void PDFWidget::keyPressEvent(QKeyEvent *event) {
    manageKey(event);
}

void PDFWidget::moveTiles(int xOffset, int yOffset) {
    for (PDFTile *tile: tiles) {
        tile->translate(xOffset, yOffset);
    }
}

void PDFWidget::showPdf(const QString &path) {
    delete pdfDoc;
    int index = 0;
//    pdfDoc->initWithRender(nullptr, tiles[0]->size().width(), tiles[0]->size().height());
    for (PDFTile* tile: tiles) {
        pdfUtil->openFile(path, &pdfDoc, ++index);
        tile->setPDfDoc(pdfDoc);
        tile->refreshRender();
    }
}

void PDFWidget::onWheelEvent(QWheelEvent *event) {
   float scaleFactor = SCALE_FACTOR;
   bool zoomDirection;
   zoomDirection = event->angleDelta().y() < 0;

   if(zoomDirection) {
       m_scale = m_scale / scaleFactor;
       m_scale = (m_scale < MIN_SCALE) ? MIN_SCALE : m_scale;
   }
   else {
       m_scale = m_scale * scaleFactor;
       m_scale = (m_scale > MAX_SCALE) ? MAX_SCALE : m_scale;
   }

   debouncer->send(m_scale);
}

void PDFWidget::refreshRender() {
//    uint8_t* render = pdfDoc->render(0, 0, RENDER_WIDTH, RENDER_HEIGHT, m_scale);
//    emit tiles[0]->onRenderReady(render);
}

void PDFWidget::onScaleChanged(float value) {

   for (PDFTile* tile: tiles) {
       tile->setScale(value);
       tile->refreshRender();
   }
}

void PDFWidget::mousePressEvent(QMouseEvent *event) {
    setCursor(Qt::ClosedHandCursor);
    previousX = event->x();
    previousY = event->y();
}

void PDFWidget::mouseReleaseEvent(QMouseEvent *event) {
    setCursor(Qt::ArrowCursor);
}

void PDFWidget::mouseMoveEvent(QMouseEvent *event) {
    int diffX = event->x() - previousX;
    int diffY = event->y() - previousY;

    moveTiles(diffX, diffY);
    previousX = event->x();
    previousY = event->y();
}
