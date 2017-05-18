#include "BoxLoader.h"


BoxLoader::BoxLoader()
{
    // This creates our identifier and puts it in vbo
    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);


    for (uint i = 0; i < NUM_PTS; i++)
    {
        colors[i * 3] = 1;
        colors[i * 3 + 1] = 0;
        colors[i * 3 + 2] = 0;
    }

    LoadDataToGraphicsCard();
}

BoxLoader::~BoxLoader()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}


/**
\brief Singleton constructor.

Create a singleton object of DataLoader.

*/

BoxLoader* BoxLoader::instance() {
    return new BoxLoader();

    // TODO: FIX. something goes wrong with singleton
    // if (!s_instance)
    // {
    //     s_instance = new DataLoader();
    // }
    // return s_instance;
}

void BoxLoader::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);\
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void BoxLoader::draw(GLuint PVMLoc, glm::mat4 projection, glm::mat4 view)
{
    glLineWidth(5);
    // printf("%d\n", boxes.size());
    for (int i = 0; i < boxes.size(); i++)
    {
        glm::mat4 boxModel = boxes[i].getModelMatrix();
        glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*boxModel));

        // Draw box
        glBindVertexArray(vboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    }
}

void BoxLoader::loadBoxes(std::vector<BoundingBox> v) {
    boxes = v;
}