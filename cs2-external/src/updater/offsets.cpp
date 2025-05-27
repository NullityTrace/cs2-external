// offsets.cpp
#include "offsets.h"
#include <iostream>
#include <regex>
#include <string>
#include "../../include/json/json.hpp"


#define CURL_STATICLIB
#include "../../libraries/curl/curl.h"
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

using json = nlohmann::json;

namespace offsets {
    int build_number = 0;

    namespace client {
        std::uintptr_t dwLocalPlayerController = 0;
        std::uintptr_t dwEntityList = 0;
        std::uintptr_t dwViewMatrix = 0;
        std::uintptr_t dwPlantedC4 = 0;
    }

    namespace engine {
        std::uintptr_t dwBuildNumber = 0;
    }

    namespace netvars {
        std::uintptr_t m_flC4Blow = 0;
        std::uintptr_t m_flNextBeep = 0;
        std::uintptr_t m_flTimerLength = 0;
        std::uintptr_t m_pInGameMoneyServices = 0;
        std::uintptr_t m_iAccount = 0;
        std::uintptr_t m_vecAbsOrigin = 0;
        std::uintptr_t m_vOldOrigin = 0;
        std::uintptr_t m_pGameSceneNode = 0;
        std::uintptr_t m_flFlashOverlayAlpha = 0;
        std::uintptr_t m_bIsDefusing = 0;
        std::uintptr_t m_szName = 0;
        std::uintptr_t m_pClippingWeapon = 0;
        std::uintptr_t m_ArmorValue = 0;
        std::uintptr_t m_iHealth = 0;
        std::uintptr_t m_hPlayerPawn = 0;
        std::uintptr_t m_sSanitizedPlayerName = 0;
        std::uintptr_t m_iTeamNum = 0;
    }

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
        size_t realsize = size * nmemb;
        buffer->append((char*)contents, realsize);
        return realsize;
    }

    std::string FetchURL(const std::string& url) {
        CURL* curl = curl_easy_init();
        std::string response;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        return response;
    }

    int GetLatestBuildNumber() {
        std::string commits = FetchURL("https://api.github.com/repos/a2x/cs2-dumper/commits");
        if (commits.empty()) return 0;

        try {
            json commits_json = json::parse(commits);
            std::regex build_regex(R"(\bGame [Uu]pdate \((\d+)(?: \(\d+\))?\b)");

            for (const auto& commit : commits_json) {
                std::string message = commit["commit"]["message"];
                std::smatch match;
                if (std::regex_search(message, match, build_regex)) {
                    return std::stoi(match[1]);
                }
            }
        }
        catch (...) {
            return 0;
        }

        return 0;
    }

    bool FetchOffsets() {
        int latest_build = GetLatestBuildNumber();
        if (latest_build == build_number) return false;

        try {
            // Fetch remote data
            std::string offsets_data = FetchURL("https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json");
            std::string client_data = FetchURL("https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client_dll.json");

            json offsets_json = json::parse(offsets_data);
            json client_json = json::parse(client_data);

            // Update engine offsets
            engine::dwBuildNumber = offsets_json["engine2.dll"]["dwBuildNumber"];

            // Update client offsets
            client::dwLocalPlayerController = offsets_json["client.dll"]["dwLocalPlayerController"];
            client::dwEntityList = offsets_json["client.dll"]["dwEntityList"];
            client::dwViewMatrix = offsets_json["client.dll"]["dwViewMatrix"];
            client::dwPlantedC4 = offsets_json["client.dll"]["dwPlantedC4"];

            // Update netvars
            auto& client_classes = client_json["client.dll"]["classes"];
            netvars::m_flC4Blow = client_classes["C_PlantedC4"]["fields"]["m_flC4Blow"];
            netvars::m_flNextBeep = client_classes["C_PlantedC4"]["fields"]["m_flNextBeep"];
            netvars::m_flTimerLength = client_classes["C_PlantedC4"]["fields"]["m_flTimerLength"];
            netvars::m_pInGameMoneyServices = client_classes["CCSPlayerController"]["fields"]["m_pInGameMoneyServices"];
            netvars::m_iAccount = client_classes["CCSPlayerController_InGameMoneyServices"]["fields"]["m_iAccount"];
            netvars::m_vecAbsOrigin = client_classes["CGameSceneNode"]["fields"]["m_vecAbsOrigin"];
            netvars::m_vOldOrigin = client_classes["C_BasePlayerPawn"]["fields"]["m_vOldOrigin"];
            netvars::m_pGameSceneNode = client_classes["C_BaseEntity"]["fields"]["m_pGameSceneNode"];
            netvars::m_flFlashOverlayAlpha = client_classes["C_CSPlayerPawnBase"]["fields"]["m_flFlashOverlayAlpha"];
            netvars::m_bIsDefusing = client_classes["C_CSPlayerPawn"]["fields"]["m_bIsDefusing"];
            netvars::m_szName = client_classes["CCSWeaponBaseVData"]["fields"]["m_szName"];
            netvars::m_pClippingWeapon = client_classes["C_CSPlayerPawnBase"]["fields"]["m_pClippingWeapon"];
            netvars::m_ArmorValue = client_classes["C_CSPlayerPawn"]["fields"]["m_ArmorValue"];
            netvars::m_iHealth = client_classes["C_BaseEntity"]["fields"]["m_iHealth"];
            netvars::m_hPlayerPawn = client_classes["CCSPlayerController"]["fields"]["m_hPlayerPawn"];
            netvars::m_sSanitizedPlayerName = client_classes["CCSPlayerController"]["fields"]["m_sSanitizedPlayerName"];
            netvars::m_iTeamNum = client_classes["C_BaseEntity"]["fields"]["m_iTeamNum"];

            // Update build number last on success
            build_number = latest_build;
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to update offsets: " << e.what() << std::endl;
            return false;
        }
    }
}