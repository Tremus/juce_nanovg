#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../NanoVGGraphics.h"
#include "../NanoVGGraphicsStructs.h"

//==============================================================================

/**
 * Binds & unbinds framebuffers in a neat RAII style way
 */
class ScopedFramebufferTest
    : public NanoVGGraphics::Renderer
{
public:
    ScopedFramebufferTest(NanoVGGraphics&);
    ~ScopedFramebufferTest() override;

    // create font
    int onContextCreated() override;
    void draw() override;

    void resized() override;

private:
    NanoVGGraphics& graphics;
    Framebuffer framebuffer;
    int fbDrawCount = 0;
    int drawCount = 0;
    int robotoFontId= -1;
};
