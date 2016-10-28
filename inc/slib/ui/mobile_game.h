#ifndef CHECKHEADER_SLIB_UI_MOBILE_GAME
#define CHECKHEADER_SLIB_UI_MOBILE_GAME

#include "definition.h"

#include "mobile_app.h"
#include "render_view.h"

SLIB_UI_NAMESPACE_BEGIN

class MobileGameView;

class SLIB_EXPORT MobileGame : public MobileApp
{
	SLIB_DECLARE_OBJECT

public:
	MobileGame();
	
public:
	static Ref<MobileGame> getApp();
	
	Ref<MobileGameView> getGameView();
	
public:
	// override
	virtual void dispatchResize(sl_ui_len width, sl_ui_len height);

private:
	Ref<MobileGameView> m_gameView;
	
};

class SLIB_EXPORT MobileGameView : public RenderView
{
	SLIB_DECLARE_OBJECT
	
public:
	MobileGameView();
	
};

SLIB_UI_NAMESPACE_END

#endif
