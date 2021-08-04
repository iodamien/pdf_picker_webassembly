//
// Created by irouva on 25/07/2021.
//

#include "mainwindow.h"
#include "image_selection_group_widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <emscripten.h>
#include "pdfium_util.h"
#include "pdfium_doc.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    containerWidget = new QWidget(this);
    imageSelectionGroupWidget = new ImageSelectionGroupWidget(this);
    boxLayout = new QVBoxLayout(containerWidget);

    imageSelectionGroupWidget->setVisible(false);

    pickPdf = new QPushButton("Pick your PDF", this);
    downloadSplit = new QPushButton("Download extract", this);
    validateSplit = new QPushButton("Validate", this);

    auto* actionBoxLayout = new QHBoxLayout(this);
    actionBoxLayout->addWidget(pickPdf);
    actionBoxLayout->addWidget(downloadSplit);
    actionBoxLayout->addWidget(validateSplit);

    downloadSplit->setVisible(false);
    validateSplit->setVisible(false);

    connect(pickPdf, &QPushButton::clicked, this, &MainWindow::selectPDF);
    connect(validateSplit, &QPushButton::clicked, this, &MainWindow::validatePDFList);
    connect(downloadSplit, &QPushButton::clicked, this, &MainWindow::extractPDF);

    boxLayout->addWidget(imageSelectionGroupWidget);
    boxLayout->addLayout(actionBoxLayout);
    boxLayout->setAlignment(pickPdf, Qt::AlignHCenter);
    containerWidget->setLayout(boxLayout);
    setCentralWidget(containerWidget);
}

MainWindow::~MainWindow() noexcept {

}

EM_JS(void, call_alert, (const char* str), {
  alert(UTF8ToString(str));
});

void MainWindow::validatePDFList() {
    QList<int> selected = imageSelectionGroupWidget->getSelected();
    QString str = "DO POST Request with ID: ";
    for (int i = 0; i < selected.count(); i++) {
        qDebug() << selected[i];
        if (i != 0) {
            str += ",";
        }
        str += QString::number(selected[i]);
    }
    call_alert(str.toLocal8Bit().data());
}

void MainWindow::extractPDF() {
    QList<int> selected = imageSelectionGroupWidget->getSelected();
    QString output = "output.pdf";
    for (int i = 0; i < selected.count(); i++) {
        pdFiumDoc->extract(output, selected.at(i));
        QFile currentFile(output);
        if (currentFile.open(QIODevice::ReadOnly)) {
            QFileDialog::saveFileContent(currentFile.readAll(), output);
        }
    }
}

void MainWindow::selectPDF() {
    auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            // No file was selected
        } else {
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.write(fileContent);
            file.close();

            delete pdFiumDoc;
            PDFiumUtil pdFiumUtil;
            pdFiumUtil.openFile(fileName, &pdFiumDoc);

            imageSelectionGroupWidget->setVisible(true);
            pickPdf->setVisible(false);
            downloadSplit->setVisible(true);
            validateSplit->setVisible(true);

            uint8_t buffer[PDF_SNAP_WIDTH * PDF_SNAP_HEIGHT * PDF_IMAGE_BYTE];
            if (pdFiumDoc != nullptr) {
                int count = pdFiumDoc->getPageCount();
                int max = count > 50 ? 50 : count;
                imageSelectionGroupWidget->setSize(max);
                for (int i = 0; i < max ; i++) {
                    auto pixmap = pdFiumDoc->render(i, PDF_SNAP_WIDTH, PDF_SNAP_HEIGHT, buffer);
                    imageSelectionGroupWidget->addImage(pixmap, i + 1);
                }
            }
        }
    };
    QFileDialog::getOpenFileContent("Images (*.pdf)",  fileContentReady);
}
