// ELU.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ELU.h"
#include <Windows.h>
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <stdio.h>
#include "Resource.h"
#include <time.h>
#include <iostream>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define ID_TIMER0		68
#define ID_TIMER1		69
#define ID_TIMER2		70
#define ID_TIMER3		71


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	WorkTime(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ShortBreak(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DisplayBitmap(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_ELU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ELU));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ELU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ELU);
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

   HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
	   CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInst, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP bmpOpenEyes;
	HBITMAP bmpCloseEyes;
	HBITMAP	bmpRelax;
	HDC MemDCBitmap;
	static int timeSet1 = 0;
	static int timeSet2 = 0;
	char szBuffer[200];
	static int count = 120;
	int seconds = 0;
	int minutes = 0;
	int hours = 0;
	RECT r;
	HDC hdc;
    switch (message)
    {
		HDC MemDCExercising;
		HBITMAP bmpExercising;
		case WM_CREATE:
		{
			INITCOMMONCONTROLSEX icc;
			icc.dwSize = sizeof(icc);
			icc.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&icc);
			// Lấy font hệ thống
			LOGFONT lf;
			GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
				lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
				lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
				lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
				lf.lfPitchAndFamily, lf.lfFaceName);
			//Chon khung gio lam viec
			HWND hwnd = CreateWindowEx(0, L"STATIC", L"WORK TIME", 
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 5, 100, 40, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"30 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 20, 60, 60, hWnd, (HMENU)IDC_30, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0 ,L"BUTTON", L"40 mins", 
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 60, 60, 60, hWnd, (HMENU)IDC_40, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"50 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 100, 60, 60, hWnd, (HMENU)IDC_50, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"60 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 140, 60, 60, hWnd, (HMENU)IDC_60, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			//Chon khung thu gian ngan
			hwnd = CreateWindowEx(0, L"STATIC", L"SHORT PAUSE",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 5, 100, 40, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"5 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 120, 20, 60, 60, hWnd, (HMENU)IDC_5, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"10 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 120, 60, 60, 60, hWnd, (HMENU)IDC_10, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"15 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 120, 100, 60, 60, hWnd, (HMENU)IDC_15, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"20 mins",
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 120, 140, 60, 60, hWnd, (HMENU)IDC_20, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);


			//Nut dieu khien

			hwnd = CreateWindowEx(0, L"BUTTON", L"Start",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 260, 20, 60, 60, hWnd, (HMENU)IDC_STARTBTN, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"Stop",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 260, 120, 60, 60, hWnd, (HMENU)IDC_STOPBTN, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			//Create an invisible windows that show up the pictures
			/*hwnd = CreateWindowEx(0, szWindowClass, szTitle, WS_EX_LAYERED, CW_USEDEFAULT, CW_USEDEFAULT,
				400, 300, NULL, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);*/

		}
		break;

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
					case IDC_30:
						timeSet1 = 1800000;						
						break;
					case IDC_40:
						timeSet1 = 2400000;
						break;
					case IDC_50:
						timeSet1 = 3000000;
						break;
					case IDC_60:
						timeSet1 = 1200000;
						break;
					case IDC_5:
						timeSet2 = 300000;						
						break;
					case IDC_10:
						timeSet2 = 600000;
						break;
					case IDC_15:
						timeSet2 = 900000;
						break;
					case IDC_20:
						timeSet2 = 1200000;
						break;
					case IDC_STARTBTN:
					{
						if (timeSet1 > 0 || timeSet2 > 0)
						{
							if (timeSet1 > 0)
							{
								SetTimer(hWnd, ID_TIMER1, timeSet1, NULL);
							}
							if (timeSet2 > 0)
							{
								SetTimer(hWnd, ID_TIMER2, timeSet2, NULL);
							}
						}
						
						if(timeSet1 == 0 && timeSet2 ==0) {
							MessageBox(hWnd, L"Vui long chon khung thoi gian", L"Error", MB_OK);
						}
						
						break;
					}
					
					case IDC_STOPBTN:
						KillTimer(hWnd, ID_TIMER1);
						KillTimer(hWnd, ID_TIMER2);
						break;
					
					
					default:
						return 0;
						break;
				}


		}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...
				EndPaint(hWnd, &ps);
			}
			break;

		case WM_TIMER:
		{
			switch (wParam)
			{
				case ID_TIMER1:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_WORKTIME), hWnd, WorkTime);

					return DefWindowProc(hWnd, message, wParam, lParam); //Magic in here costs 3 days of life time.
					break;
				}
				case ID_TIMER2:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_WORKTIME), hWnd, ShortBreak);
					return DefWindowProc(hWnd, message, wParam, lParam); //Magic in here costs 3 days of life time.
					break;
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
			}
		}
		case WM_DESTROY:
			KillTimer(hWnd, ID_TIMER0);
			KillTimer(hWnd, ID_TIMER1);
			KillTimer(hWnd, ID_TIMER2);
			KillTimer(hWnd, ID_TIMER3);
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

	case WM_DESTROY:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	break;
    }
    return (INT_PTR)FALSE;
}

