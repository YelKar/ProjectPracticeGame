#include <unordered_map>
#include "../common/types.h"
#include "../common/Image.h"
#include "../common/PixelImage.cpp"

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Size {
    int width;
    int height;
};

enum TextureType {
    IMAGE,
    PIXEL_IMAGE
};

struct Texture {
    TextureType type;
    Image img;
    PixelImage pixelImage;
};

template <typename Type>
using ComponentMap = std::unordered_map<EntityID, Type>;

using Positions = ComponentMap<Position>;
using Velocities = ComponentMap<Velocity>;
using Sizes = ComponentMap<Size>;
using Textures = ComponentMap<Texture>;

struct Components {
    Positions positions;
    Velocities velocities;
    Sizes sizes;
    Textures textures;
};