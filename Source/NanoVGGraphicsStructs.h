#pragma once
#include "NanoVGGraphics.h"


// Handles most of the tricky stuff with framebuffers
struct Framebuffer
{
    /*
    ScopedBind Usage:
    {
        // binds to the framebuffer upon construction
        Framebuffer::ScopedBind bind(b);

        // draw here
        nvgBeginPath()
        nvg.... some other methods
        ...
        ...
        // end of scope, binds back to main buffer in the destructor
    }
    fb.paint();
    */
    struct ScopedBind
    {
        ScopedBind(Framebuffer&);
        ~ScopedBind();
        Framebuffer& fb;
    };

    Framebuffer(NanoVGGraphics&);
    ~Framebuffer();

    // paints the framebuffer
    void paint();
    void setBounds(float x, float y, float width, float height);

    NanoVGGraphics& graphics;
    NVGframebuffer* fbo = nullptr;
    // the above paint() method with use these coords to paint the image
    float x = 0, y = 0, width = 0, height = 0;
    bool valid = false;
};
