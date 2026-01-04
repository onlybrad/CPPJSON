#pragma once

struct Counters {
    unsigned string          = 0U,
             number          = 0U,
             array           = 0U,
             object          = 0U,
             keyword         = 0U,
             comma           = 0U,
             chars           = 0U,
             array_elements  = 0U,
             object_elements = 0U;
};