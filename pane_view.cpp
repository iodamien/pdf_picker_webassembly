//
// Created by irouva on 30/05/2021.
//

#include "pane_view.h"
#include "pdf_tile.h"
#include "pdfium_util.h"
#include "pdfium_doc.h"

#include <QObject>
#include <QGraphicsScene>
#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QTextItem>

#define SCALE_FACTOR 1.2f
#define MIN_SCALE 0.1f
#define MAX_SCALE 128

#define EDITOR_WIDTH 16384
#define EDITOR_HEIGHT 16384

#define RENDER_WITDH 1024
#define RENDER_HEIGHT 720

PaneView::PaneView(QWidget *parent) : QGraphicsView(parent),
    scene(new QGraphicsScene(this)),
    pdfUtil(new PDFiumUtil()) {
    resize(720, 480);
    setScene(scene);
    setAttribute(Qt::WA_DeleteOnClose);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

PaneView::~PaneView() noexcept {
    delete presenter;
}

void PaneView::applyTextChange() {
    QPixmap pixmap = canvas->pixmap();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    // This provoke a memory leak on linux due to libfontconfig. Still usable as it will not increase more after the first call.
//    painter.drawText(0, 0, "Hello world");
    painter.drawEllipse(40, 40, 40, 40);
    painter.end();
    canvas->setPixmap(pixmap);
}

void PaneView::setImage(const QString &path) {
    QPixmap pixmap(path);
    if (canvas != nullptr) {
        scene->removeItem(canvas);
    }
    canvas = scene->addPixmap(pixmap);


    setSceneRect(0, 0, EDITOR_WIDTH, EDITOR_HEIGHT);
    auto midX = (EDITOR_WIDTH - canvas->pixmap().width()) / 2;
    auto midY = (EDITOR_HEIGHT - canvas->pixmap().width()) / 2;
    canvas->setX(midX);
    canvas->setY(midY);

    /** Compute rect and pos **/



//    scene->addLine(QLineF(9500, 10000, 10000, 10000));
//    scene->addEllipse(10000, 9990, 20, 20, QPen(), QBrush(Qt::black));
    applyTextChange();
    saveImage("output.png");

    /*
     * TODO
     * 1. Create item entity and save it in presenter.
     * 2. Apply on pixmap.
     * 3. Remove on scene.
     * 4. Remove on item entity list.
     */

//    QGraphicsTextItem* test = scene->addText("Hello world");
//    test->setX(200);
//    test->setY(400);
}

void PaneView::saveImage(const QString &outputFile) {
    canvas->pixmap().save(outputFile);
}

void PaneView::wheelEvent(QWheelEvent *event) {

    const QPointF currentPos = event->position();
    const QPointF p0scene = mapToScene(currentPos.toPoint());
    float scaleFactor = SCALE_FACTOR;
    bool zoomDirection;
    zoomDirection = event->angleDelta().y() < 0;

    if(zoomDirection) {
        m_scale = m_scale / scaleFactor;
        m_scale = (m_scale < MIN_SCALE) ? MIN_SCALE : m_scale;
        x /= scaleFactor;
        y /= scaleFactor;
    }
    else {
        m_scale = m_scale * scaleFactor;
        m_scale = (m_scale > MAX_SCALE) ? MAX_SCALE : m_scale;
        x *= scaleFactor;
        y *= scaleFactor;
    }

    resetTransform();
//    scale(m_scale, m_scale);
    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - currentPos;
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

    updateConfigurationChanged(x, y, m_scale);
}

QPixmap initPixmap(int width, int height, uint8_t *data) {
    int *src;
    QImage tmp(QSize(width, height), QImage::Format::Format_RGBA8888);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {

            src = reinterpret_cast<int *>(data);
            tmp.setPixelColor(x, y, *src);
            data += 4;
        }
    }
    return QPixmap::fromImage(tmp);
}

void PaneView::showPdf(const QString &path) {
    delete pdfDoc;
    int index = 0;
    buffer = new uint8_t[RENDER_WITDH * RENDER_HEIGHT * 4];
    pdfUtil->openFile(path, &pdfDoc, ++index);
    pdfDoc->initWithRender(buffer, RENDER_WITDH, RENDER_HEIGHT);
    pdfDoc->render(0, 0, RENDER_WITDH, RENDER_HEIGHT, 1.0f, buffer);
    auto pixmap = initPixmap(RENDER_WITDH, RENDER_HEIGHT, buffer);
    canvas = scene->addPixmap(pixmap);
    scene->addRect(QRectF(200, 200, 450, 450));
}

void PaneView::updateConfigurationChanged(int x, int y, float scale) {
    pdfDoc->render(x, y, RENDER_WITDH, RENDER_HEIGHT, scale, buffer);
    auto pixmap = initPixmap(RENDER_WITDH, RENDER_HEIGHT, buffer);
    canvas->setPixmap(pixmap);
}

void PaneView::mousePressEvent(QMouseEvent *event) {
    setCursor(Qt::ClosedHandCursor);
    previousX = event->x();
    previousY = event->y();
}

void PaneView::mouseReleaseEvent(QMouseEvent *event) {
    setCursor(Qt::ArrowCursor);
}

void PaneView::mouseMoveEvent(QMouseEvent *event) {
    int diffX = event->x() - previousX;
    int diffY = event->y() - previousY;

    x -= diffX;
    y -= diffY;
    updateConfigurationChanged(x, y, m_scale);


    previousX = event->x();
    previousY = event->y();
}
