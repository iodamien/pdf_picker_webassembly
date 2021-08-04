//
//

#ifndef SCARY_RENDER_DISPATCHER_H
#define SCARY_RENDER_DISPATCHER_H

#include <QList>
#include <QPoint>
#include <QSize>

class RenderDispatcher {
public:
    static QSize prepareDispatch(QList<QPoint> &outputList, int renderWidth, int renderHeight, int loadSize, int preloadCount = 0);
};

#endif //SCARY_RENDER_DISPATCHER_H
