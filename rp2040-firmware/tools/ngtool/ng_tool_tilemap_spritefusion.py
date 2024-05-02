import os
from PIL import Image
from utils import convert_to_rgb565
import struct
import json
from ng_tool_tilemap_formats import Tilemap,Layer

# Define classes for the objects in JSON
class SFTile:
    def __init__(self, id, x, y):
        self.id = id
        self.x = x
        self.y = y

class SFLayer:
    def __init__(self, name, tiles, collider):
        self.name = name
        self.tiles = tiles
        self.collider = collider

def read_spritefusion_to_tilemap(json_filename):

    # Read the JSON file
    with open(json_filename) as file:
        data = json.load(file)

    # Extract values from the JSON and create objects
    tileSize = data['tileSize']
    mapWidth = data['mapWidth']
    mapHeight = data['mapHeight']
    layers = []

    for layer_data in data['layers']:
        name = layer_data['name']
        tiles = []

        for tile_data in layer_data['tiles']:
            id = tile_data['id']
            x = tile_data['x']
            y = tile_data['y']
            tile = SFTile(id, x, y)
            tiles.append(tile)

        collider = layer_data['collider']
        layer = SFLayer(name, tiles, collider)
        layers.append(layer)

    # Print the extracted values
    print(f"Spritefusion import: {json_filename}")
    print(f"tileSize: {tileSize}")
    print(f"mapWidth: {mapWidth}")
    print(f"mapHeight: {mapHeight}")

    map = Tilemap(mapWidth,mapHeight)

    for in_layer in layers:
        layer = Layer(0,mapWidth,mapHeight,0,0,layer.name)
        map.add_layer(layer)
        print(f"\nLayer name: {layer.name}")
        #print(f"Collider: {layer.collider}")
        print("Tiles:")
        for tile in in_layer.tiles:
            layer.set_tile(tile.x,tile.y,int(tile.id))
            #print(f"  - id: {tile.id}, x: {tile.x}, y: {tile.y}")
    
    return map

    
