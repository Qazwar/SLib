#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/cursor.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_Cursor : public Cursor
{
public:
	HCURSOR m_hCursor;
	sl_bool m_flagDestroyOnRelease;

	_Win32_Cursor()
	{
	}

	~_Win32_Cursor()
	{
		if (m_flagDestroyOnRelease) {
			::DestroyCursor(m_hCursor);
		}
	}

	static Ref<_Win32_Cursor> create(HCURSOR hCursor, sl_bool flagFreeOnDestroy)
	{
		Ref<_Win32_Cursor> ret;
		if (hCursor) {
			ret = new _Win32_Cursor;
			if (ret.isNotNull()) {
				ret->m_hCursor = hCursor;
				ret->m_flagDestroyOnRelease = flagFreeOnDestroy;
				return ret;
			}
			if (flagFreeOnDestroy) {
				::DestroyCursor(hCursor);
			}
		}
		return ret;
	}
};

Ref<Cursor> UIPlatform::createCursor(HCURSOR hCursor, sl_bool flagDestroyOnRelease)
{
	Ref<_Win32_Cursor> ret = _Win32_Cursor::create(hCursor, flagDestroyOnRelease);
	return Ref<Cursor>::from(ret);
}

HCURSOR UIPlatform::getCursorHandle(const Ref<Cursor>& _cursor)
{
	Ref<Cursor> cursor = _cursor;
	if (cursor.isNull()) {
		return NULL;
	}
	_Win32_Cursor* c = (_Win32_Cursor*)(cursor.getObject());
	return c->m_hCursor;
}

Ref<Cursor> Cursor::getArrow()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	return UIPlatform::createCursor(hCursor, sl_false);
}

Ref<Cursor> Cursor::getIBeam()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
	return UIPlatform::createCursor(hCursor, sl_false);
}

Ref<Cursor> Cursor::getCross()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_CROSS);
	return UIPlatform::createCursor(hCursor, sl_false);
}

Ref<Cursor> Cursor::getHand()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_HAND);
	return UIPlatform::createCursor(hCursor, sl_false);
}

Ref<Cursor> Cursor::getResizeLeftRight()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_SIZEWE);
	return UIPlatform::createCursor(hCursor, sl_false);
}

Ref<Cursor> Cursor::getResizeUpDown()
{
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_SIZENS);
	return UIPlatform::createCursor(hCursor, sl_false);
}

void Cursor::setCurrent(const Ref<Cursor>& _cursor)
{
	Ref<Cursor> cursor = _cursor;
	if (cursor.isNull()) {
		return;
	}
	_Win32_Cursor* c = (_Win32_Cursor*)(cursor.getObject());
	::SetCursor(c->m_hCursor);
}

Ref<Cursor> Cursor::getCurrent()
{
	HCURSOR hCursor = ::GetCursor();
	return UIPlatform::createCursor(hCursor);
}

void Cursor::show()
{
	::ShowCursor(TRUE);
}

void Cursor::hide()
{
	::ShowCursor(FALSE);
}
SLIB_UI_NAMESPACE_END

#endif