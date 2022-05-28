/*
*
* < GridAndCameraMovement.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Engine/ZekrosEngine.h"

class GridAndCameraMovement : public orangelie::Engine::ZekrosEngine {
private:
	void LoadTextures() {
		m_TextureLoader.AddTexture("whiteb", L"./Textures/StoneBlock.dds", m_Device, m_CommandList.Get());
	}

	void BuildRootSignature() {
		CD3DX12_DESCRIPTOR_RANGE srvRange;
		srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);

		const size_t parameter_size = 4;
		CD3DX12_ROOT_PARAMETER rootParameters[parameter_size];

		rootParameters[0].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL); // SrvHeap
		rootParameters[1].InitAsShaderResourceView(0, 1); // Materials
		rootParameters[2].InitAsConstantBufferView(0); // Object
		rootParameters[3].InitAsConstantBufferView(1); // Pass

		auto samplers = GetStaticSamplers();

		CD3DX12_ROOT_SIGNATURE_DESC signatureDesc(parameter_size, rootParameters, (UINT)samplers.size(), samplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> ppCode = nullptr, ppErrorMsgs = nullptr;
		HRESULT hResult;

		hResult = D3D12SerializeRootSignature(&signatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			ppCode.GetAddressOf(), ppErrorMsgs.GetAddressOf());

		if (ppErrorMsgs != nullptr) {
			MessageBoxA(0, (char*)ppErrorMsgs->GetBufferPointer(), "RootSignature Error", MB_ICONWARNING);
		}
		HR(hResult);

		HR(m_Device->CreateRootSignature(0,
			ppCode->GetBufferPointer(), ppCode->GetBufferSize(),
			IID_PPV_ARGS(m_RootSignature.GetAddressOf())));
	}

	void BuildDescriptorHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC SrvHeapDesc = {};
		SrvHeapDesc.NumDescriptors = 1;
		SrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		SrvHeapDesc.NodeMask = 0;
		SrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		HR(m_Device->CreateDescriptorHeap(&SrvHeapDesc, IID_PPV_ARGS(m_SrvHeap.GetAddressOf())));

		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_SrvHeap->GetCPUDescriptorHandleForHeapStart());

		auto whiteb = m_TextureLoader.GetTexture("whiteb")->GPUHeap;
		// auto stoneb = m_TextureLoader.GetTexture("stoneb")->GPUHeap;

		D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
		SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		SrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		SrvDesc.Format = whiteb->GetDesc().Format;
		SrvDesc.Texture2D.MipLevels = whiteb->GetDesc().MipLevels;
		SrvDesc.Texture2D.MostDetailedMip = 0;
		SrvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		m_Device->CreateShaderResourceView(whiteb.Get(), &SrvDesc, hDescriptor);

		/*
		* 
		hDescriptor.Offset(1, m_CbvSrvUavSize);

		SrvDesc.Format = stoneb->GetDesc().Format;
		SrvDesc.Texture2D.MipLevels = stoneb->GetDesc().MipLevels;

		m_Device->CreateShaderResourceView(stoneb.Get(), &SrvDesc, hDescriptor);

		*/
	}

	void BuildShaderSystem() {
		orangelie::Shader::ShaderRasterizeObjects2 RasterizeObj;
		std::vector<D3D_SHADER_MACRO> shaderMacro = { { NULL, NULL } };

		RasterizeObj.VS = { L"./Shader/Default.hlsl", "vs_5_1", "VS", shaderMacro, true };
		RasterizeObj.PS = { L"./Shader/Default.hlsl", "ps_5_1", "PS", shaderMacro, true };

		m_ShaderSys->ShaderCompileFromFile("sha1", RasterizeObj);

		m_InputElementDesc =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
	}

	void BuildGridGeometry() {
		orangelie::Mesh::GeometryGenerator geoGen;
		auto gridGeo = geoGen.CreateGrid(160.0f, 160.0f, 50, 50);

		using orangelie::Mesh::Vertex2;
		std::vector<Vertex2> vertices(gridGeo.vertices.size());

		for (size_t i = 0; i < gridGeo.vertices.size(); ++i) {
			vertices[i].Position = gridGeo.vertices[i].Position;
			vertices[i].Tangent = gridGeo.vertices[i].Tangent;
			vertices[i].Normal = gridGeo.vertices[i].Normal;
			vertices[i].TexC = gridGeo.vertices[i].TexC;
		}

		std::vector<std::uint16_t> indices = gridGeo.GetIndices16();

		const UINT vertexBufferByteSize = (UINT)vertices.size() * sizeof(Vertex2);
		const UINT indexBufferByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto meshGeo = std::make_unique<orangelie::Mesh::MeshGeometry>();
		meshGeo->Name = "gridGeo";

		HR(D3DCreateBlob(vertexBufferByteSize, &meshGeo->CpuVertexBuffer));
		CopyMemory(meshGeo->CpuVertexBuffer->GetBufferPointer(), vertices.data(), vertexBufferByteSize);
		HR(D3DCreateBlob(indexBufferByteSize, &meshGeo->CpuIndexBuffer));
		CopyMemory(meshGeo->CpuIndexBuffer->GetBufferPointer(), indices.data(), indexBufferByteSize);

		meshGeo->GpuVertexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			vertices.data(), vertexBufferByteSize,
			meshGeo->GpuVertexUploader);
		meshGeo->GpuIndexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			indices.data(), indexBufferByteSize,
			meshGeo->GpuIndexUploader);

		meshGeo->VertexBufferByteSize = vertexBufferByteSize;
		meshGeo->VertexByteStride = sizeof(Vertex2);

		meshGeo->IndexBufferByteSize = indexBufferByteSize;
		meshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;

		orangelie::Mesh::SubmeshGeometry Submesh;
		Submesh.IndexCount = (UINT)indices.size();
		Submesh.BaseVertexLocation = 0;
		Submesh.StartIndexLocation = 0;

		meshGeo->Submeshes["grid"] = Submesh;
		m_Geometrics[meshGeo->Name] = std::move(meshGeo);
	}

	void BuildBoxGeometry() {
		orangelie::Mesh::GeometryGenerator geoGen;
		auto boxGeo = geoGen.CreateBox(8.0f, 8.0f, 8.0f, 3);

		using orangelie::Mesh::Vertex2;
		std::vector<Vertex2> vertices(boxGeo.vertices.size());

		for (size_t i = 0; i < boxGeo.vertices.size(); ++i) {
			vertices[i].Position = boxGeo.vertices[i].Position;
			vertices[i].Tangent = boxGeo.vertices[i].Tangent;
			vertices[i].Normal = boxGeo.vertices[i].Normal;
			vertices[i].TexC = boxGeo.vertices[i].TexC;
		}

		std::vector<std::uint16_t> indices = boxGeo.GetIndices16();

		const UINT vertexBufferByteSize = (UINT)vertices.size() * sizeof(Vertex2);
		const UINT indexBufferByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto meshGeo = std::make_unique<orangelie::Mesh::MeshGeometry>();
		meshGeo->Name = "boxGeo";

		HR(D3DCreateBlob(vertexBufferByteSize, &meshGeo->CpuVertexBuffer));
		CopyMemory(meshGeo->CpuVertexBuffer->GetBufferPointer(), vertices.data(), vertexBufferByteSize);
		HR(D3DCreateBlob(indexBufferByteSize, &meshGeo->CpuIndexBuffer));
		CopyMemory(meshGeo->CpuIndexBuffer->GetBufferPointer(), indices.data(), indexBufferByteSize);

		meshGeo->GpuVertexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			vertices.data(), vertexBufferByteSize,
			meshGeo->GpuVertexUploader);
		meshGeo->GpuIndexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			indices.data(), indexBufferByteSize,
			meshGeo->GpuIndexUploader);

		meshGeo->VertexBufferByteSize = vertexBufferByteSize;
		meshGeo->VertexByteStride = sizeof(Vertex2);

		meshGeo->IndexBufferByteSize = indexBufferByteSize;
		meshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;

		orangelie::Mesh::SubmeshGeometry Submesh;
		Submesh.IndexCount = (UINT)indices.size();
		Submesh.BaseVertexLocation = 0;
		Submesh.StartIndexLocation = 0;

		meshGeo->Submeshes["box"] = Submesh;
		m_Geometrics[meshGeo->Name] = std::move(meshGeo);
	}

	void BuildFontEngine() {
		const UINT maxLength = 16;
		XMMATRIX viewMatrix = XMLoadFloat4x4(&orangelie::CppStdUtil::unmove(m_Camera.GetViewMatrix()));

		m_FontEngine.Initialize(
			m_Device, m_CommandList.Get(), maxLength, m_ClientWidth, m_ClientHeight,
			viewMatrix, 1.0f, 1.0f, 1.0f, 0, "Hello World", 100, 100);

	}

	void BuildMaterials() {
		auto white = std::make_unique<orangelie::Lighting::Material>();
		white->MatTransform = orangelie::Utility::Tools::Identity();
		white->MatIndex = 0;
		white->SrvHeapIndex = 0;
		white->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		white->R0 = { 0.2f, 0.2f, 0.2f };
		white->Roughness = 0.0f;

		m_Materials["white"] = std::move(white);

		/*
		* 
		auto stone = std::make_unique<orangelie::Lighting::Material>();
		stone->MatTransform = orangelie::Utility::Tools::Identity();
		stone->MatIndex = 1;
		stone->SrvHeapIndex = 1;
		stone->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		stone->R0 = { 0.2f, 0.2f, 0.2f };
		stone->Roughness = 0.0f;

		m_Materials["stone"] = std::move(stone);

		*/
	}

	void BuildRenderItems() {
		auto GridRitem = std::make_unique<orangelie::Rendering::RenderItem>();
		GridRitem->ObjCBIndex = 0;
		GridRitem->World = orangelie::Utility::Tools::Identity();
		XMStoreFloat4x4(&GridRitem->TexTransform, XMMatrixScaling(5.0f, 5.0f, 1.0f));
		GridRitem->MeshGeo = m_Geometrics["gridGeo"].get();
		GridRitem->IndexCount = GridRitem->MeshGeo->Submeshes["grid"].IndexCount;
		GridRitem->StartIndexLocation = GridRitem->MeshGeo->Submeshes["grid"].StartIndexLocation;
		GridRitem->BaseVertexLocation = GridRitem->MeshGeo->Submeshes["grid"].BaseVertexLocation;
		GridRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		GridRitem->NumFramesDirty = 3;
		GridRitem->Material = m_Materials["white"].get();


		/*
		
		auto BoxRitem = std::make_unique<orangelie::Rendering::RenderItem>();
		BoxRitem->ObjCBIndex = 1;
		BoxRitem->World = orangelie::Utility::Tools::Identity();
		XMStoreFloat4x4(&BoxRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		BoxRitem->MeshGeo = m_Geometrics["boxGeo"].get();
		BoxRitem->IndexCount = BoxRitem->MeshGeo->Submeshes["box"].IndexCount;
		BoxRitem->StartIndexLocation = BoxRitem->MeshGeo->Submeshes["box"].StartIndexLocation;
		BoxRitem->BaseVertexLocation = BoxRitem->MeshGeo->Submeshes["box"].BaseVertexLocation;
		BoxRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		BoxRitem->NumFramesDirty = 3;
		BoxRitem->Material = m_Materials["stone"].get();

		m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque].push_back(BoxRitem.get());
		m_AllRitems.push_back(std::move(BoxRitem));

		*/
		
		m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque].push_back(GridRitem.get());
		m_AllRitems.push_back(std::move(GridRitem));
	}

	void BuildFrameResources() {
		for (size_t i = 0; i < gFrameResourceCount; ++i) {
			m_FrameResources.push_back(std::make_unique<orangelie::FrameResource>(
				m_Device, (UINT)m_AllRitems.size(), 1, (UINT)m_Materials.size(), 1));
		}
	}

	void BuildPSOs() {
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
		GraphicsPSODesc.SampleDesc.Quality = 0;
		GraphicsPSODesc.SampleMask = UINT_MAX;
		GraphicsPSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;

		auto shaderBin = m_ShaderSys->Resource("sha1");

		GraphicsPSODesc.VS = {
			reinterpret_cast<BYTE*>(shaderBin.VSbinCode->GetBufferPointer()),
			shaderBin.VSbinCode->GetBufferSize()
		};
		GraphicsPSODesc.PS = {
			reinterpret_cast<BYTE*>(shaderBin.PSbinCode->GetBufferPointer()),
			shaderBin.PSbinCode->GetBufferSize()
		};

		HR(m_Device->CreateGraphicsPipelineState(&GraphicsPSODesc, IID_PPV_ARGS(m_PSOs["opaque"].GetAddressOf())));

		GraphicsPSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		HR(m_Device->CreateGraphicsPipelineState(&GraphicsPSODesc, IID_PPV_ARGS(m_PSOs["opaque_wireframe"].GetAddressOf())));
	}

	void DrawRenderItems(std::vector<orangelie::Rendering::RenderItem*> rItems) {
		auto ObjConst = m_CurrFrameResource->m_ObjCB->Resource();
		const UINT objSize = orangelie::Utility::Tools::CalcConstantBufferByteSize(sizeof(orangelie::ObjConstants));

		for (auto& r : rItems) {
			D3D12_GPU_VIRTUAL_ADDRESS objAddress = ObjConst->GetGPUVirtualAddress() + objSize * r->ObjCBIndex;
			m_CommandList->SetGraphicsRootConstantBufferView(2, objAddress);

			m_CommandList->IASetVertexBuffers(0, 1, &orangelie::CppStdUtil::unmove(r->MeshGeo->VertexBufferView()));
			m_CommandList->IASetIndexBuffer(&orangelie::CppStdUtil::unmove(r->MeshGeo->IndexBufferView()));
			m_CommandList->IASetPrimitiveTopology(r->PrimitiveTopology);

			m_CommandList->DrawIndexedInstanced(r->IndexCount, 1, r->StartIndexLocation, r->BaseVertexLocation, 0);
		}
	}

	void AnimateMaterialBuffer(float dt) {
		auto mat = m_Materials["white"].get();

		float u = mat->MatTransform(3, 0);
		float v = mat->MatTransform(3, 1);

		u += 0.1f * dt;
		v += 0.02f * dt;

		if (u >= 1.0f) {
			u -= 1.0f;
		}
		if (v >= 1.0f) {
			v -= 1.0f;
		}

		mat->MatTransform(3, 0) = u;
		mat->MatTransform(3, 1) = v;

		mat->NumFramesDirty = gNumFramesDirty;
	}

	void UpdateObjectCBs() {
		auto objCB = m_CurrFrameResource->m_ObjCB.get();

		for (auto& e : m_AllRitems) {
			if (e->NumFramesDirty > 0) {
				XMMATRIX world = XMLoadFloat4x4(&e->World);
				XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

				orangelie::ObjConstants objConstants;
				XMStoreFloat4x4(&objConstants.gWorld, XMMatrixTranspose(world));
				XMStoreFloat4x4(&objConstants.gTexTransform, XMMatrixTranspose(texTransform));
				objConstants.gMatIndex = e->Material->MatIndex;

				objCB->CopyData(e->ObjCBIndex, objConstants);

				--(e->NumFramesDirty);
			}
		}
	}

	void UpdatePassCBs() {
		auto passCB = m_CurrFrameResource->m_PassCB.get();

		XMMATRIX P = XMLoadFloat4x4(&orangelie::CppStdUtil::unmove(m_Camera.GetProjectionMatrix()));
		XMMATRIX V = XMLoadFloat4x4(&orangelie::CppStdUtil::unmove(m_Camera.GetViewMatrix()));

		orangelie::PassConstants passConstants;
		XMStoreFloat4x4(&passConstants.gProj, XMMatrixTranspose(P));
		XMStoreFloat4x4(&passConstants.gView, XMMatrixTranspose(V));
		XMStoreFloat4x4(&passConstants.gViewProj, XMMatrixTranspose(XMMatrixMultiply(V, P)));

		passConstants.gEyePos = m_Camera.GetPosition();
		passConstants.gAmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
		passConstants.gLights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
		passConstants.gLights[0].Strength = { 0.6f, 0.6f, 0.6f };
		passConstants.gLights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
		passConstants.gLights[1].Strength = { 0.3f, 0.3f, 0.3f };
		passConstants.gLights[2].Direction = { 0.0f, -0.707f, -0.707f };
		passConstants.gLights[2].Strength = { 0.15f, 0.15f, 0.15f };

		passCB->CopyData(0, passConstants);
	}

	void UpdateMatVBs() {
		auto mat = m_CurrFrameResource->m_MatVB.get();

		for (auto& m : m_Materials) {
			auto r = m.second.get();

			if (r->NumFramesDirty > 0) {
				XMMATRIX matTrans = XMLoadFloat4x4(&r->MatTransform);

				orangelie::MaterialConstants MatConsts;
				XMStoreFloat4x4(&MatConsts.gMatTransform, XMMatrixTranspose(matTrans));
				MatConsts.gDiffuseAlbedo = r->DiffuseAlbedo;
				MatConsts.R0 = r->R0;
				MatConsts.Roughness = r->Roughness;
				MatConsts.SrvHeapIndex = r->SrvHeapIndex;

				mat->CopyData(r->MatIndex, MatConsts);

				--(r->NumFramesDirty);
			}
		}
	}

	void OnKeyboardInput(float dt) {
		const float speed = 5.0f;

		if ((GetAsyncKeyState('W') & 0x8000))
			m_Camera.Walk(speed * dt);
		if ((GetAsyncKeyState('S') & 0x8000))
			m_Camera.Walk(-speed * dt);
		if ((GetAsyncKeyState('A') & 0x8000))
			m_Camera.Strafe(-speed * dt);
		if ((GetAsyncKeyState('D') & 0x8000))
			m_Camera.Strafe(speed * dt);

		m_Camera.UpdateViewMatrix();

		if ((GetAsyncKeyState('1') & 0x8000))
			m_IsWireframeMode = true;
		if ((GetAsyncKeyState('2') & 0x8000))
			m_IsWireframeMode = false;
	}

