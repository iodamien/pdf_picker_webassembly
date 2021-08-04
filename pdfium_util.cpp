//
//

#include "pdfium_util.h"
#include "pdfium_doc.h"

#include <fpdfview.h>
#include <QFile>

PDFiumUtil::PDFiumUtil() {

    FPDF_LIBRARY_CONFIG config;
    config.version = 3;
    config.m_pUserFontPaths = nullptr;
    config.m_pIsolate = nullptr;
    config.m_v8EmbedderSlot = 0;
    config.m_pPlatform = nullptr;

    FPDF_InitLibraryWithConfig(&config);
}

PDFiumUtil::~PDFiumUtil() {
    FPDF_DestroyLibrary();
}

bool PDFiumUtil::openFile(const QString& path, PDFiumDoc** doc) {

    FPDF_STRING pdfPath = path.toLocal8Bit().data();
    FPDF_DOCUMENT pdfDoc = FPDF_LoadDocument(pdfPath, nullptr);

    if (pdfDoc) {
        QString newName = path + ".copy";
        QFile::copy(path, newName);
        *doc = new PDFiumDoc(pdfDoc, path);
        return true;
    }

    return false;
}
