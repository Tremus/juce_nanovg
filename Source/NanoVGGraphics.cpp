#include "NanoVGGraphics.h"
#include <nanovg_compat.h>


NanoVGGraphics::NanoVGGraphics()
{
    startTimerHz(FPS);

    // Unfortunately when the JUCE PluginEditor is constructed, we have no way to get access to
    // the windows HWND/NSView ptr. Here we post a function callback to JUCEs event queue to
    // init NanoVG, which requires a pointer to our window. 
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

    if (!wasResized)
        return;
    
    updatePixelScale();

    if (!nvg)
        return;

    nvgSetViewBounds(
        nvg,
        component->getPeer()->getNativeHandle(),
        component->getWidth() * pixelScale,
        component->getHeight() * pixelScale);

    if (mainFrameBuffer != 0)
    {
        nvgDeleteImage(nvg, mainFrameBuffer);
        mainFrameBuffer = 0;
    }

    mainFrameBuffer = nvgCreateFramebuffer(
        nvg,
        component->getWidth() * pixelScale,
        component->getHeight() * pixelScale,
        0);
}

void NanoVGGraphics::timerCallback()
{
    jassert(component != nullptr);
    if (!nvg)
    {
        // hopefully this never gets called and posting to the event queue actually works in all hosts
        jassertfalse;
        bool success = initialise();
        if (!success)
            return;
    }

    jassert(component->getWidth() > 0);
    jassert(component->getHeight() > 0);
    jassert(mainFrameBuffer != 0);

    nvgBindFramebuffer(nvg, mainFrameBuffer);
    nvgBeginFrame(
        nvg,
        getWindowWidth(),
        getWindowHeight(),
        pixelScale);

    // Draw callback
    component->draw();

    nvgEndFrame(nvg);
    // Bind to back buffer
    nvgBindFramebuffer(nvg, 0);
    nvgBeginFrame(
        nvg,
        getWindowWidth(),
        getWindowHeight(),
        pixelScale);

    nvgSave(nvg);

    NVGpaint img;
    memset(&img, 0, sizeof(NVGpaint));
    float fbscale = getFramebufferTransformScale();
    nvgTransformScale(img.xform, fbscale, fbscale);
    img.extent[0] = getWindowWidth() * pixelScale;
    img.extent[1] = getWindowHeight() * pixelScale;
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

bool NanoVGGraphics::initialise()
{
    if (nvg) return true;

    DBG("Initialising NanoVG...");

    jassert(component != nullptr);
    if (component->getBounds().isEmpty())
        return false; // Component placement is not ready yet - postpone initialisation.

    component->setVisible(true);

    updatePixelScale();

    void* nativeHandle = component->getPeer()->getNativeHandle();

    jassert(nativeHandle != nullptr);
    if (!nativeHandle) return false;

    nvg = nvgCreateContext(
        nativeHandle,
        NVG_DEFAULT_CONTEXT_FLAGS,
        component->getWidth() * pixelScale,
        component->getHeight() * pixelScale);

    jassert(nvg != NULL);
    if (!nvg) return false;

    nvgGlobalCompositeOperation(nvg, NVG_SOURCE_OVER);

    mainFrameBuffer = nvgCreateFramebuffer(
        nvg,
        getWindowWidth() * pixelScale,
        getWindowHeight() * pixelScale,
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

    mainFrameBuffer= 0;
    nvg = NULL;
}

void NanoVGGraphics::updatePixelScale()
{
    /* On my MacBook Air, the juce::Display scale is always 2.
       On my Windows PC it's always 1.
       Having a scale of 1 however will cause a bunch of aliasing on the edges of shapes & lines
       Here we set it to sensible defaults (2 macos, 1.5 windows) which were taken from iPlug2 */

    // if (const juce::Displays::Display* display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay())
    //     pixelScale = juce::jmax((float)display->scale, 1.0f);
    pixelScale = NVG_DEFAULT_PIXEL_RATIO;
}