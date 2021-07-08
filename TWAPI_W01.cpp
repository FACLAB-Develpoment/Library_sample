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

int TWAPI_MakeD2D1_Bitmap(IWICBitmapFrameDecode *ap_image_frame, ID2D1HwndRenderTarget *ap_target, ID2D1Bitmap **ap_bitmap)
{
	IWICFormatConverter *p_converter; // 이미지 변환 객체
	int result = 0;
	// IWICBitmap형식의 비트맵을 ID2D1Bitmap. 형식으로 변환하기 위한 객체 생성
	if (S_OK == gp_wic_factory->CreateFormatConverter(&p_converter)) {
		// 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정한다.
		if (S_OK == p_converter->Initialize(ap_image_frame, GUID_WICPixelFormat32bppPBGRA, 
							WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)) {
			// IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성한다.
			if (S_OK == ap_target->CreateBitmapFromWicBitmap(p_converter, NULL, ap_bitmap)) {
				result = 1;  // 성공적으로 생성한 경우
			}
		}
		TW_IRelease(&p_converter);  // 이미지 변환 객체 제거
	}
	return result;
}

int TWAPI_LoadImage(ID2D1HwndRenderTarget *ap_target, ID2D1Bitmap **ap_bitmap, const wchar_t *ap_path)
{
	// WIC(Windows Imaging Component)관련 객체를 생성하기 위한 Factory 객체 선언
	if (gp_wic_factory == NULL) TWAPI_CreateWIC();
	// WIC 객체를 생성하기 위한 Factory 객체를 생성한다.
	//CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wic_factory));

	IWICBitmapDecoder *p_decoder;     // 압축된 이미지를 해제할 객체
	IWICBitmapFrameDecode *p_frame;   // 특정 그림을 선택한 객체
	int result = 0;  // 그림 파일을 읽은 결과 값 (0이면 그림 읽기 실패, 1이면 그림 읽기 성공)
	// WIC용 Factory 객체를 사용하여 이미지 압축 해제를 위한 객체를 생성
	if (S_OK == gp_wic_factory->CreateDecoderFromFilename(ap_path, NULL, GENERIC_READ, 
											WICDecodeMetadataCacheOnDemand, &p_decoder)) {
		// 파일을 구성하는 이미지 중에서 첫번째 이미지를 선택한다.
		if (S_OK == p_decoder->GetFrame(0, &p_frame)) {
			// 그림의 크기에 맞도록 Target의 크기를 재조정한다.
			ResizeTarget(p_frame);  
			// IWICBitmap 정보를 사용하여 D2D1Bitmap을 만든다.
			result = TWAPI_MakeD2D1_Bitmap(p_frame, ap_target, ap_bitmap);
			TW_IRelease(&p_frame);   // 그림파일에 있는 이미지를 선택하기 위해 사용한 객체 제거
		}
		TW_IRelease(&p_decoder);     // 압축을 해제하기 위해 생성한 객체 제거
	}
	return result;  // PNG 파일을 읽은 결과를 반환한다.
}
//***************************************************************
// TW_wnd Functions

// 윈도우를 파괴할 때 사용하는 함수
void DestoryWindow()
{
	if (mh_wnd != NULL) 
	:: DestroyWindow(mh_wnd);
}
// 윈도우를 갱신을 대기하지 않고 즉시 갱신하도록 하는 함수
void UpdateWindow()
{
	::UpdateWindow(mh_wnd);
}
// 윈도우를 화면에 어떻게 출력시킬 것인지를 결정하는 함수
void ShowWindow(int a_show_type = SW_SHOW)
{
	::ShowWindow(mh_wnd, a_show_type);
}

void ResizeWindow(HWND ah_wnd, int a_width, int a_height)
{
	RECT client_rect, win_rect;
	::GetClientRect(ah_wnd, &client_rect);  // 현재 윈도우의 클라이언트 영역의 좌표를 얻는다.
	::GetWindowRect(ah_wnd, &win_rect);		// 그림의 크기에 맞게 윈도우 크기를 변경하기 위해서 윈도우 좌표를 얻는다.
	::SetWindowPos(ah_wnd, NULL, 0, 0, win_rect.right - win_rect.left + (int)(g_image_rect.right) - client_rect.right,
			win_rect.bottom - win_rect.top + (int)(g_image_rect.bottom) - client_rect.bottom, SWP_NOMOVE);
			// 클라이언트 영역의 크기와 그림 크기의 차이를 계산하여 윈도우 크기를 재조정한다.
}

// 윈도우 화면을 무효화 시켜서 WM_PAINT 메시지를 발생시키는 함수
void Invalidate()
{
	::InvalidateRect(mh_wnd, NULL, FALSE);
}
// 타이머를 등록하는 함수
void SetTimer(UINT a_timer_id, UINT a_elapse_time)
{
	::SetTimer(mh_wnd, a_timer_id, a_elapse_time, NULL);
}
// 타이머를 제거하는 함수
void KillTimer(UINT a_timer_id)
{
	::KillTimer(mh_wnd, a_timer_id);
}

// WM_PAINT 메시지가 발생할 때 작업할 함수. 이 함수는 파생 클래스에서 대부분 재정의 함
virtual void OnPaint()
{

}

virtual int OnCreate(CREATESTRUCT *ap_create_info)
{
	RECT client_rect;
	GetClientRect(mh_wnd, &client_rect);  // 클라이언트 영역의 좌표를 얻는다.

    // 지정한 윈도우의 클라이언트 영역에 그림을 그리기 위한 Render Target을 생성한다.
	gp_factory->CreateHwndRenderTarget(RenderTargetProperties(),
		HwndRenderTargetProperties(ah_wnd, SizeU(client_rect.right, client_rect.bottom)),
		&gp_target);

	return 0;
}
// WM_TIMER 메시지가 발생할 때 작업할 함수.
virtual void OnTimer(UINT a_timer_id)
{

}
// WM_DESTROY 메시지가 발생할 때 작업할 함수. Render Target 객체를 제거하는 작업과
// WM_QUIT 메시지를 발생시키는 작업은 매번 해야 하는 작업이라서 여기서 처리함.
virtual void OnDestroy()
{
	TW_IRelease(&mp_target); //사용하던 Render target 제거
	PostQuitMessage(0); // WM_QUIT를 Message Que에 insert 해서 Program Out.
}