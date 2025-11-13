#pragma once

//I suspect this causes a memory leak, the better option is to disable this hell
// #define AUTO_DRIVE_TASK

// A global define that skips over battery & conneciton status.
#define SKIP_OPTIONAL_CONTROLLER_UPDATES

// Smooths out rotation across x samples, but adds latency.
#define ROTATION_BUFFER 8
//This is the max amount of offset degrees for it to consider it a legit data point.
#define ROTATION_OFFSET_LIMIT 2