//Shortbreak box
INT_PTR CALLBACK ShortBreak(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP bmpOpenEyes;
	HBITMAP bmpCloseEyes;
	HBITMAP	bmpRelax;
	HDC MemDCBitmap;
	static int count = 100;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, ID_TIMER3, 2000, NULL);
		SetTimer(hDlg, ID_TIMER, 100, NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			KillTimer(hDlg, ID_TIMER2);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	case WM_PAINT:
	{
		if ((count % 2) == 0)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);

			bmpCloseEyes = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			// Create a memory device compatible with the above DC variable
			MemDCBitmap = CreateCompatibleDC(hdc);
			// Select the new bitmap
			SelectObject(MemDCBitmap, bmpCloseEyes);

			// Copy the bits from the memory DC into the current dc
			BitBlt(hdc, 10, 10, 450, 400, MemDCBitmap, 0, 0, SRCCOPY);

			// Restore the old bitmap
			DeleteDC(MemDCBitmap);
			DeleteObject(bmpCloseEyes);
			EndPaint(hDlg, &ps);
		}
		if ((count % 2) != 0)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);

			bmpCloseEyes = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			// Create a memory device compatible with the above DC variable
			MemDCBitmap = CreateCompatibleDC(hdc);
			// Select the new bitmap
			SelectObject(MemDCBitmap, bmpCloseEyes);

			// Copy the bits from the memory DC into the current dc
			BitBlt(hdc, 10, 10, 650, 400, MemDCBitmap, 0, 0, SRCCOPY);

			// Restore the old bitmap
			DeleteDC(MemDCBitmap);
			DeleteObject(bmpCloseEyes);
			EndPaint(hDlg, &ps);
		}
		
	}
	break;
	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIMER3:
			KillTimer(hDlg, ID_TIMER3);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case ID_TIMER:
			count--;
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		default:
			break;
		}
		
		
		break;
	case WM_DESTROY:
	{
		KillTimer(hDlg, ID_TIMER3);
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
	}
	break;
	}
	return (INT_PTR)FALSE;
}

//Worktime box
INT_PTR CALLBACK WorkTime(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP bmpOpenEyes;
	HBITMAP bmpCloseEyes;
	HBITMAP	bmpRelax;
	HDC MemDCBitmap;
	static int count = 100;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, ID_TIMER3, 5000, NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			KillTimer(hDlg, ID_TIMER2);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		/* TODO: Add any drawing code that uses hdc here...
		Load the bitmap from the resource*/
		bmpRelax = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		// Create a memory device compatible with the above DC variable
		MemDCBitmap = CreateCompatibleDC(hdc);
		// Select the new bitmap
		SelectObject(MemDCBitmap, bmpRelax);

		// Copy the bits from the memory DC into the current dc
		BitBlt(hdc, 10, 10, 450, 400, MemDCBitmap, 0, 0, SRCCOPY);

		// Restore the old bitmap
		DeleteDC(MemDCBitmap);
		DeleteObject(MemDCBitmap);
		EndPaint(hDlg, &ps);
		break;

	}
	break;
	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIMER3:
			KillTimer(hDlg, ID_TIMER3);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case ID_TIMER:
			count--;
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		default:
			break;
		}


		break;
	case WM_DESTROY:
	{
		KillTimer(hDlg, ID_TIMER3);
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
	}
	break;
	}
	return (INT_PTR)FALSE;
}

