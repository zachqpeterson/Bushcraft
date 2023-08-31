#include "Terrain.h"

#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "ProceduralMeshComponent.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrain::OnConstruction(const FTransform& Transform)
{
	if(firstConstruct)
	{
		firstConstruct = false;
		SetupVertices();
	}
	else
	{
		if (sizeX != SizeX) { ResizeX(); }
		else if (sizeY != SizeY) { ResizeY(); }
		else if (scale != Scale) { Rescale(); }
		else if (uvScale != UVScale) { RegenerateUVs(); }
		else if (material != Material) { material = Material; Mesh->SetMaterial(0, Material); }
		else if (seed != Seed || octaves != Octaves || persistance != Persistance || lacunarity != Lacunarity) { RegenerateHeightmap(); }
	}
}

void ATerrain::SetupVertices()
{
	seed = Seed;
	sizeX = SizeX;
	sizeY = SizeY;
	scale = Scale;
	uvScale = UVScale;

	material = Material;

	octaves = Octaves;
	persistance = Persistance;
	lacunarity = Lacunarity;

	FMath::RandInit(Seed);
	for (uint8 octave = 0; octave < Octaves; ++octave)
	{
		NoiseOffsets.Add({ FMath::RandRange(-100000000.0, 100000000.0), FMath::RandRange(-100000000.0, 100000000.0) });
	}

	Positions.SetNum((SizeX + 1) * (SizeY + 1), false);
	Indices.SetNum(SizeX * SizeY * 6, false);
	UV0.SetNum((SizeX + 1) * (SizeY + 1), false);

	OffsetX = SizeX * Scale / 2.0;
	OffsetY = SizeY * Scale / 2.0;

	int32 num = SizeX * SizeY;

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);
		int32 vertex = index + index / SizeY;

		Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		UV0[index] = { x * UVScale, y * UVScale };

		Indices[index * 6] = vertex;
		Indices[index * 6 + 1] = vertex + 1;
		Indices[index * 6 + 2] = vertex + SizeY + 1;
		Indices[index * 6 + 3] = vertex + 1;
		Indices[index * 6 + 4] = vertex + SizeY + 2;
		Indices[index * 6 + 5] = vertex + SizeY + 1;
	});

	int32 end = Positions.Num();

	for (int32 index = num; index < end; ++index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);

		Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
		UV0[index] = { x * UVScale, y * UVScale };
	}

	Mesh->CreateMeshSection(0, Positions, Indices, {}, UV0, {}, {}, true);
	Mesh->SetMaterial(0, Material);
}

void ATerrain::ResizeX()
{
	OffsetX = SizeX * Scale / 2.0;

	Positions.SetNum((SizeX + 1) * (SizeY + 1), false);
	Indices.SetNum(SizeX * SizeY * 6, false);
	UV0.SetNum((SizeX + 1) * (SizeY + 1), false);

	if (sizeX < SizeX)
	{
		int32 num = SizeX * SizeY;

		ParallelFor(num, [&](int32 index)
		{
			double x = index / (SizeY + 1);
			double y = index % (SizeY + 1);
			int32 vertex = index + index / SizeY;

			Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
			UV0[index] = { x * UVScale, y * UVScale };

			Indices[index * 6] = vertex;
			Indices[index * 6 + 1] = vertex + 1;
			Indices[index * 6 + 2] = vertex + SizeY + 1;
			Indices[index * 6 + 3] = vertex + 1;
			Indices[index * 6 + 4] = vertex + SizeY + 2;
			Indices[index * 6 + 5] = vertex + SizeY + 1;
		});

		int32 end = Positions.Num();

		for (int32 index = num; index < end; ++index)
		{
			double x = index / (SizeY + 1);
			double y = index % (SizeY + 1);

			Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
			UV0[index] = { x * UVScale, y * UVScale };
		}
	}

	sizeX = SizeX;

	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Positions, Indices, {}, UV0, {}, {}, true);
}

