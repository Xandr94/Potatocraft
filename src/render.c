#include <gl/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "game.h"
#include "camera.h"

//Вершины куба
//8 вершин куба --1--  --2--  --3--  --4--    --5--  --6--  --7--  --8--
float kube[] = {0,0,0, 0,1,0, 1,1,0, 1,0,0,   0,0,1, 0,1,1, 1,1,1, 1,0,1};
//1 грань куба из 2-х треугольников, т.е. всего 12 треугольников
//В kubeInd перечислены индексы вершин, которые образуют треугольники из массива kube[]
//                 Треуг.1, Треуг.2
GLuint kubeInd[] = {0,1,2,  2,3,0, //Грань 1
                    4,5,6,  6,7,4, //Грань 2
                    3,2,5,  6,7,3, //Грань 3
                    0,1,5,  5,4,0, //Грань 4
                    1,2,6,  6,5,1, //Грань 5
                    0,3,7,  7,4,0};//Грань 6

float verts[] = {0,0,0, 1,0,0, 1,1,0, 0,1,0,
                 0,0,1, 1,0,1, 1,1,1, 0,1,1};

float axesVerts[] = {0,0,0, 1,0,0,
                     0,0,0, 0,1,0,
                     0,0,0, 0,0,1};

//GLuint AxesInd[] = {0, 1, 2, 3, 4, 5};
float colors[] = {1,0,0, 1,0,0,
                  0,1,0, 0,1,0,
                  0,0,1, 0,0,1};

void DrawAxes()
{
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glLineWidth(3.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, axesVerts);
        glColorPointer(3, GL_FLOAT, 0, colors);
        //glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, kubeInd);
        glDrawArrays(GL_LINES, 0, 6);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
}

void DrawLocalAxes()
{
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
        glTranslatef(0,0,-1);
        glRotatef(-camera.Xrot, 1,0,0);
        glRotatef(-camera.Zrot, 0,0,1);
        glScalef(0.1,0.1,0.1);
        DrawAxes();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void Camera_Apply()
{
    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    glTranslatef(-camera.x,-camera.y,-camera.z);
}

void Enemy_Show()
{
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, kube);
        for (int i = 0; i < enemyCnt; i++)
        {
            if (!enemy[i].active) continue;
            glPushMatrix();
                glTranslatef(enemy[i].x, enemy[i].y, enemy[i].z);
                glColor3ub(244, 60, 43);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, kubeInd);
            glPopMatrix();
        }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void World_Show()
{
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, kube);
        for (int i = 0; i < pw; i++)
            for (int j = 0; j < ph; j++)
            {
                glPushMatrix();
                    glTranslatef(i-20,j-20,0);
                    glColor3f(map[i][j].clr.r,map[i][j].clr.g,map[i][j].clr.b);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, kubeInd);
                glPopMatrix();
            }
    glDisableClientState(GL_VERTEX_ARRAY);
}

//-----------------------------------------------------------------------

#define TEX_FOLDER "assets/"
#define BLOCK_COBBLESTONE TEX_FOLDER"cobblestone.png"
#define BLOCK_GLASS TEX_FOLDER"glass.png"
#define GRASS_SIDE TEX_FOLDER"grass_side.png"

unsigned int texture;

static GLuint blockDisplayList = 0;

void InitTexture()
{
    int width, height, cnt;
    unsigned char *data = stbi_load(GRASS_SIDE, &width, &height, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Render_Init()
{
    InitTexture();

    // Инициализация OpenGL 1.1
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    // Создание display list для блока
    blockDisplayList = glGenLists(1);
    glNewList(blockDisplayList, GL_COMPILE);


    // Вершины блока (24 значения - 6 граней * 4 вершины)
    GLfloat vertices[] = {
        // Front (Z+)
        1,1,1, 0,1,1, 0,0,1, 1,0,1,
        // Back (Z-)
        0,1,0, 1,1,0, 1,0,0, 0,0,0,
        // Left (X-)
        0,1,1, 0,1,0, 0,0,0, 0,0,1,
        // Right (X+)
        1,1,0, 1,1,1, 1,0,1, 1,0,0,
        // Top (Y+)
        0,1,0, 1,1,0, 1,1,1, 0,1,1,
        // Bottom (Y-)
        0,0,1, 1,0,1, 1,0,0, 0,0,0
    };

    // Текстурные координаты (24 значения)
    GLfloat texCoords[] = {
        // Front
        0,0, 1,0, 1,1, 0,1,
        // Back (��������� �� X)
        1,0, 0,0, 0,1, 1,1,
        // Left
        1,0, 0,0, 0,1, 1,1,
        // Right
        0,0, 1,0, 1,1, 0,1,
        // Top
        0,1, 1,1, 1,0, 0,0,
        // Bottom
        0,0, 1,0, 1,1, 0,1
    };

// Индексы (24 значения)
GLubyte indices[] = {
    0,1,2,3,     // Front
    4,5,6,7,     // Back
    8,9,10,11,   // Left
    12,13,14,15, // Right
    19,18,17,16, // Top
    23,22,21,20  // Bottom
};

    glBindTexture(GL_TEXTURE_2D, texture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glEndList();

}



float x = 0.0f;
float y = 2.0f;
float z = 0.0f;

void Game_Show()
{
    glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        Camera_Apply();
        DrawAxes();
        glFrontFace(GL_CW);
        World_Show();
        //Enemy_Show();
        glFrontFace(GL_CCW);
        glPushMatrix();
            glColor3f(1,1,1);
            glTranslatef(x, y, z); // Z - вверх в OpenGL
            glCallList(blockDisplayList);
        glPopMatrix();

    glPopMatrix();

    DrawLocalAxes();
}

void FreeRenderer() {
    glDeleteLists(blockDisplayList, 1);
}


void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
