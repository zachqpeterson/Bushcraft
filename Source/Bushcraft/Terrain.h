#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Terrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

USTRUCT()
struct FOctave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double Amplitude = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double Wavelength = 1.0f;

	double Frequency;
};

UCLASS()
class BUSHCRAFT_API ATerrain : public AActor
{
	GENERATED_BODY()

public:
	ATerrain();

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 1))
	uint16 SizeX = 10;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 1))
	uint16 SizeY = 10;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double Scale = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.001))
	double UVScale = 1.0f;

	UPROPERTY(EditAnywhere)
	TArray<FOctave> Octaves;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	void GenerateTerrain();
	void GenerateHeightmap();
	double GetNoiseValue(double x, double y);

	UProceduralMeshComponent* Mesh;
	TArray<FVector3d> Vertices;
	TArray<int32> Indices;
	TArray<FVector2d> UV0;
	TArray<FVector3d> Normals;
	TArray<FProcMeshTangent> Tangents;

	double OffsetX;
	double OffsetY;
};
