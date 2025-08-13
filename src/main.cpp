#include <SDL.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture {
    GLuint id = 0;
    int w = 0;
    int h = 0;
};

Texture loadTexture(const std::string& path) {
    Texture tex;
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &tex.w, &tex.h, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return tex;
    }
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

Texture renderText(const std::string& msg, TTF_Font* font, SDL_Color color) {
    Texture tex;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, msg.c_str(), color);
    if (!surf) return tex;
    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    tex.w = conv->w; tex.h = conv->h;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, conv->w, conv->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, conv->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(conv);
    SDL_FreeSurface(surf);
    return tex;
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF init failed: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_Window* window = SDL_CreateWindow("Space Combat Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();
    glViewport(0, 0, 800, 600);
    glEnable(GL_TEXTURE_2D);

    Texture background = loadTexture("assets/menu-bg.png");
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
    if (!font) {
        std::cerr << "Failed to load font" << std::endl;
    }

    std::vector<std::string> items{"New Game", "Load Game", "Options", "Quit"};
    int selected = 0;
    bool running = true;
    enum GameState { MENU, GAME } state = MENU;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN) {
                if (state == MENU) {
                    if (e.key.keysym.sym == SDLK_UP) {
                        selected = (selected + items.size() - 1) % items.size();
                    } else if (e.key.keysym.sym == SDLK_DOWN) {
                        selected = (selected + 1) % items.size();
                    } else if (e.key.keysym.sym == SDLK_RETURN) {
                        if (selected == 0) {
                            state = GAME;
                        } else if (selected == 3) {
                            running = false;
                        }
                    }
                } else if (state == GAME) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        state = MENU;
                    }
                }
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (state == MENU) {
            // Draw background
            glBindTexture(GL_TEXTURE_2D, background.id);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(-1, 1);
            glTexCoord2f(1, 0); glVertex2f(1, 1);
            glTexCoord2f(1, 1); glVertex2f(1, -1);
            glTexCoord2f(0, 1); glVertex2f(-1, -1);
            glEnd();

            // Draw menu items
            for (size_t i = 0; i < items.size(); ++i) {
                SDL_Color col = {255, 255, 255, 255};
                if (i == selected) col = {255, 215, 0, 255};
                Texture text = renderText(items[i], font, col);
                float x = -0.2f;
                float y = 0.4f - static_cast<float>(i) * 0.2f;
                float w = static_cast<float>(text.w) / 800.0f * 2.0f;
                float h = static_cast<float>(text.h) / 600.0f * 2.0f;
                glBindTexture(GL_TEXTURE_2D, text.id);
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex2f(x, y);
                glTexCoord2f(1, 0); glVertex2f(x + w, y);
                glTexCoord2f(1, 1); glVertex2f(x + w, y - h);
                glTexCoord2f(0, 1); glVertex2f(x, y - h);
                glEnd();
                glDeleteTextures(1, &text.id);
            }
        } else if (state == GAME) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60.0, 800.0/600.0, 0.1, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRotatef(SDL_GetTicks() * 0.05f, 0.0f, 1.0f, 0.0f);
            glBegin(GL_TRIANGLES);
            glColor3f(0.8f, 0.8f, 0.2f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glEnd();
            glColor3f(1.0f, 1.0f, 1.0f);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        SDL_GL_SwapWindow(window);
    }

    TTF_CloseFont(font);
    glDeleteTextures(1, &background.id);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
