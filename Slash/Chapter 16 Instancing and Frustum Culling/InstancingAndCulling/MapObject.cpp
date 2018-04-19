#include "stdafx.h"
#include "MapObject.h"
#include "Define.h"
#include "Management.h"
#include "Component_Manager.h"
#include "StaticMesh.h"

CMapObject::CMapObject(Microsoft::WRL::ComPtr<ID3D12Device> d3dDevice, ComPtr<ID3D12DescriptorHeap>& srv, UINT srvSize, wchar_t* meshName)
	: CGameObject(d3dDevice, srv, srvSize)
	, m_wstrMeshName(meshName)
{

}

CMapObject::~CMapObject()
{
}

HRESULT CMapObject::Initialize()
{
	m_pMesh = dynamic_cast<StaticMesh*>(CComponent_Manager::GetInstance()->Clone_Component(m_wstrMeshName));
	Mat = new Material;
	Mat->Name = "BarrelMat";
	Mat->MatCBIndex = 1;
	Mat->DiffuseSrvHeapIndex = 1;
	Mat->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Mat->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	Mat->Roughness = 0.3f;

	XMStoreFloat4x4(&World, XMMatrixScaling(0.1f, 0.1f, 0.1f));
	TexTransform = MathHelper::Identity4x4();
	ObjCBIndex = 1;

	Geo = dynamic_cast<StaticMesh*>(m_pMesh)->m_Geometry[0].get();
	PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	IndexCount = Geo->DrawArgs["Barrel"].IndexCount;
	StartIndexLocation = Geo->DrawArgs["Barrel"].StartIndexLocation;
	BaseVertexLocation = Geo->DrawArgs["Barrel"].BaseVertexLocation;
	Bounds = Geo->DrawArgs["Barrel"].Bounds;


	SetOOBB(XMFLOAT3(Bounds.Center.x * 0.1f, Bounds.Center.y * 0.1f, Bounds.Center.z * 0.1f), XMFLOAT3(Bounds.Extents.x * 0.1f, Bounds.Extents.y * 0.1f, Bounds.Extents.z * 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

bool CMapObject::Update(const GameTimer & gt)
{
	CGameObject::Update(gt);
	m_pMesh->Update(gt);
	m_pCamera = CManagement::GetInstance()->Get_MainCam();
	XMMATRIX view = m_pCamera->GetView();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	auto currObjectCB = m_pFrameResource->ObjectCB.get();


	//auto currObjectCB2 = m_pFrameResource->InstanceBuffer.get();

	XMMATRIX world = XMLoadFloat4x4(&World);
	XMMATRIX texTransform = XMLoadFloat4x4(&TexTransform);

	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

	// View space to the object's local space.
	XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

	// Transform the camera frustum from view space to the object's local space.
	BoundingFrustum localSpaceFrustum;
	mCamFrustum = *CManagement::GetInstance()->Get_CurScene()->Get_CamFrustum();
	mCamFrustum.Transform(localSpaceFrustum, viewToLocal);

	// Perform the box/frustum intersection test in local space.
	if ((localSpaceFrustum.Contains(Bounds) != DirectX::DISJOINT) || (mFrustumCullingEnabled == false))
	{
		//cout << "���δ�!" << endl;
		m_bIsVisiable = true;
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
		XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
		objConstants.MaterialIndex = Mat->MatCBIndex;

		currObjectCB->CopyData(ObjCBIndex, objConstants);
	}
	else
	{
		//cout << "�Ⱥ��δ�!" << endl;
		m_bIsVisiable = false;
	}



	//////////////////////////////////////////////////


	// Next FrameResource need to be updated too.
	//NumFramesDirty--;
	auto currMaterialCB = m_pFrameResource->MaterialCB.get();

	XMMATRIX matTransform = XMLoadFloat4x4(&Mat->MatTransform);

	MaterialConstants matConstants;
	matConstants.DiffuseAlbedo = Mat->DiffuseAlbedo;
	matConstants.FresnelR0 = Mat->FresnelR0;
	matConstants.Roughness = Mat->Roughness;
	XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));

	matConstants.DiffuseMapIndex = Mat->DiffuseSrvHeapIndex;

	currMaterialCB->CopyData(Mat->MatCBIndex, matConstants);
	return true;
}

void CMapObject::Render(ID3D12GraphicsCommandList * cmdList)
{
}

CMapObject * CMapObject::Create(Microsoft::WRL::ComPtr<ID3D12Device> d3dDevice, ComPtr<ID3D12DescriptorHeap>& srv, UINT srvSize, wchar_t* meshName)
{
	CMapObject* pInstance = new CMapObject(d3dDevice, srv, srvSize, meshName);
	
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(L"CMapObject Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObject::Free()
{
}