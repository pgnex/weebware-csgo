#pragma once
#include "Header.h"

#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define BLACK(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define RED(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define GREEN(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define LAWNGREEN(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define BLUE(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define DEEPSKYBLUE(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define SKYBLUE(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define YELLOW(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define ORANGE(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define DARKORANGE(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define PURPLE(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define CYAN(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define PINK(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define GRAY(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define DARKGRAY(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define DARKERGRAY(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};


enum circle_type
{
	full,
	half,
	quarter
};

enum text_alignment
{
	lefted,
	centered,
	righted
};

class c_draw
{
public:

	c_draw();

	struct sScreen
	{
		float Width;
		float Height;
		float x_center;
		float y_center;
	} Screen;


	ID3DXFont *pFont[6];

	//=============================================================================================
	void Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation);
	void Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color);
	void BoxFilled(float x, float y, float w, float h, D3DCOLOR color);
	void BoxOutlined(float x, float y, float w, float h, D3DCOLOR color);
	void Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color);
	void CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color);
	void Text(char *text, int x, int y, int orientation, int font, DWORD color);
	bool Font();
	void AddFont(char* Caption, float size, bool bold, bool italic);
	void FontReset();
	void OnLostDevice();
	void GetDevice(IDirect3DDevice9* pDev) { pDevice = pDev; }
	void Reset();
	//=============================================================================================

private:
	IDirect3DDevice9 * pDevice;
	IDirect3DVertexBuffer9* g_pVB;    // Buffer to hold vertices
	IDirect3DIndexBuffer9*  g_pIB;    // Buffer to hold indices

	int FontNr;
	LPD3DXSPRITE sSprite;
};

