#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <pthread.h>

#include "Hook.h"
#include "../main.h"
#include "Common.h"
#include "Game.h"
#include "../gui/Gui.h"

extern Gui *g_Gui;
extern Game *g_Game;

void SendOnTouchEvent(bool sendTruePos) {
    if (sendTruePos) {
        AppOnTouch(Hook::g_onTouchEvent->ontouchThiz, OnTouchEvent.ontouchObject, OnTouchEvent.ontouchType,
                   g_onTouchEvent->ontouchX, OnTouchEvent.ontouchY, OnTouchEvent.ontouchMulti);
    } else {
        AppOnTouch(OnTouchEvent.ontouchThiz, OnTouchEvent.ontouchObject, 1, -1.0f, -1.0f,
                   OnTouchEvent.ontouchMulti);
    }
}

void (*BaseApp_Draw)(void *thiz);
void BaseApp_Draw_hook(void *thiz) {
    BaseApp_Draw(thiz);

    // Initialize stuff
    static bool initialized = false;
    if (!initialized) {
        // Initialize Gui.
        g_Gui = new Gui;
        g_Gui->Init();

        // Initialize Game.
        g_Game = new Game;
        g_Game->Init();

        initialized = true;
    } else {
        // Render the gui (ImGui)
        if (g_Gui) {
            g_Gui->Render();
        }

        // Send on touch event.
        ImGuiIO& io = ImGui::GetIO();
        SendOnTouchEvent(!io.WantCaptureMouse);
    }
}

void (*BaseApp_SetFPSLimit)(void *thiz, float fps);
void BaseApp_SetFPSLimit_hook(void *thiz, float fps) {
    if (g_Game) {
        // Set custom fps limit.
        BaseApp_SetFPSLimit(thiz, g_Game->m_fpsLimit);
    } else {
        BaseApp_SetFPSLimit(thiz, fps);
    }
}

void (*AppOnTouch)(void *a1, void *a2, int type, float x, float y, int multi);
void AppOnTouch_hook(void *thiz, void *object, int type, float x, float y, int multi) {
    if (g_Gui && (x > 0.0 || y > 0.0)) {
        g_Gui->OnTouchEvent(type, multi, x, y);
    }

    // Send it later after checking any item imgui active.
    memset(&OnTouchEvent, 0, sizeof(OnTouchEvent));
    OnTouchEvent.ontouchThiz = thiz;
    OnTouchEvent.ontouchObject = object;
    OnTouchEvent.ontouchType = type;
    OnTouchEvent.ontouchX = x;
    OnTouchEvent.ontouchY = y;
    OnTouchEvent.ontouchMulti = multi;
}

void (*NetHTTP_Update)(NetHTTP *thiz);
void NetHTTP_Update_hook(NetHTTP *thiz) {
    // Check if the custom server ip not "??".
    if (g_Game && g_Game->m_growtopiaServerIp.find("??") == -1) {
        // Check if the NetHTTP server name is growtopia1.com or growtopia2.com.
        if (thiz->serverName.find("growtopia1.com") != -1 ||
            thiz->serverName.find("growtopia2.com") != -1
                ) {
            thiz->serverName = g_Game->m_growtopiaServerIp;
        }
    }

    NetHTTP_Update(thiz);
}

void (*SendPacket)(int type, std::string const &data, ENetPeer *peer);
void SendPacket_hook(int type, std::string const &data, ENetPeer *peer) {
    //LOGDD("SendPacket:\ntype: %d\ndata: %s\npeer: 0x%X", type, data.c_str(), peer);
    /*if (g_Game->m_loginSpoof && data.find("game_version|") != -1) {
        Utils::Split split = Utils::Split::Parse(data, "|");

        if (split.Get("game_version").find(g_Game->m_growtopiaVersion) == -1) {
            split.Set("game_version", g_Game->m_growtopiaVersion);
        }

        data = split.Serialize("|");
    }*/

    SendPacket(type, data, peer);
}

void Hook::Init() {
    LOGD("Initializing Hook..");

    // BaseApp::Draw()
    HOOK(GTS("_ZN7BaseApp4DrawEv"), (void *)BaseApp_Draw_hook, (void **)&BaseApp_Draw);

    // BaseApp::SetFPSLimit()
//    HOOK(GTS("_ZN7BaseApp11SetFPSLimitEf"), (void *) BaseApp_SetFPSLimit_hook,
//         (void **) &BaseApp_SetFPSLimit);

    // AppOnTouch()
    HOOK(GTS("_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void *)AppOnTouch_hook, (void **)&AppOnTouch);

    // NetHTTP::Update()
//    HOOK(GTS("_ZN7NetHTTP6UpdateEv"), (void *) NetHTTP_Update_hook, (void **) &NetHTTP_Update);

    // SendPacket()
//    HOOK(GTS("_Z10SendPacket15eNetMessageTypeRKSsP9_ENetPeer"), (void *) SendPacket_hook,
//         (void **) &SendPacket);
}