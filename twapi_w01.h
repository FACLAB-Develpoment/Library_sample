#ifndef _TWAPI_W01_H_
#define _TWAPI_W01_H_

/*
  이 라이브러리는 Win32와 Direct2D를 사용해서 프로그램을 하는 
  개발자들이 좀더 편하게 프로그램을 개발할 수 있도록 만든 라이브러리 입니다.
  기존에 제가 만들었던 인터페이스 라이브러리와 유사하게 만들었고
  안드로이드용 라이브러리인 TWAPI_A01과 클래스나 함수를 비슷하게 유지하여
  Win32 프로그램과 안드로이드 앱간에 소스 공유를 편하게 할 수 있도록 했습니다.
  이 라이브러리에 대한 문제점을 발견하거나 개선점이 필요하다면 아래의 이 메일로
  의견 남겨주시거나 블로그에 댓글로 남겨주세요.
  이메일 : tipsware@naver.com
  블로그 : http://blog.naver.com/tipsware
  
  라이브러리 생성 : 2017년 11월 2일 ( 김성엽 )
  라이브러리 최종 업데이트 : 2017년 11월 2일 ( 김성엽 )

  그리고 이 라이브러리를 사용하는 사람들이 편하게 사용할 수 있도록 
  필요한 라이브러리를 추가하는 전처리기 코드를 남깁니다.

  #pragma comment(lib, "D2D1.lib")
  #ifdef _DEBUG
      #pragma comment(lib, "DSH_TWAPI_W01.lib")
  #else
      #pragma comment(lib, "RST_TWAPI_W01.lib")
  #endif
*/

#include <Windows.h>
// Direct2D를 사용하기 위한 헤더 파일과 라이브러리 파일을 포함시킨다.
#include <d2d1.h>

// Direct2D에서 자주 사용할 네임 스페이스를 생략할 수 있도록 설정한다.
using namespace D2D1;

// Direct2D를 구성하는 각종 객체를 생성하는 Factory객체
extern ID2D1Factory *gp_factory;
// WIC(Windows Imaging Component)관련 객체를 생성하기 위한 Factory 객체


class TW_WinApp
{
protected:
	HWND mh_wnd;    // 메인 윈도우 객체의 주소를 저장
	HINSTANCE mh_instance;  // WinMain에 전달되는 첫 번째 인자값 저장
	wchar_t m_class_name[32]; // '윈도우 클래스' 이름을 저장할 변수
	// WinMain 함수의 최종 반환값을 WM_QUIT메시지의 wParam 값으로 사용하기
	// 위해서 이 값을 저장할 변수
	int m_exit_state;

public:
	TW_WinApp(HINSTANCE ah_instance, const wchar_t *ap_class_name);
	virtual ~TW_WinApp();

	int GetExitState() { return m_exit_state; }
	HWND *GetMainWindow() { return mh_wnd; }
	HINSTANCE GetInstanceHandle() { return mh_instance; }

	virtual void InitApplication();
	virtual void InitInstance();
	virtual void Run();
};

#endif