//
// Game.cpp
//

#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"SceneManager.h"
#include"GameObjectManager.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;


using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_audioEngine)
    {
        m_audioEngine->Suspend();
    }
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    auto deviceAccessor = DeviceAccessor::GetInstance();
    deviceAccessor->SetElapsedTime(&elapsedTime);

    auto sceneManager = SceneManager::GetInstance();
    sceneManager->Update();
    
    elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
    auto sceneManager = SceneManager::GetInstance();
    m_deviceResources->PIXBeginEvent(L"OffScreenRender");
    //オフスクリーン描画
    sceneManager->DrawOffScreen();

    m_deviceResources->PIXEndEvent();

    Clear();
    m_deviceResources->PIXBeginEvent(L"Render");

    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    //通常レンダリング
    sceneManager->Draw();

    m_deviceResources->PIXEndEvent();

    //LUTポストプロセス
    m_deviceResources->PIXBeginEvent(L"LUT");
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();
    ID3D11RenderTargetView* rtv[2] = { NULL,NULL };
    rtv[0] = renderTarget;
    context->OMSetRenderTargets(2, rtv, depthStencil);
    GameObjectManager::GetInstance()->DrawLUT();
    m_deviceResources->PIXEndEvent();

    //SSAOポストプロセス
    m_deviceResources->PIXBeginEvent(L"SSAO");
    GameObjectManager::GetInstance()->DrawAmbientOcclusion();
    m_deviceResources->PIXEndEvent();

    //Bloomポストプロセス
    m_deviceResources->PIXBeginEvent(L"Bloom");
    GameObjectManager::GetInstance()->DrawBloom(renderTarget, depthStencil);
    m_deviceResources->PIXEndEvent();

    context;

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->ClearRenderTargetView(GameObjectManager::GetInstance()->GetNormalDepthRTV(), Colors::Black);
    context->ClearRenderTargetView(GameObjectManager::GetInstance()->GetLUTColorRTV(), Colors::Black);
    context->ClearRenderTargetView(GameObjectManager::GetInstance()->GetBloomRTV(), Colors::Black);
    ID3D11RenderTargetView* rtv[4] = { NULL,NULL,NULL,NULL };
    rtv[0] = renderTarget;
    rtv[1] = GameObjectManager::GetInstance()->GetNormalDepthRTV();
    rtv[2] = GameObjectManager::GetInstance()->GetLUTColorRTV();
    rtv[3] = GameObjectManager::GetInstance()->GetBloomRTV();
    context->OMSetRenderTargets(4, rtv, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    auto deviceAccessor = DeviceAccessor::GetInstance();
    deviceAccessor->GetGamePad()->Suspend();
    m_audioEngine->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    auto deviceAccessor = DeviceAccessor::GetInstance();
    deviceAccessor->GetGamePad()->Resume();
    m_audioEngine->Resume();
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
    m_audioEngine = make_unique<AudioEngine>(eflags);
    Json::CreateInstance();
    DeviceAccessor::CreateInstance(device,
        context,
        m_deviceResources->GetRenderTargetView(),
        m_deviceResources->GetDepthStencilView(),
        m_deviceResources->GetOutputSize(),
        m_audioEngine.get());
    GameObjectManager::CreateInstance();
    SceneManager::CreateInstance();

    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    Json::DestroyInstance();
    DeviceAccessor::DestroyInstance();
    GameObjectManager::DestroyInstance();
    SceneManager::DestroyInstance();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
