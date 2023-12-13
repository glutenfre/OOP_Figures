#pragma once

#include "resource.h"
#include <vector>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <cmath>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

namespace DF {
    struct Point {
        double x = 0;
        double y = 0;
    };

    class Figure {
    public:
        virtual void Draw(HDC hdc) = 0;
        void SetSettingPoint(DF::Point set_point);
        virtual ~Figure() = default;
    protected:
        Point set_point_;
        int num_sides_ = 0;
        std::vector<double> side_lengths_;
    };

    class Circle : public Figure {
    public:
        Circle(double rad);
        void Draw(HDC hdc) override;
        ~Circle() = default;
    private:
    };

    class Polygon : public Figure {
    public:
        void SetNumSides(int num_sides);
        void SetSideLengths(std::vector<double> side_lengths);
        virtual ~Polygon() = default;
    private:
    };

    class Triangle : public Polygon {
    public:
        Triangle(double side_length);
        void Draw(HDC hdc) override;
        ~Triangle() = default;
    private:
    };

    class Rectangle : public Polygon {
    public:
        void Draw(HDC hdc) override;
        virtual ~Rectangle() = default;
    private:
    };

    class Square : public Rectangle {
    public:
        void Draw(HDC hdc) override;
        ~Square() = default;
    private:
    };
}