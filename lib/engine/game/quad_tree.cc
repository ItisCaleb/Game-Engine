#include "engine/quad_tree.h"

#include "engine/game.h"

#include <stack>
#include <set>

QuadTree::QuadTree(int w, int h, int max_depth)
:boundary(w,h),max_depth(max_depth){
    QuadNode root;
    root.count = 0;
    root.next = -1;
    this->nodes.push_back(root);
}


void QuadTree::findNodes(CollideShape *shape, std::vector<QuadNodeData> &nodes){
    std::stack<QuadNodeData> st;
    QuadNodeData root = {this->boundary, 0, 0};
    st.push(root);
    while (!st.empty()){
        auto node_data = st.top();st.pop();
        auto& node = this->nodes[node_data.nodeIdx];
        if(node.count != -1){
            // if leaf
            nodes.push_back(node_data);
        }else{
            BoxCollideShape &bd = node_data.boundary;
            // else if branch
            float half_w = bd.w/2;
            float half_h = bd.h/2;
            // top left
            BoxCollideShape TL(bd.offx, bd.offy, half_w, half_h);
            // top right
            BoxCollideShape TR(bd.offx + half_w, bd.offy, half_w, half_h);
            // bottom left
            BoxCollideShape BL(bd.offx, bd.offy + half_h, half_w, half_h);
            // bottom right
            BoxCollideShape BR(bd.offx + half_w, bd.offy + half_h, half_w, half_h);
            int nx = node.next;
            if(TL.isCollide(shape)) st.push({TL, nx, node_data.depth+1});
            if(TR.isCollide(shape)) st.push({TR, nx+1, node_data.depth+1});
            if(BL.isCollide(shape)) st.push({BL, nx+2, node_data.depth+1});
            if(BR.isCollide(shape)) st.push({BR, nx+3, node_data.depth+1});
        }
    }
    
}

void QuadTree::appendToElements(QuadNodeData &data, CollideShape *shape){
    int idx = this->elements.insert({-1, shape});
    auto& node = this->nodes[data.nodeIdx];
    if(node.count == 0){
        node.next = idx;
        node.count++;
    }else{
        int i = node.next;
        for(;this->elements[i].next != -1; i = this->elements[i].next){}
        this->elements[i].next = idx;
        node.count++;
        if (node.count == 4){
            this->subDivide(data);
        }
    }


}

void QuadTree::subDivide(QuadNodeData &data){
    if(data.depth == this->max_depth) return;
    float half_w = data.boundary.w/2;
    float half_h = data.boundary.h/2;
    // top left
    BoxCollideShape TL(data.boundary.offx, data.boundary.offy, half_w,half_h);
    // top right
    BoxCollideShape TR(data.boundary.offx + half_w, data.boundary.offy, half_w,half_h);
    // bottom left
    BoxCollideShape BL(data.boundary.offx, data.boundary.offy + half_h, half_w,half_h);
    // bottom right
    BoxCollideShape BR(data.boundary.offx + half_w, data.boundary.offy + half_h, half_w,half_h);

    int new_fc = this->nodes.size();
    for(int j=0;j<4;j++){
        this->nodes.push_back({-1, 0});
    }
    auto& node = this->nodes[data.nodeIdx];
    int i = node.next;
    node.count = -1;
    node.next = new_fc;
    std::vector<int> toErase;
    for(;i!=-1; i = this->elements[i].next){
        auto shape = this->elements[i].shape;
        if(shape->isCollide(&TL)) {
            QuadNodeData d = {TL, new_fc, data.depth+1};
            appendToElements(d,shape);
        }
        if(shape->isCollide(&TR)) {
            QuadNodeData d = {TR, new_fc+1, data.depth+1};
            appendToElements(d,shape);
        }
        if(shape->isCollide(&BL)) {
            QuadNodeData d = {BL, new_fc+2, data.depth+1};
            appendToElements(d,shape);
        }
        if(shape->isCollide(&BR)) {
            QuadNodeData d = {BR, new_fc+3, data.depth+1};
            appendToElements(d,shape);
        }
        toErase.push_back(i);
    }
    for(int i:toErase){
        this->elements.erase(i);
    }

}

void QuadTree::insert(CollideShape *shape){
    
    // find nodes fit the shape
    std::vector<QuadNodeData> nodes;
    this->findNodes(shape, nodes);
    for(QuadNodeData &data: nodes){
        // if no element
        this->appendToElements(data, shape);
    }
    
}
void QuadTree::erase(CollideShape *shape){
    // find nodes fit the shape
    std::vector<QuadNodeData> nodes;
    this->findNodes(shape, nodes);
    for(QuadNodeData &data: nodes){
        // if no element
        auto& node = this->nodes[data.nodeIdx];
        
        for(int i = node.next; i!=-1; i = this->elements[i].next){
            if(shape == this->elements[i].shape) {
                this->elements.erase(i);
                break;
            }
        }
    }

}
void QuadTree::query(CollideShape *shape, std::vector<CollideShape*> &collides){
    std::vector<QuadNodeData> nodes;
    this->findNodes(shape, nodes);
    std::set<CollideShape*> pushed;
    for(QuadNodeData &data: nodes){
        // if no element
        auto& node = this->nodes[data.nodeIdx];
        for(int i = node.next; i!=-1; i = this->elements[i].next){
            auto shape = this->elements[i].shape;
            if(!pushed.count(shape)){
                collides.push_back(shape);
                pushed.insert(shape);
            }
        }
    }
}


void QuadTree::drawGrid(SDL_Renderer *renderer){
    std::vector<QuadNodeData> nodes;
    this->findNodes(&this->boundary, nodes);
    std::set<CollideShape*> pushed;
    for(auto data: nodes){
                // draw grid
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

        SDL_FRect rect = {
            data.boundary.getRealX(),
            data.boundary.getRealY(),
            data.boundary.w,
            data.boundary.h
        };
        auto r = Game::getCamera()->apply(rect);
        SDL_RenderDrawRectF(renderer, &r);
        // draw shapes
        auto& node = this->nodes[data.nodeIdx];
        for(int i = node.next; i!=-1; i = this->elements[i].next){
            auto shape = this->elements[i].shape;
            if(!pushed.count(shape)){
                shape->render(renderer);
                pushed.insert(shape);
            }
        }

    }
}