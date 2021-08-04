//
// Created by irouva on 25/07/2021.
//

#ifndef IMAGE_SELECTOR_MAINWINDOW_H
#define IMAGE_SELECTOR_MAINWINDOW_H

#include <QMainWindow>

class ImageSelectionGroupWidget;
class QVBoxLayout;
class QPushButton;
class PDFiumDoc;

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
     ~MainWindow();

public slots:
    void selectPDF();
    void validatePDFList();
    void extractPDF();

private:
    ImageSelectionGroupWidget* imageSelectionGroupWidget;
    QVBoxLayout* boxLayout;
    QPushButton* pickPdf;
    QPushButton* downloadSplit;
    QPushButton* validateSplit;
    QWidget* containerWidget;
    PDFiumDoc *pdFiumDoc{nullptr};
};


#endif //IMAGE_SELECTOR_MAINWINDOW_H
