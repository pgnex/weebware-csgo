#include "Header.h"
#include "drawing.h"

c_draw::c_draw()
{
	g_pVB = NULL;
	g_pIB = NULL;
	int FontNr = 0;
}

void c_draw::Reset()
{
	D3DVIEWPORT9 screen;
	pDevice->GetViewport(&screen);

	Screen.Width = screen.Width;
	Screen.Height = screen.Height;
	Screen.x_center = Screen.Width / 2;
	Screen.y_center = Screen.Height / 2;
}

void c_draw::Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color)
{
	ID3DXLine *m_Line;

	D3DXCreateLine(pDevice, &m_Line);
	D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
	m_Line->SetWidth(width);
	if (antialias) m_Line->SetAntialias(1);
	m_Line->Begin();
	m_Line->Draw(line, 2, color);
	m_Line->End();
	m_Line->Release();
}

void c_draw::Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color)
{
	std::vector<vertex> circle;
	circle.reserve(resolution + 2);
	float angle = rotate * D3DX_PI / 180;
	float pi;

	if (type == full) pi = D3DX_PI;        // Full circle
	if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
	if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

	for (int i = 0; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - radius * cos(i*(2 * pi / resolution)));
		circle[i].y = (float)(y - radius * sin(i*(2 * pi / resolution)));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
	g_pVB->Unlock();


	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	if (smoothing)
	{
		pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
	if (g_pVB != NULL) g_pVB->Release();
}

void c_draw::CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color)
{
	std::vector<vertex> circle(resolution + 2);
	float angle = rotate * D3DX_PI / 180;
	float pi;

	if (type == full) pi = D3DX_PI;        // Full circle
	if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
	if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
	g_pVB->Unlock();

	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB != NULL) g_pVB->Release();
}

void c_draw::BoxFilled(float x0, float y0, float w0, float h0, D3DCOLOR color)
{
	//D3DRECT BarRect = { x, y, x + w, y + h };
	//pDevice->Clear(1, &BarRect, 0x00000001, color, 0, 0);
	int w = (x0 + w0) - x0;
	int h = (y0 + h0) - y0;

	ID3DXLine *m_Line;
	D3DXCreateLine(pDevice, &m_Line);
	m_Line->SetWidth(h);
	m_Line->SetAntialias(0);


	D3DXVECTOR2 VertexList[2];
	VertexList[0].x = x0;
	VertexList[0].y = y0 + (h >> 1);
	VertexList[1].x = x0 + w;
	VertexList[1].y = y0 + (h >> 1);

	m_Line->Begin();
	m_Line->Draw(VertexList, 2, color);
	m_Line->End();

}

void c_draw::BoxOutlined(float x, float y, float w, float h, D3DCOLOR color)
{
	Line(x, y, x + w, y, 0, 0, color);
	Line(x, y, x, y + h, 0, 0, color);
	Line(x, y + h, x + w, y + h, 0, 0, color);
	Line(x + w, y, x + w, y + h, 0, 0, color);
}

int c_draw::GetTextWidth(const char* szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.right - rcRect.left;
}

void c_draw::Text(LPCSTR text, int x, int y, int orientation, int font, D3DCOLOR color)
{
	RECT rect = RECT { x,y,x,y };

	switch (orientation)
	{
	case lefted:
		pFont[font]->DrawTextA(NULL, text, -1, &rect, 0x00000100, color);
		break;
	case centered:
		pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | 0x00000100, color);
		break;
	case righted:
		pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | 0x00000100, color);
		break;
	}
}

void c_draw::Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation)
{
	float screen_pos_x = x;
	float screen_pos_y = y;

	// Texture being used is 64x64:
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);

	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(scale, scale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

	//pDevice->SetRenderState(D3DRS_ZENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetPixelShader(NULL);
	sSprite->Begin(NULL);
	sSprite->SetTransform(&mat); // Tell the sprite about the matrix
	sSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
	sSprite->End();
}

bool c_draw::Font()
{
	for (int i = 0; i<FontNr; i++)
		if (pFont[i]) return false;
	return true;
}

void c_draw::AddFont(char* Caption, float size, bool bold, bool italic)
{
	D3DXCreateFontA(pDevice, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, Caption, &pFont[FontNr]);
	FontNr++;
}

void c_draw::FontReset()
{
	for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
	for (int i = 0; i < FontNr; i++) pFont[i]->OnResetDevice();
}

void c_draw::OnLostDevice()
{
	for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
}