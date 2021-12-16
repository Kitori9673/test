//===============================================
//
//�RD�A�N�V�����Q�[���kmain.cpp�l
//Author:�R���C��
//
//===============================================
//----------------------------------------
//�C���N���[�h
//----------------------------------------
#include "main.h"
#include "input.h"
#include "game.h"


//----------------------------------------
//�O���[�o���ϐ�
//----------------------------------------
LPDIRECT3D9 g_pD3D = NULL;									//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;						//Direct3D�f�o�C�X�ւ̃|�C���^

//----------------------------------------
//���C���֐�
//----------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),									//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,											//�E�B���h�E�̃X�^�C��
		WindowProc,											//�E�B���h�E�v���V�[�W��
		0,													//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,													//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,											//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),						//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),							//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),							//�N���C�A���g�̈�̔w�i�F
		NULL,												//���j���[�o�[
		CLASS_NAME,											//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)						//�t�@�C���̃A�C�R��
	};

	HWND hWnd;											//�E�B���h�E�n���h���i���ʎq�j
	MSG msg;											//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//��ʃT�C�Y�̍\����

	RegisterClassEx(&wcex);								//�E�B���h�E�N���X�̓o�^

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);//�N���C�A���g�̈���w��T�C�Y�ɒ���

														//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx(0,							//�g���E�B���h�E�X�^�C��
		CLASS_NAME,					//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,				//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,				//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),	//�E�B���h�E�̕�
		(rect.bottom - rect.top),	//�E�B���h�E�̍���
		NULL,						//�e�E�B���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,					//�C���X�^���X�n���h��
		NULL);						//�E�B���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//�����������s�����ꍇ
		return -1;
	}

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);							//�E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);									//�N���C�A���g�̈���X�V

	//���b�Z�[�W�O���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows����
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W�O���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);							//���z���b�Z�[�W�𕶎����b�Z�[�W��ϊ�
				DispatchMessage(&msg);							//�E�B���h�E�v���V�[�W���փ��b�Z�[�W��I�o
			}
		}
		else
		{//DirectX�̏���
		 //�X�V����
			Update();

			//�`�揈��
			Draw();
		}
	}

	//�I������
	Uninit();

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

//----------------------------------------
//����������
//----------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;							//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;					//�v���[���e�[�V�����p�����[�^

													//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;								//�Q�[����ʂ̃T�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;								//�Q�[����ʂ̃T�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;								//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;											//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;								//�f�u�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;											//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//�C���^�[�o��

																		//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�������n�[�h�E�F�A�ōs���j
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐����i�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs���j
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�������n�[�h�E�F�A�ōs���j
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���X�e�[�W�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//���u�����f�B���O�����ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//�ŏ��̃�����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//�Q�Ԗڂ̃�����

	//
	InitGame();

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------
//�I������
//----------------------------------------
void Uninit(void)
{
	//

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//Drect3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	//�Q�[���̏I������
	UninitGame();
}

//----------------------------------------
//�X�V����
//----------------------------------------
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�Q�[���̍X�V����
	UpdateGame();
}

//----------------------------------------
//�`�揈��
//----------------------------------------
void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A�j
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//�Q�[���̕`�揈��
		DrawGame();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//----------------------------------------
//�f�o�C�X�̎擾
//----------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//----------------------------------------
//�E�C���h�E�v���V�[�W��
//----------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:			//�E�C���h�E�j���̃��b�Z�[�W
								//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);	
		break;

	case WM_CLOSE:			    //����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�C���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			 //0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;

	//case WM_LBUTTONDOWN:       //�}�E�X���N���b�N�̃��b�Z�[�W
	//							 //�E�C���h�E�Ƀt�H�[�J�X�����킹��
	//	SetFocus(hWnd);
	//	break;

	case WM_KEYDOWN:			 //�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:			  //[ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);        //����̏�����Ԃ�
}