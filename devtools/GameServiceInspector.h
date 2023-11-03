#pragma once
class GameServiceInspector
{
public:
    static void Render();
    static void RenderServiceInspector(hh::game::GameService& service);
    static void RenderFxParamManagerInspector(hh::game::GameService& service);
    static void RenderUnknownServiceInspector(hh::game::GameService& service);
};
