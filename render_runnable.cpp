#include "render_runnable.h"

#include <QThread>
#include "render_tile.h"

RenderRunnable::RenderRunnable(RenderTile&& renderTile): QObject(), QRunnable(), tile(renderTile) {

}

void RenderRunnable::run() {
    const PDFiumDoc* doc = tile.doc;
    QPoint pdfRender = tile.pdfPos;
    QSize pdfSize = tile.size;
//    uint8_t* data = doc->render(pdfRender.x(), pdfRender.y(), pdfSize.width(), pdfSize.height(), tile.scale);
//    emitRender(data);
}
