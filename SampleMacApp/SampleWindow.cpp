#include <iostream>

#include "SampleWindow.h"
#include "gl/GrGLInterface.h"
#include "GrContext.h"
#include "SkApplication.h"
#include "SkCanvas.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkGr.h"
#include "SkPngCodec.h"
#include "SkStream.h"

void application_init() {
    SkGraphics::Init();
    SkEvent::Init();
}

void application_term() {
    SkEvent::Term();
}

SampleWindow::SampleWindow(void* hwnd)
    : INHERITED(hwnd) {
    fType = kGPU_DeviceType;
    fRotationAngle = 0;
    this->setTitle();
    this->setUpBackend();
    this->fTextLocation = SkIPoint::Make(this->width()/2, this->height()/2);
}

SampleWindow::~SampleWindow() {
    tearDownBackend();
}

void SampleWindow::tearDownBackend() {
    SkSafeUnref(fContext);
    fContext = NULL;

    SkSafeUnref(fInterface);
    fInterface = NULL;

    fGpuSurface = nullptr;

    INHERITED::release();
}

void SampleWindow::setTitle() {
    SkString title("Hello World ");
    title.appendf(fType == kRaster_DeviceType ? "raster" : "opengl");
    INHERITED::setTitle(title.c_str());
}

bool SampleWindow::setUpBackend() {
    this->setVisibleP(true);
    this->setClipToBounds(false);

    bool result = attach(kNativeGL_BackEndType, 0 /*msaa*/, false, &fAttachmentInfo);
    if (false == result) {
        SkDebugf("Not possible to create backend.\n");
        release();
        return false;
    }

    fInterface = GrGLCreateNativeInterface();
    SkASSERT(NULL != fInterface);

    fContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)fInterface);
    SkASSERT(NULL != fContext);

    this->setUpGpuBackedSurface();
    return true;
}

void SampleWindow::setUpGpuBackedSurface() {
    fGpuSurface = this->makeGpuBackedSurface(fAttachmentInfo, fInterface, fContext);
}

void SampleWindow::drawContents(SkCanvas* canvas) {
    canvas->drawColor(SK_ColorWHITE);
    if (currentBmp.getSize() > 0) {
        canvas->drawBitmap(currentBmp, 0, 0);
    }

    SkPaint paint;

    // Draw a message with a nice black paint.
    paint.setFlags(SkPaint::kAntiAlias_Flag | SkPaint::kSubpixelText_Flag);
    paint.setColor(SK_ColorBLUE);
    paint.setTextSize(44);

    static const char message[] = "Hello World!";
    
    canvas->drawText(message, strlen(message), fTextLocation.fX, fTextLocation.fY, paint);

    canvas->save();
}

void SampleWindow::draw(SkCanvas* canvas) {
    this->drawContents(canvas);
    // in case we have queued drawing calls
    fContext->flush();
    // Invalidate the window to force a redraw. Poor man's animation mechanism.
    this->inval(NULL);

    if (kRaster_DeviceType == fType) {
        // need to send the raster bits to the (gpu) window
        sk_sp<SkImage> snap = fRasterSurface->makeImageSnapshot();
        SkPixmap pmap;
        if (snap->peekPixels(&pmap)) {
            const SkImageInfo& info = pmap.info();

            SkCanvas* canvas = fGpuSurface->getCanvas();

            canvas->writePixels(info, pmap.addr(), pmap.rowBytes(), 0, 0);
            canvas->flush();
        }
    }
    INHERITED::present();
}

void SampleWindow::onSizeChange() {
    fTextLocation.fX = this->width()/2;
    fTextLocation.fY = this->height()/2;
    this->setUpGpuBackedSurface();
}

bool SampleWindow::onHandleChar(SkUnichar unichar) {
    if (' ' == unichar) {
        fType = fType == kRaster_DeviceType ? kGPU_DeviceType: kRaster_DeviceType;
        tearDownBackend();
        setUpBackend();
        this->setTitle();
        this->inval(NULL);
    }
    return true;
}

bool SampleWindow::onHandleKey(SkKey key) {
    switch (key) {
        case kUp_SkKey:
            if (0 < fTextLocation.fY) {
                fTextLocation.fY -= STEP_COUNT;
            }
            break;
        case kDown_SkKey:
            if (this->height() > fTextLocation.fY) {
                fTextLocation.fY += STEP_COUNT;
            }
            break;
        case kRight_SkKey:
            if (this->width() > fTextLocation.fX) {
                fTextLocation.fX += STEP_COUNT;
            }
            break;
        case kLeft_SkKey:
            if (0 < fTextLocation.fX) {
                fTextLocation.fX -= STEP_COUNT;
            }
            break;
        default:
            return false;
    }
    return true;
}

bool SampleWindow::onDispatchClick(int x, int y, Click::State state, void* owner, unsigned modi) {
    
    if (x > 0 && x < this->width()) {
        fTextLocation.fX = x;
    }
    if (y > 0 && y < this->height()) {
        fTextLocation.fY = y;
    }
    return true;
}

bool SampleWindow::onEvent(const SkEvent& evt) {
    return this->INHERITED::onEvent(evt);
}

void SampleWindow::loadPng(std::string path) {
    SkBitmap bitmap;
    auto stream = SkStream::NewFromFile(path.c_str());
    if (stream == NULL) {
        // TODO: Alert user of failed loading
        return;
    }
    auto codec = SkPngCodec::NewFromStream(stream);
    const auto imageInfo = codec->getInfo();
    
    currentBmp.allocPixels(codec->getInfo());
    codec->getPixels(codec->getInfo(), currentBmp.getPixels(), currentBmp.rowBytes());
}

SkOSWindow* create_sk_window(void* hwnd, int , char** ) {
    return new SampleWindow(hwnd);
}