void ATerrain::ResizeY()
{
	OffsetY = SizeY * Scale / 2.0;

	Positions.SetNum((SizeX + 1) * (SizeY + 1), false);
	Indices.SetNum(SizeX * SizeY * 6, false);
	UV0.SetNum((SizeX + 1) * (SizeY + 1), false);

	if (sizeY < SizeY)
	{
		int32 num = SizeX * SizeY;

		ParallelFor(num, [&](int32 index)
		{
			double x = index / (SizeY + 1);
			double y = index % (SizeY + 1);
			int32 vertex = index + index / SizeY;

			Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
			UV0[index] = { x * UVScale, y * UVScale };

			Indices[index * 6] = vertex;
			Indices[index * 6 + 1] = vertex + 1;
			Indices[index * 6 + 2] = vertex + SizeY + 1;
			Indices[index * 6 + 3] = vertex + 1;
			Indices[index * 6 + 4] = vertex + SizeY + 2;
			Indices[index * 6 + 5] = vertex + SizeY + 1;
		});

		int32 end = Positions.Num();

		for (int32 index = num; index < end; ++index)
		{
			double x = index / (SizeY + 1);
			double y = index % (SizeY + 1);

			Positions[index] = { x * Scale - OffsetX, y * Scale - OffsetY, GetNoiseValue(x, y) };
			UV0[index] = { x * UVScale, y * UVScale };
		}
	}

	sizeY = SizeY;

	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Positions, Indices, {}, UV0, {}, {}, true);
}

void ATerrain::Rescale()
{
	scale = Scale;
	OffsetX = SizeX * Scale / 2.0;
	OffsetY = SizeY * Scale / 2.0;

	int32 num = (SizeX + 1) * (SizeY + 1);

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);

		Positions[index].X = x * Scale - OffsetX;
		Positions[index].Y = y * Scale - OffsetY;
	});

	Mesh->UpdateMeshSection(0, Positions, {}, UV0, {}, {});
}

void ATerrain::RegenerateUVs()
{
	uvScale = UVScale;
	int32 num = (SizeX + 1) * (SizeY + 1);

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);

		UV0[index] = { x * UVScale, y * UVScale };
	});

	Mesh->UpdateMeshSection(0, Positions, {}, UV0, {}, {});
}

void ATerrain::RegenerateHeightmap()
{
	seed = Seed;

	octaves = Octaves;
	persistance = Persistance;
	lacunarity = Lacunarity;

	NoiseOffsets.Reset();

	FMath::RandInit(Seed);
	for (uint8 octave = 0; octave < Octaves; ++octave)
	{
		NoiseOffsets.Add({ FMath::RandRange(-100000000.0, 100000000.0), FMath::RandRange(-100000000.0, 100000000.0) });
	}

	int32 num = (SizeX + 1) * (SizeY + 1);

	ParallelFor(num, [&](int32 index)
	{
		double x = index / (SizeY + 1);
		double y = index % (SizeY + 1);

		Positions[index].Z = GetNoiseValue(x, y);
	});

	Mesh->UpdateMeshSection(0, Positions, {}, UV0, {}, {});
}

double ATerrain::GetNoiseValue(double x, double y)
{
	double amplitude = 30.0;
	double frequency = 0.0005;
	double value = 0.0;

	double sampleX = x * Scale;
	double sampleY = y * Scale;

	for (uint8 octave = 0; octave < Octaves; ++octave)
	{
		FVector2d& offset = NoiseOffsets[octave];
		double simplex = Noise::Simplex2(sampleX * frequency + offset.X, sampleY * frequency + offset.Y) * 2.0 + 1.1;

		value += simplex * amplitude;

		amplitude *= Persistance;
		frequency *= Lacunarity;
	}

	value += FMath::Max(Noise::Simplex2(sampleX * 0.0001, sampleY * 0.0001), 0.0) * 1500.0;

	return value;
}