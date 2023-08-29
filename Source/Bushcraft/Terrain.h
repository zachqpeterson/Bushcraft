#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

UCLASS()
class BUSHCRAFT_API ATerrain : public AActor
{
	GENERATED_BODY()

public:
	ATerrain();

	UPROPERTY(EditAnywhere)
	int64 Seed = 0;

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

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	void GenerateTerrain();
	void GenerateHeightmap(UTexture2D* texture);
	double GetNoiseValue(double x, double y);

	TArray<FVector2d> NoiseOffsets;
};
