/*
 * texture.h
 *
 *  Created on: Nov 11, 2019
 *      Author: zamfi
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include "font.h"

class Texture
{
public:
    Texture() : m_texture(nullptr) {}

    ~Texture()
    {
        unload();
    }

    Texture(const Texture&) = delete;
    Texture(Texture&& rhs) : m_texture(rhs.m_texture)
    {
    	rhs.m_texture = nullptr;
    }

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& rhs) noexcept
    {
    	unload();
    	m_texture = rhs.m_texture;
    	rhs.m_texture = nullptr;

    	return *this;
    }

    Texture& load_from_file(const std::string& file, SDL_Renderer* renderer)
    {
        unload();
        SDL_Surface* surface = IMG_Load(file.c_str());

        if(surface == nullptr)
        {
            std::cerr << "Cannot load image " << file << "! Error: " << IMG_GetError() << std::endl;
        }
        else
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            m_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if(m_texture == nullptr)
            {
                std::cerr << "Cannot create texture from " << file << "! Error: " << SDL_GetError() << std::endl;
            }
            SDL_FreeSurface(surface);
        }
        return *this;
    }

    Texture& load_from_string(const std::string& text, Font& font, uint8_t r, uint8_t g, uint8_t b, SDL_Renderer* renderer)
    {
        unload();
        SDL_Surface* surface = TTF_RenderText_Solid(font.font(), text.c_str(), {r, g, b});

        if(surface == nullptr)
        {
            std::cerr << "Cannot render string '" << text << "'! Error: " << TTF_GetError() << std::endl;
        }
        else
        {
            m_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if(m_texture == nullptr)
            {
                std::cerr << "Cannot create texture for string '" << text << "'! Error: " << SDL_GetError() << std::endl;
            }
            SDL_FreeSurface(surface);
        }
        return *this;
    }

    void unload() noexcept
    {
        if(m_texture != nullptr)
        {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
    }

    SDL_Texture* texture()
    {
        return m_texture;
    }

private:
    SDL_Texture* m_texture;
};

#endif /* TEXTURE_H_ */
