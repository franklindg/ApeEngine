#include <ApePCH.h>
#include "Model.h"

Model::Model(ID3D11Device* device, const char* filename)
	: m_pDevice(device)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);

	if (!InitializeBuffers(filename))
	{
		ReportError("Failed to initialize the model buffers.");
	}
}

Model::~Model()
{
	m_pDevice.Reset();
	m_pDeviceContext.Reset();

	mModel.VertexBuffer->Release();
	mModel.IndexBuffer->Release();
}

void Model::Render()
{
	RenderBuffers();
}

UINT Model::GetIndexCount()
{
	return mModel.NumIndices;
}

bool Model::InitializeBuffers(const char* filename)
{
	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(filename,
		aiProcess_CalcTangentSpace		|
		aiProcess_Triangulate			|
		aiProcess_GenSmoothNormals		|
		aiProcess_SplitLargeMeshes		|
		aiProcess_ConvertToLeftHanded	|
		aiProcess_SortByPType			|
		aiProcess_PreTransformVertices);

	
	UINT NumMeshes = pScene->mNumMeshes;
	vector<VertexType> vertices;
	vector<DWORD> indices;

	for (UINT i = 0; i < NumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[i];

		for (UINT j = 0; j < mesh->mNumVertices; j++)
		{
			VertexType v;

			v.position = XMFLOAT4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f);
			v.normal = XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
			if (mesh->HasTangentsAndBitangents())
			{
				v.tangent = XMFLOAT3(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				v.bitangent = XMFLOAT3(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z);
			}
			if (mesh->HasTextureCoords(0))
			{
				v.texCoord = XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
			}

			vertices.push_back(v);
		}

		for (UINT k = 0; k < mesh->mNumFaces; k++)
			for (UINT l = 0; l < mesh->mFaces[k].mNumIndices; l++)
				indices.push_back(mesh->mFaces[k].mIndices[l]);
	}

	mModel.NumVertices = vertices.size();
	mModel.NumIndices = indices.size();

	// Sets the Vertex buffer description, and Subresource data.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexType) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = vertices.data();
	vInitData.SysMemPitch = 0;
	vInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbd, &vInitData, &mModel.VertexBuffer)))
	{
		return false;
	}

	// Sets the Index buffer description and Subresource data.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indices.data();
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibd, &iInitData, &mModel.IndexBuffer)))
	{
		return false;
	}

	// Empties vertex and index vectors.
	vertices.clear();
	indices.clear();

	return true;
}

void Model::RenderBuffers()
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// Sets the vertex and index buffers.
	m_pDeviceContext->IASetVertexBuffers(0, 1, &mModel.VertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(mModel.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Sets the type of primitive to be rendered from the vertex buffer.
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}