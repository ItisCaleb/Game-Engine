#include "collision_detection.h"

float CollisionDetection::dis(float ax,float ay,float bx,float by){
    return std::sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}
bool CollisionDetection::checkCollisionBB(BoxCollideShape a,BoxCollideShape b){
    // two boxes
    if(a.y2 < b.y1)return false;
    if(a.y1 > b.y2)return false;
    if(a.x2 < b.x1)return false;
    if(a.x1 > b.x2)return false;
    return true;
}
bool CollisionDetection::checkCollisionBC(BoxCollideShape a,CircleCollideShape b){
    //get point in rectangle which is close to circle
    //init test = circle center (if circle center is in box)
    float tx = b.x;
    float ty = b.y;
    //if circle center is to the right of box take right side
    if(b.x<a.x1) tx = a.x1;
    if(b.x>a.x2) tx = a.x2;
    if(b.y<a.y1) ty = a.y1;
    if(b.y>a.y2) ty = a.y2;
    return dis(tx,ty,b.x,b.y) <= b.r;
}
bool CollisionDetection::checkCollisionBL(BoxCollideShape a,LineCollideShape b){
    bool left   =  checkCollisionLL(LineCollideShape(a.x1,a.y1,a.x1,a.y2),b);
    bool right  =  checkCollisionLL(LineCollideShape(a.x2,a.y1,a.x2,a.y2),b);
    bool top    =  checkCollisionLL(LineCollideShape(a.x1,a.y1,a.x2,a.y1),b);
    bool bottom =  checkCollisionLL(LineCollideShape(a.x1,a.y2,a.x2,a.y2),b);
    return left || right || top || bottom ;
}
bool CollisionDetection::checkCollisionBP(BoxCollideShape a,PointCollideShape b){
    if(a.x2 < b.x)return false;
    if(a.x1 > b.x)return false;
    if(a.y2 < b.y)return false;
    if(a.y1 > b.y)return false;
    return true;
}
bool CollisionDetection::checkCollisionCC(CircleCollideShape a,CircleCollideShape b){
    // distance between two points <=  (ra + rb)
    return dis(a.x,a.y,b.x,b.y) <= (a.r + b.r); 
}
bool CollisionDetection::checkCollisionCP(CircleCollideShape a,PointCollideShape b){
    // distance between circle and points <=  r
    return dis(a.x,a.y,b.x,b.y) <= a.r; 
}
bool CollisionDetection::checkCollisionCL(CircleCollideShape a,LineCollideShape b){
    // line points in circle
    if(dis(a.x,a.y,b.x1,b.y1) <= a.r || dis(a.x,a.y,b.x2,b.y2) <= a.r)return true;
    // p  is the closest point on the line to the circle center
    float u = ((a.x-b.x1)*(b.x2-b.x1)+(a.y-b.y1)*(b.y2-b.y1))/ ((b.x1-b.x2)*(b.x1-b.x2)+(b.y1-b.y2)*(b.y1-b.y2));
    float x = b.x1 + u*(b.x2-b.x1);
    float y = b.y1 + u*(b.y2-b.y1);
    PointCollideShape p(x,y);
    // point p is on line 
    if(!checkCollisionLP(b,p)){
        return false;
    }else{
        // point p is in circle
        return checkCollisionCP(a,p);
    }
}
bool CollisionDetection::checkCollisionLL(LineCollideShape a,LineCollideShape b){
    //calculate the denominator
    float den = ((b.y2-b.y1)*(a.x2-a.x1)-(b.x2-b.x1)*(a.y2-a.y1));            
    //denominator = 0
    if(den == 0) return false;
    float ua = (( b.x2 - b.x1 ) * ( a.y1 - b.y1 ) - ( b.y2 - b.y1 ) * ( a.x1 - b.x1 ) ) / den ;
    float ub = (( a.x2 - a.x1 ) * ( a.y1 - b.y1 ) - ( a.y2 - a.y1 ) * ( a.x1 - b.x1 ) ) / den ;
    // Let 's see if uA and uB tell us the lines are colliding
    return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}
bool CollisionDetection::checkCollisionLP(LineCollideShape a,PointCollideShape b){
    float epsilon = 0.0001; 
    // length of line a
    float length = dis(a.x1,a.y1,a.x2,a.y2);
    // dis between line a point1 and point b
    float dis_a1_b = dis(a.x1,a.y1,b.x,b.y);
    // dis between line a point2 and point b
    float dis_a2_b = dis(a.x2,a.y2,b.x,b.y);
    // triangle law 
    return std::abs(dis_a1_b + dis_a2_b - length) <= epsilon;
}
bool CollisionDetection::checkCollisionPP(PointCollideShape a,PointCollideShape b){
    float epsilon = 0.0001; 
    // two points
    return std::abs(a.x - b.x) <= epsilon && std::abs(a.y -b.y) <= epsilon;
}
