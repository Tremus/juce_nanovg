#include "FramebufferTest.h"
#include <BinaryData.h>

juce::Rectangle<int> getAbsoluteBounds(juce::Component* comp)
{
    auto bounds = comp->getScreenBounds();
    auto p      = comp->getTopLevelComponent()->getScreenPosition();
    bounds.translate(-p.x, -p.y);
    return bounds;
}

//==============================================================================

FramebufferTest::FramebufferTest(NanoVGGraphics& g)
    : graphics(g)
    , framebuffer(g)
{
    setCachedComponentImage(nullptr);
    setBufferedToImage(false);

    graphics.setComponent(this);
    addComponentListener(&graphics);

    addAndMakeVisible(child);
}

FramebufferTest::~FramebufferTest() {}

int FramebufferTest::onContextCreated()
{
    robotoFontId = nvgCreateFontMem(
        graphics.getContext(),
        "robotoRegular",
        (unsigned char*)BinaryData::RobotoRegular_ttf,
        BinaryData::RobotoRegular_ttfSize,
        0);
    return 0;
}

void FramebufferTest::draw()
{
    NVGcontext* ctx = graphics.getContext();

    // By not using a JUCE string, we avoid memory allocations every frame
    char text[32];

    nvgClearWithColor(ctx, nvgRGBAf(0.3f, 0.3f, 0.4f, 1.0f));

    if (! framebuffer.valid)
    {
        Framebuffer::ScopedBind bind(framebuffer);

        nvgBeginPath(ctx);
        // framebuffers always begin at 0/0
        nvgRect(ctx, 0.0f, 0.0f, framebuffer.width, framebuffer.height);
        nvgFillColor(ctx, nvgRGBAf(0.8f, 0.8f, 0.2f, 1.0f));
        nvgFill(ctx);

        // nvgFontFaceId(ctx, robotoFontId);
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

    // nvgFontFaceId(ctx, robotoFontId);
    nvgFontSize(ctx, 12.0f);
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));

    float centreX = framebuffer.width * 0.5f + framebuffer.width;
    float centreY = framebuffer.height * 0.5f;

    sprintf(text, "%d redraws", drawCount);
    nvgText(ctx, centreX, centreY, text, nullptr);

    drawCount++;

    if (child.isVisible())
        child.draw(ctx);
}

void FramebufferTest::resized()
{
    auto bounds = getBounds().toFloat();
    bounds.setWidth(bounds.getWidth() * 0.5f);
    framebuffer.setBounds(
        bounds.getX(),
        bounds.getY(),
        bounds.getWidth(),
        bounds.getHeight());

    child.setBounds(20, 20, 80, 80);
}

//==============================================================================

void FramebufferTest::Child::draw(NVGcontext* ctx)
{
    auto b = getAbsoluteBounds(this).toFloat();
    nvgBeginPath(ctx);
    nvgRect(ctx, b.getX(), b.getY(), b.getWidth(), b.getHeight());
    nvgFillColor(
        ctx,
        hover ? nvgRGBAf(0.0f, 1.0f, 0.0f, 1.0f)
              : nvgRGBAf(1.0f, 0.0f, 0.0f, 1.0f));
    nvgFill(ctx);

    if (hover)
    {
        nvgFillColor(ctx, nvgRGBAf(0, 0, 0, 1));
        nvgTextAlign(ctx, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgText(ctx, b.getCentreX(), b.getCentreY(), "HOVER", nullptr);
    }
    else
    {
        nvgFillColor(ctx, nvgRGBAf(1, 1, 1, 1));
        nvgTextAlign(ctx, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgText(ctx, b.getCentreX(), b.getCentreY(), "NO HOVER", nullptr);
    }
}