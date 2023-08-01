#pragma once
#include "../NanoVGGraphics.h"
#include "../NanoVGGraphicsStructs.h"
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================

/**
 * Binds & unbinds framebuffers in a neat RAII style way
 */
class FramebufferTest : public NanoVGGraphics::Renderer
{
public:
    FramebufferTest(NanoVGGraphics&);
    ~FramebufferTest() override;

    // initialises a font in NanoVG
    int onContextCreated() override;
    void draw() override;
    void resized() override;

private:
    // Test to check JUCE mouse events still work on child components
    struct Child : juce::Component
    {
        void mouseEnter(const juce::MouseEvent&) override { hover = true; }
        void mouseExit(const juce::MouseEvent&) override { hover = false; }
        void draw(NVGcontext*);
        bool hover = false;
    };

    NanoVGGraphics& graphics;
    Framebuffer framebuffer;
    int fbDrawCount  = 0;
    int drawCount    = 0;
    int robotoFontId = -1;

    Child child;
};
