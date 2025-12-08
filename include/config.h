#pragma once

//I suspect this causes a memory leak, the better option is to disable this hell
// #define AUTO_DRIVE_TASK

// A global define that skips over battery & conneciton status.
#define SKIP_OPTIONAL_CONTROLLER_UPDATES


// The amount of dg off for it to consider it "acceptable."
#define ROTATION_OFFSET_LIMIT 2.0f

// Basically just adds macros to make it easier to write auton code
//#define USE_EASY_MOVEMENT


// use the cooler movement (aka actual distance tracking)
//#define DO_DISTANCE_TRACKING