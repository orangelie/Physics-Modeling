#include "GridAndCameraMovement.h"

inline void GridAndCameraMovement::BuildPSOs() {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPSODesc = {};
	GraphicsPSODesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	GraphicsPSODesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	GraphicsPSODesc.DSVFormat = m_DepthStencilFormat;
	GraphicsPSODesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	GraphicsPSODesc.InputLayout = {
		m_InputElementDesc.data(),
		(UINT)m_InputElementDesc.size(),
	};
	GraphicsPSODesc.NodeMask = 0;
	GraphicsPSODesc.NumRenderTargets = 1;
	GraphicsPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	GraphicsPSODesc.pRootSignature = m_RootSignature.Get();
	GraphicsPSODesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GraphicsPSODesc.RTVFormats[0] = m_BackBufferFormat;
	GraphicsPSODesc.SampleDesc.Count = 1;
	GraphicsPSODesc




}
