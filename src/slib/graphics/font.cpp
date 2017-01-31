#include "../../../inc/slib/graphics/font.h"

#include "../../../inc/slib/core/safe_static.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

FontDesc::FontDesc()
{
	size = Font::getDefaultFontSize();
	flagBold = sl_false;
	flagItalic = sl_false;
	flagUnderline = sl_false;
}

FontDesc::~FontDesc()
{
}

sl_real _g_graphics_font_default_size = 12;

SLIB_DEFINE_ROOT_OBJECT(Font)

Font::Font()
{
	m_flagMetricsCache = sl_false;
}

Font::~Font()
{
}

sl_real Font::getDefaultFontSize()
{
	return _g_graphics_font_default_size;
}

void Font::setDefaultFontSize(sl_real size)
{
	_g_graphics_font_default_size = size;
}

Ref<Font> Font::getDefault()
{
	SLIB_SAFE_STATIC(AtomicRef<Font>, defaultFont, create(FontDesc()))
	if (SLIB_SAFE_STATIC_CHECK_FREED(defaultFont)) {
		return sl_null;
	}
	return defaultFont;
}

Ref<Font> Font::create(const FontDesc &desc)
{
	Ref<Font> ret = new Font;
	if (ret.isNotNull()) {
		ret->m_desc = desc;
	}
	return ret;
}

Ref<Font> Font::create(String familyName, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline)
{
	FontDesc desc;
	desc.familyName = familyName;
	desc.size = size;
	desc.flagBold = flagBold;
	desc.flagItalic = flagItalic;
	desc.flagUnderline = flagUnderline;
	return create(desc);
}

void Font::getDesc(FontDesc& desc)
{
	desc = m_desc;
}

String Font::getFamilyName()
{
	return m_desc.familyName;
}

sl_real Font::getSize()
{
	return m_desc.size;
}

sl_bool Font::isBold()
{
	return m_desc.flagBold;
}

sl_bool Font::isItalic()
{
	return m_desc.flagItalic;
}

sl_bool Font::isUnderline()
{
	return m_desc.flagUnderline;
}

sl_bool Font::getFontMetrics(FontMetrics& _out)
{
	if (m_flagMetricsCache) {
		_out = m_metricsCache;
		return sl_true;
	}
	if (_getFontMetrics_PO(m_metricsCache)) {
		m_flagMetricsCache = sl_true;
		_out = m_metricsCache;
		return sl_true;
	}
	return 0;
}

sl_real Font::getFontHeight()
{
	FontMetrics fm;
	if (getFontMetrics(fm)) {
		return fm.leading + fm.ascent + fm.descent;
	}
	return 0;
}

sl_real Font::getFontAscent()
{
	FontMetrics fm;
	if (getFontMetrics(fm)) {
		return fm.ascent;
	}
	return 0;
}

sl_real Font::getFontDescent()
{
	FontMetrics fm;
	if (getFontMetrics(fm)) {
		return fm.descent;
	}
	return 0;
}

Ref<Referable> Font::getPlatformObject()
{
	return m_platformObject;
}

#if !(defined(SLIB_PLATFORM_IS_APPLE)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Size Font::getTextSize(const String& text)
{
	return Size::zero();
}

#endif

SLIB_GRAPHICS_NAMESPACE_END


