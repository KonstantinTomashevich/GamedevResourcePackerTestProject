#include <ResourceSubsystem/Core.hpp>
#include <ResourceSubsystem/Defines.hpp>
#include <ResourceSubsystem/Ids.hpp>

#include <ResourceSubsystem/DataObjects/TileMap.hpp>
#include <ResourceSubsystem/DataObjects/TileSet.hpp>
#include <ResourceSubsystem/DataObjects/MapObjectSet.hpp>

#include <exception>
#include <boost/log/trivial.hpp>
#include <boost/exception/all.hpp>

int main (int argc, char **argv)
{
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


    ResourceSubsystem::DataObjects::TileMap *tileMap;
    {
        using namespace ResourceSubsystem;
        tileMap = (DataObjects::TileMap *) GetResource (
            object_class_loader_TileMap, Ids::TileMapGroupId, Ids::TileMap::Level1);
    }

    printf ("Tiles:\n");
    for (auto &tile : tileMap->GetTiles ())
    {
        printf ("    Tile: %d.\n", tile.GetType ());
    }

    printf ("Objects:\n");
    for (auto &object : tileMap->GetObjects ())
    {
        printf ("    Object: %d (%d, %d).\n", object.GetType (), object.GetX (), object.GetY ());
    }

    printf ("TileSet id: %d.\n", tileMap->GetTileSet ()->GetId ());
    printf ("ObjectSet id: %d.\n", tileMap->GetObjectSet ()->GetId ());
    return 0;
}
