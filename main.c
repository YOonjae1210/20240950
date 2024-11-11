#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

SDL_Texture* load_image(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("이미지 로드 실패: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("텍스처 생성 실패: %s\n", SDL_GetError());
    }

    return texture;
}

int main(int argc, char* argv[]) {
    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        return -1;
    }

    // SDL_image 초기화 (JPG, PNG 지원)
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // 창 생성
    SDL_Window* window = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("윈도우 생성 실패: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // 렌더러 생성
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("렌더러 생성 실패: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // JPG 이미지 로드
    SDL_Texture* jpg_texture = load_image(renderer, "jcshim.jpg");
    if (!jpg_texture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // PNG 이미지 로드
    SDL_Texture* png_texture = load_image(renderer, "jcshim.png");
    if (!png_texture) {
        SDL_DestroyTexture(jpg_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // JPG 이미지의 위치와 크기 설정
    SDL_Rect jpg_rect = { 0, 0, 400, 600 };

    // PNG 이미지의 위치와 크기 설정
    SDL_Rect png_rect = { 400, 0, 400, 600 };

    // 화면에 이미지 그리기
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, jpg_texture, NULL, &jpg_rect);
    SDL_RenderCopy(renderer, png_texture, NULL, &png_rect);
    SDL_RenderPresent(renderer);

    // 종료 대기
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // 리소스 정리
    SDL_DestroyTexture(jpg_texture);
    SDL_DestroyTexture(png_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
