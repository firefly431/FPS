#include "GLFWRenderer.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "OBJFile.h"

#include "Spear.h"
#include "AIController.h"

static const double RADS_PER_PX = -0.01;
const double GLFWRenderer::WALL_HEIGHT = 3.15;
const double GLFWRenderer::UV_HEIGHT = 0.38;
const double GLFWRenderer::FLOOR_SCALE = 0.1;

static void error_callback(int error, const char *description) {
    std::cerr << "GLFW error!" << std::endl;
    std::cerr << description << std::endl;
}

void resize_callback(GLFWwindow *window, int width, int height) {
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    glfwGetFramebufferSize(window, &width, &height);
    renderer->resize(width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_W:
        renderer->player->input.up = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_S:
        renderer->player->input.down = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_A:
        renderer->player->input.left = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_D:
        renderer->player->input.right = action != GLFW_RELEASE;
        break;
    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        renderer->player->input.fire = action != GLFW_RELEASE;
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    // doubles can hold up to around 9e15 exactly
    // but just to be safe let's clamp it at 1e5
    if (xpos > 1e5 || xpos < -1e5) {
        xpos = fmod(xpos, 1e10);
        glfwSetCursorPos(window, xpos, ypos);
    }
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    renderer->player->heading = fmod(xpos * RADS_PER_PX, 2 * M_PI);
}

GLFWRenderer::GLFWRenderer(unsigned int width, unsigned int height) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, "Spearthrowers", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // let callbacks access us
    glfwSetWindowUserPointer(window, (void *)this);
    glfwMakeContextCurrent(window);
    camera = std::unique_ptr<Camera>(new Camera());
    p_mesh = std::unique_ptr<PlayerMesh>(new PlayerMesh(
        ShaderProgram(
            VertexShader("dynamic.vert", 0),
            FragmentShader("basic.frag", 0)
        ),
        OBJFile("teapotSmooth.obj").result(),
        Texture("default.png")
    ));
    sp_mesh = std::unique_ptr<SpearMesh>(new SpearMesh(
        ShaderProgram(
            VertexShader("dynamic.vert", 0),
            FragmentShader("basic.frag", 0)
        ),
        OBJFile("spear.obj").result(),
        Texture("spear.jpg")
    ));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, (int *)&width, (int *)&height);
    camera->updateView(0, 0, 0, 1, 0, 0, 0, 0, Player::EYE_LEVEL);
    resize(width, height);
    glfwSetWindowSizeCallback(window, resize_callback);
    // print context version
    std::cout << "OpenGL version: ";
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;
    scene.loadGraph("faces.obj");
    generateSceneMesh();
    scene.addPlayer();
    scene.addPlayer();
    player = &scene.players[0];
    scene.players[1].setController(
        new AIController(*player, scene.graph, scene.walls));
}

void GLFWRenderer::mainloop() {
    while (!glfwWindowShouldClose(window)) {
        auto end = scene.players.end();
        for (auto it = scene.players.begin(); it != end; it++)
            it->move(scene.walls, scene.spears);
        auto send = scene.spears.end();
        for (auto it = scene.spears.begin(); it != send;)
            if (!it->move(scene.players, scene.walls))
                scene.spears.erase(it++);
            else
                it++;
        draw();
        glfwPollEvents();
    }
}

void GLFWRenderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the meshes
    camera->updateView(*player, Player::EYE_LEVEL);
    //camera->updateView(0, 0, 80, 0, 0, -1, 1, 0, 0);
    f_mesh->activate();
    f_mesh->updateVP(*camera);
    f_mesh->draw();
    s_mesh->activate();
    s_mesh->updateVP(*camera);
    s_mesh->draw();
    p_mesh->activate();
    p_mesh->updateVP(*camera);
    auto end = scene.players.end();
    for (auto it = scene.players.begin() + 1; it != end; ++it) {
        p_mesh->update(*it);
        p_mesh->draw();
    }
    sp_mesh->activate();
    sp_mesh->updateVP(*camera);
    auto send = scene.spears.end();
    for (auto it = scene.spears.begin(); it != send; ++it) {
        sp_mesh->update(*it);
        sp_mesh->draw();
    }
    Mesh::deactivate();
    glfwSwapBuffers(window);
}

void GLFWRenderer::resize(unsigned int width, unsigned int height) {
    camera->updateProj(45, (double)width / height, 0.5, 100);
    glViewport(0, 0, width, height);
}

