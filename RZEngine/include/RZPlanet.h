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

	class RZ_EXP PatchData
	{
	public:
		NodeConnection* ConnectorOne = nullptr;
		NodeConnection* ConnectorTwo = nullptr;

		bool Connected;
		int OneConnectedDepth;
		int TwoConnectedDepth;
	};



	///Primero. Todos los bordes los registro como BorderData con los vertices, el lado y el depth del node.
	//Segundo. Cuando subdivido le mando el hash del padre a los hijos, para que busquen colisiones con ese.
	//Tercero. Si encuentro una colisión, checo mi Side y lo comparo con el registrado para saber en que orden tomar los elementos del vector.
	//Cuarto. Si es del mismo depth nomás copio los vectores, si es un depth mayor, empiezo desde la mitad de los elementos y solamente agarro 1 de cada 2.
	//Quinto. Ya que tengo todos los vertices setteados y listos, creo el vertex buffer y termino la generación.

	//Problemas:
	///Handle los nodos hermanos al mismo nivel. Yo digo que tambien se registren de una vez y el que llegue segundo se setee.
	///Las normales averaged tal vez no funcionen. Yo digo que para las normales puedo hacer un average entre las dos normales, a ver como se ve. 
	///O mucho más complicado, buscar la forma de rehacer el average tomando en cuenta ambos parches. 
	///Idea: guardar en el parche la normal sumada para el avg sin la division  y solamente sumar las dos normales sumadas de los dos vertices, pero eso no cambiaría el node que ya está generado, tal vez mandar el updateresource es inevitable. 
	///Solamente cambiaría los edges entonces puede estar bien.


	class RZ_EXP Planet
	{
	public:
		int QuadTreesNodes;
		int Connections;

		Planet* instance;
		PlanetQuadTreeNode* ActiveQuadTree = nullptr;

		void InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng);
		void RenderPlanet(float scale);
		void RenderPlanetWater(float scale);
		void RenderAtmosphere(float scale);

		void CreateMaterial();

		FORCEINLINE Vector3 GetSpacePosition() { return m_SpacePosition; };

		ResourceHandle m_Materials;
		ResourceHandle m_HeightMap;
		Vector4 m_HeightScale;

		PlanetQuadTreeNode Quadtree[6];

		int32 Seed = 0;
		PerlinNoise3D noise;

		Map<uint32, PatchData> m_PatchInfo;

	private:
		void LoadAndProcessModel();
		
		void ProcessConnectionNode(NodeConnection& node);

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