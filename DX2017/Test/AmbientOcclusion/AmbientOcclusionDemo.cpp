//***************************************************************************************
// AmbientOcclusionApp.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates static precomputed vertex ambient occlusion.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "Octree.h"

class AmbientOcclusionApp : public D3DApp 
{
public:
	AmbientOcclusionApp(HINSTANCE hInstance);
	~AmbientOcclusionApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildVertexAmbientOcclusion(
		std::vector<Vertex::AmbientOcclusion>& vertices,
	    const std::vector<UINT>& indices);

	void BuildSkullGeometryBuffers();

private:

	ID3D11Buffer* mSkullVB;
	ID3D11Buffer* mSkullIB;

	DirectionalLight mDirLights[3];
	UINT mLightCount;
	Material mModelMat;

	XMFLOAT4X4 mSkullWorld;
	//XMFLOAT4X4 mView;
	//XMFLOAT4X4 mProj;
	XMFLOAT4X4 mTexTransform;

	UINT mSkullIndexCount;

	Camera mCam;
	XMFLOAT3 mEyePosW;

	POINT mLastMousePos;

	ID3D11ShaderResourceView *mDiffuseMapSRV;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	AmbientOcclusionApp theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}
 

AmbientOcclusionApp::AmbientOcclusionApp(HINSTANCE hInstance)
: D3DApp(hInstance), mSkullVB(0), mSkullIB(0), mSkullIndexCount(0),
mDiffuseMapSRV(nullptr)
{
	mMainWndCaption = L"Ambient Occlusion";
	
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//mCam.SetPosition(0.0f, 5.0f, -5.0f);
	mEyePosW = XMFLOAT3(-4.0f, 4.0f, -4.0f);
	mCam.LookAt(
		mEyePosW,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f));
	XMMATRIX I = XMMatrixIdentity();
	XMMATRIX skullScale = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//XMMATRIX skullScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mSkullWorld, XMMatrixMultiply(skullScale, skullOffset));
	XMStoreFloat4x4(&mTexTransform, I);
	//XMStoreFloat4x4(&mView, I);
	//XMStoreFloat4x4(&mProj, I);
	mDirLights[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirLights[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(1.4f, 1.4f, 1.4f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.707f, 0.0f, 0.707f);

	mDirLights[2].Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 3.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

}

AmbientOcclusionApp::~AmbientOcclusionApp()
{
	ReleaseCOM(mSkullVB);
	ReleaseCOM(mSkullIB);

	Effects::DestroyAll();
	InputLayouts::DestroyAll(); 
}

bool AmbientOcclusionApp::Init()
{
	if(!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);

	BuildSkullGeometryBuffers();

	return true;
}

void AmbientOcclusionApp::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void AmbientOcclusionApp::UpdateScene(float dt)
{
	//
	// Control the camera.
	//
	if( GetAsyncKeyState('W') & 0x8000 )
		mCam.Walk(10.0f*dt);

	if( GetAsyncKeyState('S') & 0x8000 )
		mCam.Walk(-10.0f*dt);

	if( GetAsyncKeyState('A') & 0x8000 )
		mCam.Strafe(-10.0f*dt);

	if( GetAsyncKeyState('D') & 0x8000 )
		mCam.Strafe(10.0f*dt);

	mEyePosW = mCam.GetPosition();

	if (GetAsyncKeyState('0') & 0x8000)
		mLightCount = 0;

	if (GetAsyncKeyState('1') & 0x8000)
		mLightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		mLightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		mLightCount = 3;
}

void AmbientOcclusionApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::AmbientOcclusion);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
	UINT stride = sizeof(Vertex::AmbientOcclusion);
    UINT offset = 0;

	mCam.UpdateViewMatrix();
 
	XMMATRIX view     = mCam.View();
	XMMATRIX proj     = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	Effects::AmbientOcclusionFX->SetDirLights(mDirLights);
	Effects::AmbientOcclusionFX->SetEyePosW(mEyePosW);
 
	ID3DX11EffectTechnique* activeTech = Effects::AmbientOcclusionFX->AmbientOcclusionTech;
	switch (mLightCount)
	{
	case 0:
		activeTech = Effects::AmbientOcclusionFX->AmbientOcclusionTech;
		break;
	case 1:
		activeTech = Effects::Effects::AmbientOcclusionFX->Light1TexTech;
		break;
	case 2:
		activeTech = Effects::Effects::AmbientOcclusionFX->Light2TexTech;
		break;
	case 3:
		activeTech = Effects::Effects::AmbientOcclusionFX->Light3TexTech;
		break;
	}

    D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		// Draw the skull.

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX world = XMLoadFloat4x4(&mSkullWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::AmbientOcclusionFX->SetWorld(world);
		Effects::AmbientOcclusionFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::AmbientOcclusionFX->SetWorldViewProj(worldViewProj);
		Effects::AmbientOcclusionFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		Effects::AmbientOcclusionFX->SetMaterial(mModelMat);
		Effects::AmbientOcclusionFX->SetDiffuseMap(mDiffuseMapSRV);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void AmbientOcclusionApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void AmbientOcclusionApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void AmbientOcclusionApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void AmbientOcclusionApp::BuildVertexAmbientOcclusion(
	std::vector<Vertex::AmbientOcclusion>& vertices,
	const std::vector<UINT>& indices)
{
	UINT vcount = vertices.size();
	UINT tcount = indices.size()/3;

	std::vector<XMFLOAT3> positions(vcount);
	for(UINT i = 0; i < vcount; ++i)
		positions[i] = vertices[i].Pos;

	Octree octree;
	octree.Build(positions, indices);

	// For each vertex, count how many triangles contain the vertex.
	std::vector<int> vertexSharedCount(vcount);
	for(UINT i = 0; i < tcount; ++i)
	{
		UINT i0 = indices[i*3+0];
		UINT i1 = indices[i*3+1];
		UINT i2 = indices[i*3+2];

		XMVECTOR v0 = XMLoadFloat3(&vertices[i0].Pos);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i1].Pos);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i2].Pos);

		XMVECTOR edge0 = v1 - v0;
		XMVECTOR edge1 = v2 - v0;

		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge0, edge1));

		XMVECTOR centroid = (v0 + v1 + v2)/3.0f;

		// Offset to avoid self intersection.
		centroid += 0.001f*normal;

		const int NumSampleRays = 32;
		float numUnoccluded = 0;
		for(int j = 0; j < NumSampleRays; ++j)
		{
			XMVECTOR randomDir = MathHelper::RandHemisphereUnitVec3(normal);

			// TODO: Technically we should not count intersections that are far 
			// away as occluding the triangle, but this is OK for demo.
			if( !octree.RayOctreeIntersect(centroid, randomDir) )
			{
				numUnoccluded++;
			}
		}
		
		float ambientAccess = numUnoccluded / NumSampleRays;

		// Average with vertices that share this face.
		vertices[i0].AmbientAccess += ambientAccess;
		vertices[i1].AmbientAccess += ambientAccess;
		vertices[i2].AmbientAccess += ambientAccess;

		vertexSharedCount[i0]++;
		vertexSharedCount[i1]++;
		vertexSharedCount[i2]++;
	}

	// Finish average by dividing by the number of samples we added.
	for(UINT i = 0; i < vcount; ++i)
	{
		vertices[i].AmbientAccess /= vertexSharedCount[i];
	}
}

