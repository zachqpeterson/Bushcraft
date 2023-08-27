#include "Terrain.h"

#include "KismetProceduralMeshLibrary.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
	Mesh->bUseAsyncCooking = true;
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();

	GenerateTerrain();
}

void ATerrain::GenerateTerrain()
{
	for (FOctave& o : Octaves)
	{
		o.Frequency = 1.0 / o.Wavelength;
	}

	OffsetX = Scale * SizeX * 0.5f;
	OffsetY = Scale * SizeY * 0.5f;

	Vertices.Reserve((SizeX + 1) * (SizeY + 1));
	Indices.Reserve(SizeX * SizeY * 6);

	GenerateHeightmap();

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Indices, UV0, Normals, Tangents);

	Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, {}, Tangents, true);
	Mesh->SetMaterial(0, Material);
}

void ATerrain::GenerateHeightmap()
{
	int32 vertex = 0;

	for (uint32 x = 0; x < SizeX; ++x)
	{
		for (uint32 y = 0; y < SizeY; ++y)
		{
			Vertices.Add({ x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) });
			UV0.Add({ x * UVScale, y * UVScale });

			Indices.Add(vertex);
			Indices.Add(vertex + 1);
			Indices.Add(vertex + SizeY + 1);
			Indices.Add(vertex + 1);
			Indices.Add(vertex + SizeY + 2);
			Indices.Add(vertex + SizeY + 1);

			++vertex;
		}

		Vertices.Add({ x * Scale - OffsetX, SizeY * Scale - OffsetY, GetNoiseValue(x, SizeY) });
		UV0.Add({ x * UVScale, SizeY * UVScale });

		++vertex;
	}

	for (uint32 y = 0; y < SizeY; ++y)
	{
		Vertices.Add({ SizeX * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(SizeX, y) });
		UV0.Add({ SizeX * UVScale, y * UVScale });
	}

	Vertices.Add({ SizeX * Scale - OffsetX, SizeY * Scale - OffsetY, GetNoiseValue(SizeX, SizeY) });
	UV0.Add({ SizeX * UVScale, SizeY * UVScale });
}

double ATerrain::GetNoiseValue(double x, double y)
{
	double value = 0.0;

	for (FOctave& o : Octaves)
	{
		value += Noise::Simplex2(x * Scale * o.Frequency, y * Scale * o.Frequency) * o.Amplitude;
	}

	return value;
}