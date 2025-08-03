#include "graphics/Grid/GridData.h"

#include "graphics/Camera/Camera.h"

namespace GRID
{
	GridData::GridData()
	{
		SetUpResources();
	}

    GridData::~GridData()
    {
        if (m_GridVAO) glDeleteVertexArrays(1, &m_GridVAO);
        if (m_GridVBO) glDeleteBuffers(1, &m_GridVBO);
    }

    void GridData::SetUpResources()
    {
        // Pseudocode for grid generation
        // for each row :
        // z = row * tileSize
        //     Add line from(0, 0, z) to(gridWidth, 0, z) // Horizontal line

        // for each column :
        // x = column * tileSize
        //     Add line from(x, 0, 0) to(x, 0, gridDepth) // Vertical line

        m_gridVerticesVec.clear();

        // Alpha values for different line types
        const float minorLineAlpha = 0.3f;
        const float majorLineAlpha = 0.6f;
        const float centerLineAlpha = 0.8f;

        // Apply alpha to colors
        glm::vec3 minorColor = m_gridColor * minorLineAlpha;
        glm::vec3 majorColor = m_majorGridColor * majorLineAlpha;
        glm::vec3 centerColor = m_centerColor * centerLineAlpha;

        float totalWidth = m_numCols * m_tileSize;
        float totalDepth = m_numRows * m_tileSize;

        // Generate horizontal lines (along X axis)
        for (int row = 0; row <= m_numRows; row++) {
            float z = row * m_tileSize - totalDepth / 2; // Center grid around origin
            bool isMajor = (row % m_majorLineInterval == 0);
            glm::vec3 color = isMajor ? majorColor : minorColor;

            // Add slight elevation for major lines
            float yOffset = isMajor ? 0.001f : 0.0f;

            m_gridVerticesVec.push_back({ {-totalWidth / 2, yOffset, z}, color });
            m_gridVerticesVec.push_back({ {totalWidth / 2, yOffset, z}, color });
        }

        // Generate vertical lines (along Z axis)
        for (int col = 0; col <= m_numCols; col++) {
            float x = col * m_tileSize - totalWidth / 2; // Center grid around origin
            bool isMajor = (col % m_majorLineInterval == 0);
            glm::vec3 color = isMajor ? majorColor : minorColor;

            float yOffset = isMajor ? 0.001f : 0.0f;

            m_gridVerticesVec.push_back({ {x, yOffset, -totalDepth / 2}, color });
            m_gridVerticesVec.push_back({ {x, yOffset, totalDepth / 2}, color });
        }

        // Center axis lines (highlighted)
        float extension = 0.05f; // 5% extension beyond grid
        m_gridVerticesVec.push_back({ {-totalWidth / 2 * (1 + extension), 0.002f, 0.0f}, centerColor });
        m_gridVerticesVec.push_back({ {totalWidth / 2 * (1 + extension), 0.002f, 0.0f}, centerColor });

        m_gridVerticesVec.push_back({ {0.0f, 0.002f, -totalDepth / 2 * (1 + extension)}, centerColor });
        m_gridVerticesVec.push_back({ {0.0f, 0.002f, totalDepth / 2 * (1 + extension)}, centerColor });

        // Set up OpenGL buffers
        if (m_GridVAO == 0) glGenVertexArrays(1, &m_GridVAO);
        if (m_GridVBO == 0) glGenBuffers(1, &m_GridVBO);

        glBindVertexArray(m_GridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);

        glBufferData(GL_ARRAY_BUFFER,
            m_gridVerticesVec.size() * sizeof(GridVertex),
            m_gridVerticesVec.data(),
            GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex),
            (void*)offsetof(GridVertex, position));
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex),
            (void*)offsetof(GridVertex, color));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

}
