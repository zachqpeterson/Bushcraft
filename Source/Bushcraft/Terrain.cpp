#include "Terrain.h"

#include "KismetProceduralMeshLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Landscape.h"
#include "Noise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

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
	if (Landscape)
	{
		UTexture2D* heightmap = UTexture2D::CreateTransient(1009, 1009, EPixelFormat::PF_R16_UINT, "Heightmap");
		heightmap->LODGroup = TEXTUREGROUP_Terrain_Heightmap;

		GenerateHeightmap(heightmap);

		Landscape->LandscapeComponents[0]->SetHeightmap(heightmap);
		Landscape->LandscapeComponents[0]->RequestHeightmapUpdate();
		Landscape->LandscapeComponents[0]->PostLoad();
	}
}

void ATerrain::GenerateHeightmap(UTexture2D* texture)
{
	FTexture2DMipMap* mipMap = &texture->PlatformData->Mips[0];
	FByteBulkData* imageData = &mipMap->BulkData;
	uint16* rawImageData = (uint16*)imageData->Lock(LOCK_READ_WRITE);

	if (rawImageData)
	{
		int32 width = texture->GetSizeX();
		int32 height = texture->GetSizeY();
		int32 total = width * height;

		ParallelFor(total, [&](int32 index)
		{
			double x = index / (height + 1);
			double y = index % (height + 1);

			rawImageData[index] = 255;//GetNoiseValue(x, y);
		});
	}

	imageData->Unlock();
	texture->UpdateResource();
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