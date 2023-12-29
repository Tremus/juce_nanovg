# juce_nanovg

[NanoVG](https://github.com/memononen/nanovg) graphics module for [JUCE](https://github.com/juce-framework/JUCE), based on https://github.com/Archie3d/juce_bgfx

Contains a `juce::LowLevelGraphicsContext` and attachable component for rendering JUCE projects with NanoVG. See the example to find out how to implement this yourself. Note that this does not yet implement masking for arbitrary shapes, which is required for a fully functioning LowLevelGraphicsContext.

A work-in-progress, currently only works on MacOS with Metal and Windows with DirectX 11. I expect to add better Linux support at some point, but if you have experience with NanoVG, OpenGL or Vulkan, let me know!

## Build

- `git clone https://github.com/Tremus/juce_nanovg`
- `cd juce_nanovg`
- `git submodule update --init --recursive --depth=1` (Use depth=1 to avoid downloading all of JUCEs git history)
- Use CMake to build the _juce_nanovg_Standalone_ target.

## Framebuffers

A framebuffer is an image/texture living in your GPUs RAM.

If you prefer to draw shapes solely using the NanoVG API, you will quickly find your GPU usage shoots up from all the draw calls.

In that case, it may be helpful to cache the static parts of your GUI by painting to an image or _framebuffer_. This way you can save draw calls by painting a single pre-rendered image (framebuffer).

An example of caching with framebuffers can be found [here](Source/example/FramebufferTest.h)

## Screenshots

<img width="712" alt="Screenshot 2022-03-25 at 00 28 24" src="https://user-images.githubusercontent.com/44585538/160026228-2c59e3ec-ce98-4492-af4a-cd9611f912c5.png">

<img width="502" alt="Screenshot 2022-03-25 at 00 28 25" src="https://user-images.githubusercontent.com/44585538/160179153-b2fa2d56-2453-4614-98d6-702d730635f3.png">
