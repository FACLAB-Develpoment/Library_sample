include "stdafx.h"
include "twapi_w01.h"

// Direct2D를 구성하는 각종 객체를 생성하는 Factory객체
ID2D1Factory *gp_factory;
// WIC(Windows Imaging Component)관련 객체를 생성하기 위한 Factory 객체
//IWICImagingFactory *gp_wic_factory;
extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

TW_WinApp::TW_WinApp(HINSTANCE ah_instance, const wchar_t *ap_class_name)  //생성함수
{
    CoInitailizeEx(NULL, COINIT_APARTMENTTHREADED);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);
    // Direct2D 초기화 

    mh_instance = ah_instance;  // instance를 Class 내부 변수에 Copy
    wcscpy_s(m_class_name, 32, ap_class_name);
    mh_wnd = NULL;
    m_exit_state = 0;
}    

TW_WinApp::~TW_WinApp()    //소멸함수, 여기서 Direct2D 정리작업
{
    gp_factory->Release();
    CoUninitialize();
}    

void TW_WinApp::InitApplication() //Window Class 등록
{
    WNDCLASS wc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = mh_instance;     // instance를 외부에서 받음.
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = m_class_name;    // class_name도 외부에서 받음.
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    ResisterClass(&wc);
}

void TW_WinApp::InitInstance() // Main Window 생성
{
// 여기서는 형식만 선언하고 외부 파생 클래스에서 윈도우 생성시 용도에 따라 구현해 사용함.
}

void TW_WinApp::Run()  //Message 처리
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) // GetMessage is fuction that reading message from Que.
    {
        TranslateMessage(&msg); //If virtual key message, create an additional message of ASCII type.
        DispatchMessage(&msg); // Function to process the converted message.
    }
    m_exit_state = msg.wParam;
}