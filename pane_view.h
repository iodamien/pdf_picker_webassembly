//
// Created by irouva on 30/05/2021.
//

#ifndef KUTER_PANE_VIEW_H
#define KUTER_PANE_VIEW_H

#include <QGraphicsView>

class PanePresenter;
class QWidget;
class QWheelEvent;
class PDFiumDoc;
class PDFiumUtil;

class PaneView : public QGraphicsView {

public:
    explicit PaneView(QWidget *parent = nullptr);
    ~PaneView() noexcept;
    void saveImage(const QString &outputFile);
    void applyTextChange();
    void showPdf(const QString &path);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void setImage(const QString &path);

private:
    PanePresenter *presenter;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *canvas{};
    float m_scale{ 1.0f };
    uint8_t* buffer{nullptr};
    PDFiumDoc *pdfDoc{nullptr};
    PDFiumUtil *pdfUtil{nullptr};

    int previousX;
    int previousY;

    int x{0};
    int y{0};
    void updateConfigurationChanged(int x, int y, float scale);
};


#endif //KUTER_PANE_VIEW_H
