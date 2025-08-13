#pragma once

#include <vector>
#include <array>
#include <string>

// Faction class describing the allegiance of a ship or base
class Faction {
public:
    enum class Type {
        TerranConfederation,
        Kilrathi,
        KilrathiDefectors,
        Landreich,
        Steltek,
        Firrekkans,
        Retros,
        Pirates,
        Merchants,
        Mercenaries,
        Mandarins,
        Privateers,
        BorderWorlds,
        Unknown
    };

    explicit Faction(Type type = Type::Unknown) : type_(type) {}

    Type type() const { return type_; }

    std::string name() const {
        switch (type_) {
            case Type::TerranConfederation: return "Terran Confederation";
            case Type::Kilrathi: return "Kilrathi";
            case Type::KilrathiDefectors: return "Kilrathi Defectors";
            case Type::Landreich: return "Landreich";
            case Type::Steltek: return "Steltek";
            case Type::Firrekkans: return "Firrekkans";
            case Type::Retros: return "Retros";
            case Type::Pirates: return "Pirates";
            case Type::Merchants: return "Merchants";
            case Type::Mercenaries: return "Mercenaries";
            case Type::Mandarins: return "Mandarins";
            case Type::Privateers: return "Privateers";
            case Type::BorderWorlds: return "Border Worlds";
            default: return "Unknown";
        }
    }

private:
    Type type_;
};

struct GunPoint {
    float Location[3];
    float direction[3];
};

class Mesh; // forward declaration

struct Turret {
    Mesh* base_mesh = nullptr;
    std::vector<Mesh*> gun_mesh;
    std::array<std::vector<GunPoint>, 3> gun_point;
};

class Mesh {
public:
    int class_id = 0;
    std::vector<int> Vertices;
    std::vector<int> Triangles;
    std::vector<int> Textures;
    std::vector<std::array<float, 3>> Dock_Points;
    std::vector<std::array<float, 3>> Missile_Points;
    std::vector<Turret> Turret_Points;
};

class PlanetSphere {
    // Placeholder for planet geometry
};

class PlanetAtmosphere {
    // Placeholder for atmospheric data
};

class Ship {
public:
    bool can_have_torpedos = false; // Allows use of Anti-Capital ship Torpedoes.
    bool can_have_temblor_bomb = false; // Only true for Excalibur.
    bool can_have_nuke = false; // Only true for Morningstar
    bool can_cloak = false; // allows cloaking, only true for Strakha and Excalibur.
    bool can_jump = false; // allows Jump Drive
    bool can_enter_atmosphere = false; // Allows for Atmospheric entry/Ground Combat.
    bool has_cockpit = false; // always true for player ships
    bool can_repair = false; // determines if repair robot is allowed or not.
    Faction faction{};
    bool friendly_to_player = false;
    int jump_fuel = 0;
    int fuel = 0;
    int hardpoints = 0; // typically 2-8
    int engine_speed = 0;
    int afterburner_speed = 0;
    int turn_left_right_speed = 0;
    int pitch_up_down_speed = 0;
    int roll_speed = 0;
    int mass = 0;
    int cargo_capacity = 0;
    int engine_health = 100; // 0-100 (0 being destroyed)
    int hull_health = 100;   // 0-100 (0 destroys ship)
    int front_armor = 0;
    int rear_armor = 0;
    int left_armor = 0;
    int right_armor = 0;
    int front_shield = 0;
    int rear_shield = 0;
    int left_shield = 0;
    int right_shield = 0;
    Mesh mesh;
};

class CapitalShip : public Ship {
public:
    bool planet_killer = false;
    bool phase_transit_cannon = false;
    bool can_skipper_missile = false;
    bool can_capship_missile = false;
    bool can_launch_fighters = false;
    bool can_launch_bombers = false;
    bool can_launch_shuttles = false;
    bool immune_to_non_torpedoes = false; // immune to gun/missile damage.
    bool has_interior = false;
    bool dockable = false;
    bool landable = false;
    int landing_radius = 0;
    bool friendly = false;
    int turrets = 0;
};

class Fighter : public Ship {};
class Bomber : public Ship {};
class Carrier : public CapitalShip {};
class Destroyer : public CapitalShip {};
class Cruiser : public CapitalShip {};
class Corvette : public CapitalShip {};

class Base {
public:
    bool can_launch_fighters = false;
    bool can_launch_bombers = false;
    bool can_launch_shuttles = false;
    bool immune_to_non_torpedoes = false; // immune to gun/missile damage.
    bool can_cloak = false;
    bool has_interior = false;
    bool dockable = false;
    bool landable = false;
    int landing_radius = 0;
    Faction faction{};
    bool friendly_to_player = false;
    int turrets = 0;
    int engine_health = 100; // 0-100 (0 being destroyed)
    int hull_health = 100;   // 0-100 (0 destroys base)
    int front_armor = 0;
    int rear_armor = 0;
    int left_armor = 0;
    int right_armor = 0;
    int front_shield = 0;
    int rear_shield = 0;
    int left_shield = 0;
    int right_shield = 0;
    Mesh mesh;
    PlanetSphere globe;
    PlanetAtmosphere atmosphere;
};

