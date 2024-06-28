#include <iostream>

using namespace std;

class Retangle {
protected:
    int width;
    int height;
public:
    Retangle(int width, int height) : width(width), height(height) {};

    int getWidth() const {
        return width;
    }

    virtual void setWidth(int width) {
        Retangle::width = width;
    }

    int getHeight() const {
        return height;
    }

   virtual void setHeight(int height) {
        Retangle::height = height;
    };

    int getArea() {
        return width * height;
    }
};

class Square: public Retangle
{
public:
    Square(int size) : Retangle(size, size) {};
    void setHeight(int height)override
    {
        this->width=this->height=height;
    }
    void setWidth(int width)override
    {
        this->height=this->width=width;
    }
};

void process(Retangle &r) {
    int w = r.getWidth();
    r.setHeight(10);
    cout << "expect area= " << w * 10 << ", and real output is " << r.getArea() << endl;
}

int main() {

    Square square(5);
    process(square);
    return 0;
}
