#pragma once

struct HAPISPACE::HAPI_TMouseData;
class GUIBase : public IHapiSpritesInputListener
{
public:
	virtual void render() = 0;
	virtual void onLeftClick(const HAPI_TMouseData& mouseData) = 0;
	virtual void onRightClick(const HAPI_TMouseData& mouseData) = 0;
};