#ifndef Sampled_DEFINED
#define Sampled_DEFINED

#include "SkSurface.h"
#include "SkWindow.h"

class GrContext;
struct GrGLInterface;
class GrRenderTarget;
class SkCanvas;

class SampleWindow : public SkOSWindow {
public:
    enum DeviceType {
        kRaster_DeviceType,
        kGPU_DeviceType,
    };
    SampleWindow(void* hwnd);
    virtual ~SampleWindow() override;

    // Changes the device type of the object.
    bool setUpBackend();

    DeviceType getDeviceType() const { return fType; }

protected:
    sk_sp<SkSurface> makeSurface() override {
        SkSurfaceProps props(INHERITED::getSurfaceProps());
        if (kGPU_DeviceType == fType) {
            return fGpuSurface;
        }
        const SkImageInfo info = SkImageInfo::MakeN32Premul(SkScalarRoundToInt(this->width()),
                                                            SkScalarRoundToInt(this->height()));
        fRasterSurface = SkSurface::MakeRaster(info, &props);
        return fRasterSurface;
    }

    void draw(SkCanvas* canvas) override;
    void drawContents(SkCanvas* canvas);

    void onSizeChange() override;

private:
    bool findNextMatch();  // Set example to the first one that matches FLAGS_match.
    void setTitle();
    void setUpGpuBackedSurface();
    bool onHandleChar(SkUnichar unichar) override;
    bool onHandleKey(SkKey key) override;

    void tearDownBackend();

    // draw contents
    SkScalar fRotationAngle;
    
    // text location
    const uint8_t STEP_COUNT = 5;
    SkIPoint fTextLocation;

    // support framework
    DeviceType fType;
    sk_sp<SkSurface> fRasterSurface;
    GrContext* fContext;
    sk_sp<SkSurface> fGpuSurface;
    AttachmentInfo fAttachmentInfo;
    const GrGLInterface* fInterface;

    typedef SkOSWindow INHERITED;
};

#endif
