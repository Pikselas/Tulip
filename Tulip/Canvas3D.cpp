#include"Canvas3D.h"

void Canvas3D::UpdateCbuff(ID3D11Buffer* CBuffer,DirectX::XMMATRIX transform_matrix) const
{	
	D3D11_MAPPED_SUBRESOURCE ms;
	ImmediateContext->Map(CBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	std::memcpy(ms.pData, &transform_matrix, sizeof(transform_matrix));
	ImmediateContext->Unmap(CBuffer, 0u);
}

Canvas3D::Canvas3D(Window& wnd) : Halfheight(wnd.GetHeight() / 2), Halfwidth(wnd.GetWidth() / 2)
{
	PtrManager<ID3D11Resource> pBackBuffer;
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;												// look at the window and use it's size
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;								// pick the default refresh rates
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferCount = 1;														// one back buffer -> one back and one front (double buffering)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;						// this is the color format (BGRA) 
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// not specifying any scaling because we want the renedred frame same as window size
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// how buffer scaning will be done for copying all to video memory
	sd.Flags = 0;															// not setting any flags
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// use the buffer for render target
	sd.OutputWindow = wnd.window_handle;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								// discard the effects for swapping frames
	sd.Windowed = TRUE;
	
	// for antialiasing we don't want it right now
	
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	if (auto hrcode = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, nullptr, &ImmediateContext); FAILED(hrcode))
	{
		throw hrcode;
	}

	SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &RenderTarget);

	//setting depth stencil
	
	//setting depth stencil state
	
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	
	PtrManager<ID3D11DepthStencilState> ds;
	Device->CreateDepthStencilState(&dsd, &ds);
	
	ImmediateContext->OMSetDepthStencilState(ds.Get(), 1u);

	//Depth stencil needs a buffer to write to
	
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = wnd.GetWidth();
	td.Height = wnd.GetHeight();
	td.MipLevels = 1u;
	td.ArraySize = 1u;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.SampleDesc.Count = 1u;
	td.SampleDesc.Quality = 0u;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	
	PtrManager<ID3D11Texture2D> tex;
	Device->CreateTexture2D(&td, nullptr, &tex);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0u;

	Device->CreateDepthStencilView(tex.Get(), &dsvd, &DepthStencilView);

	//Bind the render target view and depth stencil buffer to the output render pipeline
	
	ImmediateContext->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthStencilView.Get());

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = wnd.GetWidth();		//screen height
	vp.Height = wnd.GetHeight();	// screen width
	vp.MaxDepth = 1;				// maximum depth for z axis
	vp.MinDepth = 0;				// minimum depth for z axis
	ImmediateContext->RSSetViewports(1u, &vp);

	//draws the vertices as a list of TRIANGLE
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Canvas3D::ClearCanvas() const
{
	float color[] = { 0.0f , 0.0f , 0.0f , 0.0f };
	ImmediateContext->ClearRenderTargetView(RenderTarget.Get(), color);
	ImmediateContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Canvas3D::PresentOnWindow() const
{
	/*const auto matrix = DirectX::XMMatrixTranspose(
	   ObjectTransform  * camera.GetTransformMatrix() *
		DirectX::XMMatrixPerspectiveLH(2.0f, Halfwidth / Halfheight, 1.0f, 40.0f)
		);
	UpdateCbuff(matrix);
	DrawFunc();*/
	SwapChain->Present(1u, 0u);
}

void Canvas3D::SetPrimitiveTopology(const PrimitiveTopology primitive) const
{
	ImmediateContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitive));
}

void Canvas3D::DrawObject(std::span<const VertexType> Vertices)
{
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VertexType) * Vertices.size();						// total array size
	bd.Usage = D3D11_USAGE_DEFAULT;												// how buffer data will be used (read/write protections for GPU/CPU)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// What type of buffer would it be
	bd.CPUAccessFlags = 0u;														// we don't want any cpu access for now so setting it to 0 for now
	bd.MiscFlags = 0u;															// misscellinious flag for buffer configuration (we don't want it now either)
	bd.StructureByteStride = sizeof(VertexType);								// Size of every vertex in the array 

	//holds the data pointer that will be used in vertex buffer
	
	D3D11_SUBRESOURCE_DATA subd = { 0 };
	subd.pSysMem = Vertices.data();											// pointer to array so that it can copy all the array data to the buffer

	Microsoft::WRL::ComPtr<ID3D11Buffer> VBuffer;
	Device->CreateBuffer(&bd, &subd, &VBuffer);
	UINT stride = sizeof(VertexType);										    // size of every vertex
	UINT offset = 0u;														    // displacement after which the actual data start (so 0 because no displacement is there)
	
	//statrting slot(from 0) , number of buffers(1 buffer) , pp , 
	
	ImmediateContext->IASetVertexBuffers(0u, 1u, VBuffer.GetAddressOf(), &stride, &offset);
	const auto DrawSize = Vertices.size();
	DrawFunc = [this,DrawSize](){ ImmediateContext->Draw(DrawSize, 0u) ; };
}

