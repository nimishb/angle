//
// Copyright (c) 2012 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// SwapChain11.h: Defines a back-end specific class for the D3D11 swap chain.

#ifndef LIBANGLE_RENDERER_D3D_D3D11_SWAPCHAIN11_H_
#define LIBANGLE_RENDERER_D3D_D3D11_SWAPCHAIN11_H_

#include "common/angleutils.h"
#include "libANGLE/renderer/d3d/SwapChainD3D.h"
#include "libANGLE/renderer/d3d/d3d11/RenderTarget11.h"

namespace rx
{
class Renderer11;
class NativeWindow11;

class SwapChain11 final : public SwapChainD3D
{
  public:
    SwapChain11(Renderer11 *renderer,
                NativeWindow11 *nativeWindow,
                HANDLE shareHandle,
                IUnknown *d3dTexture,
                GLenum backBufferFormat,
                GLenum depthBufferFormat,
                EGLint orientation,
                EGLint samples);
    virtual ~SwapChain11();

    EGLint resize(const gl::Context *context,
                  EGLint backbufferWidth,
                  EGLint backbufferHeight) override;
    EGLint reset(const gl::Context *context,
                 EGLint backbufferWidth,
                 EGLint backbufferHeight,
                 EGLint swapInterval) override;
    EGLint swapRect(const gl::Context *context,
                    EGLint x,
                    EGLint y,
                    EGLint width,
                    EGLint height) override;
    void recreate() override;

    RenderTargetD3D *getColorRenderTarget() override { return &mColorRenderTarget; }
    RenderTargetD3D *getDepthStencilRenderTarget() override { return &mDepthStencilRenderTarget; }

    const TextureHelper11 &getOffscreenTexture();
    const d3d11::RenderTargetView &getRenderTarget();
    const d3d11::SharedSRV &getRenderTargetShaderResource();

    const TextureHelper11 &getDepthStencilTexture();
    const d3d11::DepthStencilView &getDepthStencil();
    const d3d11::SharedSRV &getDepthStencilShaderResource();

    EGLint getWidth() const { return mWidth; }
    EGLint getHeight() const { return mHeight; }
    void *getKeyedMutex() override { return mKeyedMutex; }
    EGLint getSamples() const { return mEGLSamples; }

    egl::Error getSyncValues(EGLuint64KHR *ust, EGLuint64KHR *msc, EGLuint64KHR *sbc) override;

  private:
    void release();
    void initPassThroughResources();

    void releaseOffscreenColorBuffer();
    void releaseOffscreenDepthBuffer();
    EGLint resetOffscreenBuffers(const gl::Context *context,
                                 int backbufferWidth,
                                 int backbufferHeight);
    EGLint resetOffscreenColorBuffer(const gl::Context *context,
                                     int backbufferWidth,
                                     int backbufferHeight);
    EGLint resetOffscreenDepthBuffer(int backbufferWidth, int backbufferHeight);

    DXGI_FORMAT getSwapChainNativeFormat() const;

    EGLint copyOffscreenToBackbuffer(const gl::Context *context,
                                     EGLint x,
                                     EGLint y,
                                     EGLint width,
                                     EGLint height);
    EGLint present(const gl::Context *context, EGLint x, EGLint y, EGLint width, EGLint height);
    UINT getD3DSamples() const;

    Renderer11 *mRenderer;
    EGLint mWidth;
    EGLint mHeight;
    const EGLint mOrientation;
    bool mAppCreatedShareHandle;
    unsigned int mSwapInterval;
    bool mPassThroughResourcesInit;

    NativeWindow11 *mNativeWindow;  // Handler for the Window that the surface is created for.

    bool mFirstSwap;
    IDXGISwapChain *mSwapChain;
    IDXGISwapChain1 *mSwapChain1;
    IDXGIKeyedMutex *mKeyedMutex;

    TextureHelper11 mBackBufferTexture;
    d3d11::RenderTargetView mBackBufferRTView;
    d3d11::SharedSRV mBackBufferSRView;

    const bool mNeedsOffscreenTexture;
    TextureHelper11 mOffscreenTexture;
    d3d11::RenderTargetView mOffscreenRTView;
    d3d11::SharedSRV mOffscreenSRView;
    TextureHelper11 mOffscreenTextureCopyForSRV;

    TextureHelper11 mDepthStencilTexture;
    d3d11::DepthStencilView mDepthStencilDSView;
    d3d11::SharedSRV mDepthStencilSRView;

    d3d11::Buffer mQuadVB;
    d3d11::SamplerState mPassThroughSampler;
    d3d11::InputLayout mPassThroughIL;
    d3d11::VertexShader mPassThroughVS;
    d3d11::PixelShader mPassThroughPS;
    d3d11::RasterizerState mPassThroughRS;

    SurfaceRenderTarget11 mColorRenderTarget;
    SurfaceRenderTarget11 mDepthStencilRenderTarget;

    EGLint mEGLSamples;
    LONGLONG mQPCFrequency;
};

}  // namespace rx
#endif // LIBANGLE_RENDERER_D3D_D3D11_SWAPCHAIN11_H_
