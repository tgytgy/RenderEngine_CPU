#ifndef RENDERENGINE_MVPTRANSFORM_H
#define RENDERENGINE_MVPTRANSFORM_H
#include <vector>
using namespace std;

class MVPTransform {
public:
    MVPTransform(vector<float> worldPos, vector<float> rotate, vector<float> scale, int projectionTp, vector<float> cameraPos);
    std::vector<float> mvp_transform(std::vector<float> worldPos, std::vector<float> rotate, std::vector<float> scale);
    
};


#endif //RENDERENGINE_MVPTRANSFORM_H
