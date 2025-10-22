#include "main.h"
#include "lua/lua.hpp"
#include <fstream>
#include <sstream>
#include <cstring>

lua_State* L = nullptr;

// Example: C++ function to expose to Lua
// Lua expects functions in this format: int function(lua_State* L)
static int lua_setMotor(lua_State* L) {
    int port = luaL_checkinteger(L, 1);  // Get first argument
    int velocity = luaL_checkinteger(L, 2);  // Get second argument
    
    // Use your C++ API here
    pros::Motor motor(port);
    motor.move_velocity(velocity);
    
    printf("Motor %d set to %d\n", port, velocity);
    return 0;  // Number of return values
}

static int lua_delay(lua_State* L) {
    int ms = luaL_checkinteger(L, 1);
    pros::delay(ms);
    return 0;
}

static int lua_printToController(lua_State* L) {
    const char* message = luaL_checkstring(L, 1);
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    master.print(0, 0, message);
    return 0;
}

static int lua_getControllerButton(lua_State* L) {
    const char* button = luaL_checkstring(L, 1);
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    
    bool pressed = false;
    if (strcmp(button, "A") == 0) {
        pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    } else if (strcmp(button, "B") == 0) {
        pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    }
    // Add more buttons as needed
    
    lua_pushboolean(L, pressed);
    return 1;  // Returning 1 value
}


// Helper function to run Lua string
bool runLuaString(lua_State* L, const char* code) {
    if (luaL_loadstring(L, code) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        printf("Lua load error: %s\n", error);
        lua_pop(L, 1);
        return false;
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        printf("Lua runtime error: %s\n", error);
        lua_pop(L, 1);
        return false;
    }
    return true;
}

// Call a Lua function with no args
void callLuaFunction(lua_State* L, const char* funcName) {
    lua_getglobal(L, funcName);
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        printf("Error calling %s: %s\n", funcName, error);
        lua_pop(L, 1);
    }
}

// Call Lua function and get string result
std::string callLuaFunctionWithString(lua_State* L, const char* funcName, const char* arg) {
    lua_getglobal(L, funcName);
    lua_pushstring(L, arg);
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        printf("Error: %s\n", error);
        lua_pop(L, 1);
        return "";
    }
    std::string result = lua_tostring(L, -1);
    lua_pop(L, 1);
    return result;
}

void initialize(){
    // Initialize Lua
    L = luaL_newstate();
    luaL_openlibs(L);
    
    // Register C++ functions to be callable from Lua
    lua_register(L, "setMotor", lua_setMotor);
    lua_register(L, "delay", lua_delay);
    lua_register(L, "printToController", lua_printToController);
    lua_register(L, "getButton", lua_getControllerButton);
    
    // Run embedded Lua string (no SD card needed)
    const char* embeddedScript = R"(
        -- Now you can call C++ functions from Lua!
        print("Setting motor 1 to velocity 100")
        setMotor(1, 100)
        delay(1000)
        setMotor(1, 0)
        
        printToController("Hello from Lua!")
        
        -- Check button
        if getButton("A") then
            print("Button A pressed!")
        end
        
        function greet(name)
            return "Hello, " .. name .. " from Lua!"
        end
        
        print("Lua initialized!")
    )";
    runLuaString(L, embeddedScript);
    
    // Call Lua functions
    std::string greeting = callLuaFunctionWithString(L, "greet", "Robot");
    printf("%s\n", greeting.c_str());
}

void disabled() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void competition_initialize() {}
void autonomous() {}

void opcontrol() {
    while (true) {
        pros::delay(20);
    }
}

