#include "Terrain.h"

#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "RealtimeMeshComponent.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent = NewObject<URealtimeMeshComponent>(this, "RMC");
	RootComponent = MeshComponent;
	Mesh = MeshComponent->InitializeRealtimeMesh<URealtimeMeshSimple>();
	SectionConfig = FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, 0);

	MeshData.Positions.SetNum((SizeX + 1) * (SizeY + 1), false);
	MeshData.Triangles.SetNum(SizeX * SizeY * 6, false);
	MeshData.UV0.SetNum((SizeX + 1) * (SizeY + 1), false);
	MeshData.Normals.SetNum((SizeX + 1) * (SizeY + 1), false);
	MeshData.Tangents.SetNum((SizeX + 1) * (SizeY + 1), false);
	OffsetX = SizeX / 2.0;
	OffsetY = SizeY / 2.0;

	NoiseOffsets.Reset();

	FMath::RandInit(Seed);
	for (uint8 octave = 0; octave < Octaves; ++octave)
	{
		NoiseOffsets.Add({ FMath::RandRange(-100000000.0, 100000000.0), FMath::RandRange(-100000000.0, 100000000.0) });
	}

	GenerateTerrain();
}

void ATerrain::GenerateTerrain()
{
	GenerateHeightmap();

	FRealtimeMeshSectionKey StaticSectionKey = Mesh->CreateMeshSection(0, SectionConfig, MeshData, true);
	//Mesh->UpdateSectionMesh(StaticSectionKey, MeshData);
}

void ATerrain::GenerateHeightmap()
{
	int32 num = SizeX * SizeY;

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);
		int32 vertex = index + index / SizeY;
	
		MeshData.Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		MeshData.UV0[index] = { x * UVScale, y * UVScale };
		MeshData.Normals[index] = { 0.0, 0.0, 0.0 };
		MeshData.Tangents[index] = { 0.0, 0.0, 0.0 };
	
		MeshData.Triangles[index * 6] = vertex;
		MeshData.Triangles[index * 6 + 1] = vertex + 1;
		MeshData.Triangles[index * 6 + 2] = vertex + SizeY + 1;
		MeshData.Triangles[index * 6 + 3] = vertex + 1;
		MeshData.Triangles[index * 6 + 4] = vertex + SizeY + 2;
		MeshData.Triangles[index * 6 + 5] = vertex + SizeY + 1;
	});
	
	int32 end = MeshData.Positions.Num();
	
	for (int32 index = num; index < end; ++index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);
	
		MeshData.Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		MeshData.UV0[index] = { x * UVScale, y * UVScale };
	}
}

double ATerrain::GetNoiseValue(double x, double y)
{
	double amplitude = 1.0;
	double frequency = 1.0;
	double value = 0.0;

	double sampleX = x * Scale;
	double sampleY = y * Scale;

	for (uint8 octave = 0; octave < Octaves; ++octave)
	{
		FVector2d& offset = NoiseOffsets[octave];
		double simplex = Noise::Simplex2(sampleX * frequency + offset.X, sampleY * frequency + offset.Y) * 2.0 - 1.1;

		value += simplex * amplitude;

		amplitude *= Persistance;
		frequency *= Lacunarity;
	}

	return value;
}