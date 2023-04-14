// LevelGenerator.h
// A class that generates levels for a roguelike game based on Unreal Engine 4.27
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

// A struct that represents a tile in the level grid
USTRUCT(BlueprintType)
struct FTile
{
    GENERATED_BODY()

        // The type of the tile, such as floor, wall, door, etc.
        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FName Type;

    // The actor that occupies the tile, such as player, enemy, item, etc.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        AActor* Occupant;

    // The index of the tile in the level grid
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 Index;

    // The position of the tile in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector Position;

    // The constructor of the tile struct
    FTile()
    {
        Type = "None";
        Occupant = nullptr;
        Index = -1;
        Position = FVector::ZeroVector;
    }
};

// A struct that represents a room in the level grid
USTRUCT(BlueprintType)
struct FRoom
{
    GENERATED_BODY()

        // The tiles that belong to the room
        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FTile> Tiles;

    // The center of the room in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector Center;

    // The constructor of the room struct
    FRoom()
    {
        Tiles.Empty();
        Center = FVector::ZeroVector;
    }
};

UCLASS()
class ROGUELIKE_API ALevelGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALevelGenerator();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    // The width of the level grid in tiles
    UPROPERTY(EditAnywhere)
        int32 GridWidth;

    // The height of the level grid in tiles
    UPROPERTY(EditAnywhere)
        int32 GridHeight;

    // The size of each tile in units
    UPROPERTY(EditAnywhere)
        float TileSize;

    // The number of rooms to generate in the level grid
    UPROPERTY(EditAnywhere)
        int32 RoomCount;

    // The minimum width of each room in tiles
    UPROPERTY(EditAnywhere)
        int32 MinRoomWidth;

    // The maximum width of each room in tiles
    UPROPERTY(EditAnywhere)
        int32 MaxRoomWidth;

    // The minimum height of each room in tiles
    UPROPERTY(EditAnywhere)
        int32 MinRoomHeight;

    // The maximum height of each room in tiles
    UPROPERTY(EditAnywhere)
        int32 MaxRoomHeight;

    // The chance to generate a door for each room edge in percentage
    UPROPERTY(EditAnywhere)
        float DoorChance;

    // The blueprint class of the floor actor
    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> FloorBP;

    // The blueprint class of the wall actor
    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> WallBP;

    // The blueprint class of the door actor
    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> DoorBP;

    // The seed for the random number generator
    UPROPERTY(EditAnywhere)
        int32 Seed;

    // The random number generator for level generation
    FRandomStream RNG;

    // The array of tiles that represents the level grid
    TArray<FTile> Grid;

    // The array of rooms that are generated in the level grid
    TArray<FRoom> Rooms;

    // Generate the level grid based on the parameters and spawn actors accordingly
    void GenerateLevel();

    // Generate a random room and add it to the rooms array if it does not overlap with existing rooms
    bool GenerateRoom();

    // Generate a random door for each room edge based on the door chance and add it to the grid array
    void GenerateDoor(FRoom & Room);

    // Generate a random floor tile and add it to the grid array
    void GenerateFloor(int32 X, int32 Y);

    // Generate a random wall tile and add it to the grid array
    void GenerateWall(int32 X, int32 Y);

    // Spawn an actor based on the tile type and position
    void SpawnActor(FTile& Tile);

    // Check if a tile is valid and within the grid bounds
    bool IsValidTile(int32 X, int32 Y);

    // Check if a tile is occupied by an actor
    bool IsOccupiedTile(int32 X, int32 Y);

    // Check if a room overlaps with any existing rooms
    bool IsOverlappingRoom(FRoom& Room);

    // Get the index of a tile based on its coordinates
    int32 GetTileIndex(int32 X, int32 Y);

    // Get the coordinates of a tile based on its index
    void GetTileCoordinates(int32 Index, int32& OutX, int32& OutY);

    // Get the position of a tile based on its coordinates
    FVector GetTilePosition(int32 X, int32 Y);

    // Get the center of a room based on its tiles
    FVector GetRoomCenter(FRoom& Room);
};
