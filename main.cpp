#include"generator.h"
#include"graphics.h"
using namespace graphics;
int main(){
    char* map=generate();
    render(map);
    delete[](map);
    return 0;
}
