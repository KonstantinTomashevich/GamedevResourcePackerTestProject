# GamedevResourcePackerTestProject

Example test project for [GamedevResourcePacker](https://github.com/KonstantinTomashevich/GamedevResourcePacker). Shows how to integrate
[GamedevResourcePacker](https://github.com/KonstantinTomashevich/GamedevResourcePacker) with `DataObjects` plugin into 
**cmake** project.

## Assets
In this example we will use this assets:
```
Maps
  - Level1.xml # Describes tile map for first level of our game.
ObjectSets
  - Default.xml # Describes default set of map objects.
TileSets
  - Default.xml # Describes default set of tiles for tile maps.
```

**Note:** As you can see, assets are organized to folders by their data classes. It's not required by 
[GamedevResourcePacker](https://github.com/KonstantinTomashevich/GamedevResourcePacker), it's just convinient for this example project.

## DataObjects configuration

Data classes should be placed in `${your_config_dir}/DataObjectClasses`. You can read more about how to define them in
[GamedevResourcePacker](https://github.com/KonstantinTomashevich/GamedevResourcePacker) readme. 
There are 5 data classes in this example project:

* [MapObjectPrefab](./Config/DataObjectClasses/MapObjectPrefab.xml) -- describes object that placed on map and has size of 
several tiles.

* [MapObject](./Config/DataObjectClasses/MapObject.xml) -- describes `MapObjectPrefab` placement on `TileMap`.

* [MapObjectSet](./Config/DataObjectClasses/MapObjectSet.xml) -- contains several of `MapObjectPrefab`. Used by `TileMap`.

* [Tile](./Config/DataObjectClasses/Tile.xml) -- contains tile type index.

* [TileSet](./Config/DataObjectClasses/TileSet.xml) -- contains atlas image name and its rows and cols counts.

* [TileMap](./Config/DataObjectClasses/TileMap.xml) -- describes tile map with tiles (surprice!) and map objects. Refers to 
`TileSet` and `MapObjectSet`.

## CMake integration

From my experience, you don't need to run resource packing software every time you building your game. For example, when you're 
debugging your game you don't want to spend time rechecking resource model (which is the same). So it's better to call
[GamedevResourcePacker](https://github.com/KonstantinTomashevich/GamedevResourcePacker) each time you reruning CMake generation 
like this:

```cmake
message(STATUS "Running resource packer...")
execute_process(COMMAND "${ResourcePackerDir}/GamedevResourcePacker${CMAKE_EXECUTABLE_SUFFIX}"
        "${CMAKE_SOURCE_DIR}/Config" "${CMAKE_SOURCE_DIR}/Assets"
        "${CMAKE_BINARY_DIR}/bin/Assets" "${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem"
        WORKING_DIRECTORY "${ResourcePackerDir}"
        RESULT_VARIABLE RPResult
        OUTPUT_FILE "${CMAKE_BINARY_DIR}/ResourcePacker.log")

if (RPResult)
    message(FATAL_ERROR "Resource packer failed with code ${RPResult}. See ResourcePacker.log.")
else ()
    message(STATUS "Resource packer executed!")
endif ()
```

And then include generated code like this:

```cmake
include_directories(${CMAKE_SOURCE_DIR}
        "${CMAKE_BINARY_DIR}/Generated"
        "${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem")
add_subdirectory(${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem)
```

Don't forget to **link** your game executable to **ResourceSubsystem** library!

## Example code

Lets take a view of small example [code](./Game/Main.cpp), that reads and prints content of `Level1` `TileMap`.
Firstly, we need to initialize resource subsystem by specifying binary assets directory:

```cpp
try
{
    ResourceSubsystem::Init ("Assets");
}
catch (boost::exception &exception)
{
    BOOST_LOG_TRIVIAL (error) << "Unable to init resource subsystem!\n" <<
                              boost::diagnostic_information (exception) << "\n"
                              << ((std::exception *) &exception)->what () << "\n";
    return 2;
}
```

In this step `ResourceSubsystem` also loads `content.list`, that contains information about how to load (from which files and with which
offset) converted assets. During `content.list` loading exceptions may be thrown, so we must handle them.

Next step is to simply load our tile map:

```cpp
ResourceSubsystem::DataObjects::TileMap *tileMap;
{
    using namespace ResourceSubsystem;
    tileMap = (DataObjects::TileMap *) GetResource (
        object_class_loader_TileMap, Ids::TileMapGroupId, Ids::TileMap::Level1);
}
```

**Note:** Remember to **unref** resources if you don't need them. 

**Note:** You can see that we don't need to call any `shutdown` functions when terminating program -- it's because resource groups
are statically allocated and able to properly autodestruct themselves on program termination.