void AmbientOcclusionApp::BuildSkullGeometryBuffers()
{
	std::wifstream fin("models/house.txt");
	if(!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::wstring texmap;
	std::wstring ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> mModelMat.Ambient.x >> mModelMat.Ambient.y >> mModelMat.Ambient.z >> mModelMat.Ambient.w;
	fin >> ignore >> mModelMat.Diffuse.x >> mModelMat.Diffuse.y >> mModelMat.Diffuse.z >> mModelMat.Diffuse.w;
	fin >> ignore >> mModelMat.Specular.x >> mModelMat.Specular.y >> mModelMat.Specular.z >> mModelMat.Specular.w;
	fin >> ignore >> texmap;  //texture path
	fin >> ignore >> ignore >> ignore >> ignore >> ignore;
	
	std::vector<Vertex::AmbientOcclusion> vertices(vcount);
	for(UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
		fin >> vertices[i].Tex.x >> vertices[i].Tex.y;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	mSkullIndexCount = 3*tcount;
	std::vector<UINT> indices(mSkullIndexCount);
	for(UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i*3+0] >> indices[i*3+1] >> indices[i*3+2];
	}

	fin.close();


	BuildVertexAmbientOcclusion(vertices, indices);


    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::AmbientOcclusion) * vcount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));

	//
	if (texmap.size() > 4) {
		texmap = L"Models/" + texmap;
		if (texmap.substr(texmap.size() - 3, 3) == L"dds") {
			HR(CreateDDSTextureFromFile(md3dDevice, texmap.c_str(), nullptr,
				&mDiffuseMapSRV));
		}
		else {
			HR(CreateWICTextureFromFile(md3dDevice, texmap.c_str(), nullptr, &mDiffuseMapSRV));
		}
	}
}

