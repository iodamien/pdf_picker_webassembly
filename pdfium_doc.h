//
//

#ifndef PDF_BOM_PDFIUM_DOC_H
#define PDF_BOM_PDFIUM_DOC_H

#include <QString>
#include <fpdfview.h>
#include <QLabel>
#include <QMutex>
#include <QPixmap>

class QPixmap;

class PDFiumDoc {

public:
    explicit PDFiumDoc(FPDF_DOCUMENT document, QString docPath);
    ~PDFiumDoc();

    int getPageCount() const;
    const QString& getPath() const;
    QPixmap render(int page, int renderWidth, int renderHeight, uint8_t* buffer);
    bool extract(const QString& pathOutput, int page);

private:
    FPDF_DOCUMENT doc;
    QString path{""};
    int pageCount{0};
    double pageWidth{0};
    double pageHeight{0};
    QPixmap convertByteToPixmap(int width, int height, uint8_t *data);

};

#endif //PDF_BOM_PDFIUM_DOC_H
