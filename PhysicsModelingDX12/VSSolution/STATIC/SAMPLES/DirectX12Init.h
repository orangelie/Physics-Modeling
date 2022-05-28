#include "HEADER/PUBLIC/Engine/ZekrosEngine.h"

class DirectX12Init : public orangelie::Engine::ZekrosEngine {
private:
	void BuildFrameResources() {
		for (size_t i = 0; i < gFrameResourceCount; ++i) {
			m_FrameResources.push_back(std::make_unique<orangelie::FrameResource>(m_Device));
		}
	}

protected:
	virtual void OnResize() {
		ZekrosEngine::OnResize();

		// TODO: Something here...
	}

	virtual void init() {
		HR(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		// TODO: Something here...
		BuildFrameResources();

		SubmitCommandList();
		FlushCommandQueue();
	}

	virtual void update(float dt) override {
		m_CurrFrameResourceIndex = (m_CurrFrameResourceIndex + 1) % gFrameResourceCount;
		m_CurrFrameResource = m_FrameResources[m_CurrFrameResourceIndex].get();
		m_CurrFrameResource->m_Fence = m_CurrentFenceCount;

		if (m_Fence->GetCompletedValue() < m_CurrFrameResource->m_Fence && m_CurrFrameResource->m_Fence != 0) {
			HANDLE hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			m_Fence->SetEventOnCompletion(m_CurrFrameResource->m_Fence, hEvent);
			WaitForSingleObject(hEvent, 0xffffffff);
			CloseHandle(hEvent);
		}
	}

	virtual void draw(float dt) override {
		HR(m_CurrFrameResource->m_CommandAllocator->Reset());
		HR(m_CommandList->Reset(m_CurrFrameResource->m_CommandAllocator.Get(), nullptr));

		auto rtvHandle = CurrentBackBufferView();
		auto dsvHandle = DepthStencilView();

		using orangelie::CppStdUtil::unmove;
		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET)));

		const FLOAT color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_CommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT)));

		SubmitCommandList();
		PresentSwapChain();

		m_CurrentSwapBufferIndex = (m_CurrentSwapBufferIndex + 1) % gBackBufferCount;

		// Only without frame resources.
		// FlushCommandQueue(); 

		m_CurrFrameResource->m_Fence = ++m_CurrentFenceCount;
		HR(m_CommandQueue->Signal(m_Fence, m_CurrFrameResource->m_Fence));
	}

	virtual void MouseDown(WPARAM btnState, int x, int y) override {

	}

	virtual void MouseUp(WPARAM btnState, int x, int y) override {

	}

	void MouseMove(WPARAM btnState, int x, int y) override {

	}

private:

};