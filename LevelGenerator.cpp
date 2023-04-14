// LevelGenerator.cpp
// A class that generates levels for a roguelike game based on Unreal Engine 4.27

#include "LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the parameters for level generation
    GridWidth = 50;
    GridHeight = 50;
    TileSize = 100.0f;
    RoomCount = 10;
    MinRoomWidth = 5;
    MaxRoomWidth = 15;
    MinRoomHeight = 5;
    MaxRoomHeight = 15;
    DoorChance = 25.0f;
    FloorBP = nullptr;
    WallBP = nullptr;
    DoorBP = nullptr;
    Seed = 0;

    // Initialize the random number generator with the seed
    RNG.Initialize(Seed);

    // Initialize the grid array with empty tiles
    Grid.Init(FTile(), GridWidth * GridHeight);
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
    Super::BeginPlay();

    // Generate the level and spawn actors
    GenerateLevel();
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Generate the level grid based on the parameters and spawn actors accordingly
void ALevelGenerator::GenerateLevel()
{
    // Generate rooms until reaching the room count or failing too many times
    int32 FailCount = 0;
    int32 MaxFailCount = 100;
    while (Rooms.Num() < RoomCount && FailCount < MaxFailCount)
    {
        if (GenerateRoom())
        {
            FailCount = 0;
        }
        else
        {
            FailCount++;
        }
    }

    // Generate doors for each room
    for (FRoom& Room : Rooms)
    {
        GenerateDoor(Room);
    }

    // Spawn actors for each tile
    for (FTile& Tile : Grid)
    {
        SpawnActor(Tile);
    }
}

// Generate a random room and add it to the rooms array if it does not overlap with existing rooms
bool ALevelGenerator::GenerateRoom()
{
    // Generate a random width and height for the room
    int32 RoomWidth = RNG.RandRange(MinRoomWidth, MaxRoomWidth);
    int32 RoomHeight = RNG.RandRange(MinRoomHeight, MaxRoomHeight);

    // Generate a random position for the room
    int32 RoomX = RNG.RandRange(1, GridWidth - RoomWidth - 1);
    int32 RoomY = RNG.RandRange(1, GridHeight - RoomHeight - 1);

    // Create a new room struct
    FRoom Room;

    // Loop through the tiles that belong to the room
    for (int32 X = RoomX; X < RoomX + RoomWidth; X++)
    {
        for (int32 Y = RoomY; Y < RoomY + RoomHeight; Y++)
        {
            // Get the index of the tile
            int32 Index = GetTileIndex(X, Y);

            // Get the reference of the tile
            FTile& Tile = Grid[Index];

            // Set the type of the tile to floor
            Tile.Type = "Floor";

            // Set the index of the tile
            Tile.Index = Index;

            // Set the position of the tile
            Tile.Position = GetTilePosition(X, Y);

            // Add the tile to the room tiles array
            Room.Tiles.Add(Tile);
        }
    }

    // Check if the room overlaps with any existing rooms
    if (IsOverlappingRoom(Room))
    {
        // Return false to indicate failure
        return false;
    }
    else
    {
        // Set the center of the room
        Room.Center = GetRoomCenter(Room);

        // Add the room to the rooms array
        Rooms.Add(Room);

        // Return true to indicate success
        return true;
    }
}

// Generate a random door for each room edge based on the door chance and add it to the grid array
void ALevelGenerator::GenerateDoor(FRoom& Room)
{
    // Loop through the four edges of the room
    for (int32 Edge = 0; Edge < 4; Edge++)
    {
        // Generate a random number between 0 and 100
        int32 Random = RNG.RandRange(0, 100);

        // Check if the random number is less than or equal to the door chance
        if (Random <= DoorChance)
        {
            // Declare variables for the door coordinates and index
            int32 DoorX;
            int32 DoorY;
            int32 DoorIndex;

            // Switch on the edge value
            switch (Edge)
            {
            case 0: // Top edge
                // Generate a random X coordinate within the room width
                DoorX = RNG.RandRange(Room.Tiles[0].Index % GridWidth, Room.Tiles.Last().Index % GridWidth);

                // Set the Y coordinate to one above the room top edge
                DoorY = (Room.Tiles[0].Index / GridWidth) - 1;

                // Get the index of the door tile
                DoorIndex = GetTileIndex(DoorX, DoorY);

                break;

            case 1: // Right edge
                // Generate a random Y coordinate within the room height
                DoorY = RNG.RandRange(Room.Tiles[0].Index / GridWidth, Room.Tiles.Last().Index / GridWidth);

                // Set the X coordinate to one right of the room right edge
                DoorX = (Room.Tiles.Last().Index % GridWidth) + 1;

                // Get the index of the door tile
                DoorIndex = GetTileIndex(DoorX, DoorY);

                break;

            case 2: // Bottom edge

                // Generate a random X coordinate within the room width
                DoorX = RNG.RandRange(Room.Tiles[0].Index % GridWidth, Room.Tiles.Last().Index % GridWidth);

                // Set the Y coordinate to one below the room bottom edge
                DoorY = (Room.Tiles.Last().Index / GridWidth) + 1;

                // Get the index of the door tile
                DoorIndex = GetTileIndex(DoorX, DoorY);

                break;

            case 3: // Left edge
                // Generate a random Y coordinate within the room height
                DoorY = RNG.RandRange(Room.Tiles[0].Index / GridWidth, Room.Tiles.Last().Index / GridWidth);

                // Set the X coordinate to one left of the room left edge
                DoorX = (Room.Tiles[0].Index % GridWidth) - 1;

                // Get the index of the door tile
                DoorIndex = GetTileIndex(DoorX, DoorY);

                break;

            default:
                // Invalid edge value, return without generating a door
                return;
            }

            // Get the reference of the door tile
            FTile& DoorTile = Grid[DoorIndex];

            // Set the type of the tile to door
            DoorTile.Type = "Door";

            // Set the index of the tile
            DoorTile.Index = DoorIndex;

            // Set the position of the tile
            DoorTile.Position = GetTilePosition(DoorX, DoorY);
        }
    }
}

