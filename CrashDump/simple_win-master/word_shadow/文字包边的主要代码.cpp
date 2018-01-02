LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(&ps);
		std::wstring wstrDraw = L"To Be No.One!���������ͣ�";
		HFONT hfont = ::CreateFont(
			30,               // height of font
			0,                // average character width
			0,           // angle of escapement
			0,          // base-line orientation angle
			FW_BOLD,              // font weight
			0,           // italic attribute option
			0,        // underline attribute option
			0,        // strikeout attribute option
			OEM_CHARSET,          // character set identifier
			OUT_DEFAULT_PRECIS,  // output precision
			CLIP_DEFAULT_PRECIS,    // clipping precision
			ANTIALIASED_QUALITY,          // output quality
			DEFAULT_PITCH,   // pitch and family
			_T("Arial")           // typeface name
			);
		::SelectObject(hdc, hfont);

		//����͸����������޸ı���
		::SetBkMode(hdc, TRANSPARENT);

//��ʽ1
		//���Ʊ�����Ҳ���ǰ��ߵ���ɫ
		::SetTextColor(hdc, RGB(2,2,2));
		for(int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				CRect rc;
				GetClientRect(&rc);
				rc.OffsetRect(i, j);
				::DrawText(hdc, wstrDraw.c_str(), wstrDraw.length(), rc, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
			}
		}

		//����ǰ��
		::SetTextColor(hdc, RGB(255, 255, 49));
		CRect rc;
		GetClientRect(&rc);
		::DrawText(hdc, wstrDraw.c_str(), wstrDraw.length(), rc, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

//��ʽ2
		LOGFONT lf = {0};
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject((HANDLE)hFont ,sizeof(LOGFONT) ,&lf);
		lf.lfHeight = 16;
		HFONT hhfont = ::CreateFontIndirect(&lf);
		::SelectObject(hdc, hhfont);

		//���Խض����֣����벻����DT_NOCLIP,�ò����ᵼ�»����ı������趨����
		CRect rcOrigin = CRect(100, 30, 175, 50);
		//����ɫ
		::SetTextColor(hdc, RGB(255, 2, 2));
		int nXExcursion[8]={1, 1, 1, 0, -1, -1, -1, 0};
		int nYExcursion[8]={-1, 0, 1, 1, 1, 0, -1, -1};
		for (int i = 0; i < 8; i++)
		{
			CRect rcTemp(rcOrigin.left, rcOrigin.top, rcOrigin.right, rcOrigin.bottom);
			rcTemp.OffsetRect(nXExcursion[i], nYExcursion[i]);
			DrawText(hdc, wstrDraw.c_str(), wstrDraw.length(), rcTemp, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
		::SetTextColor(hdc, RGB(0, 255, 0));
		//DT_NOCLIP ǰ��ɫ
		::DrawText(hdc, wstrDraw.c_str(), wstrDraw.length(), rcOrigin, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		EndPaint(&ps);
		return TRUE;
	}