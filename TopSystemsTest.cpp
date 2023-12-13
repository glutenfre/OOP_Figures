#include "framework.h"
#include "TopSystemsTest.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HWND circle_hWnd;
HWND square_hWnd;
HWND rectangle_hWnd;
HWND triangle_hWnd;
HWND child_hWnd;

bool circle_f = false;
bool square_f = false;
bool rect_f = false;
bool triangle_f = false;

DF::Figure* figure = NULL;

ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterChildClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);

void DF::Figure::SetSettingPoint(DF::Point set_point) {
    set_point_ = set_point;
}


void DF::Polygon::SetNumSides(int num_sides)
{
    num_sides_ = num_sides;
}

void DF::Polygon::SetSideLengths(std::vector<double> side_lengths)
{
    side_lengths_.swap(side_lengths);
}

DF::Circle::Circle(double rad) {
    side_lengths_.push_back(rad);
}

void DF::Circle::Draw(HDC hdc) {
    Graphics graphics(hdc);
    Pen blackPen(Color(255, 0, 0, 0), 3);
    double side = side_lengths_[0] * 2.0;
    DF::Point p = set_point_;
    RectF rect(p.x, p.y, side, side);
    Status status = graphics.DrawEllipse(&blackPen, rect);
}

void DF::Rectangle::Draw(HDC hdc) {
    Graphics graphics(hdc);
    Pen blackPen(Color(255, 0, 0, 0), 3);
    double side1 = side_lengths_[0];
    double side2 = side_lengths_[1];
    DF::Point p = set_point_;
    RectF rect(p.x, p.y, side1, side2);
    Status status = graphics.DrawRectangle(&blackPen, rect);
}

void DF::Square::Draw(HDC hdc) {
    Graphics graphics(hdc);
    Pen blackPen(Color(255, 0, 0, 0), 3);
    double side = side_lengths_[0];
    DF::Point p = set_point_;
    RectF rect(p.x, p.y, side, side);
    Status status = graphics.DrawRectangle(&blackPen, rect);
}

DF::Triangle::Triangle(double side_length)
{
    side_lengths_.push_back(side_length);
}

void DF::Triangle::Draw(HDC hdc) {
    Graphics graphics(hdc);
    Pen blackPen(Color(255, 0, 0, 0), 3);
    double side = side_lengths_[0];
    DF::Point p = set_point_;
    PointF point1(p.x, p.y);
    PointF point2(p.x + side, p.y);
    PointF point3(p.x + side*0.5, p.y - side/sqrt(3));
    PointF point4(p.x, p.y);
    
    PointF points[4] = { point1, point2, point3, point4 };
    PointF* pPoints = points;
    Status status = graphics.DrawPolygon(&blackPen, pPoints, 4);
}

void CreateCircle() {
    DF::Circle* circle = new DF::Circle(90);
    DF::Point set_point{ 120, 60 };
    circle->SetSettingPoint(set_point);
    figure = circle;
}

void CreateSquare() {
    DF::Square* square = new DF::Square;
    DF::Point set_point{ 20, 123 };
    square->SetSettingPoint(set_point);
    square->SetNumSides(4);
    std::vector<double> l{ 88 };
    square->SetSideLengths(l);
    figure = square;
}

void CreateRectangle() {
    DF::Rectangle* rectangle = new DF::Rectangle;
    DF::Point set_point{ 50, 50 };
    rectangle->SetSettingPoint(set_point);
    rectangle->SetNumSides(4);
    std::vector<double> l{50, 100};
    rectangle->SetSideLengths(l);
    figure = rectangle;
}

void CreateTriangle() {
    DF::Triangle* triangle = new DF::Triangle(100);
    DF::Point set_point{ 111, 67 };
    triangle->SetSettingPoint(set_point);
    triangle->SetNumSides(3);
    figure = triangle;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TOPSYSTEMSTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterChildClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");
    wndClass.lpszClassName = szWindowClass;

    return RegisterClass(&wndClass);
}

ATOM MyRegisterChildClass(HINSTANCE hInstance)
{
    WNDCLASS wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = ChildWndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0; // sizeof(long)
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("Figure");

    return RegisterClass(&wndClass);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 370, 450, nullptr, nullptr, hInstance, nullptr);

   circle_hWnd = CreateWindow(L"button", L"Circle",
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       10, 20, 60, 30,
       hWnd, 
       HMENU(IDC_CIRCLE),
       hInst,
       nullptr
       );
   square_hWnd = CreateWindow(L"button", L"Square",
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       90, 20, 70, 30,
       hWnd, 
       HMENU(IDC_SQUARE),
       hInst,
       nullptr
       );
   rectangle_hWnd = CreateWindow(L"button", L"Rectangle",
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       170, 20, 90, 30,
       hWnd, 
       HMENU(IDC_RECT),
       hInst,
       nullptr
       );
   triangle_hWnd = CreateWindow(L"button", L"Triangle",
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       270, 20, 75, 30,
       hWnd, 
       HMENU(IDC_TRIANGLE),
       hInst,
       nullptr
       );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   child_hWnd = CreateWindow(
       TEXT("Figure"),
       TEXT(""),
       WS_CHILD | WS_VISIBLE
       | WS_THICKFRAME
       | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
       10,
       70,
       335,
       335,
       hWnd,
       NULL,
       hInstance,
       NULL);

   ShowWindow(child_hWnd, nCmdShow);
   UpdateWindow(child_hWnd);

   delete figure;
   return TRUE;
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);

        if ((circle_f)||(square_f)||(rect_f)||(triangle_f)) {
            figure->Draw(hdc);
        }
        else {
            if (figure != nullptr) {
                delete figure;
                figure = nullptr;
            }
        }

        EndPaint(hWnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_CIRCLE:
                SendMessage(square_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(rectangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(triangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                circle_f = (SendMessage(circle_hWnd, BM_GETCHECK, BST_UNCHECKED, 0) == BST_CHECKED);
                square_f = false;
                rect_f = false;
                triangle_f = false;

                if (circle_f)
                    CreateCircle();

                ShowWindow(child_hWnd, SW_HIDE);
                ShowWindow(child_hWnd, SW_SHOW);
                break;
            case IDC_SQUARE:
                SendMessage(circle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(rectangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(triangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                circle_f = false;
                square_f = (SendMessage(square_hWnd, BM_GETCHECK, BST_UNCHECKED, 0) == BST_CHECKED);
                rect_f = false;
                triangle_f = false;

                if (square_f)
                    CreateSquare();

                ShowWindow(child_hWnd, SW_HIDE);
                ShowWindow(child_hWnd, SW_SHOW);
                break;
            case IDC_RECT:
                SendMessage(square_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(circle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(triangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                circle_f = false;
                square_f = false;
                rect_f = (SendMessage(rectangle_hWnd, BM_GETCHECK, BST_UNCHECKED, 0) == BST_CHECKED);
                triangle_f = false;

                if (rect_f)
                    CreateRectangle();

                ShowWindow(child_hWnd, SW_HIDE);
                ShowWindow(child_hWnd, SW_SHOW);
                break;
            case IDC_TRIANGLE:
                SendMessage(square_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(rectangle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(circle_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                circle_f = false;
                square_f = false;
                rect_f = false;
                triangle_f = (SendMessage(triangle_hWnd, BM_GETCHECK, BST_UNCHECKED, 0) == BST_CHECKED);

                if (triangle_f)
                    CreateTriangle();

                ShowWindow(child_hWnd, SW_HIDE);
                ShowWindow(child_hWnd, SW_SHOW);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

