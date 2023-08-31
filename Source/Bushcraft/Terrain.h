#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class BUSHCRAFT_API ATerrain : public AActor
{
	GENERATED_BODY()

public:
	ATerrain();

	UPROPERTY(EditAnywhere)
	int64 Seed = 0;

	UPROPERTY(EditAnywhere)
	uint32 SizeX = 100;
	UPROPERTY(EditAnywhere)
	uint32 SizeY = 100;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double Scale = 10.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double UVScale = 0.1f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	uint8 Octaves = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0, ClampMax = 1.0))
	double Persistance = 0.5f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 1.0))
	double Lacunarity = 1.0f;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	void SetupVertices();
	void ResizeX();
	void ResizeY();
	void Rescale();
	void RegenerateUVs();
	void RegenerateHeightmap();

	double GetNoiseValue(double x, double y);

	bool firstConstruct = true;

	int64 seed;
	uint32 sizeX;
	uint32 sizeY;
	double scale;
	double uvScale;

	uint8 octaves;
	double persistance;
	double lacunarity;

	UMaterialInterface* material;

	UProceduralMeshComponent* Mesh;

	TArray<FVector3d> Positions;
	TArray<FVector2d> UV0;
	TArray<int32> Indices;

	TArray<FVector2d> NoiseOffsets;
	double OffsetX;
	double OffsetY;
};