GLFWRenderer::~GLFWRenderer() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void GLFWRenderer::generateSceneMesh() {
    std::cout << scene.islands.size() << " island(s)" << std::endl;
    std::vector<GLfloat> vertices, normals, texcoords;
    double minx = std::numeric_limits<double>::infinity(),
           maxx = -std::numeric_limits<double>::infinity(),
           miny = minx, maxy = maxx;
#if defined(_MSC_VER) && _MSC_VER < 1800
    auto i_it = scene.islands.end();
    for (auto itt = scene.islands.begin(); itt != i_it; itt++) {
        auto &island = *itt;
#if 0
    }
#endif
#else
    for (auto &island : scene.islands) {
#endif
        double u = 0;
        auto e_it = island.end();
        auto it = island.begin();
        auto prev = *(it++);
        for (vector pt; it != e_it; prev = *(it++)) {
            pt = *it;
            if (pt.x < minx) minx = pt.x;
            if (pt.x > maxx) maxx = pt.x;
            if (pt.y < miny) miny = pt.y;
            if (pt.y > maxy) maxy = pt.y;
            vertices.push_back(prev.x);
            vertices.push_back(prev.y);
            vertices.push_back(0);
            vertices.push_back(prev.x);
            vertices.push_back(prev.y);
            vertices.push_back(WALL_HEIGHT);
            vertices.push_back(pt.x);
            vertices.push_back(pt.y);
            vertices.push_back(WALL_HEIGHT);
            vertices.push_back(pt.x);
            vertices.push_back(pt.y);
            vertices.push_back(WALL_HEIGHT);
            vertices.push_back(pt.x);
            vertices.push_back(pt.y);
            vertices.push_back(0);
            vertices.push_back(prev.x);
            vertices.push_back(prev.y);
            vertices.push_back(0);
            vector lv = (pt - prev);
            vector lvn = lv.normalized();
            double ll = (double)lv;
            for (int _ = 0; _ < 6; _++) {
                normals.push_back(-lvn.y);
                normals.push_back(lvn.x);
                normals.push_back(0);
            }
            double nu = u + (UV_HEIGHT / WALL_HEIGHT) * ll;
            texcoords.push_back(u);
            texcoords.push_back(0);
            texcoords.push_back(u);
            texcoords.push_back(UV_HEIGHT);
            texcoords.push_back(nu);
            texcoords.push_back(UV_HEIGHT);
            texcoords.push_back(nu);
            texcoords.push_back(UV_HEIGHT);
            texcoords.push_back(nu);
            texcoords.push_back(0);
            texcoords.push_back(u);
            texcoords.push_back(0);
            u = nu;
        }
    }
    VertexArray &va = *new VertexArray(vertices.size() / 3, GL_TRIANGLES);
    va.activate();
    va.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW,
                     &vertices[0]), ATTRIBUTE_POSITION, 3, GL_FLOAT, 0);
    va.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * texcoords.size(), GL_STATIC_DRAW,
                     &texcoords[0]), ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0);
    va.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * normals.size(), GL_STATIC_DRAW,
                     &normals[0]), ATTRIBUTE_NORMAL, 3, GL_FLOAT, 0);
    va.deactivate();
    s_mesh.reset(new Mesh(
        ShaderProgram(
            VertexShader("static.vert", 0),
            FragmentShader("basic.frag", 0)
        ),
        std::move(va),
        Texture("stone.jpg")
    ));
    // floor
    vertices.clear();
    normals.clear();
    texcoords.clear();
    vertices.push_back(minx);
    vertices.push_back(miny);
    vertices.push_back(0);
    vertices.push_back(maxx);
    vertices.push_back(miny);
    vertices.push_back(0);
    vertices.push_back(maxx);
    vertices.push_back(maxy);
    vertices.push_back(0);
    vertices.push_back(minx);
    vertices.push_back(maxy);
    vertices.push_back(0);
    normals.push_back(0); normals.push_back(0); normals.push_back(1);
    normals.push_back(0); normals.push_back(0); normals.push_back(1);
    normals.push_back(0); normals.push_back(0); normals.push_back(1);
    normals.push_back(0); normals.push_back(0); normals.push_back(1);
    texcoords.push_back(minx * FLOOR_SCALE);
    texcoords.push_back(miny * FLOOR_SCALE);
    texcoords.push_back(maxx * FLOOR_SCALE);
    texcoords.push_back(miny * FLOOR_SCALE);
    texcoords.push_back(maxx * FLOOR_SCALE);
    texcoords.push_back(maxy * FLOOR_SCALE);
    texcoords.push_back(minx * FLOOR_SCALE);
    texcoords.push_back(maxy * FLOOR_SCALE);
    VertexArray &fva = *new VertexArray(4, GL_TRIANGLE_FAN);
    fva.activate();
    fva.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW,
                     &vertices[0]), ATTRIBUTE_POSITION, 3, GL_FLOAT, 0);
    fva.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * texcoords.size(), GL_STATIC_DRAW,
                     &texcoords[0]), ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0);
    fva.bindBuffer(
        VertexBuffer(sizeof(GLfloat) * normals.size(), GL_STATIC_DRAW,
                     &normals[0]), ATTRIBUTE_NORMAL, 3, GL_FLOAT, 0);
    fva.deactivate();
    f_mesh.reset(new Mesh(
        ShaderProgram(
            VertexShader("static.vert", 0),
            FragmentShader("basic.frag", 0)
        ),
        std::move(fva),
        Texture("floor.jpg")
    ));
}
