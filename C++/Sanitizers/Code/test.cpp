#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int arr[5] = {0, 1, 2, 3, 4};
    std::cout << arr[5] << std::endl;  // 越界访问

    int max_int = 2147483647;  // 最大值为 2^31 - 1
    int overflow = max_int + 1;  // 触发整数溢出
    std::cout << overflow << std::endl;

    //如果要检查内存泄漏需要先保证之前的错误已经修改正确
    int *parr = new int[5]; // 内存泄漏

    return 0;
}
