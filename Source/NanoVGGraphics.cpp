#include "NanoVGGraphics.h"
#include <nanovg_compat.h>


NanoVGGraphics::NanoVGGraphics()
{
    startTimerHz(60);

    juce::MessageManager::callAsync([this]()
    {
        initialise();
    });
}

NanoVGGraphics::~NanoVGGraphics()
{
    shutdown();
}

void NanoVGGraphics::setComponent(Renderer* comp)
{
    component = comp;
}

void NanoVGGraphics::componentMovedOrResized (juce::Component& comp, bool, bool wasResized)
{
    jassert(component != nullptr);

    if (wasResized)
    {
        if (auto* display {juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()})
            pixelScale = std::max<float>((float)display->scale, 1.0f);
        framebufferTransformScale = 1.0f / (pixelScale * drawScale);

        if (nvg)
        {
            if (mainFrameBuffer != 0)
            {
                nvgDeleteImage(nvg, mainFrameBuffer);
                mainFrameBuffer = 0;
            }

            nvgSetViewBounds(
                nvg,
                component->getPeer()->getNativeHandle(),
                component->getWidth() * getPixelScale(),
                component->getHeight() * getPixelScale());

            mainFrameBuffer = nvgCreateFramebuffer(
                nvg,
                component->getWidth() * getPixelScale(),
                component->getHeight() * getPixelScale(),
                0);
        }
    }
}

void NanoVGGraphics::timerCallback()
{
    render();
}

bool NanoVGGraphics::initialise()
{
    if (nvg) return true;

    DBG("Initialising NanoVG...");

    jassert(component != nullptr);
    if (component->getBounds().isEmpty())
    {
        // Component placement is not ready yet - postpone initialisation.
        jassertfalse;
        return false;
    }

    component->setVisible(true);

    if (auto* display {juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()})
        pixelScale = std::max<float>((float)display->scale, 1.0f);
    framebufferTransformScale = 1.0f / (pixelScale * drawScale);

    jassert(pixelScale > 0);
    jassert(component->getWidth() > 0);
    jassert(component->getHeight() > 0);

    void* nativeHandle = component->getPeer()->getNativeHandle();
    jassert(nativeHandle != nullptr);

    nvg = nvgCreateContext(
        nativeHandle,
        0,
        component->getWidth() * pixelScale,
        component->getHeight() * pixelScale);
    jassert(nvg != nullptr);

    nvgGlobalCompositeOperation(nvg, NVG_SOURCE_OVER);

    mainFrameBuffer = nvgCreateFramebuffer(
        nvg,
        getWindowWidth() * getPixelScale(),
        getWindowHeight() * getPixelScale(),
        0);
    jassert(mainFrameBuffer != 0);

    component->onContextCreated();

    DBG("Initialised successfully");
    return true;
}

void NanoVGGraphics::shutdown()
{
    if (mainFrameBuffer)
        nvgDeleteImage(nvg, mainFrameBuffer);
    if (nvg)
        nvgDeleteContext(nvg);
}

void NanoVGGraphics::render()
{
    jassert(component != nullptr);
    if (!nvg)
    {
        bool success = initialise();
        if (!success)
            return;
    }

    beginFrame();
    component->draw();
    endFrame();
}

void NanoVGGraphics::beginFrame()
{
    jassert(component->getWidth() > 0);
    jassert(component->getHeight() > 0);
    jassert(mainFrameBuffer != 0);

    nvgBindFramebuffer(nvg, mainFrameBuffer);
    nvgBeginFrame(
        nvg,
        getWindowWidth(),
        getWindowHeight(),
        getPixelScale());
}

void NanoVGGraphics::endFrame()
{
    nvgEndFrame(nvg);
    // bind to back buffer
    nvgBindFramebuffer(nvg, 0);
    nvgBeginFrame(
        nvg,
        getWindowWidth(),
        getWindowHeight(),
        getPixelScale());

    nvgSave(nvg);

    NVGpaint img;
    memset(&img, 0, sizeof(NVGpaint));
    nvgTransformScale(img.xform, framebufferTransformScale, framebufferTransformScale);
    img.extent[0] = getWindowWidth() * getPixelScale();
    img.extent[1] = getWindowHeight() * getPixelScale();
    img.innerColor = nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
    img.image = mainFrameBuffer;

    nvgBeginPath(nvg);
    nvgRect(
        nvg,
        0.0f,
        0.0f,
        getWindowWidth(),
        getWindowHeight());
    nvgFillPaint(nvg, img);
    nvgFill(nvg);
    nvgResetTransform(nvg);
    nvgRestore(nvg);

    nvgEndFrame(nvg);

#ifdef _WIN32
    d3dnvgPresent(nvg);
#endif
}
