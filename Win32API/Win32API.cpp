// Direct3D 프로그래밍에 필요한 최소한의 Win32 코드를 제시한다.

// windows.h 헤더 파일을 포함시킨다. 여기에 Windows 프로그래밍에 필요한
// 모든 Win32 API 구조체, 형식, 함수 선언이 들어 있다.
#include <windows.h>

// 주 창의 핸들. 창 핸들은 생성된 창을 식별하는 용도로 쓰인다.
HWND ghMainWnd = 0;

// Windows 응용 프로그램의 초기화에 필요한 코드를 담은
// 함수. 초기화에 성공하면 true를, 그렇지 않으면
// false를 돌려준다.
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// 메시지 루프 코드를 담은 함수.
int Run();

// 주 창이 받은 사건들을 처리하는 창 프로시저 함수.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows 응용 프로그램의 주 진입점. 콘솔 프로그램의 main()에 해당
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR pCmdLine, int nShowCmd)
{
	// 우선 hInstance와 nShowCmd로 초기화 함수(InitWindowsApp)를
	// 호출해서 응용 프로그램 주 창을 초기화한다.
	if (!InitWindowsApp(hInstance, nShowCmd))
		return 0;


	// 응용 프로그램이 성공적으로 생성, 초기화되었다면 메시지 루프로
	// 진입한다. 그 루프는 응용 프로그램이 종료되어야 함을 뜻하는
	// WM_QUIT 메시지를 받을 때까지 계속 돌아간다.
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// 창을 생성할 때 가장 먼저 할 일은 창의 몇몇 특성을 서술하는
	// WNDCLASS 구조체를 채우는 것이다.
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWndClass";

	// 다음으로, 이 WNDCLASS 인스턴스('창 클래스')를 Windows에 등록한다.
	// 그래야 다음 단계에서 이 창 클래스에 기초해서 창을 생성할 수 있다.
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// WNDCLASS 인스턴스가 성공적으로 등록되었다면 CreateWindow 함수로
	// 창을 생성할 수 있다. 이 함수는 성공 시에는 생성된 창의 핸들(HWND
	// 형식의 값)을, 실패 시에는 값이 0인 핸들을 돌려준다. 창 핸들은 특정
	// 창을 지칭하는 데 쓰이는 값으로, Windows가 내부적으로 관리한다. 창을
	// 다루는 Win32 API 함수 중에는 자신이 작업할 창을 식별하기 위해 이
	// HWND 값을 받는 것들이 많다.

	ghMainWnd = CreateWindow(
		L"BasicWndClass",
		L"Win32Basic",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instanceHandle,
		0);

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// 창이 생성되어도 바로 화면에 나타나지는 않는다. 생성한 창을
	// 실제로 화면에 표시하고 갱신하기 위해서는 다음 두 함수를 호출해
	// 주어야 한다. 두 함수 모두, 표시 또는 갱신하 창의 핸들을 받는다는
	// 점에 주목하기 바란다. 그 핸들은 함수가 표시 또는 갱신할 창이
	// 무엇인지 알려주는 역할을 한다.
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

int Run()
{
	MSG msg = { 0 };

	// WM_QUIT 메시지를 받을 때까지 루프를 돌린다. GetMessage() 함수는
	// WM_QUIT 메시지를 받은 경우에만 0을 돌려주며, 그러면 결과적으로
	// 루프가 종료된다. 만일 메시지 수신에서 오류가 있었으면 이 함수는
	// -1을 돌려준다. 또한, GetMessage()를 호출하면 메시지가 도달할 때까지
	// 응용 프로그램 스레드가 수면(sleep) 상태가 된다는 점도 주의하기
	// 바란다.
	BOOL bRet = 1;
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 몇몇 메시지를 명시적으로 처리한다. 처리한 메시지에
	// 대해서는 반드시 0을 반환해야 함을 주의할 것.
	switch (msg)
	{
		// 왼쪽 마우스 버튼이 눌렸으면 메시지 상자를 표시한다.
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;

		// Esc 키가 눌렸으면 주 응용 프로그램 창을 파괴한다.
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

		// 파괴 메시지의 경우에는 종료 메시지를 보낸다. 그러면
		// 결과적으로 메시지 루프가 종료된다.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// 여기서 명시적으로 처리하지 않은 다른 메시지들은 기본 창
	// 프로시저(DefWindowProc)에게 넘겨 준다. 이 창 프로시저가
	// 반드시 DefWindowProc의 반환값을 돌려주어야 함을 주의하기
	// 바란다.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}