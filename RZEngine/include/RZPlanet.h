#pragma once
#define PERMUTATION_TABLE_SIZE 256

namespace rczEngine
{
	class RZ_EXP SpaceManager;

	class RZ_EXP PlanetVertex
	{
		Vector3 VertexPosition;
		Vector2 TextureCoordinates;
		Vector3 VertexNormals;
		Vector3 Tangents;
		Vector3 BiNormals;
		int Gradient[4];
	};

	struct RZ_EXP PatchData
	{
		Vector<Gfx::Vertex> Vertices;
		String VertexStartHash;
		String VertexEndHash;
		int DepthOfData;
		bool First;
	};

	///Primero. Todos los bordes los registro como BorderData con los vertices, el lado y el depth del node.
	//Segundo. Cuando subdivido le mando el hash del padre a los hijos, para que busquen colisiones con ese.
	//Tercero. Si encuentro una colisi�n, checo mi Side y lo comparo con el registrado para saber en que orden tomar los elementos del vector.
	//Cuarto. Si es del mismo depth nom�s copio los vectores, si es un depth mayor, empiezo desde la mitad de los elementos y solamente agarro 1 de cada 2.
	//Quinto. Ya que tengo todos los vertices setteados y listos, creo el vertex buffer y termino la generaci�n.

	//Problemas:
	///Handle los nodos hermanos al mismo nivel. Yo digo que tambien se registren de una vez y el que llegue segundo se setee.
	///Las normales averaged tal vez no funcionen. Yo digo que para las normales puedo hacer un average entre las dos normales, a ver como se ve. 
	///O mucho m�s complicado, buscar la forma de rehacer el average tomando en cuenta ambos parches. 
	///Idea: guardar en el parche la normal sumada para el avg sin la division  y solamente sumar las dos normales sumadas de los dos vertices, pero eso no cambiar�a el node que ya est� generado, tal vez mandar el updateresource es inevitable. 
	///Solamente cambiar�a los edges entonces puede estar bien.


	class RZ_EXP Planet
	{
	public:
		Planet* instance;
		PlanetQuadTreeNode* ActiveQuadTree;

		void InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng);
		void RenderPlanet(float scale);
		void RenderPlanetWater(float scale);
		void RenderAtmosphere(float scale);

		void CreateMaterial();

		void ProcessBorderData(String hash, PlanetQuadTreeNode* node, eSide side, String Start, String End);
		const PatchData* GetPatchData(String hash);

		FORCEINLINE Vector3 GetSpacePosition() { return m_SpacePosition; };

		Vector2 m_PolarPos;

		float m_PlanetNorm = 1.0f;
		float m_PlanetScale = 400.0f;
		float m_SpaceDist = 0.0f;
		float m_RealDist = 0.0f;

		ResourceHandle m_Materials;
		ResourceHandle m_HeightMap;
		Vector4 m_HeightScale;

		PlanetQuadTreeNode Quadtree[6];

		int32 Seed = 0;
		PerlinNoise3D noise;

		MMap<float, PlanetQuadTreeNode*> m_NodeAdyacency;
		Map<String, PatchData> m_PatchInfo;

	private:
		void LoadAndProcessModel();
		
		bool m_OnLand = false;

		StrPtr<Model> m_Planet;

		SpaceManager* m_SpaceMng;
		Gfx::GfxCore* m_GfxCore;
		StrPtr<Scene> m_CurrentScene;

		Gfx::ConstantBuffer m_GradientCB;
		Gfx::ConstantBuffer m_GradientsReal;
		Gfx::ConstantBuffer m_ScaleCB;

		Gfx::ConstantBuffer m_PosView;
		Vector4 PlanetPos;

		Gfx::ConstantBuffer m_HeightCameracb;
		Vector4 m_HeightCamera;
		
		Vector4 PermutationTable[PERMUTATION_TABLE_SIZE];

		Vector3 m_SpacePosition;

		Matrix4 m_PlanetMatrix;

		ResourceHandle Water;
		StrPtr<CameraCmp> PlayerCamera;
	};
}