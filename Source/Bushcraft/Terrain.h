#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealtimeMeshSimple.h"
#include "Terrain.generated.h"

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
	double Scale = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double UVScale = 1.0f;

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
	void GenerateTerrain();
	void GenerateHeightmap();
	double GetNoiseValue(double x, double y);

	URealtimeMeshComponent* MeshComponent;
	URealtimeMeshSimple* Mesh;
	FRealtimeMeshSimpleMeshData MeshData{};
	FRealtimeMeshSectionConfig SectionConfig;

	TArray<FVector2d> NoiseOffsets;
	double OffsetX;
	double OffsetY;
};
