#include<iostream>


struct vector2d {
    double x, y;
};

struct vector4d {
    union {
        struct {
            double x, y, z, w;
        };
        struct {
            vector2d v1, v2;
        };
    };
    
};

void printVector2(const vector2d& vec) {
    std::cout << vec.x << vec.y << std::endl;
}
union myUnion {
    int i;
    double f;
};
//检查编译器是大端存储还是小端存储
union endian
{
    int a;
    char ch;
};

int main() {
    myUnion u;
    u.i = 42; // 将整数值 42 存储在联合体中
   u.f = 3.14; // 此时整数值将被覆盖，浮点数 3.14 将存储在联合体中

    std::cout << u.f << std::endl;

    std::cout << "----------" << std::endl;

    vector4d vec4 = { 1.0f,2.0f,3.0f,4.0f };
    printVector2(vec4.v1);
    printVector2(vec4.v2);
    vec4.z = 30;
    printVector2(vec4.v2);

    endian value;
    value.a = 0x1234;
    //a和ch共用4字节的内存空间
    if (value.ch == 0x12)
        std::cout << "big endian" << std::endl;
    else if (value.ch == 0x34)
        std::cout << "little endian" << std::endl;

}