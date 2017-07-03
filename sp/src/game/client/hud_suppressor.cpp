#include "cbase.h"
#include "hud.h"
#include "hud_integrity.h"
#include "hud_macros.h" 
#include "iclientmode.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "c_baseplayer.h"
#include "tier0/memdbgon.h" 

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include "vgui_helpers.h"
#include "vgui\IPanel.h"
#include "hl2\hl2_player_shared.h"

class CHudSuppressor : public CHudElement, public CHudNumericDisplay {
	DECLARE_CLASS_SIMPLE(CHudSuppressor, CHudNumericDisplay);
public:
	CHudSuppressor(const char* pElementName);
	virtual void Init(void);
	virtual void VidInit(void);
	virtual void Reset(void);
	virtual void OnThink(void);
protected:
	//virtual void Paint();
private:
	CPanelAnimationVar(Color, m_HullColor, "HullColor", "255 0 0 255");
	CPanelAnimationVar(Color, m_TextColor, "Normal", "255 208 64 255");
	CPanelAnimationVar(Color, m_ChunkColor, "ChunkColor", "255 220 0 220");
	int m_iDurability;
	int m_iMaxDurability;
	int m_nBarHeight;
	float m_fDurabilityPercent;
};

DECLARE_HUDELEMENT(CHudSuppressor);

CHudSuppressor::CHudSuppressor(const char* pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudSuppressor")
{
	vgui::Panel * pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudSuppressor::Init()
{
	Reset();
	
}

void CHudSuppressor::VidInit()
{
	Reset();
}

void CHudSuppressor::Reset()
{
	SetBgColor(Color(0, 0, 0, 128));
	m_iDurability = 0;
	m_iMaxDurability = 32;
	m_nBarHeight = GetTall() / 3;
	SetPaintEnabled(false);
	SetPaintBackgroundEnabled(false);
	SetShouldDisplayValue(true);

	wchar_t *tempString = g_pVGuiLocalize->Find("#Compliance_Hud_Suppressor");

	if (tempString)
	{
		SetLabelText(tempString);
	}
	else
	{
		SetLabelText(L"SUPPRESSOR");
	}

	SetDisplayValue(0);
}
/*void CHudSuppressor::Paint()
{
	vgui::surface()->DrawSetColor(m_HullColor);
	
	vgui::surface()->DrawFilledRect(10, m_nBarHeight, GetWide() - 10, m_nBarHeight * 2);
	vgui::surface()->DrawSetColor(m_ChunkColor);
	vgui::surface()->DrawFilledRect(10, m_nBarHeight, (int)((GetWide() - 10) * ((float)m_iDurability / (float)m_iMaxDurability)), m_nBarHeight * 2);
}*/

void CHudSuppressor::OnThink(void)
{
	auto pWeapon = GetActiveWeapon();
	if (!pWeapon)
	{
		return;
	}
	if (pWeapon->m_bIsSuppressed)
	{
		if (m_iDurability != pWeapon->m_iSuppressorDurability)
		{
			m_iDurability = pWeapon->m_iSuppressorDurability;
			m_iMaxDurability = pWeapon->m_iSuppressorMaxDurability;
			SetDisplayValue((int)(100.0 * ((float)m_iDurability / (float)m_iMaxDurability)));
		}
		SetPaintEnabled(true);
		SetPaintBackgroundEnabled(true);
	}
	else
	{
		SetPaintEnabled(false);
		SetPaintBackgroundEnabled(false);
	}
}