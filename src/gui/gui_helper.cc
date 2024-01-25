#include "gui/gui_helper.h"


char buttonMap[256] = {};
char keyMap[256] = {};


int getTextWidth(mu_Font font, const char *str, int len){
    return 32;
}

int getTextHeight(mu_Font font){
    return 18;
}


void GUIHelper::init(){
    // init gui context
    GUIHelper::ctx = new mu_Context();
    mu_init(ctx);
    ctx->text_width = getTextWidth;
    ctx->text_height = getTextHeight;

    // set button map
    buttonMap[SDL_BUTTON_LEFT & 0xff] = MU_MOUSE_LEFT;
    buttonMap[SDL_BUTTON_RIGHT & 0xff] = MU_MOUSE_RIGHT;
    buttonMap[SDL_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE;

    //set ket map
    keyMap[SDLK_LSHIFT    & 0xff ] = MU_KEY_SHIFT;
    keyMap[SDLK_RSHIFT    & 0xff ] = MU_KEY_SHIFT;
    keyMap[SDLK_LCTRL     & 0xff ] = MU_KEY_CTRL;
    keyMap[SDLK_RCTRL     & 0xff ] = MU_KEY_CTRL;
    keyMap[SDLK_LALT      & 0xff ] = MU_KEY_ALT;
    keyMap[SDLK_RALT      & 0xff ] = MU_KEY_ALT;
    keyMap[SDLK_RETURN    & 0xff ] = MU_KEY_RETURN;
    keyMap[SDLK_BACKSPACE & 0xff ] = MU_KEY_BACKSPACE;
}

void GUIHelper::handleInput(SDL_Event &e){
    switch (e.type){
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int b = buttonMap[e.button.button & 0xff];
            if (b && e.type == SDL_MOUSEBUTTONDOWN) { 
                mu_input_mousedown(ctx, e.button.x, e.button.y, b); 
            }
            if (b && e.type == SDL_MOUSEBUTTONUP) {
                 mu_input_mouseup(ctx, e.button.x, e.button.y, b);   
            }
            break;
        }
        case SDL_MOUSEMOTION: mu_input_mousemove(ctx, e.motion.x, e.motion.y); break;
        case SDL_MOUSEWHEEL: mu_input_scroll(ctx, 0, e.wheel.y * -30); break;
        case SDL_TEXTINPUT: mu_input_text(ctx, e.text.text); break;
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
          int c = keyMap[e.key.keysym.sym & 0xff];
          if (c && e.type == SDL_KEYDOWN) { mu_input_keydown(ctx, c); }
          if (c && e.type ==   SDL_KEYUP) { mu_input_keyup(ctx, c);   }
          break;
        }
        default:
            break;
    }
}

void drawRect(SDL_Renderer *renderer, mu_Rect *rect, mu_Color &color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect *r = (SDL_Rect *)(void*)rect;
    SDL_RenderFillRect(renderer, r);
}

void GUIHelper::handleRender(SDL_Renderer *renderer){
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
        //case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
        case MU_COMMAND_RECT: 
            drawRect(renderer,&cmd->rect.rect, cmd->rect.color); 
            break;
        //case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
        //case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
      }
    }
}

void GUIHelper::begin(){
    mu_begin(ctx);
}

void GUIHelper::end(){
    mu_end(ctx);
}

bool GUIHelper::beginWindow(std::string name, SDL_Rect &rect){
    return mu_begin_window(ctx, name.c_str(), mu_rect(rect.w, rect.y, rect.w, rect.h));
}

void GUIHelper::endWindow(){
    mu_end_window(ctx);
}

bool GUIHelper::button(std::string name){
    return mu_button(ctx, name.c_str());
}

