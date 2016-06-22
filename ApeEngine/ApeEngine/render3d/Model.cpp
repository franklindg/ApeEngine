/////////////////////////////////////////////
// Filename: Model.cpp
/////////////////////////////////////////////
#include "Model.h"


Model::Model(ID3D11Device* device, const char* filename)
{
	if (!InitializeBuffers(device, filename))
	{
		ReportError("Failed to initialize the model buffers.");
	}
}

Model::Model(const Model& other)
{}

Model::~Model()
{
	mModel.VertexBuffer->Release();
	mModel.IndexBuffer->Release();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

UINT Model::GetIndexCount()
{
	return mModel.NumFaces * 3;
}

bool Model::InitializeBuffers(ID3D11Device* device, const char* filename)
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
	mModel.NumFaces = indices.size() / 3;

	// Sets the Vertex buffer description, and Subresource data.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexType) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &vertices[0];
	vInitData.SysMemPitch = 0;
	vInitData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vbd, &vInitData, &mModel.VertexBuffer)))
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
	iInitData.pSysMem = &indices[0];
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&ibd, &iInitData, &mModel.IndexBuffer)))
	{
		return false;
	}

	// Empties vertex and index vectors.
	vertices.clear();
	indices.clear();

	return true;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{

	UINT stride = sizeof(VertexType);
	UINT offset = 0;


	deviceContext->IASetVertexBuffers(0, 1, &mModel.VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mModel.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Model::ImportMaterials(const aiMaterial& material)
{
	aiString materialName;
	aiString aiTexturePath;
	aiColor4D diffuseColor;
	aiColor4D specularColor;
	//aiColor4D ambientColor;
	//aiColor4D emissiveColor;
	//float opacity;
	//float indexOfRefraction;
	//float reflectivity;
	float shininess;
	//float bumpIntensity;
	//aiTextureOp aiBlendOperation;
	//float blendFactor;

	std::shared_ptr<Materials> pMaterial = std::make_shared<Materials>();

	if (material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == aiReturn_SUCCESS)
	{
		pMaterial->SetAmbient(XMFLOAT4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));
	}
	if (material.Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == aiReturn_SUCCESS)
	{
		pMaterial->SetSpecular(XMFLOAT4(specularColor.r, specularColor.g, specularColor.b, specularColor.a));
	}
	//if (material.Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetAmbient(XMFLOAT4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a));
	//}
	//if (material.Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetEmissive(XMFLOAT4(emissiveColor.r, emissiveColor.g, emissiveColor.b, emissiveColor.a));
	//}
	//if (material.Get(AI_MATKEY_OPACITY, opacity) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetOpacity(opacity);
	//}
	//if (material.Get(AI_MATKEY_REFRACTI, indexOfRefraction) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetIndexOfRefraction(indexOfRefraction);
	//}
	//if (material.Get(AI_MATKEY_REFLECTIVITY, reflectivity) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetReflectance(XMFLOAT4(reflectivity, reflectivity, reflectivity, 1.0f));
	//}
	if (material.Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
	{
		pMaterial->SetSpecularPower(shininess);
	}
	//if (material.Get(AI_MATKEY_BUMPSCALING, bumpIntensity) == aiReturn_SUCCESS)
	//{
	//	pMaterial->SetBumpIntensity(bumpIntensity);
	//}
	//
	//if (material.GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
	//	material.GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath, nullptr, nullptr, &blendFactor, &aiBlendOperation) == aiReturn_SUCCESS)
	//{
	//}


}
