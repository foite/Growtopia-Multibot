#pragma once

#include <string>
#include <thread>
#include <memory>

// need to be here smh
#include "../Network/Http/HttpClient.h"

#include <enet/enet.h>
#include <proton/Variant.h>

#include "../Logger/ILogger.h"
#include "../Packet/PacketTypes.h"
#include "../Network/Enet/EnetClient.h"
#include "NetAvatar.h"

class Bot : public EnetClient
{
public:
    Bot(std::shared_ptr<ILogger> logger)
        : EnetClient::EnetClient{ logger },
        m_logger{ logger }
    {
        GenerateNewSpoof();
    }

        
    void __nothrow GenerateNewSpoof();

    void SetLoginGuest(std::string mac, std::string rid);
    void SetLoginGrowID(std::string growid, std::string password);

    bool ConnectWithHttp();
    bool Connect(std::string ip, std::string port, std::string meta, bool use_new_packet);

    void StartBotThread();
    void StopBotThread();

    void JoinWorld(std::string world);
    void Move(float x, float y);

    bool IsInGame() { return m_is_in_game; }

    void AlwaysReconnect(bool reconnect) { m_always_reconnect = reconnect; }

    std::unordered_map<int32_t, NetAvatar> GetPlayerList() { return m_player_list; }
    const NetAvatar* GetLocal() { return &m_local; }

private:
    void bot_thread();

    void on_connect() override;
    void on_receive(ENetPacket* pkt) override;
    void on_disconnect() override;

    void on_login();
    void on_login_fail();
    void on_redirect(VariantList* varlist);
    void on_spawn_avatar(VariantList varlist);

    // net message track
    void on_world_visit(std::string world_name);
    void on_world_exit(std::string world_name);

    void on_incoming_packet(ePacketType type, TextPacket pkt);
    void on_incoming_tank_packet(TankPacket pkt);
    void on_incoming_varlist(VariantList varlist, TankPacket pkt);

private:
    std::string m_login_mac{}, m_login_rid{};
    std::string m_login_wk{}, m_login_hash{};

    std::string m_login_meta{};
    std::string m_login_uuid_token{};
    std::string m_login_door_id{};
    std::string m_login_token{};

    std::string m_login_growid{}, m_login_growid_pass{};

    std::string m_game_version = "4.48";
    std::string m_game_proto_version = "203";

    bool m_is_in_world = false, m_is_running = false, m_is_in_game = false;
    bool m_is_redirected = false, m_reconnect = false, m_always_reconnect = false;
    bool m_using_new_packet = false;

    std::string m_server_ip{}, m_server_port{};

    std::string m_user_id{};

    std::thread m_bot_thread{};

    NetAvatar m_local{};
    // netid, netavatar
    std::unordered_map<int32_t, NetAvatar> m_player_list{};

    bool m_is_bot_moving = false;

    std::shared_ptr<ILogger> m_logger{};
    HttpClient m_http_cl { "https://www.growtopia1.com" };
};

