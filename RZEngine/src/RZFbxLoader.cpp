#include "RZEnginePCH.h"

namespace rczEngine
{
	/* Tab character ("\t") counter */
	int numTabs = 0;

	/**
	* Print the required number of tabs.
	*/
	void PrintTabs() {
		for (int i = 0; i < numTabs; i++)
			printf("\t");
	}

	/**
	* Return a string-based representation based on the attribute type.
	*/
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}

	/**
	* Print an attribute.
	*/
	void PrintAttribute(FbxNodeAttribute* pAttribute) {
		if (!pAttribute) return;

		FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());

		if (typeName == "mesh")
		{

		}

		FbxString attrName = pAttribute->GetName();
	}

	StrPtr<Model> FbxLoader::LoadModel(const String& filePath)
	{
		m_Res = ResVault::Pointer();

		StrPtr<Model> model = std::make_shared<Model>();

		model->SetFilePath(filePath);
		model->GetPath().GetFileName();

		m_Res->InsertResource(model);

		// Change the following filename to a suitable filename value.
		const String lFilename = filePath;

		// Initialize the SDK manager. This object handles memory management.
		m_Manager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(m_Manager, IOSROOT);
		m_Manager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(m_Manager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(lFilename.c_str(), -1, m_Manager->GetIOSettings())) {
			Logger::Pointer()->Log("Call to FbxImporter::Initialize() failed.\n");
			Logger::Pointer()->Log("Error returned: " + String(lImporter->GetStatus().GetErrorString()));
			exit(-1);
		}

		// Create a new scene so that it can be populated by the imported file.
		FbxScene* lScene = FbxScene::Create(m_Manager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		FbxGeometryConverter converter(m_Manager);
		converter.Triangulate(lScene, true, false);

		// The file is imported, so get rid of the importer.
		lImporter->Destroy();

		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.
		FbxNode* lRootNode = lScene->GetRootNode();

		auto node = SceneManager::Pointer()->GetActiveScene()->
			CreateActor("ModelFbx", NULL);

		if (lRootNode) {
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				AddNodeMesh(lRootNode->GetChild(i), "ModelFbx");
		}
		// Destroy the SDK manager and all the other objects it was handling.
		m_Manager->Destroy();

		return model;
	}

	ResourceHandle FbxLoader::LoadMesh(fbxsdk::FbxNode * pNode, bool bones)
	{
		m_Res = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		StrPtr<Model> model = std::make_shared<Model>();

		model->SetFilePath(String(pNode->GetNameOnly()));
		m_Res->InsertResource(model);

		///Clear the index and vertex buffer's lists
		model->m_IndexBuffer.ClearIndexList();
		model->m_VertexBuffer.ClearVertexList();
		model->m_VectorMeshes.clear();

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		//uint32 VertexOffset = 0;
		//uint32 IndexOffset = 0;
		//uint32 IndexCount = 0;

		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		{
			fbxsdk::FbxNodeAttribute* attribute = (pNode->GetNodeAttributeByIndex(i));

			FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());

			if (typeName == "mesh")
			{
				fbxsdk::FbxMesh* mesh = (fbxsdk::FbxMesh*)attribute;

				mesh->SplitPoints();
				auto vertices = mesh->GetControlPoints();

				fbxsdk::FbxGeometryConverter* conv = new fbxsdk::FbxGeometryConverter(m_Manager);
				conv->ComputeEdgeSmoothingFromNormals(mesh);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* normals;
				mesh->GetNormals(&normals);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* binormals;
				mesh->GetBinormals(&binormals);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* tangents;
				mesh->GetTangents(&tangents);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector2>* texCoords;
				mesh->GetTextureUV(&texCoords);

				Gfx::Vertex CurrentVertex;
				for (int k = 0; k < mesh->GetControlPointsCount(); ++k)
				{
					CurrentVertex.VertexPosition.m_x = float(vertices[k].mData[0]);
					CurrentVertex.VertexPosition.m_y = float(vertices[k].mData[1]);
					CurrentVertex.VertexPosition.m_z = float(vertices[k].mData[2]);

					CurrentVertex.TextureCoordinates.m_x = float(texCoords[0][k].mData[0]);
					CurrentVertex.TextureCoordinates.m_y = 1.0f - float(texCoords[0][k].mData[1]);

					if (!binormals || !tangents)
					{

						mesh->InitBinormals();
						mesh->InitTangents();

						mesh->GenerateNormals(true, true, true);
						mesh->GenerateTangentsDataForAllUVSets(true);

						mesh->GetNormals(&normals);
						mesh->GetTangents(&tangents);
						mesh->GetBinormals(&binormals);
					}

					CurrentVertex.VertexNormals.m_x = float(normals[0][k].mData[0]);
					CurrentVertex.VertexNormals.m_y = float(normals[0][k].mData[1]);
					CurrentVertex.VertexNormals.m_z = float(normals[0][k].mData[2]);
					CurrentVertex.VertexNormals.Normalize();

					CurrentVertex.BiNormals.m_x = float(binormals[0][k].mData[0]);
					CurrentVertex.BiNormals.m_y = float(binormals[0][k].mData[1]);
					CurrentVertex.BiNormals.m_z = float(binormals[0][k].mData[2]);
					CurrentVertex.BiNormals.Normalize();

					CurrentVertex.Tangents.m_x = float(tangents[0][k].mData[0]);
					CurrentVertex.Tangents.m_y = float(tangents[0][k].mData[1]);
					CurrentVertex.Tangents.m_z = float(tangents[0][k].mData[2]);
					CurrentVertex.Tangents.Normalize();


					model->m_VertexBuffer.AddVertex(CurrentVertex);
				}

				int* CurrentIndex = mesh->GetPolygonVertices();
				for (int32 k = 0; k < mesh->GetPolygonVertexCount(); ++k)
				{
					model->m_IndexBuffer.AddIndex(CurrentIndex[k]);
				}

				model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
				model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

				Mesh meshReal;
				meshReal.m_Material = "mat";
				meshReal.m_Name = "NoSe";
				meshReal.m_MeshCore.m_IndexCount = mesh->GetPolygonVertexCount();
				meshReal.m_MeshCore.m_IndexOffset = 0;
				meshReal.m_MeshCore.m_VertexOffset = 0;

				model->m_VectorMeshes.push_back(meshReal);

				StrPtr<Material> mat = std::make_shared<Material>();
				mat->InitMaterial(MAT_PBR_MetRough, gfx);
				model->m_MaterialMap["mat"] = m_Res->InsertResource(mat);
			}
		}

		return model->GetHandle();
	}

	bool FbxLoader::AddNodeMesh(fbxsdk::FbxNode* pNode, const String& parent = "")
	{
		const char* nodeName = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();

		Vector3 trans(translation.mData[0], translation.mData[1], translation.mData[2]);
		Vector3 rot(rotation.mData[0], rotation.mData[1], rotation.mData[2]);
		Vector3 scale(scaling.mData[0], scaling.mData[1], scaling.mData[2]);

		WeakGameObjPtr node;

		if (parent == "")
		{
			node = SceneManager::Pointer()->GetActiveScene()->
				CreateActor(nodeName, NULL, trans, rot, scale);
		}
		else
		{
			auto scene = SceneManager::Pointer()->GetActiveScene();

			auto ptrParent = scene->FindActor(parent);
			if (ptrParent.expired())
			{
				node = scene->CreateActor(nodeName, ptrParent.lock().get(), trans, rot, scale);
			}
		}

		
		ResourceHandle handle = LoadMesh(pNode, false);
		SceneManager::Pointer()->GetActiveScene()->CreateComponent(CMP_MODEL_RENDERER, node.lock());
		node.lock()->GetComponent<ModelRenderer>(CMP_MODEL_RENDERER).lock()->SetModel(handle, m_Res);

		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			AddNodeMesh(pNode->GetChild(j), nodeName);

		return true;
	}
}