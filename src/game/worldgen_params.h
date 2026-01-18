#pragma once

namespace cubexx::worldgen {
    struct Params {
        int sea_level = 60;

        float continent_freq = 0.0015f;
        int continent_amp = 40;

        float hills_freq = 0.012f;
        int hills_amp = 26;

        float detail_freq = 0.05f;
        int detail_amp = 6;

        float cave_freq = 0.055f;
        float cave_threshold = 0.62f;
        int cave_min_y = 10;
        int cave_surface_buffer = 3;

        float cave_entrance_freq = 0.02f;      // low-ish: entrance “regions”
        float cave_entrance_threshold = 0.72f; // higher => fewer entrances
        int cave_entrance_max_depth = 10;      // how close to surface entrances can carve

        int topsoil_depth = 6;
        int base_offset = -6;
        bool ocean_bias = true;
    };
}
