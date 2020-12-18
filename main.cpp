#include <iostream>
using namespace std;

#include "cglscene.h"

int main(int argc, char** argv)
{

    CGLScene gl_scene(800, 600);
    gl_scene.setup(&argc, argv);
    gl_scene.render();

    return 0;
}
