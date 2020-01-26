// HelloWorld.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "HelloWorld.h"

#define MAX_LOADSTRING 100

#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")

//#pragma comment(lib, "PhysXExtensions_static_64.lib")
//#pragma comment(lib, "PhysXPvdSDK_static_64.lib")

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HELLOWORLD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOWORLD));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOWORLD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HELLOWORLD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

#pragma region ADD_CODE
	case WM_CREATE:
        SetTimer(hWnd, NULL, 1000 / 60, nullptr);
        {
		    Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, Allocator, ErrorCallback);
            if (nullptr != Foundation) {

#ifdef USE_PVD
				//!< PVDを立ち上げてから、アプリを起動すること
                Pvd = physx::PxCreatePvd(*Foundation);
                //auto Transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
                //auto Transport = physx::PxDefaultPvdFileTransportCreate(PVD_FILE);
                //Pvd->connect(*Transport, physx::PxPvdInstrumentationFlag::eALL);
#endif

                Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, physx::PxTolerancesScale(), true, Pvd);
                if (nullptr != Physics) {
                    //PxInitExtensions(*Physics, Pvd);

                    physx::PxSceneDesc SceneDesc(Physics->getTolerancesScale());
                    SceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
                    //!< PxDefaultCpuDispatcherCreateはextension
                    //Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
                    //SceneDesc.cpuDispatcher = Dispatcher;
                    //!< PxDefaultSimulationFilterShaderはextension
                    //SceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
                    Scene = Physics->createScene(SceneDesc);
                    if (nullptr != Scene) {
                        auto PVDClient = Scene->getScenePvdClient();
                        if (PVDClient) {
							PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
							PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
							PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
						}

                        //!< マテリアル(摩擦や跳ね返り係数等)を作成
                        Material = Physics->createMaterial(0.5f, 0.5f, 0.6f);
                        if (nullptr != Material) {
                            //!< 地面を作成
                            //!< PxCreatePlaneはextension
							//auto GroundPlane = physx::PxCreatePlane(*Physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *Material);
							//Scene->addActor(*GroundPlane);

                            //!< 箱のスタックを作成
                            auto GlobalTransform = physx::PxTransform(physx::PxVec3(physx::PxZERO::PxZero));
                            physx::PxReal HalfExtent = 2.0f;
							auto Shape = Physics->createShape(physx::PxBoxGeometry(HalfExtent, HalfExtent, HalfExtent), *Material);
							for (auto i = 0; i < 10; i++) {
								for (auto j = 0; j < 10 - i; j++) {
                                    physx::PxTransform LocalTransform(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(10 - i), physx::PxReal(i * 2 + 1), 0) * HalfExtent);
									auto RigidBody = Physics->createRigidDynamic(GlobalTransform.transform(LocalTransform));
									RigidBody->attachShape(*Shape);
									//physx::PxRigidBodyExt::updateMassAndInertia(*RigidBody, 10.0f);
									Scene->addActor(*RigidBody);
								}
							}
							Shape->release();
                        }
                    }
                }
            }
        }
		break;
	case WM_TIMER:
		if (nullptr != Scene) {
			Scene->simulate(1.0f / 60.0f);
			Scene->fetchResults(true);
		}
		break;
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam) {
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		}
		break;
#pragma endregion

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
#pragma region ADD_CODE
		PX_RELEASE(Scene);
		PX_RELEASE(Dispatcher);
		PX_RELEASE(Physics);
#ifdef USE_PVD
        if (nullptr != Pvd) {
            auto Transport = Pvd->getTransport();
            PX_RELEASE(Pvd);
            PX_RELEASE(Transport);
        }
#endif
		PX_RELEASE(Foundation);
#pragma endregion
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
