#include "../../../inc/slib/render/texture.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/graphics/image.h"

SLIB_RENDER_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TextureInstance, RenderBaseObjectInstance)

void TextureInstance::notifyUpdated(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	ObjectLocker lock(this);
	if (m_flagUpdated) {
		m_updatedRegion.mergeRectangle(Rectanglei(x, y, x + width, y + height));
	} else {
		m_updatedRegion.left = x;
		m_updatedRegion.top = y;
		m_updatedRegion.right = x + width;
		m_updatedRegion.bottom = y + height;
		m_flagUpdated = sl_true;
	}
}

SLIB_DEFINE_OBJECT(Texture, RenderBaseObject)

Texture::Texture()
{
	setMinFilter(TextureFilterMode::Linear);
	setMagFilter(TextureFilterMode::Linear);
	setWrapX(TextureWrapMode::Clamp);
	setWrapY(TextureWrapMode::Clamp);
	setFreeSourceOnUpdate(sl_false);
}

Ref<Texture> Texture::create(const Ref<Bitmap>& source)
{
	if (source.isNotNull()) {
		sl_uint32 width = source->getWidth();
		if (width > 0) {
			sl_uint32 height = source->getHeight();
			if (height > 0) {
				Ref<Texture> ret = new Texture;
				if (ret.isNotNull()) {
					ret->m_source = source;
					ret->m_width = width;
					ret->m_height = height;
					return ret;
				}
			}
		}
	}
	return Ref<Texture>::null();
}

Ref<Texture> Texture::create(const BitmapData& bitmapData)
{
	return create(Image::create(bitmapData));
}

Ref<Texture> Texture::create(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	return create(Image::create(width, height, pixels, stride));
}

Ref<Texture> Texture::loadFromMemory(const void* mem, sl_size size)
{
	if (size == 0) {
		return Ref<Texture>::null();
	}
	return create(Image::loadFromMemory(mem, size));
}

Ref<Texture> Texture::loadFromMemory(const Memory& mem)
{
	if (mem.isEmpty()) {
		return Ref<Texture>::null();
	}
	return loadFromMemory(mem.getData(), mem.getSize());
}

Ref<Texture> Texture::loadFromFile(const String& filePath)
{
	return create(Image::loadFromFile(filePath));
}

Ref<Texture> Texture::loadFromAsset(const String& path)
{
	return create(Image::loadFromAsset(path));
}

class _TextureBitmap : public Bitmap
{
	friend class Texture;
};

class _TextureBitmapCache : public BitmapCache
{
public:
	Ref<Texture> texture;
	
	// override
	void update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		texture->update(x, y, width, height);
	}
	
};

Ref<Texture> Texture::getBitmapRenderingCache(const Ref<Bitmap>& source)
{
	if (source.isNotNull()) {
		sl_uint32 width = source->getWidth();
		if (width > 0) {
			sl_uint32 height = source->getHeight();
			if (height > 0) {
				_TextureBitmap* tb = (_TextureBitmap*)(source.ptr);
				Ref<BitmapCache> cache = tb->m_renderingTextureCached;
				if (cache.isNotNull()) {
					return ((_TextureBitmapCache*)(cache.ptr))->texture;
				}
				Ref<Texture> ret = new Texture;
				if (ret.isNotNull()) {
					ret->m_sourceWeak = tb;
					ret->m_width = width;
					ret->m_height = height;
					Ref<_TextureBitmapCache> tc = new _TextureBitmapCache;
					if (tc.isNotNull()) {
						tc->texture = ret;
						tb->m_renderingTextureCached = tc;
						return ret;
					}
				}
			}
		}
	}
	return Ref<Texture>::null();
}

Ref<Bitmap> Texture::getSource()
{
	Ref<Bitmap> source = m_source;
	if (source.isNotNull()) {
		return source;
	}
	return m_sourceWeak;
}

sl_bool Texture::setSource(const Ref<Bitmap>& source)
{
	if (source.isNull()) {
		return sl_false;
	}
	if (source->getWidth() < m_width || source->getHeight() < m_height) {
		return sl_false;
	}
	m_source = source;
	update();
	return sl_true;
}

void Texture::freeSource()
{
	m_source.setNull();
}

sl_uint32 Texture::getWidth()
{
	return m_width;
}

sl_uint32 Texture::getHeight()
{
	return m_height;
}

void Texture::update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	if (x >= m_width) {
		return;
	}
	if (y >= m_height) {
		return;
	}
	if (width > m_width - x) {
		width = m_width - x;
	}
	if (height > m_height - y) {
		height = m_height - y;
	}
	for (int i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
		Ref<RenderBaseObjectInstance> instance = m_instances[i];
		if (instance.isNotNull()) {
			((TextureInstance*)(instance.ptr))->notifyUpdated(x, y, width, height);
		}
	}
}

void Texture::update()
{
	update(0, 0, m_width, m_height);
}

Ref<TextureInstance> Texture::getInstance(RenderEngine* engine)
{
	return Ref<TextureInstance>::from(RenderBaseObject::getInstance(engine));
}

SLIB_RENDER_NAMESPACE_END
