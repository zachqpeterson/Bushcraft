#include "Terrain.h"

#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();

void ATerrain::OnConstruction(const FTransform& Transform)
{
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
	GenerateHeightmap(heightmap);
}

void ATerrain::GenerateHeightmap(UTexture2D* texture)
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