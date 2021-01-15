/*
 * rendering_system.h
 *
 *  Created on: Nov 11, 2019
 *      Author: zamfi
 */

#ifndef SYSTEMS_RENDERING_SYSTEM_H_
#define SYSTEMS_RENDERING_SYSTEM_H_

#include "../types.h"
#include <set>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "../components/visuals.h"
#include "../components/collision.h"
#include "../components/movement.h"
#include "../components/position.h"
#include "../components/control.h"
#include "../components/health.h"
#include "../math_ext.h"
#include "../components/absolute_position.h"
#include "resource_system.h"
#include "../globals.h"

class RenderingSystem
{
public:
	RenderingSystem(SDL_Renderer* renderer) : m_renderer(renderer), m_resolution_x{800}, m_resolution_y{600} {}
	RenderingSystem() : RenderingSystem(nullptr) {}

	void add_id(const EntityID entity, const Visuals::VisualLayer layer)
	{
		entities[int(layer)].insert(entity);
		entity_layer[entity] = layer;
	}

	void remove_id(const EntityID entity, const Visuals::VisualLayer layer)
	{
		entities[int(layer)].erase(entity);
		entity_layer.erase(entity);
	}

	void clear()
    {
    	for(int layer = 0; layer < Visuals::NUM_OF_LAYERS; ++layer)
    		entities[layer].clear();
    }

    void component_updated(const Visuals& visuals, const EntityID id, const int8_t change)
    {
    	//1 = find=end
    	//0 = find!=end layer=layer
    	//-1 = find!=end layer!=layer
    	const int8_t layer_change = entity_layer.find(id) == cend(entity_layer) ? 1 : -(visuals.layer() != entity_layer[id]);

    	if(change < 0 || layer_change < 0)
    		remove_id(id, entity_layer[id]);
    	if(change > 0 || layer_change != 0)
    		add_id(id, visuals.layer());
    }

    template<typename EntitySystemT>
    void render_entities(const Time time_diff, EntitySystemT& entity_system, ResourceSystem& resource_system, Globals& globals)
    {
        //SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);

        const auto& screen_zone_position = entity_system.entity_component(EntityID{0}, (Position*)nullptr);

        const double m_screen_to_view_scale = screen_zone_position.h() ? 1.0*m_resolution_y/screen_zone_position.h() : 1.0;

        for(auto layer = 0; layer < Visuals::NUM_OF_LAYERS; ++layer)
        {
    		for(const EntityID id : entities[layer])
    		{
    			auto& visuals = entity_system.entity_component(id, (Visuals*)nullptr);
    			if(visuals)
    			{
    				const auto& position = entity_system.entity_component(id, (Position*)nullptr);
    				const auto& control = entity_system.entity_component(id, (Control*)nullptr);

    				if(globals.app_paused == false)
    					visuals.update_animation(time_diff);

    				const optional_ref<Spritesheet> spritesheet = resource_system.spritesheet(visuals.spritesheet_id());
    				if(spritesheet)
    				{
    					double scale_factor = spritesheet->scale_factor();
    					const SDL_RendererFlip flip = (control.look_dir()==Control::LookDir::LEFT) ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;
    					AbsolutePosition screen_pos;
    					SDL_Rect dest;
    					const double pos_x = position.x() + position.w()/visuals.repeat_x()/2.0 - screen_zone_position.x();
    					const double pos_y = position.y() + position.h()/visuals.repeat_y()/2.0 - screen_zone_position.y();

    					for(int16_t rx = 0; rx < visuals.repeat_x(); ++rx)
    						for(int16_t ry = 0; ry < visuals.repeat_y(); ++ry)
    						{
    							const optional_ref<const Sprite> sprite = spritesheet->sprite(visuals.animation_sprite(rx, ry));
    							if(sprite)
    							{
    								if(resource_system.texture(sprite->texture_id))
    								{
    									//TODO: to optimize and add constness
    									SDL_Texture* texture = resource_system.texture(sprite->texture_id)->texture();
    									screen_pos.set_w(sprite->clip.w*scale_factor);
    									screen_pos.set_h(sprite->clip.h*scale_factor);
    									screen_pos.set_x(pos_x + rx*screen_pos.w() - screen_pos.w()/2.0);
    									screen_pos.set_y(pos_y + ry*screen_pos.h() - screen_pos.h()/2.0);
    									dest.w = screen_pos.w()*m_screen_to_view_scale + 0.5;
    									dest.h = screen_pos.h()*m_screen_to_view_scale + 0.5;
    									dest.x = screen_pos.x()*m_screen_to_view_scale + 0.5;
    									dest.y = m_resolution_y - dest.h - screen_pos.y()*m_screen_to_view_scale + 0.5;

    									if(objects_collide(dest.x, dest.y, dest.w, dest.h, 0, 0, m_resolution_x, m_resolution_y))
    									{
    										const int err = SDL_RenderCopyEx(m_renderer, texture, &sprite->clip, &dest, 0, nullptr, flip);
    										if(err)
    										{
    											std::cerr << "Error when rendering: " << SDL_GetError() << std::endl;
    										}
    									}
    								}
    								else
    								{
    									//error sprite->texture_id
    								}

    							}
    							else
    							{
    								//error visuals->animation_sprite(rx, ry)
    							}
    						}
    					if(globals.show_hitboxes)
    					{
    						const SDL_Rect hitbox
    							{ int((position.x() - screen_zone_position.x())*m_screen_to_view_scale)
    							, int(m_resolution_y + (-position.h() - position.y() + screen_zone_position.y())*m_screen_to_view_scale)
    							, int(position.w()*m_screen_to_view_scale)
    							, int(position.h()*m_screen_to_view_scale)
    							};
    						SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    						SDL_RenderDrawRect(m_renderer, &hitbox);
    					}
    				}
    				else
    				{
    					//error visuals->spritesheet_id()
    				}
    			}
    			else
    			{
    				//error *it
    			}
        	}
        }

        SDL_RenderPresent(m_renderer);

    }

    void set_renderer(SDL_Renderer* renderer) { m_renderer = renderer; }
    SDL_Renderer* renderer() { return m_renderer; }
    void set_resolution_x(const int res_x) { m_resolution_x = res_x; }
    void set_resolution_y(const int res_y) { m_resolution_y = res_y; }

protected:
    SDL_Renderer* m_renderer;
    int m_resolution_x, m_resolution_y;
    std::set<EntityID> entities[Visuals::NUM_OF_LAYERS];
    std::unordered_map<EntityID, Visuals::VisualLayer> entity_layer;
};

#endif /* SYSTEMS_RENDERING_SYSTEM_H_ */
