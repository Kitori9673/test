//===============================================
//
//３Dアクションゲーム〔main.cpp〕
//Author:山根佑太
//
//===============================================
//----------------------------------------
//インクルード
//----------------------------------------
#include "main.h"
#include "input.h"
#include "game.h"


//----------------------------------------
//グローバル変数
//----------------------------------------
LPDIRECT3D9 g_pD3D = NULL;									//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;						//Direct3Dデバイスへのポインタ

//----------------------------------------
//メイン関数
//----------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),									//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,											//ウィンドウのスタイル
		WindowProc,											//ウィンドウプロシージャ
		0,													//０にする（通常は使用しない）
		0,													//０にする（通常は使用しない）
		hInstance,											//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),						//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),							//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),							//クライアント領域の背景色
		NULL,												//メニューバー
		CLASS_NAME,											//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)						//ファイルのアイコン
	};

	HWND hWnd;											//ウィンドウハンドル（識別子）
	MSG msg;											//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//画面サイズの構造体

	RegisterClassEx(&wcex);								//ウィンドウクラスの登録

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);//クライアント領域を指定サイズに調整

														//ウィンドウを生成
	hWnd = CreateWindowEx(0,							//拡張ウィンドウスタイル
		CLASS_NAME,					//ウィンドウクラスの名前
		WINDOW_NAME,				//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		//ウィンドウスタイル
		CW_USEDEFAULT,				//ウィンドウの左上X座標
		CW_USEDEFAULT,				//ウィンドウの左上Y座標
		(rect.right - rect.left),	//ウィンドウの幅
		(rect.bottom - rect.top),	//ウィンドウの高さ
		NULL,						//親ウィンドウのハンドル
		NULL,						//メニューハンドルまたは子ウィンドウID
		hInstance,					//インスタンスハンドル
		NULL);						//ウィンドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化が失敗した場合
		return -1;
	}

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);							//ウィンドウの表示状態を設定
	UpdateWindow(hWnd);									//クライアント領域を更新

	//メッセージグループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージグループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);							//仮想メッセージを文字メッセージを変換
				DispatchMessage(&msg);							//ウィンドウプロシージャへメッセージを選出
			}
		}
		else
		{//DirectXの処理
		 //更新処理
			Update();

			//描画処理
			Draw();
		}
	}

	//終了処理
	Uninit();

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

//----------------------------------------
//初期化処理
//----------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;							//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;					//プレゼンテーションパラメータ

													//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;								//ゲーム画面のサイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;								//ゲーム画面のサイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;								//バックバッファの形式
	d3dpp.BackBufferCount = 1;											//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;								//デブスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;											//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//インターバル

																		//Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成（描画処理はハードウェア、頂点処理をCPUで行う）
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチャステージステートパラメータの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//αブレンディング処理設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//最初のα引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//２番目のα引数

	//
	InitGame();

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------
//終了処理
//----------------------------------------
void Uninit(void)
{
	//

	//キーボードの終了処理
	UninitKeyboard();

	//Drect3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクト
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	//ゲームの終了処理
	UninitGame();
}

//----------------------------------------
//更新処理
//----------------------------------------
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ゲームの更新処理
	UpdateGame();
}

//----------------------------------------
//描画処理
//----------------------------------------
void Draw(void)
{
	//画面クリア（バックバッファ＆Zバッファのクリア）
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//ゲームの描画処理
		DrawGame();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//----------------------------------------
//デバイスの取得
//----------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//----------------------------------------
//ウインドウプロシージャ
//----------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:			//ウインドウ破棄のメッセージ
								//WM_QUITメッセージを送る
		PostQuitMessage(0);	
		break;

	case WM_CLOSE:			    //閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウインドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			 //0を返さないと終了してしまう
		}
		break;

	//case WM_LBUTTONDOWN:       //マウス左クリックのメッセージ
	//							 //ウインドウにフォーカスを合わせる
	//	SetFocus(hWnd);
	//	break;

	case WM_KEYDOWN:			 //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:			  //[ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);        //既定の処理を返す
}