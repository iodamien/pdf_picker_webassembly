#ifndef RENDER_TILE_H
#define RENDER_TILE_H

#include "pdfium_doc.h"
#include <QPoint>
#include <QSize>

struct RenderTile {
    PDFiumDoc* doc;
    QPoint pdfPos;
    QSize size;
    float scale;
};

#endif // RENDER_TILE_H
