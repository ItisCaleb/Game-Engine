#include "engine/quad_tree.h"

#include "engine/game.h"

#include <stack>
#include <set>

QuadTree::QuadTree(int w, int max_depth, int split_threshold)
:boundary(w,w),max_depth(max_depth),split_threshold(split_threshold){
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
            BoundingBox &bd = node_data.boundary;
            // else if branch
            float half_w = bd.w/2;
            float half_h = bd.h/2;
            // top left
            BoundingBox TL(bd.x, bd.y, half_w, half_h);
            // top right
            BoundingBox TR(bd.x + half_w, bd.y, half_w, half_h);
            // bottom left
            BoundingBox BL(bd.x, bd.y + half_h, half_w, half_h);
            // bottom right
            BoundingBox BR(bd.x + half_w, bd.y + half_h, half_w, half_h);
            int nx = node.next;
            if(TL.isCollide(shape)) st.push({TL, nx, node_data.depth+1});
            if(TR.isCollide(shape)) st.push({TR, nx+1, node_data.depth+1});
            if(BL.isCollide(shape)) st.push({BL, nx+2, node_data.depth+1});
            if(BR.isCollide(shape)) st.push({BR, nx+3, node_data.depth+1});
        }
    }
    
}

void QuadTree::findNodes(BoundingBox *box, std::vector<QuadNodeData> &nodes){
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
            BoundingBox &bd = node_data.boundary;
            // else if branch
            float half_w = bd.w/2;
            float half_h = bd.h/2;
            // top left
            BoundingBox TL(bd.x, bd.y, half_w, half_h);
            // top right
            BoundingBox TR(bd.x + half_w, bd.y, half_w, half_h);
            // bottom left
            BoundingBox BL(bd.x, bd.y + half_h, half_w, half_h);
            // bottom right
            BoundingBox BR(bd.x + half_w, bd.y + half_h, half_w, half_h);
            int nx = node.next;
            if(TL.isCollide(box)) st.push({TL, nx, node_data.depth+1});
            if(TR.isCollide(box)) st.push({TR, nx+1, node_data.depth+1});
            if(BL.isCollide(box)) st.push({BL, nx+2, node_data.depth+1});
            if(BR.isCollide(box)) st.push({BR, nx+3, node_data.depth+1});
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
        if (node.count == this->split_threshold)
            this->subDivide(data);
    }
}

void QuadTree::subDivide(QuadNodeData &data){
    if(data.depth == this->max_depth) return;
    float half_w = data.boundary.w/2;
    float half_h = data.boundary.h/2;
    // top left
    BoundingBox TL(data.boundary.x, data.boundary.y, half_w,half_h);
    // top right
    BoundingBox TR(data.boundary.x + half_w, data.boundary.y, half_w,half_h);
    // bottom left
    BoundingBox BL(data.boundary.x, data.boundary.y + half_h, half_w,half_h);
    // bottom right
    BoundingBox BR(data.boundary.x + half_w, data.boundary.y + half_h, half_w,half_h);

    int new_fc = this->nodes.size();
    if(this->free_node == -1){
        for(int j=0;j<4;j++){
            this->nodes.push_back({-1, 0});
        }
    }else{
        new_fc = this->free_node;
        this->free_node = this->nodes[free_node].next;
    }

    auto& node = this->nodes[data.nodeIdx];
    int i = node.next;
    node.count = -1;
    node.next = new_fc;
    
    std::vector<int> toErase;
    for(;i!=-1; i = this->elements[i].next){
        int shapeIdx = this->elements[i].shapeIdx;
        auto shape = this->shapes[shapeIdx];
        if(TL.isCollide(shape)) {
            QuadNodeData d = {TL, new_fc, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(TR.isCollide(shape)) {
            QuadNodeData d = {TR, new_fc+1, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(BL.isCollide(shape)) {
            QuadNodeData d = {BL, new_fc+2, data.depth+1};
            appendToElements(d,shapeIdx);
        }
        if(BR.isCollide(shape)) {
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
    int shapeIdx = -1;
    for(QuadNodeData &data: this->nodeData){
        // if no element
        auto &node = this->nodes[data.nodeIdx];
        auto *last = &node.next;
        for(int i = node.next; i!=-1; i = this->elements[i].next){
            if(shape == this->shapes[this->elements[i].shapeIdx]) {
                if(shapeIdx == -1) shapeIdx = this->elements[i].shapeIdx;
                *last = this->elements[i].next;
                this->elements.erase(i);
                node.count--;
                break;
            }
            last = &this->elements[i].next;
        }
    }
    if(shapeIdx != -1) this->shapes.erase(shapeIdx);
}
void QuadTree::query(CollideShape *shape, std::vector<CollideShape*> &collides){
    this->nodeData.clear();
    this->findNodes(shape, this->nodeData);
    std::set<int> pushed;
    for(QuadNodeData &data: this->nodeData){
        // if no element
        auto &node = this->nodes[data.nodeIdx];
        getNodeShapes(node.next, collides, pushed);
    }
}

void QuadTree::query(BoundingBox *box, std::vector<CollideShape*> &collides){
    this->nodeData.clear();
    this->findNodes(box, this->nodeData);
    std::set<int> pushed;
    for(QuadNodeData &data: this->nodeData){
        // if no element
        auto &node = this->nodes[data.nodeIdx];
        getNodeShapes(node.next, collides, pushed);
    }
}

void QuadTree::cleanup(){
    // Only process the root if it's not a leaf.
    this->leaves.clear();
    std::stack<int> st;
    std::set<int> pushed;
    if (nodes[0].count == -1)
        st.push(0);
    else{
        getNodeShapes(nodes[0].next, this->leaves, pushed);
    }
    while (!st.empty())
    {
        const int node_index = st.top(); st.pop();
        QuadNode& node = nodes[node_index];

        // Loop through the children.
        int num_empty_leaves = 0;
        for (int j=0; j < 4; ++j)
        {
            const int child_index = node.next + j;
            const QuadNode& child = nodes[child_index];

            // Increment empty leaf count if the child is an empty 
            // leaf. Otherwise if the child is a branch, add it to
            // the stack to be processed in the next iteration.
            if (child.count == 0)
                ++num_empty_leaves;
            else if (child.count == -1)
                st.push(child_index);
            else{
                getNodeShapes(child.next, this->leaves, pushed);
            }
        }
        // If all the children were empty leaves, remove them and 
        // make this node the new empty leaf.
        if (num_empty_leaves == this->split_threshold)
        {
            // Push all 4 children to the free list.
            nodes[node.next].next = this->free_node;
            this->free_node = node.next;

            // Make this node the new empty leaf.
            node.next = -1;
            node.count = 0;
        }
    }
}



std::vector<CollideShape*>& QuadTree::getAllShape(){
    return this->leaves;
}

void QuadTree::drawGrid(SDL_Renderer *renderer){
    this->nodeData.clear();
    BoxCollideShape bs(this->boundary.w, this->boundary.h);
    this->findNodes(&bs, this->nodeData);
    std::set<int> pushed;
    for(auto data: this->nodeData){
                // draw grid
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

        SDL_FRect rect = {
            data.boundary.x,
            data.boundary.y,
            data.boundary.w,
            data.boundary.h
        };
        auto r = Game::getCamera()->apply(rect);
        SDL_RenderDrawRectF(renderer, &r);
    }
}