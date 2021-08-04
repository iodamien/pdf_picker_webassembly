//
//

#ifndef SCARY_PDF_VIEW_H
#define SCARY_PDF_VIEW_H

#include <QWidget>
#include <QList>
#include "debouncer.h"

class PDFTile;
class PDFiumDoc;
class PDFiumUtil;

class PDFWidget: public QWidget {

    Q_OBJECT

public:
    PDFWidget(QWidget* parent = nullptr);
    ~PDFWidget() noexcept;
    void manageKey(QKeyEvent *event);
    void showPdf(const QString &path);

    void onWheelEvent(QWheelEvent* event);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void refreshRender();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void onScaleChanged(float scale);

private:
    Debouncer *debouncer;
    void moveTiles(int xOffset, int yOffset);
    QList<PDFTile*> tiles;
    PDFiumDoc *pdfDoc{nullptr};
    PDFiumUtil *pdfUtil{nullptr};
    float m_scale{1.0f};

    int x{0};
    int y{0};
    int previousX;
    int previousY;
};


#endif //SCARY_PDF_VIEW_H
