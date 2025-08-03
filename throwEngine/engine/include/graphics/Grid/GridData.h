#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace GRID
{

	struct GridVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};


    class GridData
    {
    public:
        GridData();
        ~GridData();

        std::vector<GridVertex> getGridVerticesVec() const { return m_gridVerticesVec; };
        uint32_t getVAO() const { return m_GridVAO; };
        uint32_t getVBO() const { return m_GridVBO; };
        float getTileSize() const { return m_tileSize; };
        int getNumRows() const { return m_numRows; };
        int getNumCols() const { return m_numCols; };
        float getCamTileRange() const { return m_cameraTileRange; };

        // New getters for added properties
        glm::vec3 getMajorGridColor() const { return m_majorGridColor; };
        glm::vec3 getCenterColor() const { return m_centerColor; };
        int getMajorLineInterval() const { return m_majorLineInterval; };

        // Setters for configuration
        void setGridColor(const glm::vec3& color) {
            m_gridColor = color;
            SetUpResources(); // Regenerate grid when colors change
        }
        void setMajorGridColor(const glm::vec3& color) {
            m_majorGridColor = color;
            SetUpResources();
        }
        void setCenterColor(const glm::vec3& color) {
            m_centerColor = color;
            SetUpResources();
        }
        void setTileSize(float size) {
            m_tileSize = size;
            SetUpResources();
        }
        void setGridDimensions(int rows, int cols) {
            m_numRows = rows;
            m_numCols = cols;
            SetUpResources();
        }

    private:
        std::vector<GridVertex> m_gridVerticesVec;
        uint32_t m_GridVAO = 0;
        uint32_t m_GridVBO = 0;

        // Grid configuration
        float m_tileSize = 1.0f;
        float m_cameraTileRange = 50.0f;
        int m_numRows = 300;
        int m_numCols = 300;
        int m_majorLineInterval = 10; // Every 10th line is major

        // Color settings
        glm::vec3 m_gridColor = { 0.7f, 0.7f, 0.7f };       // Normal grid color
        glm::vec3 m_majorGridColor = { 1.0f, 1.0f, 1.0f };  // Major lines color
        glm::vec3 m_centerColor = { 0.9f, 0.3f, 0.3f };     // Center axis color

        void SetUpResources();
    };
}