#include "CacheTest.h"
#include <BinaryData.h>


//==============================================================================

ScopedFramebufferTest::ScopedFramebufferTest(NanoVGGraphics& g)
    : graphics(g)
    , framebuffer(g)
{
    setCachedComponentImage(nullptr);
    setBufferedToImage(false);

    graphics.setComponent(this);
    addComponentListener(&graphics);
}

ScopedFramebufferTest::~ScopedFramebufferTest()
{
}

int ScopedFramebufferTest::onContextCreated()
{
    robotoFontId = nvgCreateFontMem(graphics.getContext(), "robotoRegular", (unsigned char*)BinaryData::RobotoRegular_ttf, BinaryData::RobotoRegular_ttfSize, 0);
}

void ScopedFramebufferTest::draw()
{
    NVGcontext* ctx = graphics.getContext();

    char text[32];
    memset(text, 0, sizeof(text));

    nvgClearWithColor(ctx, nvgRGBAf(0.3f, 0.3f, 0.4f, 1.0f));

    if (!framebuffer.valid)
    {
        Framebuffer::ScopedBind bind(framebuffer);

        nvgBeginPath(ctx);
        // framebuffers always begin at 0/0
        nvgRect(ctx, 0.0f, 0.0f, framebuffer.width, framebuffer.height);
        nvgFillColor(ctx, nvgRGBAf(0.8f, 0.8f, 0.2f, 1.0f));
        nvgFill(ctx);

        nvgFontFaceId(ctx, robotoFontId);
        nvgFontSize(ctx, 12.0f);
        nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(ctx, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));

        float centreX = framebuffer.width * 0.5f;
        float centreY = framebuffer.height * 0.5f;

        sprintf(text, "Cache: %d redraws", fbDrawCount);
        nvgText(ctx, centreX, centreY, text, nullptr);

        fbDrawCount++;
    }
    framebuffer.paint();

    nvgFontFaceId(ctx, robotoFontId);
    nvgFontSize(ctx, 12.0f);
    nvgTextAlign(ctx, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));

    float centreX = framebuffer.width * 0.5f + framebuffer.width;
    float centreY = framebuffer.height * 0.5f;

    sprintf(text, "%d redraws", drawCount);
    nvgText(ctx, centreX, centreY, text, nullptr);

    drawCount++;
}

void ScopedFramebufferTest::resized()
{
    juce::Rectangle<float> bounds = getBounds().toFloat();
    bounds.setWidth(bounds.getWidth() * 0.5f);
    framebuffer.setBounds(
        bounds.getX(),
        bounds.getY(),
        bounds.getWidth(),
        bounds.getHeight());
}
