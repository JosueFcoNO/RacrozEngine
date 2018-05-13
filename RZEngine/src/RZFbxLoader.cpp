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

	bool FbxLoader::LoadModel()
	{
		// Change the following filename to a suitable filename value.
		const char* lFilename = "Models/Necris/Necris.fbx";

		// Initialize the SDK manager. This object handles memory management.
		m_Manager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(m_Manager, IOSROOT);
		m_Manager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(m_Manager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(lFilename, -1, m_Manager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
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

		return true;
	}

	bool FbxLoader::AddNodeMesh(FbxNode* pNode, const char* parent)
	{
		const char* nodeName = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();

		Vector3 trans(translation.mData[0], translation.mData[1], translation.mData[2]);
		Vector3 rot(rotation.mData[0], rotation.mData[1], rotation.mData[2]);
		Vector3 scale(scaling.mData[0], scaling.mData[1], scaling.mData[2]);

		WeakGameObjectPtr node;

		if (!parent)
		{
			node = SceneManager::Pointer()->GetActiveScene()->
				CreateActor(nodeName, NULL, trans, rot, scale);
		}
		else
		{
			auto scene = SceneManager::Pointer()->GetActiveScene();

			auto ptrParent = scene->FindActor(parent);

			node = scene->
				CreateActor(nodeName, ptrParent.lock().get(), trans, rot, scale);
		}

		ResVault* res = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		Model* modelo;

		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		{
			FbxNodeAttribute* attribute = (pNode->GetNodeAttributeByIndex(i));

			FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());

			if (typeName == "mesh")
			{
				FbxMesh* mesh = (FbxMesh*)attribute;

				mesh->SplitPoints();

				auto vertices = mesh->GetControlPoints();

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* normals;
				mesh->GetNormals(&normals);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* binormals;
				mesh->GetBinormals(&binormals);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* tangents;
				mesh->GetTangents(&tangents);

				fbxsdk::FbxLayerElementArrayTemplate<FbxVector2>* texCoords;
				mesh->GetTextureUV(&texCoords);

				modelo = new Model;


				Gfx::Vertex CurrentVertex;
				for (int i = 0; i < mesh->GetControlPointsCount(); ++i)
				{
					CurrentVertex.VertexPosition.m_x = float(vertices[i].mData[0]);
					CurrentVertex.VertexPosition.m_y = float(vertices[i].mData[1]);
					CurrentVertex.VertexPosition.m_z = float(vertices[i].mData[2]);

					CurrentVertex.TextureCoordinates.m_x = float(texCoords[0][i].mData[0]);
					CurrentVertex.TextureCoordinates.m_y = 1.0f-float(texCoords[0][i].mData[1]);

					if (!binormals || !tangents)
					{
						mesh->InitBinormals();
						mesh->InitTangents();

						mesh->GenerateNormals(false, true);
						mesh->GenerateTangentsDataForAllUVSets(true);

						mesh->GetNormals(&normals);
						mesh->GetTangents(&tangents);
						mesh->GetBinormals(&binormals);
					}

					CurrentVertex.VertexNormals.m_x = float(normals[0][i].mData[0]);
					CurrentVertex.VertexNormals.m_y = float(normals[0][i].mData[1]);
					CurrentVertex.VertexNormals.m_z = float(normals[0][i].mData[2]);
					CurrentVertex.VertexNormals.Normalize();

					CurrentVertex.BiNormals.m_x = float(binormals[0][i].mData[0]);
					CurrentVertex.BiNormals.m_y = float(binormals[0][i].mData[1]);
					CurrentVertex.BiNormals.m_z = float(binormals[0][i].mData[2]);
					CurrentVertex.BiNormals.Normalize();

					CurrentVertex.Tangents.m_x = float(tangents[0][i].mData[0]);
					CurrentVertex.Tangents.m_y = float(tangents[0][i].mData[1]);
					CurrentVertex.Tangents.m_z = float(tangents[0][i].mData[2]);
					CurrentVertex.Tangents.Normalize();


					modelo->m_VertexBuffer.AddVertex(CurrentVertex);
				}

				int* CurrentIndex = mesh->GetPolygonVertices();
				for (int32 k = 0; k < mesh->GetPolygonVertexCount(); ++k)
				{
					modelo->m_IndexBuffer.AddIndex(CurrentIndex[k]);
				}

				modelo->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
				modelo->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);

				Mesh meshReal;
				meshReal.m_Material = "mat";
				meshReal.m_Name = "NoSe";
				meshReal.m_MeshCore.m_IndexCount = mesh->GetPolygonVertexCount();
				meshReal.m_MeshCore.m_IndexOffset = 0;
				meshReal.m_MeshCore.m_VertexOffset = 0;

				modelo->m_VectorMeshes.push_back(meshReal);

				StrPtr<Material> mat = std::make_shared<Material>();
				mat->InitMaterial(MAT_PBR_MetRough, gfx);
				modelo->m_MaterialMap["mat"] = res->InsertResource(mat);

				//node.lock()->AddComponent<ModelRenderer>().lock()->SetModel(res->InsertResource(modelo), res);
			}

			return true;
		}


		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			AddNodeMesh(pNode->GetChild(j), nodeName);
	}
}