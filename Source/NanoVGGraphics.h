#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "nanovg_compat/nanovg_compat.h"


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
        // Call to draw frame
        virtual void draw()=0;
    };

    //==============================================================================

    NanoVGGraphics();
    ~NanoVGGraphics() override;

    void setComponent(Renderer*);
    void componentMovedOrResized (juce::Component& component, bool wasMoved, bool wasResized) override;
    void timerCallback() override;

    NVGcontext* getContext() const { return nvg; }
    NVGframebuffer* getMainFramebuffer() const { return mainFrameBuffer; }

    // eg. 2 for mac retina, 1.5 for windows
    float getPixelScale() const noexcept { return pixelScale; }
    // scale deviation from  default width and height i.e stretching the UI by dragging bottom right hand corner
    float getDrawScale() const noexcept { return drawScale; }
    float getFramebufferTransformScale() const noexcept { return framebufferTransformScale; }

    float getWindowWidth() const noexcept { return component->getWidth() * getDrawScale(); }
    float getWindowHeight() const noexcept { return component->getHeight() * getDrawScale(); }
protected:
    void render();
private:
    // begin main frame buffer update
    void beginFrame();
    // end main frame buffer update and draw to back buffer
    void endFrame();

    bool initialise();
    void shutdown();

    Renderer* component;

    NVGcontext* nvg = nullptr;
    NVGframebuffer* mainFrameBuffer = nullptr;
    float pixelScale = 1.0f;
    float drawScale = 1.0f;
    // Used for scaling frame buffers when painting them
    // 1 / (pixelscale / drawscale)
    float framebufferTransformScale = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoVGGraphics)
};
