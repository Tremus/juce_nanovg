#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <nanovg_compat.h>

#define FPS 60

// A lot of things in here will look similar to iplug::iGraphics
// It's how I learned how to get iPlug working correctly
class NanoVGGraphics
    : public juce::ComponentListener
    , public juce::Timer
{
public:
    struct Renderer : public juce::Component
    {
        // Called after Graphics engine is initiliased
        virtual int onContextCreated() { return 0; }
        // Called before Graphics engine is shutdown
        virtual void onContextDestroyed() {}
        // Override to do your components drawing
        virtual void draw()=0;
    };

    //==============================================================================

    NanoVGGraphics();
    ~NanoVGGraphics() override;

    void setComponent(Renderer*);
    void componentMovedOrResized (juce::Component& component, bool wasMoved, bool wasResized) override;
    void timerCallback() override;

    NVGcontext* getContext() const { return nvg; }
    int getMainFramebuffer() const { return mainFrameBuffer; }

    // Used for downscaling frame buffers when painting them
    float getFramebufferTransformScale() const noexcept { return 1.0f / (pixelScale * drawScale); }

    float getWindowWidth() const noexcept { return component->getWidth() * drawScale; }
    float getWindowHeight() const noexcept { return component->getHeight() * drawScale; }

    // begin main frame buffer update
    void beginFrame();
    // end main frame buffer update and draw to back buffer
    void endFrame();

    bool initialise();
    void shutdown();

    void updatePixelScale();

    Renderer* component = NULL;

    NVGcontext* nvg = NULL;
    int mainFrameBuffer = 0;

    // eg. 2 for mac retina, 1.5 for windows
    float pixelScale = 1.0f;
    // scale deviation from  default width and height i.e stretching the UI by dragging bottom right hand corner
    float drawScale = 1.0f;

private:
    JUCE_DECLARE_NON_COPYABLE(NanoVGGraphics)
};
