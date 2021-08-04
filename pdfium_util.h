//
//

#ifndef PDF_BOM_PDFIUM_UTIL_H
#define PDF_BOM_PDFIUM_UTIL_H

#include <QString>

class PDFiumDoc;

class PDFiumUtil {

public:
    PDFiumUtil();
    ~PDFiumUtil();

    bool openFile(const QString& path, PDFiumDoc** doc);
};

#endif //PDF_BOM_PDFIUM_UTIL_H
