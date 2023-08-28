#include "Terrain.h"

#include "KismetProceduralMeshLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
	Mesh->bUseAsyncCooking = true;
	Mesh->bUseComplexAsSimpleCollision = false;
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrain::OnConstruction(const FTransform& Transform)
{
	Vertices.SetNum((SizeX + 1) * (SizeY + 1));
	Indices.SetNum(SizeX * SizeY * 6);
	UV0.SetNum((SizeX + 1) * (SizeY + 1));

	Mesh->ClearAllMeshSections();

	Frequency = 1.0 / Wavelength;

	GenerateTerrain();
}

void ATerrain::GenerateTerrain()
{
	OffsetX = Scale * SizeX * 0.5f;
	OffsetY = Scale * SizeY * 0.5f;

	GenerateHeightmap();

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Indices, UV0, Normals, Tangents);

	Mesh->CreateMeshSection(0, Vertices, Indices, {}, UV0, {}, {}, true);
	Mesh->SetMaterial(0, Material);
}

void ATerrain::GenerateHeightmap()
{
	int32 num = SizeX * SizeY;

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);
		int32 vertex = index + index / SizeY;
	
		Vertices[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		UV0[index] = {x * UVScale, y * UVScale};

		Indices[index * 6] = vertex;
		Indices[index * 6 + 1] = vertex + 1;
		Indices[index * 6 + 2] = vertex + SizeY + 1;
		Indices[index * 6 + 3] = vertex + 1;
		Indices[index * 6 + 4] = vertex + SizeY + 2;
		Indices[index * 6 + 5] = vertex + SizeY + 1;
	});

	int32 end = Vertices.Num();

	for (int32 index = num; index < end; ++index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);
	
		Vertices[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		UV0[index] = { x * UVScale, y * UVScale };
	}
}

double ATerrain::GetNoiseValue(double x, double y)
{
	return Noise::Simplex2(x * Scale * Frequency, y * Scale * Frequency) * Amplitude;
}