#include "gui/gui_helper.h"

#include <SDL2/SDL_ttf.h>

#include <game/game.h>

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

#ifdef _WIN32
    // set windows ime
    hwnd = GetForegroundWindow();
    himc = ImmAssociateContext(hwnd, NULL);
#endif

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
        case SDL_TEXTEDITING: mu_input_edit(ctx, e.edit.text); break;
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

void GUIHelper::setCurrentFont(TTF_Font *font){
    ctx->style->font = font;
}

mu_Context* GUIHelper::getContext(){
    return GUIHelper::ctx;
}

void drawText(SDL_Renderer *renderer, mu_Font font, char *str, mu_Vec2 pos, mu_Color color){
    
    TTF_Font *f = (TTF_Font*)(void*)font;
    SDL_Color c = *((SDL_Color*)(void*)&color);
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(f, str, c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int w, h;
    auto q = SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect renderRect = {pos.x, pos.y - 2, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &renderRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(texture);
}

void drawRect(SDL_Renderer *renderer, mu_Rect *rect, mu_Color &color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect *r = (SDL_Rect *)(void*)rect;
    SDL_RenderFillRect(renderer, r);
}

void setClipRect(SDL_Renderer *renderer, mu_Rect &rect){
    SDL_Rect r = {rect.x, Game::getHeight() - (rect.y + rect.h), rect.w, rect.h};
    SDL_RenderSetClipRect(renderer, &r);
}

void drawImage(SDL_Renderer *renderer, void* texture, mu_Rect *rect){
    SDL_Rect renderRect = {rect->x, rect->y, rect->w, rect->h};
    SDL_RenderCopy(renderer, (SDL_Texture *)texture, NULL, &renderRect);
}

void GUIHelper::handleRender(SDL_Renderer *renderer){
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
        case MU_COMMAND_TEXT:
            drawText(renderer, cmd->text.font, cmd->text.str, cmd->text.pos, cmd->text.color); 
            break;
        case MU_COMMAND_RECT:
            drawRect(renderer,&cmd->rect.rect, cmd->rect.color); 
            break;
        case MU_COMMAND_IMAGE:
            drawImage(renderer, cmd->image.texture, &cmd->image.rect);
            break;
        //case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
        case MU_COMMAND_CLIP: 
            setClipRect(renderer, cmd->clip.rect); 
            break;
      }
    }
}

void GUIHelper::begin(){
    mu_begin(ctx);
}

void GUIHelper::end(){
    mu_end(ctx);
}

bool GUIHelper::beginWindow(std::string name, SDL_Rect &rect, int opt){
    return mu_begin_window_ex(ctx, name.c_str(),
     mu_rect(rect.x, rect.y, rect.w, rect.h), opt);
}

void GUIHelper::endWindow(){
    mu_end_window(ctx);
}

bool GUIHelper::button(std::string name, int opt){
    return mu_button_ex(ctx, name.c_str(), NULL, opt);
}

bool GUIHelper::image(SDL_Texture *texture, int w, int h){
    mu_image(ctx, texture, w, h);
}

bool GUIHelper::textbox(char *buf, size_t len){
    mu_Id id = mu_get_id(ctx, &buf, sizeof(buf));
    mu_Rect r = mu_layout_next(ctx);
    int res = mu_textbox_raw(ctx, buf, len, id, r, 0);
#ifdef _WIN32
    mu_update_control(ctx, id, r, MU_OPT_HOLDFOCUS);
    SDL_Rect rect = {
        .x = r.x, .y = r.y + r.h,
        .w = r.w, .h = r.h};
    if (ctx->focus == id) {
        SDL_StartTextInput();
        SDL_SetTextInputRect(&rect);
        ImmAssociateContext(hwnd, himc);
    }else{
        SDL_StopTextInput();
        ImmAssociateContext(hwnd, NULL);
    }
#endif
    return res;
}
