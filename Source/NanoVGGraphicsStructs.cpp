#include "NanoVGGraphicsStructs.h"
#include "NanoVGGraphics.h"


//==============================================================================

Framebuffer::Framebuffer(NanoVGGraphics& g): graphics(g)
{
}

Framebuffer::~Framebuffer()
{
    if (auto* ctx = graphics.getContext())
        nvgDeleteFramebuffer(ctx, fbo);
}

void Framebuffer::paint()
{
    jassert(fbo != nullptr);
    auto* ctx = graphics.getContext();
    float scale = graphics.getFramebufferTransformScale();
    NVGpaint img;
    memset(&img, 0, sizeof(NVGpaint));

    nvgTransformScale(img.xform, scale, scale);
    img.extent[0] = width * graphics.getPixelScale();
    img.extent[1] = height * graphics.getPixelScale();
    img.innerColor = nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
    img.image = fbo->image;

    nvgSave(ctx);

    nvgBeginPath(ctx); // clears existing path cache
    nvgRect(ctx, x, y, width, height); // fills the path with a rectangle
    nvgFillPaint(ctx, img); // sets the paint settings to the current state
    nvgFill(ctx); // draw call happens here, using the paint settings

    nvgResetTransform(ctx);
    nvgRestore(ctx);
}

void Framebuffer::setBounds(float x_, float y_, float width_, float height_)
{
    x = x_;
    y = y_;
    width = width_;
    height = height_;

    // invalidate
    valid = false;
}

//==============================================================================

Framebuffer::ScopedBind::ScopedBind(Framebuffer& f)
    : fb(f)
{
    DBG("binding texture");
    auto* ctx = fb.graphics.getContext();
    float pixelScale = fb.graphics.getPixelScale();
    jassert(ctx != nullptr);
    jassert(!fb.valid);

    jassert(fb.width > 0);
    jassert(fb.height > 0);

    // Unfornately there is no way to simply resize the existing texture...
    // https://github.com/Microsoft/DirectXTK/issues/93
    if (fb.fbo == nullptr)
        nvgDeleteFramebuffer(ctx, fb.fbo);

    fb.fbo = nvgCreateFramebuffer(
        ctx,
        juce::roundToInt(fb.width * pixelScale),
        juce::roundToInt(fb.height * pixelScale),
        0);

    nvgEndFrame(ctx);

    nvgBindFramebuffer(fb.fbo);
    nvgBeginFrame(ctx, fb.width, fb.height, pixelScale);
}

Framebuffer::ScopedBind::~ScopedBind()
{
    DBG("releasing bind");
    auto* ctx = fb.graphics.getContext();
    nvgEndFrame(ctx);
    fb.valid = true;

    nvgBindFramebuffer(fb.graphics.getMainFramebuffer());
    nvgBeginFrame(
        ctx,
        fb.graphics.getWindowWidth(),
        fb.graphics.getWindowHeight(),
        fb.graphics.getPixelScale());
}
