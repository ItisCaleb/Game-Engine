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

void QuadTree::appendToElements(QuadNodeData &data, int shapdIdx){
    int idx = this->elements.insert({-1, shapdIdx});
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
        int shapeIdx = this->elements[i].shapeIdx;
        auto shape = this->shapes[shapeIdx];
        if(shape->isCollide(&TL)) {
            QuadNodeData d = {TL, new_fc, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(shape->isCollide(&TR)) {
            QuadNodeData d = {TR, new_fc+1, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(shape->isCollide(&BL)) {
            QuadNodeData d = {BL, new_fc+2, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(shape->isCollide(&BR)) {
            QuadNodeData d = {BR, new_fc+3, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        toErase.push_back(i);
    }
    for(int i:toErase){
        this->elements.erase(i);
    }

}

void QuadTree::insert(CollideShape *shape){
    
    // find nodes fit the shape
    this->nodeData.clear();
    this->findNodes(shape, this->nodeData);
    int shapeIdx =  this->shapes.insert(shape);
    for(QuadNodeData &data: this->nodeData){
        // if no element
        this->appendToElements(data, shapeIdx);
    }
    
}
void QuadTree::erase(CollideShape *shape){
    // find nodes fit the shape
    this->nodeData.clear();
    this->findNodes(shape, this->nodeData);
    for(QuadNodeData &data: this->nodeData){
        // if no element
        auto& node = this->nodes[data.nodeIdx];
        
        for(int i = node.next; i!=-1; i = this->elements[i].next){
            if(shape == this->shapes[this->elements[i].shapeIdx]) {
                this->elements.erase(i);
                break;
            }
        }
    }

}
void QuadTree::query(CollideShape *shape, std::vector<CollideShape*> &collides){
    this->nodeData.clear();
    this->findNodes(shape, this->nodeData);
    std::set<int> pushed;
    for(QuadNodeData &data: this->nodeData){
        // if no element
        auto &node = this->nodes[data.nodeIdx];
        for(int i = node.next; i!=-1;){

            auto ele = this->elements[i];
            int shapeIdx = ele.shapeIdx;
            i = ele.next;
            if(!pushed.count(shapeIdx)){
                collides.push_back(this->shapes[shapeIdx]);
                pushed.insert(shapeIdx);
            }
        }
    }
}


void QuadTree::drawGrid(SDL_Renderer *renderer){
    this->nodeData.clear();
    this->findNodes(&this->boundary, this->nodeData);
    std::set<int> pushed;
    for(auto data: this->nodeData){
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
            int shapeIdx = this->elements[i].shapeIdx;
            if(!pushed.count(shapeIdx)){
                pushed.insert(shapeIdx);
                this->shapes[shapeIdx]->render(renderer);
            }
        }

    }
}