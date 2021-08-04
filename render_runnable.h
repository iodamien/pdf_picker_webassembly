#ifndef THREAD_RENDER_H
#define THREAD_RENDER_H

#include <QRunnable>
#include <QObject>

#include "render_tile.h"

class RenderRunnable : public QObject, public QRunnable{

    Q_OBJECT

public:
    RenderRunnable(RenderTile&& tile);
    void run() override;

signals:
    void emitRender(uint8_t* data);

private:
    RenderTile tile;
};

#endif // THREAD_RENDER_H
