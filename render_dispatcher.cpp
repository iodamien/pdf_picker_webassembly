//
//

#include <thread>
#include "render_dispatcher.h"

#include <QSize>

QSize RenderDispatcher::prepareDispatch(QList<QPoint> &outputList, int renderWidth, int renderHeight, int loadSize, int preloadCount) {

    if (preloadCount < 0) {
        preloadCount = 0;
    }

    if (renderWidth < 1 || renderHeight < 1 || loadSize < 0) {
        return QSize(0, 0);
    }

    outputList.clear();
    if (loadSize == 1) {
        outputList.push_back(QPoint(0, 0));
        return QSize(renderWidth, renderHeight);
    }

    unsigned num_cpus = std::thread::hardware_concurrency();

    int restWidth = renderWidth % loadSize;
//    int offsetWidth = restWidth > 1 ? 2 : restWidth;
    int offsetWidth = 0;
    int totalWidth = loadSize + offsetWidth + (preloadCount << 1);
    int tileWidth = renderWidth / loadSize;

    int restHeight = renderHeight % loadSize;
//    int offsetHeight = restHeight > 1 ? 2 : restHeight;
    int offsetHeight = 0;
    int totalHeight = loadSize + offsetHeight + (preloadCount << 1);
    int tileHeight = renderHeight / loadSize;
    for (int y = -preloadCount; y < totalHeight - preloadCount; y++) {
        for (int x = -preloadCount; x < totalWidth - preloadCount; x++) {
            int pX = x * tileWidth;
            int pY = y * tileHeight;
            QPoint point(pX, pY);
            outputList.push_back(point);
        }
    }

    return QSize(tileWidth, tileHeight);
}
