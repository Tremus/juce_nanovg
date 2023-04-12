#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../NanoVGGraphics.h"
#include "../NanoVGGraphicsStructs.h"

//==============================================================================

/**
 * Binds & unbinds framebuffers in a neat RAII style way
 */
class FramebufferTest
    : public NanoVGGraphics::Renderer
{
public:
    FramebufferTest(NanoVGGraphics&);
    ~FramebufferTest() override;

    // initialises a font in NanoVG
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
