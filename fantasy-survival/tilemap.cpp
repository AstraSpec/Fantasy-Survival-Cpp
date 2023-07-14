#include <SFML/Graphics.hpp>

//sf::VertexArray vertices;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        m_vertices.clear();

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                sf::Vector2f tileSize_f = static_cast<sf::Vector2f>(tileSize);

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(j * tileSize_f.x, i * tileSize_f.y);
                triangles[1].position = sf::Vector2f((j + 1) * tileSize_f.x, i * tileSize_f.y);
                triangles[2].position = sf::Vector2f(j * tileSize_f.x, (i + 1) * tileSize_f.y);
                triangles[3].position = sf::Vector2f(j * tileSize_f.x, (i + 1) * tileSize_f.y);
                triangles[4].position = sf::Vector2f((j + 1) * tileSize_f.x, i * tileSize_f.y);
                triangles[5].position = sf::Vector2f((j + 1) * tileSize_f.x, (i + 1) * tileSize_f.y);

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize_f.x, tv * tileSize_f.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize_f.x, tv * tileSize_f.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize_f.x, (tv + 1) * tileSize_f.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize_f.x, (tv + 1) * tileSize_f.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize_f.x, tv * tileSize_f.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize_f.x, (tv + 1) * tileSize_f.y);
            }

        return true;
    }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};