protected:
	virtual void OnResize() {
		ZekrosEngine::OnResize();

		// TODO: Something here...
		m_Camera.SetLens((float)m_ClientWidth, (float)m_ClientHeight, 0.25f * XM_PI, ((float)m_ClientWidth / m_ClientHeight), 1.0f, 1000.0f);
	}

	virtual void init() {
		m_Camera.SetPosition(0.0f, 1.0f, -5.0f);

		HR(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		// TODO: Something here...
		LoadTextures();
		BuildRootSignature();
		BuildDescriptorHeap();
		BuildShaderSystem();
		BuildGridGeometry();
		BuildBoxGeometry();
		BuildFontEngine();
		BuildMaterials();
		BuildRenderItems();
		BuildFrameResources();
		BuildPSOs();


		SubmitCommandList();
		FlushCommandQueue();
	}

	virtual void update(float dt) override {
		OnKeyboardInput(dt);

		m_CurrFrameResourceIndex = (m_CurrFrameResourceIndex + 1) % gFrameResourceCount;
		m_CurrFrameResource = m_FrameResources[m_CurrFrameResourceIndex].get();
		m_CurrFrameResource->m_Fence = m_CurrentFenceCount;

		if (m_Fence->GetCompletedValue() < m_CurrFrameResource->m_Fence && m_CurrFrameResource->m_Fence != 0) {
			HANDLE hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			m_Fence->SetEventOnCompletion(m_CurrFrameResource->m_Fence, hEvent);
			WaitForSingleObject(hEvent, 0xffffffff);
			CloseHandle(hEvent);
		}

		//AnimateMaterialBuffer(dt);
		UpdateObjectCBs();
		UpdatePassCBs();
		UpdateMatVBs();
	}

	virtual void draw(float dt) override {
		HR(m_CurrFrameResource->m_CommandAllocator->Reset());

		if (m_IsWireframeMode) {
			HR(m_CommandList->Reset(m_CurrFrameResource->m_CommandAllocator.Get(), m_PSOs["opaque_wireframe"].Get()));
		}
		else if (!m_IsWireframeMode) {
			HR(m_CommandList->Reset(m_CurrFrameResource->m_CommandAllocator.Get(), m_PSOs["opaque"].Get()));
		}

		auto rtvHandle = CurrentBackBufferView();
		auto dsvHandle = DepthStencilView();

		using orangelie::CppStdUtil::unmove;
		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET)));

		const FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_CommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		ID3D12DescriptorHeap* descHeaps[] = { m_SrvHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		auto PassConst = m_CurrFrameResource->m_PassCB->Resource();
		m_CommandList->SetGraphicsRootConstantBufferView(3, PassConst->GetGPUVirtualAddress());

		auto MatVB = m_CurrFrameResource->m_MatVB->Resource();
		m_CommandList->SetGraphicsRootShaderResourceView(1, MatVB->GetGPUVirtualAddress());

		CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_SrvHeap->GetGPUDescriptorHandleForHeapStart());
		m_CommandList->SetGraphicsRootDescriptorTable(0, srvHandle);

		DrawRenderItems(m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque]);

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
		HR(m_CommandQueue->Signal(m_Fence, m_CurrentFenceCount));
	}

	virtual void MouseDown(WPARAM btnState, int x, int y) override {
		m_LastPrevPoint.x = x;
		m_LastPrevPoint.y = y;

		SetCapture(m_hWnd);
	}

	virtual void MouseUp(WPARAM btnState, int x, int y) override {
		ReleaseCapture();
	}

	virtual void MouseMove(WPARAM btnState, int x, int y, float dt) override {
		if ((btnState & MK_RBUTTON) != 0) {

		}

		else if ((btnState & MK_LBUTTON) != 0) {
			float dx = XMConvertToRadians(0.25f * (static_cast<float>(x - m_LastPrevPoint.x)));
			float dy = XMConvertToRadians(0.25f * (static_cast<float>(y - m_LastPrevPoint.y)));

			m_Camera.Pitch(dy, dt);
			m_Camera.RotateY(dx, dt);
		}

		m_LastPrevPoint.x = x;
		m_LastPrevPoint.y = y;
	}

private:
	// RootSignature & Shaders & PSO
	ComPtr<ID3D12RootSignature> m_RootSignature;
	std::unique_ptr<orangelie::Shader::ShaderSystem> m_ShaderSys = std::make_unique<orangelie::Shader::ShaderSystem>();
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputElementDesc;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_PSOs;
	bool m_IsWireframeMode = true;

	// Rendering & RenderItems
	std::vector<std::unique_ptr<orangelie::Rendering::RenderItem>> m_AllRitems;
	std::vector<orangelie::Rendering::RenderItem*> m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Count];
	std::unordered_map<std::string, std::unique_ptr<orangelie::Mesh::MeshGeometry>> m_Geometrics;

	// Materials & Lighting & Textures
	std::unordered_map<std::string, std::unique_ptr<orangelie::Lighting::Material>> m_Materials;
	orangelie::Texture::TextureLoader m_TextureLoader;

	// Win32 Utils
	POINT m_LastPrevPoint;

	// Camera
	orangelie::Camera::DefaultCamera m_Camera;

	// Descriptor Heaps
	ComPtr<ID3D12DescriptorHeap> m_SrvHeap;

	// Font Engine
	orangelie::Font::FontEngine m_FontEngine;

};