// Generate a random floor tile and add it to the grid array
void ALevelGenerator::GenerateFloor(int32 X, int32 Y)
{
    // Get the index of the tile
    int32 Index = GetTileIndex(X, Y);

    // Get the reference of the tile
    FTile& Tile = Grid[Index];

    // Set the type of the tile to floor
    Tile.Type = "Floor";

    // Set the index of the tile
    Tile.Index = Index;

    // Set the position of the tile
    Tile.Position = GetTilePosition(X, Y);
}

// Generate a random wall tile and add it to the grid array
void ALevelGenerator::GenerateWall(int32 X, int32 Y)
{
    // Get the index of the tile
    int32 Index = GetTileIndex(X, Y);

    // Get the reference of the tile
    FTile& Tile = Grid[Index];

    // Set the type of the tile to wall

    Tile.Type = "Wall";

    // Set the index of the tile
    Tile.Index = Index;

    // Set the position of the tile
    Tile.Position = GetTilePosition(X, Y);
}

// Spawn an actor based on the tile type and position
void ALevelGenerator::SpawnActor(FTile& Tile)
{
    // Check if the tile type is valid
    if (Tile.Type != "None")
    {
        // Declare a variable for the actor class to spawn
        TSubclassOf<AActor> ActorClass;

        // Switch on the tile type
        switch (Tile.Type)
        {
        case "Floor":
            // Set the actor class to the floor blueprint class
            ActorClass = FloorBP;
            break;

        case "Wall":
            // Set the actor class to the wall blueprint class
            ActorClass = WallBP;
            break;

        case "Door":
            // Set the actor class to the door blueprint class
            ActorClass = DoorBP;
            break;

        default:
            // Invalid tile type, return without spawning an actor
            return;
        }

        // Check if the actor class is valid
        if (ActorClass)
        {
            // Spawn an actor of the given class at the tile position with no rotation
            AActor* Actor = GetWorld()->SpawnActor<AActor>(ActorClass, Tile.Position, FRotator::ZeroRotator);

            // Check if the actor is valid
            if (Actor)
            {
                // Set the occupant of the tile to the spawned actor
                Tile.Occupant = Actor;
            }
        }
    }
}

// Check if a tile is valid and within the grid bounds
bool ALevelGenerator::IsValidTile(int32 X, int32 Y)
{
    // Return true if the coordinates are within the grid width and height, false otherwise
    return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

// Check if a tile is occupied by an actor
bool ALevelGenerator::IsOccupiedTile(int32 X, int32 Y)
{
    // Check if the tile is valid
    if (IsValidTile(X, Y))
    {
        // Get the index of the tile
        int32 Index = GetTileIndex(X, Y);

        // Get the reference of the tile
        FTile& Tile = Grid[Index];

        // Return true if the tile has an occupant, false otherwise
            return Tile.Occupant != nullptr;
    }
    else
    {
        // Return false if the tile is invalid
        return false;
    }
}

// Check if a room overlaps with any existing rooms
bool ALevelGenerator::IsOverlappingRoom(FRoom& Room)
{
    // Loop through the existing rooms
    for (FRoom& OtherRoom : Rooms)
    {
        // Check if the room center is within the other room bounds
        if (Room.Center.X > OtherRoom.Tiles[0].Position.X &&
            Room.Center.X < OtherRoom.Tiles.Last().Position.X &&
            Room.Center.Y > OtherRoom.Tiles[0].Position.Y &&
            Room.Center.Y < OtherRoom.Tiles.Last().Position.Y)
        {
            // Return true if there is an overlap
            return true;
        }
    }

    // Return false if there is no overlap
    return false;
}

// Get the index of a tile based on its coordinates
int32 ALevelGenerator::GetTileIndex(int32 X, int32 Y)
{
    // Return the index by multiplying the Y coordinate by the grid width and adding the X coordinate
    return Y * GridWidth + X;
}

// Get the coordinates of a tile based on its index
void ALevelGenerator::GetTileCoordinates(int32 Index, int32& OutX, int32& OutY)
{
    // Set the X coordinate by getting the remainder of dividing the index by the grid width
    OutX = Index % GridWidth;

    // Set the Y coordinate by getting the quotient of dividing the index by the grid width
    OutY = Index / GridWidth;
}

// Get the position of a tile based on its coordinates
FVector ALevelGenerator::GetTilePosition(int32 X, int32 Y)
{
    // Return the position by multiplying the coordinates by the tile size and adding half of it
    return FVector(X * TileSize + TileSize / 2.0f, Y * TileSize + TileSize / 2.0f, 0.0f);
}

// Get the center of a room based on its tiles
FVector ALevelGenerator::GetRoomCenter(FRoom& Room)
{
    // Declare variables for the sum of the tile positions and the number of tiles
    FVector Sum = FVector::ZeroVector;
    int32 Count = 0;

    // Loop through the room tiles
    for (FTile& Tile : Room.Tiles)
    {
        // Add the tile position to the sum
        Sum += Tile.Position;

        // Increment the count
        Count++;
    }

    // Return the average of the sum by dividing it by the count
    return Sum / Count;
    }   
}