//
//

#ifndef SCARY_PDF_TILE_H
#define SCARY_PDF_TILE_H

#include <QWidget>
#include <QPoint>
#include <QSize>

class PDFiumDoc;
class RenderRunnable;

class PDFTile: public QWidget {
    Q_OBJECT

public:
    PDFTile(uint8_t* buff, QSize size, QPoint renderPos, PDFiumDoc *pdfDoc, QWidget* parent = nullptr);
    ~PDFTile() noexcept override;
    void translate(int offsetX, int offsetY);
    void setPdfRenderPos(const QPoint &pos);
    void setScale(float newScale);
    void setPDfDoc(PDFiumDoc* doc);
    void refreshRender();
    const PDFiumDoc* getPdfDoc() const;

public slots:
    void updateRender(uint8_t* data);

signals:
    void onRenderReady(uint8_t* data);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    uint8_t* buffer{nullptr};
    QPoint pdfRenderPos;
    float scale{1.0f};
    RenderRunnable* renderRunnable;
    PDFiumDoc* pdfDoc;
    bool needRefresh{true};

    Qt::GlobalColor tmpColor{Qt::red};
};

#endif //SCARY_PDF_TILE_H
