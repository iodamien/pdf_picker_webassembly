//
//

#include "pdfium_doc.h"

#include <fpdfview.h>
#include <utility>
#include <QDebug>
#include <QDateTime>
#include <fpdf_edit.h>
#include <fpdf_save.h>
#include <fpdf_ppo.h>

PDFiumDoc::PDFiumDoc(FPDF_DOCUMENT document, QString docPath) :
        doc(document),
        path(std::move(docPath)) {

    pageCount = FPDF_GetPageCount(doc);
}

PDFiumDoc::~PDFiumDoc() {
    FPDF_CloseDocument(doc);
}

int PDFiumDoc::getPageCount() const {
    return pageCount;
}

const QString &PDFiumDoc::getPath() const {
    return path;
}

static FILE * pFile;
static int writeBlock(FPDF_FILEWRITE* pThis, const void* pData, unsigned long size)
{
    int result = fwrite(pData, 1, size, pFile);
    return result;
}

bool PDFiumDoc::extract(const QString& pathOutput, int page) {

    FPDF_DOCUMENT outputDoc = FPDF_CreateNewDocument();
    const char* p = pathOutput.toLocal8Bit().data();
    pFile = fopen(p, "wb");

    FPDF_FILEWRITE pFileWrite;
    pFileWrite.version = 1;
    pFileWrite.WriteBlock = writeBlock;

    FPDF_ImportPages(outputDoc, doc, QString::number(page).toLocal8Bit().data(), 0);

    FPDF_SaveAsCopy(outputDoc, &pFileWrite, FPDF_INCREMENTAL);
    fclose(pFile);
    return true;
}

QPixmap PDFiumDoc::render(int page, int renderWidth, int renderHeight, uint8_t *buffer) {

    if (renderWidth <= 0 || renderHeight <= 0 || buffer == nullptr) {
        return QPixmap();
    }

    uint background = 0xFFFFFFFF;


    FPDF_BITMAP pdfBitmap = FPDFBitmap_CreateEx((int) renderWidth, (int) renderHeight, 4, buffer,
                                                (int) renderWidth * 4);

    FPDF_PAGE pdfPage = FPDF_LoadPage(doc, page);


    FPDF_GetPageSizeByIndex(doc, 0, &pageWidth, &pageHeight);

    float scaleWidth = (float)renderWidth / pageWidth;
    float scaleHeight = (float)renderHeight / pageHeight;
    float scale = scaleWidth < scaleHeight ? scaleWidth : scaleHeight;

    int scaledWidth = pageWidth * scale;
    int scaledHeight = pageHeight * scale;

    FPDFBitmap_FillRect(pdfBitmap, 0, 0, (int) renderWidth, (int) renderHeight, background);
    FPDF_RenderPageBitmap(pdfBitmap, pdfPage, 0, 0, (int) scaledWidth, (int) scaledHeight, 0, 0);

    return convertByteToPixmap(renderWidth, renderHeight, buffer);
}

QPixmap PDFiumDoc::convertByteToPixmap(int width, int height, uint8_t *data) {
    int *src;
    QImage tmp(QSize(width, height), QImage::Format::Format_RGBA8888);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {

            src = reinterpret_cast<int *>(data);
            tmp.setPixelColor(x, y, *src);
            data += 4;
        }
    }
    return QPixmap::fromImage(tmp);
}
