#pragma once

/*
*
* < PhysicsEngine.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Engine/ZekrosEngine.h"

struct Vector3f {
	float x, y, z;

	friend Vector3f operator+(Vector3f v1, Vector3f v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	friend Vector3f operator-(Vector3f v1, Vector3f v2) {
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3f& operator+=(Vector3f v1) {
		*this = { this->x + v1.x, this->y + v1.y, this->z + v1.z };
		return *this;
	}

	friend Vector3f operator*(Vector3f v1, Vector3f v2) {
		return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
	}

	friend Vector3f operator*(Vector3f v1, float s) {
		return { v1.x * s, v1.y * s, v1.z * s };
	}

	friend Vector3f operator*(float s, Vector3f v1) {
		return { v1.x * s, v1.y * s, v1.z * s };
	}

	friend Vector3f operator/(Vector3f v1, float s) {
		return { v1.x / s, v1.y / s, v1.z / s };
	}
};

struct GeometryOpt {
public:
	Vector3f force = { 0.0f, 0.0f, 0.0f };
	Vector3f acceleration = { 0.0f, 0.0f, 0.0f };
	Vector3f velocity = { 0.0f, 0.0f, 0.0f };
	Vector3f position = { 0.0f, 0.0f, 0.0f };
	float m = 0.00001f;

public:
	virtual void SetScale(float x, float y, float z) = 0;
	virtual Vector3f GetScale() = 0;
	virtual UINT GetID() = 0;

};

struct SphereOpt {
	float x, y, z;
	float scale_x, scale_y, scale_z;
	std::string color;

	SphereOpt(float x, float y, float z, float sx, float sy, float sz, std::string color) {
		this->x = x;
		this->y = y;
		this->z = z;

		this->scale_x = sx;
		this->scale_y = sy;
		this->scale_z = sz;

		this->color = color;
	}
};

struct Sphere : public GeometryOpt {
private:
	UINT ID;
	float radius;
	float sx, sy, sz;

public:
	Sphere() {}
	Sphere(UINT id) : ID(id) {}

	void SetRadius(float sx, float sy, float sz) { radius = 1.0f * max(sx, max(sy, sz)); }
	float GetRadius() { return radius; }

	virtual UINT GetID() override { return ID; }

	void SetScale(float x, float y, float z) { this->sx = x; this->sy = y; this->sz = z; }
	Vector3f GetScale() { return { sx, sy, sz }; }
};

struct BoxOpt {
	float x, y, z;
	float scale_x, scale_y, scale_z;
	std::string color;

	BoxOpt(float x, float y, float z, float sx, float sy, float sz, std::string color) {
		this->x = x;
		this->y = y;
		this->z = z;

		this->scale_x = sx;
		this->scale_y = sy;
		this->scale_z = sz;

		this->color = color;
	}
};

struct Box : public GeometryOpt {
private:
	UINT ID;
	float length, height, width;
	float sx, sy, sz;

public:
	Box() {}
	Box(UINT id) : ID(id) {}

	void SetSize(float sx, float sy, float sz) {
		length = 1.0f * sx;
		height = 1.0f * sy;
		width = 1.0f * sz;
	}
	Vector3f GetSize() { return { length, height, width }; }

	UINT GetID() { return ID; }

	void SetScale(float x, float y, float z) { this->sx = x; this->sy = y; this->sz = z; }
	Vector3f GetScale() { return { sx, sy, sz }; }
};

class PhysicsEngine : public orangelie::Engine::ZekrosEngine {
protected:
	void SetCameraSpeed(float d) {
		m_CameraSpeed = d;
	}

	void SetTitle(const std::string title) {
		std::string n = "[ " + title + " ]            by ZekrosEngine@orangelie";
		SetWindowTextA(m_hWnd, n.c_str());
	}

	std::unique_ptr<Sphere> sphere(SphereOpt sphereOpt) {
		auto sph = std::make_unique<Sphere>(m_ID);
		sph->position = { sphereOpt.x, sphereOpt.y, sphereOpt.z };
		sph->SetScale(sphereOpt.scale_x, sphereOpt.scale_y, sphereOpt.scale_z);
		sph->SetRadius(sphereOpt.scale_x, sphereOpt.scale_y, sphereOpt.scale_z);

		auto EnvSphereRitem = std::make_unique<orangelie::Rendering::RenderItem>();
		EnvSphereRitem->ObjCBIndex = m_ID++;
		XMStoreFloat4x4(&EnvSphereRitem->World, XMMatrixScaling(sphereOpt.scale_x, sphereOpt.scale_y, sphereOpt.scale_z) * XMMatrixTranslation(sphereOpt.x, sphereOpt.y, sphereOpt.z));
		EnvSphereRitem->TexTransform = orangelie::Utility::Tools::Identity();
		EnvSphereRitem->MeshGeo = m_Geometrics["sphereGeo"].get();
		EnvSphereRitem->IndexCount = EnvSphereRitem->MeshGeo->Submeshes["sphere"].IndexCount;
		EnvSphereRitem->StartIndexLocation = EnvSphereRitem->MeshGeo->Submeshes["sphere"].StartIndexLocation;
		EnvSphereRitem->BaseVertexLocation = EnvSphereRitem->MeshGeo->Submeshes["sphere"].BaseVertexLocation;
		EnvSphereRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		EnvSphereRitem->NumFramesDirty = 3;
		EnvSphereRitem->Material = m_Materials[sphereOpt.color].get();

		m_CreativeRitems.push_back(std::move(EnvSphereRitem));
		m_DUpdateObjs.push_back(sph.get());

		return std::move(sph);
	}

	std::unique_ptr<Box> box(BoxOpt boxOpt) {
		auto bx = std::make_unique<Box>(m_ID);
		bx->position = { boxOpt.x, boxOpt.y, boxOpt.z };
		bx->SetScale(boxOpt.scale_x, boxOpt.scale_y, boxOpt.scale_z);
		bx->SetSize(boxOpt.scale_x, boxOpt.scale_y, boxOpt.scale_z);

		auto BoxRitem = std::make_unique<orangelie::Rendering::RenderItem>();
		BoxRitem->ObjCBIndex = m_ID++;
		XMStoreFloat4x4(&BoxRitem->World, XMMatrixScaling(boxOpt.scale_x, boxOpt.scale_y, boxOpt.scale_z) * XMMatrixTranslation(boxOpt.x, boxOpt.y, boxOpt.z));
		BoxRitem->TexTransform = orangelie::Utility::Tools::Identity();
		BoxRitem->MeshGeo = m_Geometrics["boxGeo"].get();
		BoxRitem->IndexCount = BoxRitem->MeshGeo->Submeshes["box"].IndexCount;
		BoxRitem->StartIndexLocation = BoxRitem->MeshGeo->Submeshes["box"].StartIndexLocation;
		BoxRitem->BaseVertexLocation = BoxRitem->MeshGeo->Submeshes["box"].BaseVertexLocation;
		BoxRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		BoxRitem->NumFramesDirty = 3;
		BoxRitem->Material = m_Materials[boxOpt.color].get();

		m_CreativeRitems.push_back(std::move(BoxRitem));
		m_DUpdateObjs.push_back(bx.get());

		return std::move(bx);
	}

private:
	void LoadTextures() {
		m_TextureLoader.AddTexture("whiteb", L"./Textures/WhiteBlock.dds", m_Device, m_CommandList.Get());
		m_TextureLoader.AddTexture("env", L"./Textures/sunsetcube1024.dds", m_Device, m_CommandList.Get());
	}

	void BuildRootSignature() {
		CD3DX12_DESCRIPTOR_RANGE srvRange;
		srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 1, 0);

		CD3DX12_DESCRIPTOR_RANGE cubemapRange;
		cubemapRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);

		const size_t parameter_size = 5;
		CD3DX12_ROOT_PARAMETER rootParameters[parameter_size];

		rootParameters[0].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL); // SrvHeap
		rootParameters[1].InitAsShaderResourceView(0, 1); // Materials
		rootParameters[2].InitAsConstantBufferView(0); // Object
		rootParameters[3].InitAsConstantBufferView(1); // Pass
		rootParameters[4].InitAsDescriptorTable(1, &cubemapRange, D3D12_SHADER_VISIBILITY_PIXEL); // SrvHeap

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
		SrvHeapDesc.NumDescriptors = 2;
		SrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		SrvHeapDesc.NodeMask = 0;
		SrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		HR(m_Device->CreateDescriptorHeap(&SrvHeapDesc, IID_PPV_ARGS(m_SrvHeap.GetAddressOf())));

		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_SrvHeap->GetCPUDescriptorHandleForHeapStart());

		auto whiteb = m_TextureLoader.GetTexture("whiteb")->GPUHeap;
		auto env = m_TextureLoader.GetTexture("env")->GPUHeap;

		D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
		SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		SrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		SrvDesc.Format = whiteb->GetDesc().Format;
		SrvDesc.Texture2D.MipLevels = whiteb->GetDesc().MipLevels;
		SrvDesc.Texture2D.MostDetailedMip = 0;
		SrvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		m_Device->CreateShaderResourceView(whiteb.Get(), &SrvDesc, hDescriptor);


		hDescriptor.Offset(1, m_CbvSrvUavSize);

		SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		SrvDesc.Format = env->GetDesc().Format;
		SrvDesc.Texture2D.MipLevels = env->GetDesc().MipLevels;

		m_Device->CreateShaderResourceView(env.Get(), &SrvDesc, hDescriptor);
	}

	void BuildShaderSystem() {
		orangelie::Shader::ShaderRasterizeObjects2 RasterizeObj;
		std::vector<D3D_SHADER_MACRO> shaderMacro = { { NULL, NULL } };

		RasterizeObj.VS = { L"./Shader/Default.hlsl", "vs_5_1", "VS", shaderMacro, true };
		RasterizeObj.PS = { L"./Shader/Default.hlsl", "ps_5_1", "PS", shaderMacro, true };
		m_ShaderSys->ShaderCompileFromFile("sha1", RasterizeObj);

		RasterizeObj.VS = { L"./Shader/CubeMap.hlsl", "vs_5_1", "VS", shaderMacro, true };
		RasterizeObj.PS = { L"./Shader/CubeMap.hlsl", "ps_5_1", "PS", shaderMacro, true };
		m_ShaderSys->ShaderCompileFromFile("sha2", RasterizeObj);

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
		auto gridGeo = geoGen.CreateGrid(100.0f, 100.0f, 50, 50);

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
		auto boxGeo = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 4);

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

	void BuildSphereGeometry() {
		orangelie::Mesh::GeometryGenerator geoGen;
		auto sphereGeo = geoGen.CreateSphere(1.0f, 16, 16);

		using orangelie::Mesh::Vertex2;
		std::vector<Vertex2> vertices(sphereGeo.vertices.size());

		for (size_t i = 0; i < sphereGeo.vertices.size(); ++i) {
			vertices[i].Position = sphereGeo.vertices[i].Position;
			vertices[i].Tangent = sphereGeo.vertices[i].Tangent;
			vertices[i].Normal = sphereGeo.vertices[i].Normal;
			vertices[i].TexC = sphereGeo.vertices[i].TexC;
		}

		std::vector<std::uint16_t> indices = sphereGeo.GetIndices16();

		const UINT vertexBufferByteSize = (UINT)vertices.size() * sizeof(Vertex2);
		const UINT indexBufferByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto meshGeo = std::make_unique<orangelie::Mesh::MeshGeometry>();
		meshGeo->Name = "sphereGeo";

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

		meshGeo->Submeshes["sphere"] = Submesh;
		m_Geometrics[meshGeo->Name] = std::move(meshGeo);
	}

	void BuildMaterials() {
		auto white = std::make_unique<orangelie::Lighting::Material>();
		white->MatTransform = orangelie::Utility::Tools::Identity();
		white->MatIndex = 0;
		white->SrvHeapIndex = 0;
		white->DiffuseAlbedo = { 0.0f, 0.0f, 0.1f, 1.0f };
		white->R0 = { 0.98f, 0.97f, 0.95f };
		white->Roughness = 0.1f;
		white->SelectedColor = 0;

		auto env = std::make_unique<orangelie::Lighting::Material>();
		env->MatTransform = orangelie::Utility::Tools::Identity();
		env->MatIndex = 1;
		env->SrvHeapIndex = 1;
		env->SelectedColor = 0;

		m_Materials["white"] = std::move(white);
		m_Materials["env"] = std::move(env);

		XMFLOAT4 colors[] = {
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		};

		std::string names[] = {
			"red",
			"green",
			"blue",
			"yellow",
			"magenta",
			"cyan",
			"white",
			"black",
		};

		UINT MatIndex = 2;
		for (int i = 0; i < _countof(colors); ++i) {
			auto color = std::make_unique<orangelie::Lighting::Material>();
			color->MatIndex = MatIndex++;
			color->SelectedColor = 1;
			color->Color = colors[i];

			m_Materials[names[i]] = std::move(color);
		}
	}

	void BuildRenderItems() {
		/*
		* 
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

		m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque].push_back(GridRitem.get());
		m_AllRitems.push_back(std::move(GridRitem));

		*/


		auto EnvSphereRitem = std::make_unique<orangelie::Rendering::RenderItem>();
		EnvSphereRitem->ObjCBIndex = 0;
		XMStoreFloat4x4(&EnvSphereRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
		EnvSphereRitem->TexTransform = orangelie::Utility::Tools::Identity();
		EnvSphereRitem->MeshGeo = m_Geometrics["sphereGeo"].get();
		EnvSphereRitem->IndexCount = EnvSphereRitem->MeshGeo->Submeshes["sphere"].IndexCount;
		EnvSphereRitem->StartIndexLocation = EnvSphereRitem->MeshGeo->Submeshes["sphere"].StartIndexLocation;
		EnvSphereRitem->BaseVertexLocation = EnvSphereRitem->MeshGeo->Submeshes["sphere"].BaseVertexLocation;
		EnvSphereRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		EnvSphereRitem->NumFramesDirty = 3;
		EnvSphereRitem->Material = m_Materials["env"].get();

		m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Sky].push_back(EnvSphereRitem.get());
		m_AllRitems.push_back(std::move(EnvSphereRitem));


		for (size_t i = 0; i < m_CreativeRitems.size(); ++i) {
			m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque].push_back(m_CreativeRitems[i].get());
			m_AllRitems.push_back(std::move(m_CreativeRitems[i]));
		}
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

		GraphicsPSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		GraphicsPSODesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		GraphicsPSODesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		auto shaderBinSky = m_ShaderSys->Resource("sha2");

		GraphicsPSODesc.VS = {
			reinterpret_cast<BYTE*>(shaderBinSky.VSbinCode->GetBufferPointer()),
			shaderBinSky.VSbinCode->GetBufferSize()
		};
		GraphicsPSODesc.PS = {
			reinterpret_cast<BYTE*>(shaderBinSky.PSbinCode->GetBufferPointer()),
			shaderBinSky.PSbinCode->GetBufferSize()
		};
		HR(m_Device->CreateGraphicsPipelineState(&GraphicsPSODesc, IID_PPV_ARGS(m_PSOs["sky"].GetAddressOf())));
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
				MatConsts.SelectedColor = r->SelectedColor;
				MatConsts.Color = r->Color;

				mat->CopyData(r->MatIndex, MatConsts);

				--(r->NumFramesDirty);
			}
		}
	}

	void OnKeyboardInput(float dt, float speed) {

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
	virtual void startup() = 0;
	virtual void render(float dt) = 0;

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
		BuildSphereGeometry();
		BuildMaterials();

		startup();

		BuildRenderItems();
		BuildFrameResources();
		BuildPSOs();


		SubmitCommandList();
		FlushCommandQueue();
	}

	virtual void update(float dt) override {
		OnKeyboardInput(dt, m_CameraSpeed);

		render(dt);

		for (size_t i = 0; i < m_DUpdateObjs.size(); ++i) {
			auto pos = m_DUpdateObjs[i]->position;
			auto scale = m_DUpdateObjs[i]->GetScale();

			auto r = m_AllRitems[(size_t)m_DUpdateObjs[i]->GetID()].get();
			XMStoreFloat4x4(&r->World, XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(pos.x, pos.y, pos.z));
			r->NumFramesDirty = 3;
		}

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

		CD3DX12_GPU_DESCRIPTOR_HANDLE envHandle(m_SrvHeap->GetGPUDescriptorHandleForHeapStart());
		envHandle.Offset(1, m_CbvSrvUavSize);
		m_CommandList->SetGraphicsRootDescriptorTable(4, envHandle);

		DrawRenderItems(m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Opaque]);

		m_CommandList->SetPipelineState(m_PSOs["sky"].Get());
		DrawRenderItems(m_RitemsLayer[(int)orangelie::Rendering::RenderLayer::Sky]);

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
	bool m_IsWireframeMode = false;

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

	// Dynamic Update Objects
	std::vector<std::unique_ptr<orangelie::Rendering::RenderItem>> m_CreativeRitems;
	std::vector<GeometryOpt*> m_DUpdateObjs;
	UINT m_ID = 1;
	float m_CameraSpeed = 5.0f;

};