void Canvas3D::DrawObject(std::span<const VertexType> Vertices, std::span<const unsigned int> indices)
{
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VertexType) * Vertices.size();						// total array size
	bd.Usage = D3D11_USAGE_DEFAULT;												// how buffer data will be used (read/write protections for GPU/CPU)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// What type of buffer would it be
	bd.CPUAccessFlags = 0u;														// we don't want any cpu access for now so setting it to 0 for now
	bd.MiscFlags = 0u;															// misscellinious flag for buffer configuration (we don't want it now either)
	bd.StructureByteStride = sizeof(VertexType);								// Size of every vertex in the array 

	//holds the data pointer that will be used in vertex buffer

	D3D11_SUBRESOURCE_DATA subd = { 0 };
	subd.pSysMem = Vertices.data();											// pointer to array so that it can copy all the array data to the buffer

	Microsoft::WRL::ComPtr<ID3D11Buffer> VBuffer;
	Device->CreateBuffer(&bd, &subd, &VBuffer);
	UINT stride = sizeof(VertexType);										    // size of every vertex
	UINT offset = 0u;														    // displacement after which the actual data start (so 0 because no displacement is there)

	//statrting slot(from 0) , number of buffers(1 buffer) , pp , 

	ImmediateContext->IASetVertexBuffers(0u, 1u, VBuffer.GetAddressOf(), &stride, &offset);

	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(size_t) * indices.size();
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(size_t);

	D3D11_SUBRESOURCE_DATA isubd = { 0 };
	isubd.pSysMem = indices.data();

	Microsoft::WRL::ComPtr<ID3D11Buffer> IBuffer;
	Device->CreateBuffer(&ibd, &isubd, &IBuffer);
	
	ImmediateContext->IASetIndexBuffer(IBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	const auto IndexSize = indices.size();
	DrawFunc = [this,IndexSize]() { ImmediateContext->DrawIndexed(IndexSize, 0u, 0u); };
}

void Canvas3D::DrawObject(const Object& obj)
{
	UINT offset = 0u;														    // displacement after which the actual data start (so 0 because no displacement is there)
	ImmediateContext->IASetVertexBuffers(0u, 1u, obj.GetVBuff().GetAddressOf(), &obj.stride, &offset);
	ImmediateContext->IASetIndexBuffer(obj.GetIBuff().Get(), DXGI_FORMAT_R32_UINT, 0u);
	ImmediateContext->VSSetShader(obj.GetVShader().Get(), nullptr, 0u);
	ImmediateContext->IASetInputLayout(obj.GetILayout().Get());
	ImmediateContext->PSSetShader(obj.GetPShader().Get(), nullptr, 0u);
	const auto IndexSize = obj.m_IndexCount;
	DrawFunc = [this, IndexSize]() { ImmediateContext->DrawIndexed(IndexSize, 0u, 0u); };
	ObjectTransform = obj.GetTansformMatrix();
	const auto matrix = DirectX::XMMatrixTranspose(
		ObjectTransform * camera.GetTransformMatrix() *
		DirectX::XMMatrixPerspectiveLH(2.0f, Halfwidth / Halfheight, 1.0f, 40.0f)
	);
	if (auto cbuffer = obj.GetCBuffer())
	{
		ImmediateContext->VSSetConstantBuffers(0u, 1u, cbuffer->GetAddressOf());
		UpdateCbuff(cbuffer->Get(), matrix);
	}
	if (auto texture = obj.GetTexture())
	{
		ImmediateContext->PSSetSamplers(0u, 1u, texture->first.GetAddressOf());
		ImmediateContext->PSSetShaderResources(0u, 1u, texture->second.GetAddressOf());
	}
	DrawFunc();
}

std::pair<float, float> Canvas3D::GetNormalizedWindowPos(int x, int y) const
{
	return { (x / Halfwidth) - 1.0f,  -((y / Halfheight) - 1.0f) };
}

void Canvas3D::Camera::Transform()
{
	const auto Pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(pos_x,pos_y, pos_z, 1.0f),
		DirectX::XMMatrixRotationRollPitchYaw(rot_x, rot_y, 0.0f));
	transform_matrix = DirectX::XMMatrixLookAtLH(Pos, DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
}

Canvas3D::Camera::Camera()
{
	Transform();
}

void Canvas3D::Camera::RotateOrientation(const int x, const int y)
{
	rot_x = DirectX::XM_PI * x / 180.0f;
	rot_y = DirectX::XM_PI * y / 180.0f;
	Transform();
}

void Canvas3D::Camera::RotatePosition(const int x, const int y, const int z)
{
	roll = DirectX::XM_PI * x / 180.0f;
	pitch = DirectX::XM_PI * y / 180.0f;
	yaw = DirectX::XM_PI * z / 180.0f;
	Transform();
}

void Canvas3D::Camera::SetPosition(const float x, const float y, const float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	Transform();
}

DirectX::XMMATRIX Canvas3D::Camera::GetTransformMatrix() const
{
	return transform_matrix;
}
