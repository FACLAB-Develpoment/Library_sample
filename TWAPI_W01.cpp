// Direct2D를 구성하는 각종 객체를 생성하는 Factory객체
ID2D1Factory *gp_factory;
// WIC(Windows Imaging Component)관련 객체를 생성하기 위한 Factory 객체
IWICImagingFactory *gp_wic_factory;

class TW_WinApp
{
protected:
    HWND mh_wnd;
    HINSTANCE mh_instance;
    wchar_t m_class_name[32];
    int m_exit_state;
public:
    TW_WinApp(HINSTANCE ah_instance, const wchar_t *ap_class_name)  //생성함수
    {

        CoInitailizeEx(NULL, COINIT_APARTMENTTHREADED);
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREAD, &gp_factory);
        // Direct2D 초기화 

        mh_instance = ah_instance;  // instance를 Class 내부 변수에 Copy
        mh_wnd = NULL;
        m_exit_state = 0;
    }    

    virtual ~TW_WinApp()    //소멸함수, 여기서 Direct2D 정리작업
    {
        if (gp_bitmap !=NULL) gp_bitmap->Release();
        gp_factory->Release();
        CoUninitialize();
    }    

    int GetExitState() //WinMain 함수의 최종 반환값
    {
        return  m_exit_state;
    }

    virtual void InitApplication() //Window Class 등록
    {
        WNDCLASS wc;
        wc.cbClsExtra = NULL;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)NULL;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hInstance = mh_instance;     // instance를 외부에서 받음.
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = L"faclabsoft";
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        ResisterClass(&wc);
    }

    virtual void InitInstance() // Main Window 생성
    {
        mh_wnd = CreateWindow(L"faclabsoft", L"www.faclab.kr", WS_OVERLAPPEDWINDOW,
        100, 90, 400, 350, NULL, NULL, mh_instance, NULL);

        ShowWindow(mh_wnd, SW_Show);
        UpdateWindow(mh_wnd);
    }

    virtual void Run()  //Message 처리
    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) // GetMessage is fuction that reading message from Que.
        {
            TranslateMessage(&msg); //If virtual key message, create an additional message of ASCII type.
            DispatchMessage(&msg); // Function to process the converted message.
        }
        m_exit_state = msg.wParam;
    }
}

void TWAPI_CreateWIC();
int TWAPI_MakeD2D1_Bitmap(IWICBitmapFrameDecode *ap_image_frame, ID2D1HwndRenderTarget *ap_target, ID2D1Bitmap **ap_bitmap)
{
    IWICFormatConverter *pConverter;
    int result_flag = 0;

    if (S_OK == a_pWicFactory->CreateFormatConverter(&pConverter)) // WICBitmap 형식을 ID2D1Bitmap으로 변환용 객체 생성
    {
        if (S_OK == pConverter->Initialize(ap_image_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom))
        {
            //IWICBitmap 형식으로 ID2D1Bitmap 객체 생성
            if (S_OK == g_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &ap_bitmap)) result_flag = 1;
        }
        pConverter->Release();
    } return result_flag;
}
int TWAPI_LoadImage(ID2D1HwndRenderTarget *ap_target, ID2D1Bitmap **ap_bitmap, const wchar_t *ap_path)
{

}