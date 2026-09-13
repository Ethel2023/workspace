#include <iostream>
#include <vector>

class Image {
public:
    int width;
    int height;
    std::vector<std::vector<int>> data;//vector动态数组

    // 初始化
    Image(int w, int h, int n = 0) : width(w), height(h) 
    {
        data = std::vector<std::vector<int>>(height, std::vector<int>(width, n));
    }

    //取来该像素点的值
    int getPixel(int x, int y) 
     {
        if (x < 0 || x >= width || y < 0 || y >= height)
         {
            return 0; 
        }
        return data[y][x];
    }

    // 改变该像素点的值
    void setPixel(int x, int y, int n) 
    {
        if (x >= 0 && x < width && y >= 0 && y < height) 
        {
            data[y][x] = n;
        }
    }

    // 输出
    void print() 
     {
        for (int y = 0; y < height; ++y) 
        {
            for (int x = 0; x < width; ++x) 
            {
                std::cout << (data[y][x] == 1 ? "* " : ". ");
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

class SE {
public:
    int width;
    int height;
    std::vector<std::vector<int>> data;

    SE(int w, int h, int n = 0) : width(w), height(h) 
    {
        data = std::vector<std::vector<int>>(height, std::vector<int>(width, n));
    }

    int getPixel(int x, int y) const 
    {
        if (x < 0 || x >= width || y < 0 || y >= height) 
        {
            return 0;
        }
        return data[y][x];
    }

    void setPixel(int x, int y, int n) 
    {
        if (x >= 0 && x < width && y >= 0 && y < height) 
        {
            data[y][x] = n;
        }
    }  
};

//膨胀函数
Image dilate(Image& input, SE& se) {
    int kw = se.width;
    int kh = se.height;
    int cx = kw / 2; 
    int cy = kh / 2; 

    Image output(input.width, input.height, 0);

    // 思路：1. 遍历原图所有像素
    for (int y = 0; y < input.height; ++y) 
    {
        for (int x = 0; x < input.width; ++x)
         {
            // 2. 只选择前景点 
            if (input.getPixel(x, y) == 1)
             {

                
                // 3. 以该点为中心，遍历结构元素
                for (int ky = 0; ky < kh; ++ky) 
                {
                    for (int kx = 0; kx < kw; ++kx) 
                    {
                        
                        // 4. 如果相对位置是1 
                        if (se.getPixel(kx, ky) == 1) 
                        {
                            // 计算需要被膨胀的坐标
                            int outx = x + kx - cx;
                            int outy = y + ky - cy;
                            
                            // 该点位直接暴力变成1
                            output.setPixel(outx, outy, 1); 
                        }
                    }
                }
            }
        }
    }
    return output;
}
// 腐蚀函数
Image erode(Image& input, SE& se) {
    int kw = se.width;
    int kh = se.height;
    int cx = kw / 2; 
    int cy = kh / 2; 

    Image output(input.width, input.height, 0);

    // 思路：1. 遍历原图所有像素
    for (int y = 0; y < input.height; ++y) 
    {
        for (int x = 0; x < input.width; ++x)
        {
            // 2. 只取前景点
            if (input.getPixel(x, y) == 1)
            {
                bool fit = true; 

                // 3. 以该点为中心，遍历结构元素
                for (int ky = 0; ky < kh; ++ky) 
                {
                    for (int kx = 0; kx < kw; ++kx) 
                    {
                        // 4. 如果相对位置处是1
                        if (se.getPixel(kx, ky) == 1) 
                        {
                            int inx = x + kx - cx;
                            int iny = y + ky - cy;
                            
                            // 5. 只要原图中对应位置不匹配就停止循环
                            if (input.getPixel(inx, iny) == 0) 
                            {
                                fit = false;
                                break; 
                            }
                        }
                    }
                    if (!fit) break; 
                }
                // 6. 如果结构元素全落在前景内，该点保留
                if (fit) 
                {
                    output.setPixel(x, y, 1);
                }
            }
        }
    }
    return output;
}
int main() 
{
    int w, h;
    std::cout << "输入图像的宽度和高度: ";
    std::cin >> w >> h;

    Image img(w, h, 0);

    std::cout << "输入图像矩阵:" << "\n";
    for (int i = 0; i < h; ++i) 
    {
        for (int j = 0; j < w; ++j) 
        {
            int val;
            std::cin >> val;
            img.setPixel(j, i, (val == 1 ? 1 : 0));
        }
    }

    int sw, sh;
    std::cout << "输入结构元素的宽度和高度: ";
    std::cin >> sw >> sh;

    SE se(sw, sh, 0);
    std::cout << "请输入结构元素矩阵:" << std::endl;
    for (int i = 0; i < sh; ++i) 
    {
        for (int j = 0; j < sw; ++j) 
        {
            int n;
            std::cin >> n;
            se.setPixel(j, i, (n == 1 ? 1 : 0));
        }
    }

    std::cout << "原图：" << "\n";
    img.print();

    Image dilated_img = dilate(img, se);
    std::cout << "膨胀后：" << "\n";
    dilated_img.print();

    Image eroded_img = erode(img, se);
    std::cout << "腐蚀后：" << "\n";
    eroded_img.print();

    Image final_img = erode(dilated_img, se);
    std::cout << "先膨胀再腐蚀：" << "\n";
    final_img.print();

    return 0;
}