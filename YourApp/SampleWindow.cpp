#include "SampleWindow.h"

#include "gl/GrGLInterface.h"
#include "GrContext.h"
#include "SkApplication.h"
#include "SkCanvas.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkGr.h"

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
    
    this->fTextLocation = SkIPoint::Make(0, 0);
    return true;
}

void SampleWindow::setUpGpuBackedSurface() {
    fGpuSurface = this->makeGpuBackedSurface(fAttachmentInfo, fInterface, fContext);
}

void SampleWindow::drawContents(SkCanvas* canvas) {
    // Clear background
    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorRED);

    // Draw a message with a nice black paint.
    paint.setFlags(
            SkPaint::kAntiAlias_Flag |
            SkPaint::kSubpixelText_Flag  // ... avoid waggly text when rotating.
            );
    paint.setColor(SK_ColorGRAY);
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
            break;
    }
    return true;
}


SkOSWindow* create_sk_window(void* hwnd, int , char** ) {
    return new SampleWindow(hwnd);
}
