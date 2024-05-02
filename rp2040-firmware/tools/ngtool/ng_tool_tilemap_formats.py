# Tilemap-Formats:
# 
# Format1:
# - all layers are supposed to have the same size
# - layers don't know what their tilesheet and palette is
# - layers consist of one tile per position. Tile 0 is none-Tile
#
# uint8_t: format-id
# uint8_t: layer-amount 
# uint8_t: width
# uint8_t: height:
# uint8_t: flags
# uint16_t layer-offsets (according to layer-amount), relative to beginning of file
# 
# LAYERDATA:
# uint8_t: layer-id
# uint8_t: flags
# uint8_t[]: width*height tile-indices
# uint8_t: 95 (checksum)

class Tilemap:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.flags = 0
        self.layers = []

    def add_layer(self, layer):
        self.layers.append(layer)

    def remove_layer(self, layer):
        self.layers.remove(layer)

class Layer:
    def __init__(self, tileset_id, width, height, offsetx, offsety, name):
        self.tileset_id = tileset_id
        self.width = width
        self.height = height
        self.offsetx = offsetx
        self.offsety = offsety
        self.tiles = [255] * (width*height)
        self.name = name
    
    def set_tile(self, x, y, tile):
        index = y * self.width + x
        self.tiles[index] = tile
